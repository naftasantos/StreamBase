#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include <tuple>
#include <vector>

#include "comm.h"
#include "named_pipe_io.h"

class NamedPipeServer{
  public:
    NamedPipeServer(bool async);
    virtual ~NamedPipeServer();

    bool Start();
    bool Write(StreamComm::Message& message, StreamComm::IStreamCallback *callback, void *data);
    bool Read(StreamComm::IStreamCallback *callback, void *data);
  private:
    // disabling copy constructor
    NamedPipeServer(const NamedPipeServer &other) {}
    void CloseNamedPipe();

    OVERLAPPED overlap;
    HANDLE _handle;
    bool async;
};