#include "named_pipe_io.h"

#include <iostream>

static void FreeCallbackData(StreamComm::CallbackData* callback) {
  if (callback != nullptr) {
    if (callback->data != nullptr) {
      delete callback->data;
      callback->data = nullptr;
    }

    delete callback;
  }
}

static VOID WINAPI ReadCallback(DWORD error, DWORD bytes_written, LPOVERLAPPED overlap) {
  bool status = error == ERROR_SUCCESS;
  StreamComm::CallbackData *callback_data = nullptr;

  std::cout << "ReadCallback" << std::endl;

  if (overlap != nullptr) {
    std::cout << "Overlap is not null" << std::endl;
    callback_data = (StreamComm::CallbackData*)overlap;
    StreamComm::Message *message = (StreamComm::Message*)callback_data->data;
    StreamComm::IReadCallback *callback = dynamic_cast<StreamComm::IReadCallback*>(callback_data->context);

    if (callback != nullptr) {
      std::cout << "Callback is not null" << std::endl;
      callback->OnRead(status, (*message), callback_data->context_data);
    }
  }

  FreeCallbackData(callback_data);
}

bool StreamComm::NamedPipeIO::ReadAsync(HANDLE handle, StreamComm::IReadCallback *callback, void *data) {
  bool status = true;
  
  std::cout << "StreamComm::NamedPipeIO::ReadAsync" << std::endl;
  
  if (handle == INVALID_HANDLE_VALUE) {
    std::cerr << "Invalid handle" << std::endl;
    status = false;
    SetLastError(ERROR_INVALID_PARAMETER);
  } else {
    CallbackData *callback_data = new CallbackData();
    std::cerr << "Created callback" << std::endl;

    memset(callback_data, 0, sizeof(CallbackData));

    std::cerr << "Setting to 0" << std::endl;
    callback_data->context_data = data;
    callback_data->context = dynamic_cast<IStreamCallback*>(callback);
    callback_data->data = (void*)new Message();
    memset(callback_data->data, 0, sizeof(Message));
    std::cerr << "Created context" << std::endl;

    status = ReadFileEx(handle,
                        callback_data->data,
                        sizeof(Message),
                        (LPOVERLAPPED)callback_data,
                        (LPOVERLAPPED_COMPLETION_ROUTINE)ReadCallback);
    std::cerr << "Read file: " << (status ? "Success" : "Fail") << std::endl;
  }
  return status;
}

bool StreamComm::NamedPipeIO::Read(HANDLE handle, StreamComm::Message *message) {
  bool status = true;

  if (handle == INVALID_HANDLE_VALUE || message == NULL) {
    status = false;
    SetLastError(ERROR_INVALID_PARAMETER);
  } else {
        status = ReadFile(handle,
                          message,
                          sizeof(StreamComm::Message),
                          NULL,
                          NULL);

  }

  return status;
}

bool StreamComm::NamedPipeIO::WriteAsync(HANDLE handle, StreamComm::IWriteCallback *callback, void *data) {
  return true;
}

bool StreamComm::NamedPipeIO::Write(HANDLE handle, StreamComm::Message &message) {
  bool status = true;

  if (handle == INVALID_HANDLE_VALUE) {
    status = false;
    SetLastError(ERROR_INVALID_PARAMETER);
  } else {
    status = WriteFile(handle,
                       &message,
                       sizeof(StreamComm::Message),
                       NULL,
                       NULL);

  }

  return status;

}