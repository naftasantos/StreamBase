#include "store_class_command.h"
#include "custom_classes/custom_class_factory.h"

#include "data_objects/stored_class_data.h"

#include <map>
#include <string>
#include <sstream>
#include <iostream>

StoreClassCommand::StoreClassCommand() {

}

StoreClassCommand::~StoreClassCommand() {

}

StreamComm::ResponseCommand StoreClassCommand::Execute(StreamComm::Message& message, 
                                                       std::map<std::string, IServerDataObject*>& data) {
  StreamComm::ResponseCommand response = { 0 };
  StreamComm::StoreClassCommand *command = nullptr;

  std::cout << "Store Class command" << std::endl;
  command = (StreamComm::StoreClassCommand*) message.data;
  std::cout << "Storing class '" << command->class_name << "'" << std::endl;

  std::string class_name(command->class_name);
  std::string key_name("stored_class");

  if (data.find(key_name) == data.end()) {
    std::cout << "No storage yet. Creating..." << std::endl;
    data.insert(std::make_pair(key_name, new StoredClassData()));
    std::cout << "Created!" << std::endl;
  }

  std::cout << "Retrieving class storage" << std::endl;
  StoredClassData *stored_class = dynamic_cast<StoredClassData*>(data.at(key_name));

  if (stored_class != nullptr) {
    std::cout << "Retrieved!" << std::endl;

    if (stored_class->Store(class_name)) {
      std::cout << "Class Stored!" << std::endl;
      std::vector<std::string> methods;

      std::cout << "Getting methods!" << std::endl;
      if (CustomClass::Factory::GetAvailableMethods(class_name, methods)) {
        std::stringstream methodsstream;
        std::string methods_joined;

        for(auto method_name : methods) {
          methodsstream << method_name << ";";
        }

        methods_joined = methodsstream.str();
        if (methods_joined.length() > 100) {
          response.status = false;
          memcpy(response.message, "the protocol still does not support this many items", 51);
        } else {
          response.status = true;
          memcpy(response.message, methods_joined.c_str(), methods_joined.length() * sizeof(char));
        }
      } else {
        response.status = false;
        memcpy(response.message, "unable to list methods", 22);
      }
    } else {
      response.status = false;
      memcpy(response.message, "unable to load store class", 26);
    }
  } else {
    response.status = false;
    memcpy(response.message, "unable to load class storage", 28);
  }

  return response;
}