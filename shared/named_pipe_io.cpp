#include "named_pipe_io.h"

bool StreamComm::NamedPipeIO::Read(HANDLE handle, StreamComm::Message *message) {
  bool status = true;

  if (handle == INVALID_HANDLE_VALUE || message == NULL) {
    status = false;
  } else {
        status = ReadFile(handle,
                          message,
                          sizeof(StreamComm::Message),
                          NULL,
                          NULL);

  }

  return status;
}

bool StreamComm::NamedPipeIO::Write(HANDLE handle, StreamComm::Message &message) {
  bool status = true;

  if (handle == INVALID_HANDLE_VALUE) {
    status = false;
  } else {
    status = WriteFile(handle,
                       &message,
                       sizeof(StreamComm::Message),
                       NULL,
                       NULL);

  }

  return status;

}