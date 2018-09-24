#pragma once

#include <windows.h>

#define MAX_NAME_CHARS 10
#define MAX_VALUE_CHARS 100

class ScreenData {
  public:
    static void SetHandle(HANDLE handle);
    static HANDLE GetHandle();
    static void Init();
    static void Finish();
  private:
    ScreenData() {}
    ScreenData(ScreenData&) {}
    virtual ~ScreenData() {}

    static HANDLE handle;
};