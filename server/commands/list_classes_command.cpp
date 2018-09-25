#include "list_classes_command.h"
#include "custom_classes/custom_class_factory.h"

#include <sstream>
#include <string>
#include <iostream>

ListClassesCommand::ListClassesCommand() {

}

ListClassesCommand::~ListClassesCommand() {

}

StreamComm::ResponseCommand ListClassesCommand::Execute(StreamComm::Message& message,
                                                       std::map<std::string,
                                                       IServerDataObject*>& data) {
  StreamComm::ResponseCommand response = { 0 };

  if (message.header.message_command == StreamComm::kCommandListClasses) {
    std::stringstream stream;
    std::string result;
    size_t len;

    for(auto class_name : CustomClass::Factory::AvailableClasses) {
      stream << class_name << ";";
    }

    result = stream.str();
    len = result.length();

    if (len > 100) {
      // IMPROVEMENT: Dynamic resize of message buffer, or multiple messages. For now I'll just not support.
      response.status = false;
      memcpy(response.message, "The protocol still does not support this many classes", 53);
    } else if (len == 0) {
      response.status = false;
      memcpy(response.message, "No custom class implemented", 27);
    } else {
      response.status = true;
      memcpy(response.message, result.c_str(), len * sizeof(char));
    }
  } else {
    response.status = false;
    memcpy(response.message, "Invalid command", 15);
  }

  return response;
}