#pragma once

#include "command_interface.h"

class StoreClassCommand : public ICommand {
  public:
    StoreClassCommand();
    virtual ~StoreClassCommand();
  
    virtual StreamComm::ResponseCommand Execute(StreamComm::Message& message,
                                                std::map<std::string, IServerDataObject*>& data);
  private:
    StoreClassCommand(const StoreClassCommand&) {}
};