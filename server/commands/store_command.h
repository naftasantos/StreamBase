#pragma once

#include "command_interface.h"
#include <vector>

class StoreCommand : public ICommand {
  public:
    StoreCommand();
    virtual ~StoreCommand();

    virtual StreamComm::ResponseCommand Execute(StreamComm::Message& message,
                                                std::map<std::string, IServerDataObject*>& data);
  private:
    StoreCommand(StoreCommand &) {}
};