#pragma once

#include "screen_interface.h"

class HomeScreen : public IScreen {
  public:
    HomeScreen();
    virtual ~HomeScreen();
    virtual Screen Show();
    virtual void OnRead(bool success, StreamComm::Message message, void *data);
    virtual void OnWrite(bool success, StreamComm::Message message, void *data);
  private:
    HomeScreen(const HomeScreen&) {}

    bool NextScreenFromOption(int option, Screen& next);
};