#pragma once

#include <string>
#include <windows.h>

namespace Helper {
  class WindowsHelper {
    public:
      static std::string GetLastErrorMessage();
    private:
      WindowsHelper() {};
      WindowsHelper(const WindowsHelper &other) {};
      ~WindowsHelper() {};
  };
};