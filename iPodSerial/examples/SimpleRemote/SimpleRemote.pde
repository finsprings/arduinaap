// Simple example of using iPodSerial library to send
// Simple Remote Play command on the press of a button.
// This will alternate between Play and Pause on the iPod.
#include "iPodSerial.h"

const int BUTTON = 5;
const long debounce = 200;
int lastButtonState = LOW;
int buttonState;
long now;
long lastToggleTime = 0;
iPodSerial is;

void setup()
{
    pinMode(BUTTON, INPUT);
    is.setup();
}

void loop()
{
    // need to service iPod serial as often as we can
    // TODO: how often?
    is.loop();
    
    buttonState = digitalRead(BUTTON);
    now = millis();
  
    if ((buttonState != lastButtonState) && ((now - lastToggleTime) > debounce))
    {
        if (buttonState == HIGH)
        {
            is.sendPlay();
        }
        else
        {
            is.sendRelease();
        }
    
        lastButtonState = buttonState;
        lastToggleTime = now;
    }
}
