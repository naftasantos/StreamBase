#include "named_pipe_io.h"
#include "windows_helper.h"

#include <iostream>

static void FreeCallbackData(StreamComm::CallbackData* callback) {
  if (callback != nullptr) {
    if (callback->data != nullptr) {
      delete callback->data;
      callback->data = nullptr;
    }

    if (callback->overlap.hEvent != nullptr) {
      CloseHandle(callback->overlap.hEvent);
      callback->overlap.hEvent = nullptr;
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

static VOID WINAPI WriteCallback(DWORD error, DWORD bytes_written, LPOVERLAPPED overlap) {
  bool status = error == ERROR_SUCCESS;
  StreamComm::CallbackData *callback_data = nullptr;

  std::cout << "WriteCallback" << std::endl;

  if (overlap != nullptr) {
    std::cout << "Overlap is not null" << std::endl;
    callback_data = (StreamComm::CallbackData*)overlap;
    StreamComm::Message *message = (StreamComm::Message*)callback_data->data;
    StreamComm::IWriteCallback *callback = dynamic_cast<StreamComm::IWriteCallback*>(callback_data->context);

    if (callback != nullptr) {
      std::cout << "Callback is not null" << std::endl;
      callback->OnWrite(status, *message, callback_data->context_data);
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
    std::cout << "Created callback" << std::endl;

    std::cout << "Setting to 0" << std::endl;
    memset(callback_data, 0, sizeof(CallbackData));

    callback_data->overlap.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

    callback_data->context_data = data;
    callback_data->context = dynamic_cast<IStreamCallback*>(callback);
    callback_data->data = (void*)new StreamComm::Message();
    memset(callback_data->data, 0, sizeof(StreamComm::Message));
    std::cout << "Created context" << std::endl;

    status = ReadFileEx(handle,
                        callback_data->data,
                        sizeof(StreamComm::Message),
                        (LPOVERLAPPED)callback_data,
                        (LPOVERLAPPED_COMPLETION_ROUTINE)ReadCallback);
    std::cout << "Current status: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
    if (status) {
      std::cout << "Read file: Success" << std::endl;
    } else {
      std::cerr << "Read file: Fail" << std::endl;
    }
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

bool StreamComm::NamedPipeIO::WriteAsync(HANDLE handle,
                                         StreamComm::Message &message,
                                         StreamComm::IWriteCallback *callback,
                                         void *data) {
  bool status = true;

  std::cout << "StreamComm::NamedPipeIO::WriteAsync" << std::endl;
  
  if (handle == INVALID_HANDLE_VALUE) {
    std::cerr << "Invalid handle" << std::endl;
    status = false;
    SetLastError(ERROR_INVALID_PARAMETER);
  } else {
    CallbackData *callback_data = new CallbackData();
    std::cout << "Created callback" << std::endl;

    std::cout << "Setting to 0" << std::endl;
    memset(callback_data, 0, sizeof(CallbackData));

    callback_data->overlap.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

    callback_data->context_data = data;
    callback_data->context = dynamic_cast<IStreamCallback*>(callback);
    callback_data->data = new StreamComm::Message();
    memcpy(callback_data->data, &message, sizeof(StreamComm::Message));
    std::cout << "Created context" << std::endl;

    status = WriteFileEx(handle,
                         callback_data->data,
                         sizeof(StreamComm::Message),
                         (LPOVERLAPPED)callback_data,
                         (LPOVERLAPPED_COMPLETION_ROUTINE)WriteCallback);
    std::cout << "Current status: " << Helper::WindowsHelper::GetLastErrorMessage() << std::endl;
    if (status) {
      std::cout << "Write file: Success" << std::endl;
    } else {
      std::cout << "Write file: Fail" << std::endl;
    }
  }

  return status;
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