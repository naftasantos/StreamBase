#include "command_factory.h"
#include "store_command.h"
#include "read_stored_command.h"
#include "list_classes_command.h"

ICommand* CommandFactory::GetCommand(StreamComm::Command command) {
  ICommand *result = nullptr;

  switch(command) {
    case StreamComm::kCommandStoreData:
      result = new StoreCommand();
      break;
    case StreamComm::kCommandReadStoredData:
      result = new ReadStoredCommand();
      break;
    case StreamComm::kCommandListClasses:
      result = new ListClassesCommand();
      break;
  }

  return result;
}