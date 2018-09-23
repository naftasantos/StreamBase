#pragma once

typedef enum {
  kScreenNone = 0,
  kScreenConnect,
  kScreenHome,
  kScreenStoreData
} Screen;

class IScreen {
  public:
    virtual ~IScreen() {};
    virtual Screen Show() = 0;
};