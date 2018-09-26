#pragma once

#include "custom_class_interface.h"

#include <vector>
#include <string>
#include <map>

namespace CustomClass{
class Factory {
  public:
    static const std::vector<std::string> AvailableClasses;

    static bool GetAvailableMethods(const std::string& class_name, std::vector<std::string>& result);
    static bool GetMethodParams(const std::string& class_name,
                                const std::string& method,
                                std::vector<std::string>& result);
    static bool CreateObject(const std::string& class_name,
                             CustomClass::ICustomClass **custom_object);
  private:
    Factory() {}
    virtual ~Factory() {}
    Factory(const Factory&) {}
};
};