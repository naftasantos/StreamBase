#include "list_classes_command.h"

ListClassesCommand::ListClassesCommand() {

}

ListClassesCommand::~ListClassesCommand() {

}

StreamComm::ResponseCommand ListClassesCommand::Execute(StreamComm::Message& message,
                                                       std::map<std::string,
                                                       IServerDataObject*>& data) {
  StreamComm::ResponseCommand response = { 0 };

  response.status = false;
  memcpy(response.message, "not implemented yet", 19);

  return response;
}