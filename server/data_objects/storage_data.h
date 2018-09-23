#pragma once

#include "data_object_interface.h"
#include <map>
#include <string>

class StorageData : public IServerDataObject {
  public:
    StorageData();
    virtual ~StorageData();
    StorageData(const StorageData& other);

    void Store(std::string name, std::string value);

  private:
    std::map<std::string, std::string> storage;

    const std::map<std::string, std::string>& GetStorage() const;
    void SetStorage(const std::map<std::string, std::string>& data);
};