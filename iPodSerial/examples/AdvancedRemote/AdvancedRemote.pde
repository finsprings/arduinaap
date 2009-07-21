// Example of Advanced Remote (Mode 4) getting back current track position information
#include "iPodSerial.h"

#if !defined(__AVR_ATmega1280__)
#error "This example is for the Mega, because it uses one Serial1 for the iPod and Serial for debug msgs"
#endif

const int BUTTON = 5;
const long debounce = 200;
int lastButtonState = LOW;
int buttonState;
long now;
long lastToggleTime = 0;
iPodSerial is;

// Function that will be called with the title of the current track
// (in response to Mode 4 command 0x20)
void songTitleHandler(const char *title)
{
    Serial.print("Title: ");
    Serial.println(title);
}

// Helper function to display time nicely
void printTime(const unsigned long ms)
{
    unsigned long secs = ms / 1000;
    const unsigned int mins = secs / 60;
    const unsigned int partialSecs = secs % 60;

    Serial.print(mins, DEC);
    Serial.print("m ");
    Serial.print(partialSecs, DEC);
    Serial.println("s");
}

// Function that will be called with time and current status of the track
// (in response to Mode 4 command 0x1C)
void timeAndStatusHandler(unsigned long length, unsigned long elapsed, iPodSerial::Status status)
{
    Serial.print("Length of track is ");
    printTime(length);
    
    Serial.print("Elapsed time is ");
    printTime(elapsed);
    
    Serial.print("Track is ");
    
    switch (status)
    {
    case iPodSerial::STATUS_STOP:
        Serial.println("stopped.");
        break;

    case iPodSerial::STATUS_PLAYING:
        Serial.println("playing.");
        break;

    case iPodSerial::STATUS_PAUSED:
        Serial.println("pause.");
        break;

    default:
        Serial.println("?");
        break;
    }
}

// Function that will be called with the playlist position
// (in response to Mode 4 command 0x1E)
void currentPlaylistPositionHandler(unsigned long position)
{
    Serial.print("Current position is ");
    printTime(position);
}

void setup()
{
    pinMode(BUTTON, INPUT);

    Serial.begin(9600);

    // direct library's debug msgs to the default serial port
    is.setDebugPrint(Serial);

    // use Serial1 (Mega-only) to talk to the iPod
    is.setSerial(Serial1);

    // register callback functions for the 3 things we're going to read
    is.setSongTitleHandler(songTitleHandler);
    is.setTimeAndStatusHandler(timeAndStatusHandler);
    is.setCurrentPositionHandler(currentPlaylistPositionHandler);

    // let the library set itself up, now we've done our configuration of it
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
            is.askForPos();
        }
        else
        {
            // don't need to do anything when user lets go of the button
        }
    
        lastButtonState = buttonState;
        lastToggleTime = now;
    }
}
