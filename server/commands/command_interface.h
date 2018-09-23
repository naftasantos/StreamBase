#pragma once

#include <string>
#include <map>
#include <vector>

#include "comm.h"
#include "data_objects/data_object_interface.h"

class ICommand {
  public:
    virtual StreamComm::ResponseCommand Execute(StreamComm::Message& message,
                                                std::map<std::string, IServerDataObject*>& data) = 0;
};