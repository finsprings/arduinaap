// Simple example of using iPodSerial library to send
// Simple Remote Play command on the press of a button.
// This will alternate between Play and Pause on the iPod.
#include <SimpleRemote.h>

// We'll use a button connected between digital pin 5
// and ground, taking advantage of the Arduino's internal
// pull-up resistor feature to avoid having to use an
// external resistor. For this reason the button pin
// will go LOW when the button is pressed and HIGH
// when it's release.
const int BUTTON = 5;
const long DEBOUNCE_MILLIS = 100;
int lastButtonState = LOW;
long lastToggleTime = 0;
SimpleRemote simpleRemote;

void setup()
{
  pinMode(BUTTON, INPUT);

  // turn on the pull-up resistor
  digitalWrite(BUTTON, HIGH);
  
  // if you're using a Mega you could talk to the iPod
  // on Serial3 and turn on debugging messages
  // to Serial (i.e. back to the Arduino IDE) with
  // the following three lines. Obviosuly for this to
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

  const int buttonState = digitalRead(BUTTON);
  const long now = millis();

  if ((buttonState != lastButtonState) && ((now - lastToggleTime) > DEBOUNCE_MILLIS))
  {
    if (buttonState == LOW)
    {
      simpleRemote.sendPlay();
    }
    else
    {
      simpleRemote.sendButtonReleased();
    }

    lastButtonState = buttonState;
    lastToggleTime = now;
  }
}


