#pragma once

#include <windows.h>
#include "comm.h"

namespace StreamComm {
class IStreamCallback {
  public:
    virtual ~IStreamCallback() {}
};

class IReadCallback : public IStreamCallback {
  public:
    virtual void OnRead(bool success, StreamComm::Message message, void *data) = 0;
};

class IWriteCallback : public IStreamCallback {
  public:
    virtual void OnWrite(bool success, StreamComm::Message message, void *data) = 0;
};

typedef struct {
  OVERLAPPED overlap;
  IStreamCallback *context;
  void *context_data;
  void *data;
} CallbackData;

class NamedPipeIO {
  public:
    static bool Read(HANDLE handle, StreamComm::Message *message);
    static bool Write(HANDLE handle, StreamComm::Message &message);

    static bool ReadAsync(HANDLE handle, IReadCallback *context, void *data);
    static bool WriteAsync(HANDLE handle, IWriteCallback *context, void *data);
  private:
    // This class should be static only
    NamedPipeIO() {}
    virtual ~NamedPipeIO() {}
    NamedPipeIO(const NamedPipeIO&) {}
};
};
