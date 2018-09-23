#pragma once

#include <windows.h>
#include "comm.h"

namespace StreamComm {
class NamedPipeIO {
  public:
    static bool Read(HANDLE handle, StreamComm::Message *message);
    static bool Write(HANDLE handle, StreamComm::Message &message);
  private:
    // This class should be static only
    NamedPipeIO() {}
    virtual ~NamedPipeIO() {}
    NamedPipeIO(const NamedPipeIO&) {}
};
};
