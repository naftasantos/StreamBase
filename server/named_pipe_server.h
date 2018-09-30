#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include <tuple>
#include <vector>

#include "comm.h"
#include "named_pipe_io.h"

class INamedPipeCallback {
  public:
    virtual ~INamedPipeCallback() {};
};

typedef std::function<void(StreamComm::Message&, INamedPipeCallback*)> MessageCallback;
typedef std::function<void(INamedPipeCallback*)> ConnectedCallback;

class NamedPipeServer{
  public:
    NamedPipeServer();
    virtual ~NamedPipeServer();

    bool Start(bool async);
    bool Write(StreamComm::Message &message);
    bool Read(StreamComm::Message* message);
    bool WriteAsync(StreamComm::Message& message, StreamComm::IStreamCallback *callback, void *data);
    bool ReadAsync(StreamComm::IStreamCallback *callback, void *data);
  private:
    // disabling copy constructor
    NamedPipeServer(const NamedPipeServer &other) {}
    void CloseNamedPipe();

    OVERLAPPED overlap;
    HANDLE _handle;
};