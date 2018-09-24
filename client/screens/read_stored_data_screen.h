#pragma once

#include "screen_interface.h"

#include <string>

class ReadStoredDataScreen : public IScreen {
  public:
    ReadStoredDataScreen();
    virtual ~ReadStoredDataScreen();

    virtual Screen Show();
  private:
    ReadStoredDataScreen(const ReadStoredDataScreen&) {}

    void Send(std::string data_name);
};