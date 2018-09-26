#include "stored_class_data.h"
#include "custom_classes/custom_class_factory.h"

#include <iostream>

StoredClassData::StoredClassData() {
  this->stored_object = nullptr;
}

StoredClassData::~StoredClassData() {
  if (this->stored_object != nullptr) {
    delete this->stored_object;
    this->stored_object = nullptr;
  }
}

bool StoredClassData::Store(std::string class_name) {
  bool ok = false;

  std::cout << "StoredClassData::Store" << std::endl;
  if (this->stored_object != nullptr) {
    std::cout << "Deleting stored object" << std::endl;
    delete this->stored_object;
    this->stored_object = nullptr;
  }

  std::cout << "Calling custom class factory" << std::endl;
  ok = CustomClass::Factory::CreateObject(class_name, &(this->stored_object));

  return ok;
}

bool StoredClassData::Execute(const std::string &name,
                              const std::vector<std::string> &params,
                              std::string &output) {
  bool ok = true;

  if (this->stored_object == nullptr) {
    ok = false;
    output.assign("no object assigned yet");
  } else {
    ok = this->stored_object->Execute(name, params, output);
  }

  return ok;
}
