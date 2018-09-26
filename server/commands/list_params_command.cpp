#include "list_params_command.h"
#include "custom_classes/custom_class_factory.h"

#include <sstream>
#include <string>
#include <iostream>

ListParamsCommand::ListParamsCommand() {

}

ListParamsCommand::~ListParamsCommand() {

}

StreamComm::ResponseCommand ListParamsCommand::Execute(StreamComm::Message& message,
                                                       std::map<std::string, IServerDataObject*>& data) {
  StreamComm::ResponseCommand response = { 0 };
  StreamComm::ListParamsCommand* command = nullptr;

  std::cout << "ListParamsCommand!" << std::endl;

  if (message.header.message_command == StreamComm::kCommandListParams) {
    command = (StreamComm::ListParamsCommand*)message.data;
    std::string class_name(command->class_name);
    std::string method_name(command->method_name);
    std::vector<std::string> params;

    std::cout << "Getting params for " << class_name << "::" << method_name << std::endl;
    if (CustomClass::Factory::GetMethodParams(class_name, method_name, params)) {
      std::stringstream stream;
      std::string result;
      size_t len;

      for(auto param : params) {
        stream << param << ";";
      }

      result = stream.str();
      len = result.length();

      if (len > 100) {
        // IMPROVEMENT: Dynamic resize of message buffer, or multiple messages. For now I'll just not support.
        response.status = false;
        memcpy(response.message, "The protocol still does not support this many params", 52);
      } else {
        response.status = true;
        memcpy(response.message, result.c_str(), len * sizeof(char));
      }
    } else {
      response.status = false;
      memcpy(response.message, "unable to load params", 21);
    }
  } else {
    response.status = false;
    memcpy(response.message, "Invalid command", 15);
  }

  return response;
}