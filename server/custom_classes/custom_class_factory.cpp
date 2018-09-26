#include "custom_class_factory.h"
#include "custom_class_runner.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>

#define RUNNER_CUSTOM_CLASS_RUNNER_NAME "runner"

const std::vector<std::string> CustomClass::Factory::AvailableClasses = {
  RUNNER_CUSTOM_CLASS_RUNNER_NAME
};

bool CustomClass::Factory::GetAvailableMethods(const std::string& class_name, std::vector<std::string>& result) {
  bool ok = true;

  if (class_name == RUNNER_CUSTOM_CLASS_RUNNER_NAME) {
    ok = CustomClass::Runner::GetMethods(result);
  } else {
    ok = false;
  }

  return ok;
}

bool CustomClass::Factory::GetMethodParams(const std::string& class_name,
                                           const std::string& method,
                                           std::vector<std::string>& result) {
  bool ok = true;

  if (class_name == RUNNER_CUSTOM_CLASS_RUNNER_NAME) {
    ok = CustomClass::Runner::GetParams(method, result);
  } else {
    ok = false;
  }

  return ok;
}

bool CustomClass::Factory::CreateObject(const std::string& class_name,
                                        CustomClass::ICustomClass **custom_object) {
  bool ok = true;

  std::cout << "CustomClass::Factory::CreateObject" << std::endl;
  if (custom_object == nullptr) {
    std::cout << "Invalid parameter" << std::endl;
    ok = false;
  } else {
    if (class_name == RUNNER_CUSTOM_CLASS_RUNNER_NAME) {
      std::cout << "Creating instance of runner" << std::endl;
      (*custom_object) = new CustomClass::Runner();
      ok = true;
    } else {
      std::cout << "No class found with name '" << class_name << "'" << std::endl;
      ok = false;
    }
  }

  return ok;
}
