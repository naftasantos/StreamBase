#include "store_classes_screen.h"
#include "screen_data.h"
#include "named_pipe_io.h"
#include "windows_helper.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

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
      std::string chosen_class = ChooseClass(std::string(response.message));
      std::cout << "You chose " << chosen_class << std::endl;
      if (StoreClass(chosen_class, response)) {
        if (response.status) {
          std::cout << "Class stored!" << std::endl;
          if (CallMethod(chosen_class, response)) {
            std::cout << "Command executed successfully" << std::endl;
          }
        } else {
          std::cerr << "Command to register class failed with message: " << response.message << std::endl;
        }
      } else {
        std::cerr << "Unable to store class" << std::endl;
      }
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

std::string StoreClassesScreen::ChooseClass(std::string classes) {
  std::istringstream stream(classes);
  std::string class_name;
  std::vector<std::string> classes_names;
  int count = 0;
  int choice = -1;
  bool valid = false;

  std::cout << "Choose a class" << std::endl;

  while(std::getline(stream, class_name, ';')) {
    std::cout << "[" << count++ << "]: " << class_name << std::endl;
    classes_names.push_back(class_name);
  }

  if (count == 0) {
    return std::string();
  }

  do {
    std::cin >> choice;

    if (choice >= 0 && choice < count) {
      valid = true;
    } else {
      std::cerr << "Invalid choice" << std::endl;
    }
  } while(!valid);

  return classes_names.at(choice);
}

bool StoreClassesScreen::StoreClass(std::string class_name, StreamComm::ResponseCommand& response) {
  bool ok = false;

  HANDLE handle = ScreenData::GetHandle();
  StreamComm::Message message;
  StreamComm::StoreClassCommand command = { 0 };
  message.header.message_command = StreamComm::kCommandStoreClass;
  message.header.data_size = sizeof(StreamComm::StoreClassCommand);
  memset(message.data, 0, MAX_DATA_SIZE);

  if (class_name.length() > 100) {
    ok = false;
    std::cerr << "Class name too big" << std::endl;
  } else {
    memcpy(command.class_name, class_name.c_str(), class_name.length() * sizeof(char));
    memcpy(message.data, &command, sizeof(StreamComm::StoreClassCommand));

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
  }

  return ok;
}

bool StoreClassesScreen::CallMethod(std::string class_name, StreamComm::ResponseCommand& response) {
  bool ok = false;

  std::istringstream methods(response.message);
  std::vector<std::string> method_names;
  std::string method_name;
  int count = 0;

  std::cout << "Choose a method to call" << std::endl;
  while(std::getline(methods, method_name, ';')) {
    std::cout << "[" << count++ << "]: " << method_name << std::endl;
    method_names.push_back(method_name);
  }

  if (count == 0) {
    ok = false;
  } else {
    bool valid = false;
    int choice;
    std::string chosen_method;

    do {
      std::cin >> choice;

      if (choice >= 0 && choice < count) {
        valid = true;
      } else {
        std::cerr << "Invalid choice" << std::endl;
      }
    } while(!valid);

    chosen_method = method_names.at(choice);
    std::cout << "Method chosen: " << chosen_method << std::endl;

    if (ListParams(class_name, chosen_method, response)) {
      if (response.status) {
        std::istringstream params_stream(response.message);
        std::stringstream param_values;
        std::string param_name;

        while(std::getline(params_stream, param_name, ';')) {
          std::string param_value;
          std::cout << "Type in the value for the param: " << param_name << std::endl;
          std::cin >> param_value;
          param_values << param_value << ";";
        }

        std::cout << "will execute " << chosen_method << " with params: " << param_values.str() << std::endl;
      } else {
        std::cerr << "Params listing failed with message: " << response.message << std::endl;
      }
    }
  }

  return ok;
}

bool StoreClassesScreen::ListParams(std::string class_name,
                                    std::string method_name,
                                    StreamComm::ResponseCommand& response) {
  bool ok = false;

  HANDLE handle = ScreenData::GetHandle();
  StreamComm::Message message;
  StreamComm::ListParamsCommand command = { 0 };
  message.header.message_command = StreamComm::kCommandListParams;
  message.header.data_size = sizeof(StreamComm::ListParamsCommand);
  memset(message.data, 0, MAX_DATA_SIZE);

  if (class_name.length() > 100) {
    ok = false;
  } else if (method_name.length() > 100) {
    ok = false;
  } else {
    memcpy(command.class_name, class_name.c_str(), class_name.length() * sizeof(char));
    memcpy(command.method_name, method_name.c_str(), method_name.length() * sizeof(char));
    memcpy(message.data, &command, sizeof(StreamComm::ListParamsCommand));

    std::cout << "Class name: " << command.class_name << std::endl;

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
  }

  return ok;
}