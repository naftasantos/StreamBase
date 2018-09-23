#include "server.h"
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
    std::cerr << "Error: " << WindowsHelper::GetLastErrorMessage() << std::endl;
    this->_handle = NULL;
    ok = false;
  } else {
    std::cout << "Named Pipe created" << std::endl;

    while(this->_handle != INVALID_HANDLE_VALUE) {
      std::cout << "Waiting for connection" << std::endl;

      if (ConnectNamedPipe(this->_handle, NULL) != false) {
        std::cout << "Client connected!" << std::endl;

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

void NamedPipeServer::AddMessageCallback(CallbackFunction callback, void *data) {
  std::tuple<CallbackFunction, void*> tuple(callback, data);

  this->callbacks.push_back(tuple);
}

void NamedPipeServer::DispatchOnMessage() {
  for (std::vector<std::tuple<CallbackFunction, void*>>::iterator i = this->callbacks.begin();
       i != this->callbacks.end();
       i++) {
    Message msg;
    msg.header.message_command = kCommandGreeting;
    msg.header.data_size = 0;
    memset(msg.data, 0, MAX_DATA_SIZE);

    CallbackFunction callback = std::get<0>(*i);
    void *data = std::get<1>(*i);

    callback(msg, data);
  }
}