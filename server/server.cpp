#include "server.h"
#include "comm.h"
#include "commands/command_factory.h"
#include "windows_helper.h"
#include "config.h"

#include <iostream>

Server::Server()
  : status(true),
    named_pipe(Config::Async) {
}

Server::~Server() {
  for(std::map<std::string, IServerDataObject*>::iterator i = this->data.begin(); i != this->data.end(); i++) {
    delete i->second;
  }
}

void Server::Start() {
  this->status = this->named_pipe.Start();

  if (this->status) {
    OnConnected();
  }
}

void Server::OnMessage(StreamComm::Message &message) {
  std::cout << "Message received" << std::endl;
  ICommand *command = nullptr;
  StreamComm::ResponseCommand response = { 0 };

  command = CommandFactory::GetCommand(message.header.message_command);

  if (command != nullptr) {
    response = command->Execute(message, this->data);
    std::cout << "Command executed" << std::endl;
  } else {
    std::cout << "Unable to find command" << std::endl;
    response.status = false;
    memcpy(response.message, "Unable to find command", 22);
  }

  message.header.message_command = StreamComm::kCommandResponse;
  message.header.data_size = sizeof(StreamComm::ResponseCommand);
  memset(message.data, 0, MAX_DATA_SIZE);
  memcpy(message.data, &response, message.header.data_size);

  std::cout << "Sending response with status '" << (response.status ? "true" : "false") <<
                "' and message '" << response.message << "'" << std::endl;

  if (!this->named_pipe.Write(message, this, nullptr)) {
    this->status = false;
    std::cout << "Unable to send response: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
  }
}

void Server::ReadNextMessage() {
  if (!this->named_pipe.Read(this, nullptr)) {
    this->status = false;
    std::cerr << "Error Reading message Async: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
  }
}

void Server::OnConnected() {
  std::cout << "Client connected! Sending greeting" << std::endl;
  StreamComm::Message message;

  message.header.message_command = StreamComm::kCommandGreeting;
  message.header.data_size = 0;
  memset(message.data, 0, MAX_DATA_SIZE);

  if (!this->named_pipe.Write(message, this, nullptr)) {
    std::cerr << "Error sending greeting: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
    this->status = false;
  }
}

void Server::OnWrite(bool success, StreamComm::Message message, void *data) {
  if (success) {
    std::cout << "On Write!" << std::endl;
    ReadNextMessage();
  } else {
    this->status = false;
    std::cerr << "Error on write" << std::endl;
  }
}

void Server::OnRead(bool success, StreamComm::Message message, void *data) {
  if (success) {
    std::cout << "On Read!" << std::endl;
    OnMessage(message);
  } else {
    this->status = false;
    std::cerr << "Error on read" << std::endl;
  }
}

bool Server::GetStatus() {
  return this->status;
}