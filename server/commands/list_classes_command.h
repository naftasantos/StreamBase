#pragma once

#include "command_interface.h"

class ListClassesCommand : public ICommand {
  public:
    ListClassesCommand();
    virtual ~ListClassesCommand();

    virtual StreamComm::ResponseCommand Execute(StreamComm::Message& message,
                                                std::map<std::string, IServerDataObject*>& data);
  private:
    ListClassesCommand(ListClassesCommand &) {}
};