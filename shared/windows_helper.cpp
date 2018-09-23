#include "windows_helper.h"

std::string Helper::WindowsHelper::GetLastErrorMessage() {
  std::string message;
  DWORD error = GetLastError();
  LPSTR messageBuffer = NULL;

  size_t size = FormatMessageA(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    error,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPSTR)&messageBuffer,
    0,
    NULL);

  message = std::string(messageBuffer, size);

  LocalFree(messageBuffer);

  return message;
}