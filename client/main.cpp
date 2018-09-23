#include <windows.h>
#include <iostream>

#include "windows_helper.h"
#include "comm.h"

int main(int argc, char** argv) {
  HANDLE handle;
  // DWORD bytesWritten;
  StreamComm::Message message;

  // std::string message("Sending message over pipe\n");

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

    if (ReadFile(handle, &message, sizeof(StreamComm::Message), NULL, nullptr)) {
      std::cout << "Data received, reading it..." << std::endl;

      switch(message.header.message_command) {
        case StreamComm::kCommandGreeting:
          std::cout << "Greeting command received" << std::endl;
          break;
        default:
          std::cerr << "Invalid command received" << std::endl;
          break;
      }
    } else {
      std::cerr << "Error reading from pipe: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
    }

    // WriteFile(handle,
    //           message.c_str(),
    //           (DWORD)(message.length() + 1) * sizeof(char),
    //           &bytesWritten,
    //           NULL);
    // std::cout << "Info sent. Disconnecting..." << std::endl;
    CloseHandle(handle);
  }

  return EXIT_SUCCESS;
}