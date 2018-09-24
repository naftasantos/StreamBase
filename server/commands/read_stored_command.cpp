#include "read_stored_command.h"
#include "data_objects/storage_data.h"

#include <iostream>

ReadStoredCommand::ReadStoredCommand() {

}

ReadStoredCommand::~ReadStoredCommand() {

}

StreamComm::ResponseCommand ReadStoredCommand::Execute(StreamComm::Message& message,
                                                       std::map<std::string,
                                                       IServerDataObject*>& data) {
  StreamComm::ResponseCommand response = { 0 };
  StreamComm::ReadStoredDataCommand *command = nullptr;

  std::cout << "Read stored data command" << std::endl;
  command = (StreamComm::ReadStoredDataCommand*) message.data;
  std::cout << "Reading stored data '" << command->data_name << std::endl;

  std::string name(command->data_name);

  if (name.length() > 10) {
    response.status = false;
    memcpy(response.message, "data name size is greater than 10", 33);
    std::cout << "Invalid name" << std::endl;
  } else {
    std::string key_name("storage");

    if (data.find(key_name) == data.end()) {
      response.status = false;
      memcpy(response.message, "nothing was stored yet", 22);
      std::cout << "No storage" << std::endl;
    } else {
      StorageData *storage_data = dynamic_cast<StorageData*>(data.at(key_name));

      if (storage_data != nullptr) {
        std::cout << "Storage retrieved" << std::endl;
        std::string value;

        if (storage_data->Read(name, &value)) {
          response.status = true;
          memcpy(response.message, value.c_str(), value.size());
        } else {
          response.status = false;
          memcpy(response.message, "there is no data stored with the given name", 43);
        }
      } else {
        response.status = false;
        memcpy(response.message, "unable to open storage", 22);
      }
    }
  }

  return response;
}