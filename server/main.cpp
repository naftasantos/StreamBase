#include <iostream>
#include <string>

#include "server.h"
#include "config.h"

int main(int argc, char** argv) {
  Config::ReadConfig(argc, argv);

  Server server;

  server.Start();

  if (Config::Async) {
    do {
      SleepEx(1000, TRUE);
    } while(server.GetStatus());
  }

  return EXIT_SUCCESS;
}