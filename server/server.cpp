#include "server.h"

#include <iostream>

void on_message(StreamComm::Message& message, INamedPipeCallback* context) {
  Server *server = dynamic_cast<Server*>(context);

  if (server != nullptr) {
    server->OnMessage(message);
  }
}

void on_connected(INamedPipeCallback *context) {
  Server *server = dynamic_cast<Server*>(context);

  if (server != nullptr) {
    server->OnConnected();
  }
}

Server::Server() {

}

Server::~Server() {

}

void Server::Start() {
  this->named_pipe.AddConnectCallback(std::cref(on_connected), this);
  this->named_pipe.AddMessageCallback(std::cref(on_message), this);
  this->named_pipe.Start();
}

void Server::OnMessage(StreamComm::Message &message) {
  std::cout << "Received callback" << std::endl;
}

void Server::OnConnected() {
  std::cout << "Client connected!" << std::endl;
}
