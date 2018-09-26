#pragma once

#define PIPE_NAME "\\\\.\\pipe\\StreamBase"
#define MAX_DATA_SIZE 1024

namespace StreamComm{
typedef enum {
  kCommandGreeting = 0x00,
  kCommandStoreData,
  kCommandReadStoredData,
  kCommandListClasses,
  kCommandStoreClass,
  kCommandListParams,
  kCommandResponse = 0xFF
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
  char data_name[11];
} ReadStoredDataCommand;

typedef struct {
  char class_name[101];
} StoreClassCommand;

typedef struct {
  char class_name[101];
  char method_name[101];
} ListParamsCommand;

typedef struct {
  bool status;
  char message[101];
} ResponseCommand;
};