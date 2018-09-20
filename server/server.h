#pragma once

#include <windows.h>
#include <string>

class NamedPipeServer{
  public:
    NamedPipeServer();
    virtual ~NamedPipeServer();

    bool Start();
  private:
    // disabling copy constructor
    NamedPipeServer(const NamedPipeServer &other) {}
    void CloseNamedPipe();
    
    HANDLE _handle;
};