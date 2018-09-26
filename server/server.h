#pragma once

#include "named_pipe_server.h"
#include "comm.h"
#include "commands/command_interface.h"
#include "named_pipe_io.h"

class Server : public INamedPipeCallback, public StreamComm::IReadCallback, public StreamComm::IWriteCallback {
  public:
    Server();
    virtual ~Server();

    void Start();
    void OnConnected();

    virtual void OnWrite(bool success, StreamComm::Message message, void *data);
    virtual void OnRead(bool success, StreamComm::Message message, void *data);
  private:
    Server(const Server&); // disabling copy constructor
    NamedPipeServer named_pipe;
    std::map<std::string, IServerDataObject*> data;

    void OnMessage(StreamComm::Message &message);
    void ReadNextMessage();
    static bool Status;
};