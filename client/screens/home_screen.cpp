#include "home_screen.h"

#include <iostream>

typedef enum {
  kHomeChoiceInvalid = -1,
  kHomeChoiceExit = 0,
  kHomeChoiceStoreData,
  kHomeChoiceReadStoredData,
  kHomeChoiceStoreClass
} HomeChoice;

HomeScreen::HomeScreen() {

}

HomeScreen::~HomeScreen() {

}

Screen HomeScreen::Show() {
  Screen next_screen = kScreenNone;
  bool invalid = true;
  int option;

  do {
    option = kHomeChoiceInvalid;

    std::cout << "Select one option below:" << std::endl;
    std::cout << "1. Store Data" << std::endl;
    std::cout << "2. Read Stored Data" << std::endl;
    std::cout << "3. Store Class" << std::endl;
    std::cout << "0. Exit" << std::endl;

    std::cin >> option;

    if (option == kHomeChoiceInvalid) {
      invalid = true;
    } else {
      invalid = NextScreenFromOption(option, next_screen);
    }

    if (invalid) {
      std::cout << "Invalid option. Try again" << std::endl;
    }
  } while(invalid);

  return next_screen;
}

bool HomeScreen::NextScreenFromOption(int option, Screen& next) {
  bool invalid = false;

  switch(option) {
    case kHomeChoiceExit:
      next = kScreenNone;
      break;
    case kHomeChoiceStoreData:
      next = kScreenStoreData;
      break;
    case kHomeChoiceReadStoredData:
      next = kScreenReadStoredData;
      break;
    case kHomeChoiceStoreClass:
      next = kScreenStoreClasses;
      break;
    default:
      invalid = true;
      break;
  }

  return invalid;
}