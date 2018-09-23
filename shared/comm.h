#pragma once

#define PIPE_NAME "\\\\.\\pipe\\StreamBase"
#define MAX_DATA_SIZE 1024

typedef enum {
  kCommandGreeting,
  kCommandStoreData
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
  int id;
} GreetCommand;