#include "server.h"
#include "windows_helper.h"

#include <iostream>

#define PIPE_NAME "\\\\.\\pipe\\StreamBase"
#define BUFFER_SIZE 1024 * 16

NamedPipeServer::NamedPipeServer() {
  this->_handle = NULL;
}

NamedPipeServer::~NamedPipeServer() {
  if (this->_handle != NULL) {
    this->CloseNamedPipe();
  }
}

void NamedPipeServer::CloseNamedPipe() {
  if (this->_handle != NULL) {
    DisconnectNamedPipe(this->_handle);
  }
}

bool NamedPipeServer::Start() {
  bool ok = false;
  char buffer[1024];
  DWORD bytesRead = 0;

  this->_handle = CreateNamedPipe(TEXT(PIPE_NAME),
                                  PIPE_ACCESS_DUPLEX,
                                  PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
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
        }
      }

      DisconnectNamedPipe(this->_handle);
    }
  }

  return ok;
}