#include <iostream>

#include "screens/screen_data.h"
#include "screens/screen_interface.h"
#include "screens/screen_factory.h"

#include "config.h"

int main(int argc, char** argv) {
  Screen current_screen = kScreenConnect;
  Config::ReadConfig(argc, argv);

  do {
    IScreen* screen = ScreenFactory::CreateScreen(current_screen);
    
    if (screen != nullptr) {
      current_screen = screen->Show();
      delete screen;
    } else {
      current_screen = kScreenNone;
    }

    std::cout << "next_screen: " << current_screen << std::endl;
  } while(current_screen != kScreenNone);
  
  ScreenData::Finish();
  return EXIT_SUCCESS;
}