#include "execute_method_command.h"
#include "custom_classes/custom_class_factory.h"
#include "data_objects/stored_class_data.h"
#include "comm.h"

#include <string>
#include <iostream>
#include <sstream>

ExecuteMethodCommand::ExecuteMethodCommand() {

}

ExecuteMethodCommand::~ExecuteMethodCommand() {

}

StreamComm::ResponseCommand ExecuteMethodCommand::Execute(StreamComm::Message& message,
                                                          std::map<std::string,
                                                          IServerDataObject*>& data) {
  StreamComm::ResponseCommand response = { 0 };
  StreamComm::ExecuteMethodCommand *command = nullptr;

  if (message.header.message_command == StreamComm::kCommandRunMethod) {
    command = (StreamComm::ExecuteMethodCommand*)message.data;
    std::string key_name("stored_class");
    std::string method_name(command->method_name);
    std::string params(command->params);

    std::cout << "Calling method " << command->method_name << " with params " << command->params << std::endl;

    if (data.find(key_name) != data.end()) {
      StoredClassData* stored_class = dynamic_cast<StoredClassData*>(data.at(key_name));

      if (stored_class != nullptr) {
        std::vector<std::string> params_list;
        std::istringstream params_stream(params);
        std::string param_value;
        std::string output;

        while(std::getline(params_stream, param_value, ';')) {
          params_list.push_back(param_value);
        }

        response.status = stored_class->Execute(method_name, params_list, output);
        std::cout << "Executed with output: " << output << std::endl;

        if (output.length() <= 100) {
          response.status = true;
          memcpy(response.message, output.c_str(), output.length() * sizeof(char));
        }
      } else {
        response.status = false;
        memcpy(response.message, "unable to load stored class", 27);
      }
    } else {
      response.status = false;
      memcpy(response.message, "nothing stored yet", 18);
    }
  } else {
    response.status = false;
    memcpy(response.message, "Invalid command", 15);
  }

  return response;
}