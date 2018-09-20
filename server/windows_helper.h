#pragma once

#include <string>
#include <windows.h>

class WindowsHelper {
  public:
    static std::string GetLastErrorMessage();
  private:
    WindowsHelper() {};
    WindowsHelper(const WindowsHelper &other) {};
    ~WindowsHelper() {};
};