#pragma once

#include <windows.h>
#include "comm.h"

namespace StreamComm {
class IStreamCallback {
  public:
    virtual void OnRead(bool success, StreamComm::Message message, void *data) = 0;
    virtual void OnWrite(bool success, StreamComm::Message message, void *data) = 0;
    virtual ~IStreamCallback() {}
};

typedef struct {
  OVERLAPPED overlap;
  IStreamCallback *context;
  void *context_data;
  void *data;
} CallbackData;

class NamedPipeIO {
  public:
    static bool Read(HANDLE handle, IStreamCallback *context, void *data);
    static bool Write(HANDLE handle, StreamComm::Message& message, IStreamCallback *context, void *data);

    static void SetAsync(bool async);
  private:
    // This class should be static only
    NamedPipeIO() {}
    virtual ~NamedPipeIO() {}
    NamedPipeIO(const NamedPipeIO&) {}
    static bool Async;

    static bool ReadAsync(HANDLE handle, IStreamCallback *context, void *data);
    static bool WriteAsync(HANDLE handle, StreamComm::Message& message, IStreamCallback *context, void *data);
};
};
