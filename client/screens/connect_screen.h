#pragma once

#include "screen_interface.h"
#include "named_pipe_io.h"

class ConnectScreen : public IScreen {
  public:
    ConnectScreen();
    virtual ~ConnectScreen();
    virtual Screen Show();

    virtual void OnRead(bool success, StreamComm::Message message, void *data);
    virtual void OnWrite(bool success, StreamComm::Message message, void *data);
  private:
    ConnectScreen(const ConnectScreen& other) {}

    void ReceivedMessage(StreamComm::Message& message);

    bool finished;
    Screen next_screen;
};