#include "custom_class_runner.h"

#include <string>
#include <sstream>

#define METHOD_RUN "run"
#define METHOD_RUN_PARAM_TIME "time"

CustomClass::Runner::Runner() {

}

CustomClass::Runner::~Runner() {

}

bool CustomClass::Runner::Execute(const std::string &name,
                                  const std::vector<std::string> &params,
                                  std::string *output) {
  if (name == METHOD_RUN) {
    return Run(params, output);
  } else {
    (*output).assign("function not found");
  }

  return false;
}

bool CustomClass::Runner::Run(const std::vector<std::string> &params, std::string *output) {
  bool ok = true;

  if (output = nullptr) {
    ok = false;
  } else if (params.size() < 1) {
    ok = false;
    (*output).assign("not enough arguments");
  } else {
    int param = 0;

    try {
      param = std::stoi(params.at(0));
    } catch(std::exception e) {
      ok = false;
      (*output) = "argument is not a valid number";
    }

    if (ok) {
      std::stringstream stream;
      stream << "the runner ran " << param << " meters.";
      (*output).assign(stream.str());
    }
  }

  return ok;
}

bool CustomClass::Runner::GetMethods(std::vector<std::string> *methods) {
  bool ok = true;

  if (methods == nullptr) {
    ok = false;
  } else {
    methods->push_back(METHOD_RUN);
  }

  return ok;
}

bool CustomClass::Runner::GetParams(std::string name, std::vector<std::string> *params) {
  bool ok = true;

  if (params == nullptr) {
    ok = false;
  } else {
    if (name == METHOD_RUN) {
      params->push_back(METHOD_RUN_PARAM_TIME);
    } else {
      ok = false;
    }
  }

  return ok;
}