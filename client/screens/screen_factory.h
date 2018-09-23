#pragma once

#include "screen_interface.h"

class ScreenFactory {
  public:
    static IScreen* CreateScreen(Screen screen);
  private:
    ScreenFactory() {}
    ~ScreenFactory() {}
    ScreenFactory(const ScreenFactory&) {}
};