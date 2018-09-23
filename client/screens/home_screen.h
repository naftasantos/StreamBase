#pragma once

#include "screen_interface.h"

class HomeScreen : public IScreen {
  public:
    HomeScreen();
    virtual ~HomeScreen();
    virtual Screen Show();
  private:
    HomeScreen(const HomeScreen&) {}

    bool NextScreenFromOption(int option, Screen& next);
};