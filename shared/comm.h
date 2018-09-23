#pragma once

#define PIPE_NAME "\\\\.\\pipe\\StreamBase"
#define MAX_DATA_SIZE 1024

namespace StreamComm{
typedef enum {
  kCommandGreeting,
  kCommandStoreData,
  kCommandResponse
} Command;

typedef struct {
  Command message_command; 
  int data_size;
} MessageHeader;

typedef struct {
  MessageHeader header;
  char data[MAX_DATA_SIZE];
} Message;

typedef struct {
  char data_name[11];
  char data_value[101];
} StoreDataCommand;

typedef struct {
  bool status;
  char message[100];
} ResponseCommand;
};