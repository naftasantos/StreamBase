#pragma once

#include "command_interface.h"
#include "comm.h"

class CommandFactory {
  public:
    static ICommand* GetCommand(StreamComm::Command command);
  private:
    CommandFactory() {}
    virtual ~CommandFactory() {}
    CommandFactory(const CommandFactory&) {}
};