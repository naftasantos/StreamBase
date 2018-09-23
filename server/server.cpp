#include "server.h"

#include <iostream>

void on_message(StreamComm::Message& message, INamedPipeCallback* context) {
  Server *server = dynamic_cast<Server*>(context);

  if (server != nullptr) {
    server->OnMessage(message);
  } else {
    std::cout << "context is invalid" << std::endl;
  }
}

void on_connected(INamedPipeCallback *context) {
  Server *server = dynamic_cast<Server*>(context);

  if (server != nullptr) {
    server->OnConnected();
  } else {
    std::cout << "context is invalid" << std::endl;
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
  std::cout << "Client connected! Sending greeting" << std::endl;
  StreamComm::Message message;

  message.header.message_command = StreamComm::kCommandGreeting;
  message.header.data_size = 0;
  memset(message.data, 0, MAX_DATA_SIZE);

  if (this->named_pipe.Write(message)) {
    std::cout << "Message sent successfully" << std::endl;
  } else {
    std::cerr << "Error sending greeting" << std::endl;
  }
}
