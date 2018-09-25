#include "custom_class_factory.h"
#include "custom_class_runner.h"

#include <string>
#include <vector>
#include <map>

#define RUNNER_CUSTOM_CLASS_RUNNER_NAME "runner"

const std::vector<std::string> CustomClass::Factory::AvailableClasses = {
  RUNNER_CUSTOM_CLASS_RUNNER_NAME
};

bool CustomClass::Factory::GetAvailableMethods(std::string class_name, std::vector<std::string> *result) {
  bool ok = true;

  if (result == nullptr) {
    ok = false;
  } else {
    if (class_name == RUNNER_CUSTOM_CLASS_RUNNER_NAME) {
      ok = CustomClass::Runner::GetMethods(result);
    } else {
      ok = false;
    }
  }

  return ok;
}

bool CustomClass::Factory::GetMethodParams(std::string class_name,
                                           std::string method,
                                           std::vector<std::string> *result) {
  bool ok = true;

  if (result == nullptr) {
    ok = false;
  } else {
    if (class_name == RUNNER_CUSTOM_CLASS_RUNNER_NAME) {
      ok = CustomClass::Runner::GetParams(method, result);
    } else {
      ok = false;
    }
  }

  return ok;
}
