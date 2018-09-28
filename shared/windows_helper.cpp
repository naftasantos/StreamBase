#include "windows_helper.h"

#include <sstream>

std::string Helper::WindowsHelper::GetLastErrorMessage() {
  DWORD error = GetLastError();
  LPSTR messageBuffer = NULL;
  std::stringstream sstream;

  size_t size = FormatMessageA(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    error,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPSTR)&messageBuffer,
    0,
    NULL);

  sstream << "[" << error << "] - " << std::string(messageBuffer, size);

  LocalFree(messageBuffer);

  return sstream.str();
}