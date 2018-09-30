#include "store_classes_screen.h"
#include "screen_data.h"
#include "named_pipe_io.h"
#include "windows_helper.h"
#include "config.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

StoreClassesScreen::StoreClassesScreen()
  : finished(false),
    next_screen(kScreenHome) {

}

StoreClassesScreen::~StoreClassesScreen() {

}

void StoreClassesScreen::OnRead(bool success, StreamComm::Message message, void *data) {
  if (success) {
    if (message.header.message_command == StreamComm::kCommandResponse) {
      StreamComm::ResponseCommand response = { 0 };
      memcpy(&response, message.data, sizeof(StreamComm::ResponseCommand));

      if (response.status) {
        switch(this->state) {
          case kStateListClass:
            ListClassesFinished(response);
          break;
          case kStateStoreClass:
            StoreClassFinished(response);
          break;
          case kStateListParams:
            ListParamsFinished(response);
          break;
          case kStateCallMethod:
            CallMethodFinished(response);
          break;
        }
      } else {
        std::cerr << "Command failed with message: " << response.message << std::endl;
        this->finished = true;
      }
    } else {
      std::cerr << "Invalid response received: " << message.header.message_command << std::endl;
      this->finished = true;
    }
  } else {
    std::cerr << "Error Reading data" << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
    this->finished = true;
  }
}

void StoreClassesScreen::OnWrite(bool success, StreamComm::Message message, void *data) {
  if (success) {
    if (!StreamComm::NamedPipeIO::Read(ScreenData::GetHandle(), this, nullptr)) {
      std::cerr << "Unable to read response: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
      this->finished = true;
    }
  } else {
    std::cerr << "Error Writing data" << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
    this->finished = true;
  }
}

void StoreClassesScreen::ListParamsFinished(StreamComm::ResponseCommand response) {
  std::istringstream params_stream(response.message);
  std::stringstream param_values;
  std::string param_name;

  while(std::getline(params_stream, param_name, ';')) {
    std::string param_value;
    std::cout << "Type in the value for the param: " << param_name << std::endl;
    std::cin >> param_value;
    param_values << param_value << ";";
  }

  std::cout << "Executing " << this->chosen_method << std::endl;

  if (!ExecuteMethod(this->chosen_method, param_values.str(), response)) {
    std::cerr << "Unable to execute method" << std::endl;
    this->finished = true;
  }
}

void StoreClassesScreen::CallMethodFinished(StreamComm::ResponseCommand response) {
  std::cout << "Call Method executed with message: " << response.message << std::endl;
  this->finished = true;
}

void StoreClassesScreen::StoreClassFinished(StreamComm::ResponseCommand response) {
  std::cout << "Class stored!" << std::endl;
  if (!CallMethod(this->chosen_class, response)) {
    std::cerr << "Unable to call method" << std::endl;
    this->finished = true;
  }
}

void StoreClassesScreen::ListClassesFinished(StreamComm::ResponseCommand response) {
  this->chosen_class = ChooseClass(std::string(response.message));
  std::cout << "You chose " << this->chosen_class << std::endl;

  this->state = kStateStoreClass;
  if (!StoreClass(this->chosen_class, response)) {
    std::cerr << "Unable to store class" << std::endl;
    this->finished = true;
  }
}

Screen StoreClassesScreen::Show() {
  std::cout << "Listing classes" << std::endl;
  if (!ListClasses()) {
    std::cerr << "Unable to list classes" << std::endl;
  }

  if (Config::Async) {
    do {
      SleepEx(1000, TRUE);
    } while(!this->finished);
  }

  return next_screen;
}

bool StoreClassesScreen::ListClasses() {
  bool ok = true;

  HANDLE handle = ScreenData::GetHandle();
  StreamComm::Message message;
  message.header.message_command = StreamComm::kCommandListClasses;
  message.header.data_size = 0;
  memset(message.data, 0, MAX_DATA_SIZE);

  this->state = kStateListClass;

  if (!StreamComm::NamedPipeIO::Write(handle, message, this, nullptr)) {
    std::cerr << "Error writing message: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
    this->finished = true;
    ok = false;
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
  bool ok = true;

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

    if (!StreamComm::NamedPipeIO::Write(handle, message, this, nullptr)) {
      std::cerr << "Error writing message: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
      this->finished = true;
      ok = false;
    }
  }

  return ok;
}

bool StoreClassesScreen::CallMethod(std::string class_name, StreamComm::ResponseCommand& response) {
  bool ok = true;

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

    do {
      std::cin >> choice;

      if (choice >= 0 && choice < count) {
        valid = true;
      } else {
        std::cerr << "Invalid choice" << std::endl;
      }
    } while(!valid);
    std::cout << "Getting chosen method: " << std::endl;
    this->chosen_method = method_names.at(choice);
    std::cout << "Method chosen: " << this->chosen_method << std::endl;

    if (!ListParams(class_name, chosen_method, response)) {
      std::cerr << "Params listing failed with message: " << response.message << std::endl;
      ok = false;
    }
  }

  return ok;
}

bool StoreClassesScreen::ListParams(std::string class_name,
                                    std::string method_name,
                                    StreamComm::ResponseCommand& response) {
  bool ok = true;

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

    this->state = kStateListParams;
    if (!StreamComm::NamedPipeIO::Write(handle, message, this, nullptr)) {
      std::cerr << "Error writing message: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
      ok = false;
      this->finished = false;
    }
  }

  return ok;
}

bool StoreClassesScreen::ExecuteMethod(std::string method_name,
                                       std::string params,
                                       StreamComm::ResponseCommand& response) {
  bool ok = true;

  HANDLE handle = ScreenData::GetHandle();
  StreamComm::Message message;
  StreamComm::ExecuteMethodCommand command = { 0 };
  message.header.message_command = StreamComm::kCommandRunMethod;
  message.header.data_size = sizeof(StreamComm::ExecuteMethodCommand);
  memset(message.data, 0, MAX_DATA_SIZE);

  if (method_name.length() > 100) {
    ok = false;
  } else if (params.length() > 100) {
    ok = false;
  } else {
    memcpy(command.method_name, method_name.c_str(), method_name.length() * sizeof(char));
    memcpy(command.params, params.c_str(), params.length() * sizeof(char));
    memcpy(message.data, &command, sizeof(StreamComm::ExecuteMethodCommand));

    this->state = kStateCallMethod;
    if (!StreamComm::NamedPipeIO::Write(handle, message, this, nullptr)) {
      std::cerr << "Error writing message: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
      this->finished = true;
      ok = false;
    }
  }

  return ok;
}