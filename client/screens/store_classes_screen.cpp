#include "store_classes_screen.h"
#include "screen_data.h"
#include "named_pipe_io.h"
#include "windows_helper.h"

#include <iostream>
#include <string>

StoreClassesScreen::StoreClassesScreen() {

}

StoreClassesScreen::~StoreClassesScreen() {

}

Screen StoreClassesScreen::Show() {
  Screen next_screen = kScreenHome;

  std::cout << "Listing classes" << std::endl;
  StreamComm::ResponseCommand response;

  if (ListClasses(response)) {
    if (response.status) {
      std::cout << "Command success" << std::endl;
    } else {
      std::cerr << "Command failed with message: " << response.message << std::endl;
    }
  } else {
    std::cerr << "Unable to list classes" << std::endl;
  }

  return next_screen;
}

bool StoreClassesScreen::ListClasses(StreamComm::ResponseCommand& response) {
  bool ok = false;

  HANDLE handle = ScreenData::GetHandle();
  StreamComm::Message message;
  message.header.message_command = StreamComm::kCommandListClasses;
  message.header.data_size = 0;
  memset(message.data, 0, MAX_DATA_SIZE);

  if (StreamComm::NamedPipeIO::Write(handle, message)) {
    StreamComm::Message response_message;

    if (StreamComm::NamedPipeIO::Read(handle, &response_message)) {
      if (response_message.header.message_command == StreamComm::kCommandResponse) {
        memcpy(&response, response_message.data, sizeof(StreamComm::ResponseCommand));
        ok = true;
      } else {
        std::cerr << "Invalid response received: " << response_message.header.message_command << std::endl;
      }
    } else {
      std::cerr << "Unable to read response: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
    }
  } else {
    std::cerr << "Error writing message: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
  }

  return ok;
}