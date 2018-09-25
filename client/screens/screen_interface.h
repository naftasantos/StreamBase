#pragma once

typedef enum {
  kScreenNone = 0,
  kScreenConnect,
  kScreenHome,
  kScreenStoreData,
  kScreenReadStoredData,
  kScreenStoreClasses
} Screen;

class IScreen {
  public:
    virtual ~IScreen() {};
    virtual Screen Show() = 0;
};