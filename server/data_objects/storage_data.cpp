#include "storage_data.h"

#include <string>

StorageData::StorageData() {

}

StorageData::StorageData(const StorageData& other)
  : StorageData() {
  this->SetStorage(other.GetStorage());
}

StorageData::~StorageData() {

}

const std::map<std::string, std::string>& StorageData::GetStorage() const {
  return this->storage;
}

void StorageData::SetStorage(const std::map<std::string, std::string>& data) {
  this->storage = data;
}

void StorageData::Store(std::string name, std::string value) {
  if (this->storage.find(name) == this->storage.end()) {
    this->storage.insert(make_pair(name, value));
  } else {
    this->storage[name] = value;
  }
}

bool StorageData::Read(std::string name, std::string *data) {
  bool ret = false;

  if (data != nullptr) {
    if (this->storage.find(name) != this->storage.end()) {
      (*data).assign(this->storage[name]);
      ret = true;
    }
  }

  return ret;
}