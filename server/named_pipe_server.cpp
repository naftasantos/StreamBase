#include "named_pipe_server.h"
#include "windows_helper.h"

#include <iostream>

#define BUFFER_SIZE 1024 * 16

NamedPipeServer::NamedPipeServer() {
  this->_handle = NULL;
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

bool NamedPipeServer::Start() {
  bool ok = false;
  char buffer[1024];
  DWORD bytesRead = 0;

  this->_handle = CreateNamedPipe(TEXT(PIPE_NAME),
                                  PIPE_ACCESS_DUPLEX,
                                  PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
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
    while(this->_handle != INVALID_HANDLE_VALUE) {
      std::cout << "Waiting for connection" << std::endl;

      if (ConnectNamedPipe(this->_handle, NULL) != false) {
        this->DispatchOnConnected();

        while(ReadFile(this->_handle, buffer, sizeof(buffer) - 1, &bytesRead, NULL) != false) {
          buffer[bytesRead] = '\0';

          std::cout << buffer << std::endl;
          this->DispatchOnMessage();
        }
      }

      std::cout << "disconnecting..." << std::endl;
      DisconnectNamedPipe(this->_handle);
    }
  }

  return ok;
}

void NamedPipeServer::AddConnectCallback(ConnectedCallback callback, INamedPipeCallback *context) {
  std::tuple<ConnectedCallback, INamedPipeCallback*> tuple(callback, context);

  this->connected_callbacks.push_back(tuple);
}

void NamedPipeServer::AddMessageCallback(MessageCallback callback, INamedPipeCallback *context) {
  std::tuple<MessageCallback, INamedPipeCallback*> tuple(callback, context);

  this->message_callbacks.push_back(tuple);
}

void NamedPipeServer::DispatchOnMessage() {
  for (std::vector<std::tuple<MessageCallback, INamedPipeCallback*>>::iterator i = this->message_callbacks.begin();
       i != this->message_callbacks.end();
       i++) {
    StreamComm::Message msg;
    msg.header.message_command = StreamComm::kCommandGreeting;
    msg.header.data_size = 0;
    memset(msg.data, 0, MAX_DATA_SIZE);

    MessageCallback callback = std::get<0>(*i);
    INamedPipeCallback *context = std::get<1>(*i);

    callback(msg, context);
  }
}

void NamedPipeServer::DispatchOnConnected() {
  for (std::vector<std::tuple<ConnectedCallback, INamedPipeCallback*>>::iterator i = this->connected_callbacks.begin();
       i != this->connected_callbacks.end();
       i++) {
    ConnectedCallback callback = std::get<0>(*i);
    INamedPipeCallback *context = std::get<1>(*i);

    callback(context);
  }
}