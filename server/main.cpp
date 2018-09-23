#include <iostream>

#include "server.h"
#include "comm.h"

void on_message(Message& message, void* context) {
  std::cout << "Received callback" << std::endl;
}

int main(int argc, char** argv) {
  NamedPipeServer server;

  server.AddMessageCallback(std::cref(on_message), nullptr);
  server.Start();

  return EXIT_SUCCESS;
}