#pragma once

#include "data_object_interface.h"
#include <map>
#include <string>

class StorageData : public IServerDataObject {
  public:
    StorageData();
    virtual ~StorageData();

    void Store(std::string name, std::string value);
    bool Read(std::string name, std::string *value);

  private:
    StorageData(const StorageData& other) {}
    std::map<std::string, std::string> storage;

    const std::map<std::string, std::string>& GetStorage() const;
    void SetStorage(const std::map<std::string, std::string>& data);
};