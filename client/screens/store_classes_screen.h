#pragma once

#include "screen_interface.h"
#include "comm.h"

#include <string>

class StoreClassesScreen : public IScreen {
  public:
    StoreClassesScreen();
    virtual ~StoreClassesScreen();
    virtual Screen Show();
  private:
    StoreClassesScreen(const StoreClassesScreen&) {}

    bool ListClasses(StreamComm::ResponseCommand& response);
    std::string ChooseClass(std::string classes);
    bool StoreClass(std::string class_name, StreamComm::ResponseCommand& response);
    bool CallMethod(std::string class_name, StreamComm::ResponseCommand& response);
    bool ListParams(std::string class_name, std::string method_name, StreamComm::ResponseCommand& response);
};