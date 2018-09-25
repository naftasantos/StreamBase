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
};