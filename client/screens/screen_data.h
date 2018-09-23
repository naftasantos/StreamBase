#pragma once

#include <windows.h>

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