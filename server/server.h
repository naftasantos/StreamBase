#pragma once

#include "named_pipe_server.h"
#include "comm.h"
#include "commands/command_interface.h"

class Server : public INamedPipeCallback {
  public:
    Server();
    virtual ~Server();

    void Start();
    void OnMessage(StreamComm::Message &message);
    void OnConnected();
  private:
    Server(const Server&); // disabling copy constructor
    NamedPipeServer named_pipe;
    std::map<std::string, IServerDataObject*> data;
};