// Example of Advanced Remote (Mode 4) dumping all the playlists on an iPod
//
// If your iPod ends up stuck with the "OK to disconnect" message on its display,
// reset the Arduino. There's a called to AdvancedRemote::disable() in the setup()
// function which should put the iPod back to its normal mode. If that doesn't
// work, or you are unable to reset your Arduino for some reason, resetting the
// iPod will put it back to its normal mode.

#include <AdvancedRemote.h>
#include <Bounce.h>

// This sketch needs to be adapted (change serial port config in setup())
// to be used on a non-Mega, so check the board here so people notice.
#if !defined(__AVR_ATmega1280__)
#error "This example is for the Mega, because it uses Serial3 for the iPod and Serial for debug messages"
#endif

const byte BUTTON_PIN = 22;
const unsigned long DEBOUNCE_MS = 50;

Bounce button(BUTTON_PIN, DEBOUNCE_MS);
AdvancedRemote advancedRemote;

// we'll ask the iPod for this
unsigned long playlistCount = 0;

//
// our handler (aka callback) implementations;
// these are what get sent data back from the iPod
// when we make requests of it.
//

void feedbackHandler(AdvancedRemote::Feedback feedback, byte cmd)
{
  Serial.print("got feedback of 0x");
  Serial.print(feedback, HEX);
  Serial.print(" for cmd 0x");
  Serial.println(cmd, HEX);

  if (feedback != AdvancedRemote::FEEDBACK_SUCCESS)
  {
    Serial.println("Giving up as feedback wasn't SUCCESS");
    return;
  }
}

void itemCountHandler(unsigned long count)
{
  Serial.print("Playlist Count: ");
  Serial.println(count);

  playlistCount = count;

  // that ought to be the count of playlists, since that's what we asked for.
  // so, let's start printing out their names
  advancedRemote.getItemNames(AdvancedRemote::ITEM_PLAYLIST, 0, playlistCount);
}

void itemNameHandler(unsigned long offset, const char *name)
{
  Serial.print("Playlist ");
  Serial.print(offset, DEC);
  Serial.print(" is named '");
  Serial.print(name);
  Serial.println("'");

  if (offset == playlistCount - 1)
  {
    Serial.println("Got last playlist name, coming out of advanced mode");
    advancedRemote.disable();
  }
}

void setup()
{
  pinMode(BUTTON_PIN, INPUT);

  // enable pull-up resistor
  digitalWrite(BUTTON_PIN, HIGH);

  Serial.begin(9600);

  // enable debugging
  // #define IPOD_SERIAL_DEBUG

  // direct library's log msgs to the default serial port
  //advancedRemote.setLogPrint(Serial);

  // direct library's debug msgs to the default serial port
  //advancedRemote.setDebugPrint(Serial);

  // use Serial3 (Mega-only) to talk to the iPod
  Serial3.begin(iPodSerial::IPOD_SERIAL_RATE);
  advancedRemote.setSerial(Serial3);

  // register callback functions for the things we're going to read
  advancedRemote.setFeedbackHandler(feedbackHandler);
  advancedRemote.setItemCountHandler(itemCountHandler);
  advancedRemote.setItemNameHandler(itemNameHandler);

  // start disabled, i.e. in good old Simple Remote mode
  advancedRemote.disable();
}

void loop()
{
  // service iPod serial. this is who will end up
  // calling our handler functions when responses
  // come back from the iPod
  advancedRemote.loop();

  if (button.update() && (button.read() == LOW))
  {
    if (advancedRemote.isCurrentlyEnabled())
    {
      advancedRemote.disable();
    }
    else
    {
      advancedRemote.enable();

      // start our commands - we'll do them in sequence
      // as our handlers get called. for example, this
      // one will call our Feedback handler when it's done
      advancedRemote.getItemCount(AdvancedRemote::ITEM_PLAYLIST);
    }
  }
}







