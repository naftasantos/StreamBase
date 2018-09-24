#include "screen_factory.h"
#include "connect_screen.h"
#include "home_screen.h"
#include "store_data_screen.h"
#include "read_stored_data_screen.h"

IScreen* ScreenFactory::CreateScreen(Screen screen) {
  IScreen *screen_to_show = nullptr;

  switch(screen) {
    case kScreenConnect:
      screen_to_show = new ConnectScreen();
      break;
    case kScreenHome:
      screen_to_show = new HomeScreen();
      break;
    case kScreenStoreData:
      screen_to_show = new StoreDataScreen();
      break;
    case kScreenReadStoredData:
      screen_to_show = new ReadStoredDataScreen();
      break;
  }

  return screen_to_show;
}