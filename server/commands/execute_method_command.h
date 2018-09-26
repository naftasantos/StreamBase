#pragma once

#include "command_interface.h"

class ExecuteMethodCommand : public ICommand {
  public:
    ExecuteMethodCommand();
    virtual ~ExecuteMethodCommand();

    virtual StreamComm::ResponseCommand Execute(StreamComm::Message& message,
                                                std::map<std::string, IServerDataObject*>& data);
  private:
    ExecuteMethodCommand(ExecuteMethodCommand &) {}
};