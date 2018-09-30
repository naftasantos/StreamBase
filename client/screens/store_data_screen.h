#pragma once

#include "screen_interface.h"
#include "named_pipe_io.h"

#include <string>

class StoreDataScreen : public IScreen {
  public:
    StoreDataScreen();
    virtual ~StoreDataScreen();
    virtual Screen Show();

    virtual void OnRead(bool success, StreamComm::Message message, void *data);
    virtual void OnWrite(bool success, StreamComm::Message message, void *data);
  private:
    StoreDataScreen(const StoreDataScreen&) {}
    void Send(std::string data_name, std::string data_value);

    bool finished;
    Screen next_screen;
};