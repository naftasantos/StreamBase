#include <iostream>

#include "named_pipe_server.h"
#include "comm.h"

void on_message(StreamComm::Message& message, void* context) {
  std::cout << "Received callback" << std::endl;
}

void on_connected(void *context) {
  std::cout << "Client connected!" << std::endl;
}

int main(int argc, char** argv) {
  NamedPipeServer server;

  server.AddConnectCallback(std::cref(on_connected), nullptr);
  server.AddMessageCallback(std::cref(on_message), nullptr);
  server.Start();

  return EXIT_SUCCESS;
}