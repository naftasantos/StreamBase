#include "command_factory.h"
#include "store_command.h"
#include "read_stored_command.h"

ICommand* CommandFactory::GetCommand(StreamComm::Command command) {
  ICommand *result = nullptr;

  switch(command) {
    case StreamComm::kCommandStoreData:
      result = new StoreCommand();
      break;
    case StreamComm::kCommandReadStoredData:
      result = new ReadStoredCommand();
      break;
  }

  return result;
}