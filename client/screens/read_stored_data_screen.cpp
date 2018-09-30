#include "read_stored_data_screen.h"
#include "screen_data.h"

#include "comm.h"
#include "named_pipe_io.h"
#include "windows_helper.h"

#include <iostream>
#include <string>

ReadStoredDataScreen::ReadStoredDataScreen() {

}

ReadStoredDataScreen::~ReadStoredDataScreen() {

}

void ReadStoredDataScreen::OnRead(bool success, StreamComm::Message message, void *data) {

}

void ReadStoredDataScreen::OnWrite(bool success, StreamComm::Message message, void *data) {

}

Screen ReadStoredDataScreen::Show() {
  Screen next_screen = kScreenHome;
  std::string data_name;
  bool ok = true;

  do {
    ok = true;

    std::cout << "Type in the name of the data you want to read (max " << MAX_NAME_CHARS << " chars.)" << std::endl;
    std::cin >> data_name;

    if (data_name.length() > MAX_NAME_CHARS) {
      std::cout << "Invalid data size" << std::endl;
      ok = false;
    } 
  } while(!ok);

  this->Send(data_name);

  return next_screen;
}

void ReadStoredDataScreen::Send(std::string name) {
  HANDLE handle = ScreenData::GetHandle();

  StreamComm::ReadStoredDataCommand read_command = { 0 };
  memcpy(read_command.data_name, name.c_str(), name.length());

  StreamComm::Message message;
  message.header.message_command = StreamComm::kCommandReadStoredData;
  message.header.data_size = sizeof(StreamComm::ReadStoredDataCommand);
  memset(message.data, 0, MAX_DATA_SIZE);
  memcpy(message.data, &read_command, sizeof(StreamComm::ReadStoredDataCommand));

  if (StreamComm::NamedPipeIO::Write(handle, message)) {
    std::cout << "Message sent. Waiting for response." << std::endl;
    StreamComm::Message response_message;

    if (StreamComm::NamedPipeIO::Read(handle, &response_message)) {
      if (response_message.header.message_command == StreamComm::kCommandResponse) {
        std::cout << "Response received. Verifying Status..." << std::endl;
        StreamComm::ResponseCommand *response = nullptr;
        response = (StreamComm::ResponseCommand*)response_message.data;

        if (response->status) {
          std::cout << "Stored value: " << response->message << std::endl;
        } else {
          std::cout << "Read stored command failed with response: " << response->message << std::endl;
        }
      } else {
        std::cout << "Invalid Response received: " << response_message.header.message_command << std::endl;
      }
    } else {
      std::cout << "Unable to read data: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
    }
  } else {
    std::cout << "Unable to send data: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
  }
}