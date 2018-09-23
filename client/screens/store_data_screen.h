#pragma once

#include "screen_interface.h"

#include <string>

class StoreDataScreen : public IScreen {
  public:
    StoreDataScreen();
    virtual ~StoreDataScreen();
    virtual Screen Show();
  private:
    StoreDataScreen(const StoreDataScreen&);
};