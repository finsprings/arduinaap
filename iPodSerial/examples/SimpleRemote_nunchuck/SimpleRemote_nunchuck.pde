// Example of Simple Remote using a Wii Nunchuck for control
//
// The Nunchuck should be connected to Analog 2,3,4,5
// via a WiiChuck or equivalent - see
// http://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/
//
#include <SimpleRemote.h>
#include <Wire.h>
#include "nunchuck_funcs.h"

bool is_pressing = false;

// joy x y readings from nunchuck_print_data:
// center: 124,126
// x range: 22-224
// y range: 31-223
bool joy_is_left()
{
  return nunchuck_joyx() < 80;
}

bool joy_is_right()
{
  return nunchuck_joyx() > 180;
}

bool joy_is_up()
{
  return nunchuck_joyy() > 160;
}

bool joy_is_down()
{
  return nunchuck_joyy() < 80;
}

// accel x y:
// center 132 139
// fwd 141 171
// bwd 133 95
// left 89 130
// right 162 134
bool accel_is_tilted_back()
{
  return nunchuck_accely() < 100;
}

SimpleRemote sr;

void setup()
{
  nunchuck_setpowerpins();
  nunchuck_init();
  sr.setup();
}

void loop()
{
  sr.loop();
  nunchuck_get_data();

  if (nunchuck_zbutton())
  {
    sr.sendPlay();
    is_pressing = true;
  }
  else if (nunchuck_cbutton())
  {
    sr.sendOkSelectButton();
    is_pressing = true;
  }
  else if (joy_is_left())
  {
    sr.sendSkipBackward();
    is_pressing = true;
  }
  else if (joy_is_right())
  {
    sr.sendSkipForward();
    is_pressing = true;
  }
  else if (joy_is_up())
  {
    sr.sendScrollUp();
    is_pressing = true;
  }
  else if (joy_is_down())
  {
    sr.sendScrollDown();
    is_pressing = true;
  }
  else if (accel_is_tilted_back())
  {
    sr.sendMenuButton();
    is_pressing = true;
  }
  else if (is_pressing)
  {
    sr.sendButtonReleased();
    is_pressing = false;
  }
  
  // we need a little delay to avoid
  // swamping the iPod with commands
  delay(50);
}

