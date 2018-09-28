#include "named_pipe_server.h"
#include "windows_helper.h"
#include "named_pipe_io.h"

#include <iostream>

#define BUFFER_SIZE 1024 * 16

NamedPipeServer::NamedPipeServer() {
  this->_handle = NULL;
  memset(&(this->overlap), 0, sizeof(OVERLAPPED));

  this->overlap.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
  if (this->overlap.hEvent == NULL) {
    std::cerr << "Failed to create event" << std::endl;
  }
}

NamedPipeServer::~NamedPipeServer() {
  this->CloseNamedPipe();
}

void NamedPipeServer::CloseNamedPipe() {
  if (this->_handle != NULL) {
    DisconnectNamedPipe(this->_handle);
    this->_handle = NULL;
  }
}

bool NamedPipeServer::Start(bool async) {
  bool ok = false;
  DWORD options = PIPE_ACCESS_DUPLEX;
  DWORD pipe_mode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT;

  if (async) {
    std::cout << "Async mode. Setting overlapped to the options" << std::endl;
    options |= FILE_FLAG_OVERLAPPED;
  }

  this->_handle = CreateNamedPipe(
    TEXT(PIPE_NAME),
    options,
    pipe_mode,
    PIPE_UNLIMITED_INSTANCES,
    BUFFER_SIZE,
    BUFFER_SIZE,
    NMPWAIT_USE_DEFAULT_WAIT,
    NULL);

  if (this->_handle == INVALID_HANDLE_VALUE) {
    std::cerr << "Unable to create Named Pipe" << std::endl;
    std::cerr << "Error: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
    this->_handle = NULL;
    ok = false;
  } else {
    std::cout << "Named Pipe created" << std::endl;
    std::cout << "Waiting for connection" << std::endl;

    if (async) {
      std::cout << "Async waiting..." << std::endl;
      if (ConnectNamedPipe(this->_handle, (LPOVERLAPPED)&(this->overlap))) {
        // According to Microsoft at
        // https://docs.microsoft.com/en-us/windows/desktop/sync/synchronization-and-overlapped-input-and-output,
        // When a function is called to perform an overlapped operation, the operation might be completed before the
        // function returns. When this happens, the results are handled as if the operation had been performed
        // synchronously.
        this->DispatchOnConnected();
      } else {
        DWORD wait_result;
        std::cout << "Current connect state: "
                  << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
        switch (GetLastError()) 
        {
          case ERROR_IO_PENDING:
            ok = true;
            break; 
          case ERROR_PIPE_CONNECTED: 
            if (SetEvent(this->overlap.hEvent)) {
              break; 
            }
          default: 
            std::cerr << "Connect failed: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
            ok = false;
            break;
        }

        wait_result = WaitForSingleObjectEx(this->overlap.hEvent, INFINITE, TRUE);

        if (wait_result == 0) { // The first handle was signaled
          this->DispatchOnConnected();
        } else{
          ok = false;
        }
      }
    } else {
      if (ConnectNamedPipe(this->_handle, NULL)) {
        this->DispatchOnConnected();
      }
    }
  }

  return ok;
}

void NamedPipeServer::AddConnectCallback(ConnectedCallback callback, INamedPipeCallback *context) {
  std::tuple<ConnectedCallback, INamedPipeCallback*> tuple(callback, context);

  this->connected_callbacks.push_back(tuple);
}

// void NamedPipeServer::AddMessageCallback(MessageCallback callback, INamedPipeCallback *context) {
//   std::tuple<MessageCallback, INamedPipeCallback*> tuple(callback, context);

//   this->message_callbacks.push_back(tuple);
// }

// void NamedPipeServer::DispatchOnMessage() {
//   for (std::vector<std::tuple<MessageCallback, INamedPipeCallback*>>::iterator i = this->message_callbacks.begin();
//        i != this->message_callbacks.end();
//        i++) {
//     StreamComm::Message msg;
//     msg.header.message_command = StreamComm::kCommandGreeting;
//     msg.header.data_size = 0;
//     memset(msg.data, 0, MAX_DATA_SIZE);

//     MessageCallback callback = std::get<0>(*i);
//     INamedPipeCallback *context = std::get<1>(*i);

//     callback(msg, context);
//   }
// }

void NamedPipeServer::DispatchOnConnected() {
  for (auto item : this->connected_callbacks) {
    ConnectedCallback callback = std::get<0>(item);
    INamedPipeCallback *context = std::get<1>(item);

    callback(context);
  }
}

bool NamedPipeServer::Write(StreamComm::Message &message) {
  return StreamComm::NamedPipeIO::Write(this->_handle, message);
}

bool NamedPipeServer::Read(StreamComm::Message *message) {
  return StreamComm::NamedPipeIO::Read(this->_handle, message);
}

bool NamedPipeServer::WriteAsync(StreamComm::IWriteCallback *callback, void *data) {
  return StreamComm::NamedPipeIO::WriteAsync(this->_handle, callback, data);
}

bool NamedPipeServer::ReadAsync(StreamComm::IReadCallback *callback, void *data) {
  return StreamComm::NamedPipeIO::ReadAsync(this->_handle, callback, data);
}