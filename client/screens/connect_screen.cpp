#include "connect_screen.h"
#include "windows_helper.h"
#include "comm.h"
#include "named_pipe_io.h"
#include "screen_data.h"
#include "config.h"

#include <iostream>

ConnectScreen::ConnectScreen()
  : finished(false),
    next_screen(kScreenNone) {

}

ConnectScreen::~ConnectScreen() {

}

void ConnectScreen::OnWrite(bool success, StreamComm::Message message, void *data) {

}

void ConnectScreen::OnRead(bool success, StreamComm::Message message, void *data) {
  if (success) {
    ReceivedMessage(message);
  } else {
    std::cerr << "Message received with error: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
  }

  this->finished = true;
}

void ConnectScreen::ReceivedMessage(StreamComm::Message& message) {
  switch(message.header.message_command) {
    case StreamComm::kCommandGreeting:
      std::cout << "Greeting command received" << std::endl;
      next_screen = kScreenHome;
      break;
    default:
      std::cerr << "Invalid command received" << std::endl;
      break;
  }
}

Screen ConnectScreen::Show() {
  HANDLE handle;
  DWORD mode = PIPE_READMODE_MESSAGE;
  DWORD file_flags = 0;

  if (Config::Async) {
    std::cout << "Async mode" << std::endl;
    file_flags |= FILE_FLAG_OVERLAPPED;
  }

  StreamComm::NamedPipeIO::SetAsync(Config::Async);

  std::cout << "Connecting to Pipe" << std::endl;
  handle = CreateFile(TEXT(PIPE_NAME),
                      GENERIC_READ | GENERIC_WRITE,
                      0,
                      NULL,
                      OPEN_EXISTING,
                      file_flags,
                      NULL);

  if (handle == INVALID_HANDLE_VALUE) {
    std::cerr << "Error connecting to pipe: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
  } else {
    std::cout << "Connected!" << std::endl;
    ScreenData::SetHandle(handle);

    if (SetNamedPipeHandleState(handle, &mode, NULL, NULL)) {
      if (!StreamComm::NamedPipeIO::Read(handle, this, nullptr)) {
        std::cerr << "Error reading from pipe: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
        this->finished = false;
      }
    } else {
      std::cerr << "Error setting named pipe mode: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
      this->finished = true;
    }
  }

  if (Config::Async) {
    do {
      SleepEx(1000, TRUE);
    } while(!this->finished);
  }

  return next_screen;
}