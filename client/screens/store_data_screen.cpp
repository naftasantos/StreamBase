#include "store_data_screen.h"

#include <iostream>
#include <string>

#define MAX_NAME_CHARS 10
#define MAX_VALUE_CHARS 100

StoreDataScreen::StoreDataScreen() {

}

StoreDataScreen::~StoreDataScreen() {

}

Screen StoreDataScreen::Show() {
  Screen next_screen = kScreenHome;
  std::string data_name;
  std::string data_value;

  bool ok = true;

  do {
    std::cout << "Type in the name of the data you want to store (max " << MAX_NAME_CHARS << " chars.)" << std::endl;
    std::cin >> data_name;

    if (data_name.length() > MAX_NAME_CHARS) {
      std::cout << "Invalid data size" << std::endl;
      ok = false;
    } 
  } while(!ok);

  ok = true;
  std::cin.ignore(INT_MAX, '\n');
  std::cin.clear();

  do {
    std::cout << "Type in the value you want to store (max " << MAX_VALUE_CHARS << " chars.)" << std::endl;
    std::getline(std::cin, data_value);

    if (data_value.length() > MAX_VALUE_CHARS) {
      ok = false;
    }
  } while(!ok);

  std::cout << "Sending data '" << data_name << "' with the value '" << data_value << "' to the server" << std::endl;

  return next_screen;
}