#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include <tuple>
#include <vector>

#include "comm.h"

typedef std::function<void(Message&, void*)> CallbackFunction;

class NamedPipeServer{
  public:
    NamedPipeServer();
    virtual ~NamedPipeServer();

    void AddMessageCallback(CallbackFunction callback, void *data);
    bool Start();
  private:
    // disabling copy constructor
    NamedPipeServer(const NamedPipeServer &other) {}
    void CloseNamedPipe();
    void DispatchOnMessage();
    
    std::vector<std::tuple<CallbackFunction, void*>> callbacks;
    
    HANDLE _handle;
};