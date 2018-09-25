#pragma once

#include <vector>
#include <string>
#include <map>

namespace CustomClass{
class Factory {
  public:
    static const std::vector<std::string> AvailableClasses;

    static bool GetAvailableMethods(std::string class_name, std::vector<std::string> *result);
    static bool GetMethodParams(std::string class_name,
                                std::string method,
                                std::vector<std::string> *result);
  private:
    Factory() {}
    virtual ~Factory() {}
    Factory(const Factory&) {}
};
};