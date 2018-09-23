#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include <tuple>
#include <vector>

#include "comm.h"

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

    void AddMessageCallback(MessageCallback callback, INamedPipeCallback *context);
    void AddConnectCallback(ConnectedCallback callback, INamedPipeCallback *context);
    bool Start();
    bool Write(StreamComm::Message &message);
    bool Read(StreamComm::Message* message);
  private:
    // disabling copy constructor
    NamedPipeServer(const NamedPipeServer &other) {}
    void CloseNamedPipe();
    void DispatchOnMessage();
    void DispatchOnConnected();
    
    std::vector<std::tuple<MessageCallback, INamedPipeCallback*>> message_callbacks;
    std::vector<std::tuple<ConnectedCallback, INamedPipeCallback*>> connected_callbacks;

    HANDLE _handle;
};