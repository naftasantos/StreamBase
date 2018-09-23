#include "store_data_screen.h"
#include "screen_data.h"
#include "named_pipe_io.h"
#include "comm.h"
#include "windows_helper.h"

#include <iostream>
#include <string>

#define MAX_NAME_CHARS 10
#define MAX_VALUE_CHARS 100

StoreDataScreen::StoreDataScreen() {

}

StoreDataScreen::~StoreDataScreen() {

}

Screen StoreDataScreen::Show() {
  Screen next_screen = kScreenHome;
  std::string data_name;
  std::string data_value;

  bool ok = true;

  do {
    std::cout << "Type in the name of the data you want to store (max " << MAX_NAME_CHARS << " chars.)" << std::endl;
    std::cin >> data_name;

    if (data_name.length() > MAX_NAME_CHARS) {
      std::cout << "Invalid data size" << std::endl;
      ok = false;
    } 
  } while(!ok);

  ok = true;
  std::cin.ignore(INT_MAX, '\n');
  std::cin.clear();

  do {
    std::cout << "Type in the value you want to store (max " << MAX_VALUE_CHARS << " chars.)" << std::endl;
    std::getline(std::cin, data_value);

    if (data_value.length() > MAX_VALUE_CHARS) {
      ok = false;
    }
  } while(!ok);

  std::cout << "Sending data '" << data_name << "' with the value '" << data_value << "' to the server" << std::endl;

  this->Send(data_name, data_value);

  return next_screen;
}

void StoreDataScreen::Send(std::string data_name, std::string data_value) {
  HANDLE handle = ScreenData::GetHandle();

  StreamComm::StoreDataCommand store = { 0 };
  memcpy(store.data_name, data_name.c_str(), data_name.length());
  memcpy(store.data_value, data_value.c_str(), data_value.length());

  StreamComm::Message message;
  message.header.message_command = StreamComm::kCommandStoreData;
  message.header.data_size = sizeof(StreamComm::StoreDataCommand);
  memset(message.data, 0, MAX_DATA_SIZE);
  memcpy(message.data, &store, sizeof(StreamComm::StoreDataCommand));

  if (StreamComm::NamedPipeIO::Write(handle, message)) {
    std::cout << "Message sent. Waiting for response." << std::endl;
    StreamComm::Message response_message;
    StreamComm::NamedPipeIO::Read(handle, &response_message);

    if (response_message.header.message_command == StreamComm::kCommandResponse) {
      std::cout << "Response received. Verifying Status..." << std::endl;
      StreamComm::ResponseCommand *response = nullptr;
      response = (StreamComm::ResponseCommand*)message.data;
      
      if (response->status) {
        std::cout << "Storage command completed successfully!" << std::endl;
      } else {
        std::cout << "Storage command faile with response: " << response->message << std::endl;
      }
    } else {
      std::cout << "Invalid Response received: " << message.header.message_command << std::endl;
    }
  } else {
    std::cout << "Unable to send data: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
  }
}