#pragma once

#include "custom_class_interface.h"

#include <vector>
#include <string>

namespace CustomClass {
class Runner : public ICustomClass {
  public:
    Runner();
    virtual ~Runner();

    static bool GetMethods(std::vector<std::string>& methods);
    static bool GetParams(const std::string& name, std::vector<std::string>& params);

    virtual bool Execute(const std::string &name,
                         const std::vector<std::string> &params,
                         std::string &output);
  private:
    Runner(const Runner&) {}

    bool Run(const std::vector<std::string> &params, std::string &output);
};
};