#include "screen_factory.h"
#include "connect_screen.h"

IScreen* ScreenFactory::CreateScreen(Screen screen) {
  IScreen *screen_to_show = nullptr;

  switch(screen) {
    default:
    case kScreenConnect:
      screen_to_show = new ConnectScreen();
      break;
  }

  return screen_to_show;
}