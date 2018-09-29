#include <iostream>
#include <string>

#include "server.h"
#include "config.h"

void ReadConfig(int argc, char** argv) {
  if (argc > 1) {
    std::string first_arg = std::string(argv[1]);

    if (first_arg == "-async") {
      Config::Async = true;
    }
  }
}

int main(int argc, char** argv) {
  ReadConfig(argc, argv);

  Server server;

  server.Start();

  if (Config::Async) {
    do {
      SleepEx(1000, TRUE);
    } while(server.GetStatus());
  }

  return EXIT_SUCCESS;
}