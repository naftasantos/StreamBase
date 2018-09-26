#pragma once

#include <string>
#include <vector>

namespace CustomClass {
class ICustomClass {
  public:
    virtual bool Execute(const std::string &name,
                         const std::vector<std::string> &params,
                         std::string &output) = 0;
    virtual ~ICustomClass() {}
};
};