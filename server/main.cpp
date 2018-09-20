#include <iostream>

#include "server.h"

int main(int argc, char** argv) {
  NamedPipeServer server;

  server.Start();

  return EXIT_SUCCESS;
}