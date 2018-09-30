#include "config.h"

#include <string>

bool Config::Async = false;

void Config::ReadConfig(int argc, char** argv) {
  if (argc > 1) {
    std::string first_arg = std::string(argv[1]);

    if (first_arg == "-async") {
      Config::Async = true;
    }
  }
}