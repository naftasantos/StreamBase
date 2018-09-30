#pragma once

#include "screen_interface.h"

#include <string>

class ReadStoredDataScreen : public IScreen {
  public:
    ReadStoredDataScreen();
    virtual ~ReadStoredDataScreen();

    virtual Screen Show();
    virtual void OnRead(bool success, StreamComm::Message message, void *data);
    virtual void OnWrite(bool success, StreamComm::Message message, void *data);
  private:
    ReadStoredDataScreen(const ReadStoredDataScreen&) {}

    void Send(std::string data_name);
    bool finished;
    Screen next_screen;
};