#include "connect_screen.h"
#include "windows_helper.h"
#include "comm.h"
#include "named_pipe_io.h"
#include "screen_data.h"

#include <iostream>

ConnectScreen::ConnectScreen() {

}

ConnectScreen::~ConnectScreen() {

}

Screen ConnectScreen::Show() {
  Screen next_screen = kScreenNone;
  HANDLE handle;
  StreamComm::Message message;
  DWORD mode = PIPE_READMODE_MESSAGE;

  std::cout << "Connecting to Pipe" << std::endl;
  handle = CreateFile(TEXT(PIPE_NAME),
                      GENERIC_READ | GENERIC_WRITE,
                      0,
                      NULL,
                      OPEN_EXISTING,
                      0,
                      NULL);

  if (handle == INVALID_HANDLE_VALUE) {
    std::cerr << "Error connecting to pipe: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
  } else {
    std::cout << "Connected!" << std::endl;
    ScreenData::SetHandle(handle);

    if (SetNamedPipeHandleState(handle, &mode, NULL, NULL)) {
      if (StreamComm::NamedPipeIO::Read(handle, &message)) {
        switch(message.header.message_command) {
          case StreamComm::kCommandGreeting:
            std::cout << "Greeting command received" << std::endl;
            next_screen = kScreenHome;
            break;
          default:
            std::cerr << "Invalid command received" << std::endl;
            break;
        }
      } else {
        std::cerr << "Error reading from pipe: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
      }
    } else {
      std::cerr << "Error setting named pipe mode: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
    }
  }

  return next_screen;
}