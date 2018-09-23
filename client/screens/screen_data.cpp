#include "screen_data.h"

HANDLE ScreenData::handle = INVALID_HANDLE_VALUE;

void ScreenData::Init() {
  ScreenData::handle = INVALID_HANDLE_VALUE;
}

void ScreenData::SetHandle(HANDLE handle) {
  ScreenData::handle = handle;
}

HANDLE ScreenData::GetHandle() {
  return ScreenData::handle;
}

void ScreenData::Finish() {
  if (ScreenData::handle != INVALID_HANDLE_VALUE) {
    CloseHandle(ScreenData::handle);
    ScreenData::handle = INVALID_HANDLE_VALUE;
  }
}