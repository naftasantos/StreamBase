#include "server.h"
#include "comm.h"
#include "commands/command_factory.h"
#include "windows_helper.h"
#include "config.h"

#include <iostream>

// void on_message(StreamComm::Message& message, INamedPipeCallback* context) {
//   Server *server = dynamic_cast<Server*>(context);

//   if (server != nullptr) {
//     server->OnMessage(message);
//   } else {
//     std::cout << "context is invalid" << std::endl;
//   }
// }

bool Server::Status = true;

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
  for(std::map<std::string, IServerDataObject*>::iterator i = this->data.begin(); i != this->data.end(); i++) {
    delete i->second;
  }
}

void Server::Start() {
  this->named_pipe.AddConnectCallback(std::cref(on_connected), this);
  // this->named_pipe.AddMessageCallback(std::cref(on_message), this);
  this->named_pipe.Start();
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

  if (this->named_pipe.Write(message)) {
    std::cout << "Response sent" << std::endl;
    ReadNextMessage();
  } else {
    std::cout << "Unable to send response: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
  }
}

void Server::ReadNextMessage() {
  if (Config::Async) {
    this->named_pipe.ReadAsync(this, nullptr);
  } else {
    StreamComm::Message message;
    if(this->named_pipe.Read(&message)) {
      OnMessage(message);
    } else {
      std::cerr << "Error reading message" << std::endl;
    }
  }
}

void Server::OnConnected() {
  std::cout << "Client connected! Sending greeting" << std::endl;
  StreamComm::Message message;

  message.header.message_command = StreamComm::kCommandGreeting;
  message.header.data_size = 0;
  memset(message.data, 0, MAX_DATA_SIZE);

  if (this->named_pipe.Write(message)) {
    ReadNextMessage();
    std::cout << "Message sent successfully" << std::endl;
  } else {
    std::cerr << "Error sending greeting" << std::endl;
  }
}

void Server::OnWrite(bool success, StreamComm::Message message, void *data) {

}

void Server::OnRead(bool success, StreamComm::Message message, void *data) {
  if (success) {
    OnMessage(message);
  }
}