#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include <tuple>
#include <vector>

#include "comm.h"

typedef std::function<void(StreamComm::Message&, void*)> MessageCallback;
typedef std::function<void(void*)> ConnectedCallback;

class NamedPipeServer{
  public:
    NamedPipeServer();
    virtual ~NamedPipeServer();

    void AddMessageCallback(MessageCallback callback, void *context);
    void AddConnectCallback(ConnectedCallback callback, void *context);
    bool Start();
  private:
    // disabling copy constructor
    NamedPipeServer(const NamedPipeServer &other) {}
    void CloseNamedPipe();
    void DispatchOnMessage();
    void DispatchOnConnected();
    
    std::vector<std::tuple<MessageCallback, void*>> message_callbacks;
    std::vector<std::tuple<ConnectedCallback, void*>> connected_callbacks;

    HANDLE _handle;
};