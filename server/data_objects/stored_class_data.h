#pragma once

#include "data_object_interface.h"
#include "custom_classes/custom_class_interface.h"

#include <string>

class StoredClassData : public IServerDataObject {
  public:
    StoredClassData();
    virtual ~StoredClassData();

    bool Store(std::string class_name);
    bool Execute(const std::string &name,
                 const std::vector<std::string> &params,
                 std::string &output);
  private:
    CustomClass::ICustomClass *stored_object;

    StoredClassData(const StoredClassData& other) {}
};