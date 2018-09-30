#pragma once

#include "named_pipe_io.h"

typedef enum {
  kScreenNone = 0,
  kScreenConnect,
  kScreenHome,
  kScreenStoreData,
  kScreenReadStoredData,
  kScreenStoreClasses
} Screen;

class IScreen : public StreamComm::IStreamCallback {
  public:
    ~IScreen() {};
    virtual Screen Show() = 0;
    virtual void OnRead(bool success, StreamComm::Message message, void *data) = 0;
    virtual void OnWrite(bool success, StreamComm::Message message, void *data) = 0;
};