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

    // void AddMessageCallback(MessageCallback callback, INamedPipeCallback *context);
    void AddConnectCallback(ConnectedCallback callback, INamedPipeCallback *context);
    bool Start();
    bool Write(StreamComm::Message &message);
    bool Read(StreamComm::Message* message);
    bool WriteAsync(StreamComm::IWriteCallback *callback, void *data);
    bool ReadAsync(StreamComm::IReadCallback *callback, void *data);
  private:
    // disabling copy constructor
    NamedPipeServer(const NamedPipeServer &other) {}
    void CloseNamedPipe();
    // void DispatchOnMessage();
    void DispatchOnConnected();
    
    // std::vector<std::tuple<MessageCallback, INamedPipeCallback*>> message_callbacks;
    std::vector<std::tuple<ConnectedCallback, INamedPipeCallback*>> connected_callbacks;

    HANDLE _handle;
};