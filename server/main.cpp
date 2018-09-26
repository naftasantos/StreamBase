#include <iostream>

#include "server.h"
#include "config.h"

int main(int argc, char** argv) {
  Server server;

  server.Start();

  return EXIT_SUCCESS;
}