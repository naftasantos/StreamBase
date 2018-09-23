#include "storage_data.h"

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