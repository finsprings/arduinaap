// Simple example of using iPodSerial library to send
// Simple Remote Play command on the press of a button.
// This will alternate between Play and Pause on the iPod.
//
// Uses the Arduino Bounce library for debouncing.
// You can get this from http://arduino.cc/playground/code/Bounce
#include <SimpleRemote.h>
#include <Bounce.h>

const byte BUTTON_PIN = 5;
const unsigned long DEBOUNCE_MS = 20;

Bounce button(BUTTON_PIN, DEBOUNCE_MS);
SimpleRemote simpleRemote;

void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  
  // enable pull-up
  digitalWrite(BUTTON_PIN, HIGH);
  
  // if you're using a Mega you could talk to the iPod
  // on Serial3 and turn on debugging messages
  // to Serial (i.e. back to the Arduino IDE) with
  // the following three lines. Obviously for this to
  // work you'll actually need to wire the iPod serial
  // lines up to Serial3...
  //
  //simpleRemote.setSerial(Serial3);
  //Serial.begin(9600);
  //simpleRemote.setDebugPrint(Serial);

  simpleRemote.setup();
}

void loop()
{
  simpleRemote.loop();

  if (button.update())
  {
    if (button.read() == LOW)
    {
      simpleRemote.sendPlay();
    }
    else
    {
      simpleRemote.sendButtonReleased();
    }
  }
}

