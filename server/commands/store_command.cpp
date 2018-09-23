#include "store_command.h"
#include "data_objects/storage_data.h"

#include <iostream>

StoreCommand::StoreCommand() {

}

StoreCommand::~StoreCommand() {

}

StreamComm::ResponseCommand StoreCommand::Execute(StreamComm::Message& message, std::map<std::string, IServerDataObject*>& data) {
  StreamComm::ResponseCommand response = { 0 };
  StreamComm::StoreDataCommand* command = nullptr;

  std::cout << "Store data command" << std::endl;
  command = (StreamComm::StoreDataCommand*)message.data;
  std::cout << "Storing data '" << command->data_name << "' with value '" << command->data_value << "'" << std::endl;
  
  std::string name(command->data_name);
  std::string value(command->data_value);

  if(name.length() > 10) {
    response.status = false;
    memcpy(response.message, "data name size is greater than 10", 33);
    std::cout << "Invalid name" << std::endl;
  } else if (name.length() > 100) {
    response.status = false;
    memcpy(response.message, "data value size is greater than 10", 34);
    std::cout << "Invalid value" << std::endl;
  } else {
    std::string key_name("storage");
    StorageData *storage_data = nullptr;

    if (data.find(key_name) == data.end()) {
      std::cout << "Creating storage..." << std::endl;
      data.insert(make_pair(key_name, new StorageData()));
    }

    storage_data = dynamic_cast<StorageData*>(data.at(key_name));

    if (storage_data != nullptr) {
      std::cout << "Storage retrieved" << std::endl;
      storage_data->Store(name, value);
      std::cout << "Stored!" << std::endl;

      response.status = true;
    } else {
      response.status = false;
      memcpy(response.message, "Unable to open storage", 22);
    }
  }

  return response;
}