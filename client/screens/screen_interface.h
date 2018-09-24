#pragma once

typedef enum {
  kScreenNone = 0,
  kScreenConnect,
  kScreenHome,
  kScreenStoreData,
  kScreenReadStoredData
} Screen;

class IScreen {
  public:
    virtual ~IScreen() {};
    virtual Screen Show() = 0;
};