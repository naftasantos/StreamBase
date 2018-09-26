#pragma once

#include "command_interface.h"

class ListParamsCommand : public ICommand {
  public:
    ListParamsCommand();
    virtual ~ListParamsCommand();

    virtual StreamComm::ResponseCommand Execute(StreamComm::Message& message,
                                                std::map<std::string, IServerDataObject*>& data);
  private:
    ListParamsCommand(ListParamsCommand &) {}
};