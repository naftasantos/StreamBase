#pragma once

#include "command_interface.h"
#include <vector>

class ReadStoredCommand : public ICommand {
  public:
    ReadStoredCommand();
    virtual ~ReadStoredCommand();

    virtual StreamComm::ResponseCommand Execute(StreamComm::Message& message,
                                                std::map<std::string, IServerDataObject*>& data);
  private:
    ReadStoredCommand(ReadStoredCommand &) {}
};