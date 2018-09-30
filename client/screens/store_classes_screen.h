#pragma once

#include "screen_interface.h"
#include "comm.h"

#include <string>

typedef enum {
  kStateListClass,
  kStateStoreClass,
  kStateListParams,
  kStateCallMethod
} StoreClassState;

class StoreClassesScreen : public IScreen {
  public:
    StoreClassesScreen();
    virtual ~StoreClassesScreen();
    virtual Screen Show();

    virtual void OnRead(bool success, StreamComm::Message message, void *data);
    virtual void OnWrite(bool success, StreamComm::Message message, void *data);
  private:
    StoreClassesScreen(const StoreClassesScreen&) {}

    bool ListClasses();
    std::string ChooseClass(std::string classes);
    bool StoreClass(std::string class_name, StreamComm::ResponseCommand& response);
    bool CallMethod(std::string class_name, StreamComm::ResponseCommand& response);
    bool ListParams(std::string class_name, std::string method_name, StreamComm::ResponseCommand& response);
    bool ExecuteMethod(std::string method_name, std::string params, StreamComm::ResponseCommand& response);

    void ListClassesFinished(StreamComm::ResponseCommand response);
    void StoreClassFinished(StreamComm::ResponseCommand response);
    void ListParamsFinished(StreamComm::ResponseCommand response);
    void CallMethodFinished(StreamComm::ResponseCommand response);

    bool finished;
    Screen next_screen;
    StoreClassState state;
    std::string chosen_class;
    std::string chosen_method;
};