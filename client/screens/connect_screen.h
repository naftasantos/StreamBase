#pragma once

#include "screen_interface.h"

class ConnectScreen : public IScreen {
  public:
    ConnectScreen();
    virtual ~ConnectScreen();
    virtual Screen Show();

  private:
    ConnectScreen(const ConnectScreen& other) {}
};