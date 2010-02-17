/*******************************************************************************
 * Copyright (c) 2009 David Findlay
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    - Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

// Example of Advanced Remote (Mode 4) getting back current track position information
//
// WARNING:
// Note that this example could leave your iPod in Advanced Mode, which puts your
// iPod's screen into "OK to disconnect" mode. It should come back out of that
// mode after a few seconds, but if not you can power it off and back on,
// or send it a command to go back into Simple Remote mode.

#include <AdvancedRemote.h>

// This sketch needs to be adapted (change serial port config in setup())
// to be used on a non-Mega, so check the board here so people notice.
#if !defined(__AVR_ATmega1280__)
#error "This example is for the Mega, because it uses one Serial3 for the iPod and Serial for debug msgs"
#endif

// We'll use a button connected between digital pin 5
// and ground, taking advantage of the Arduino's internal
// pull-up resistor feature to avoid having to use an
// external resistor. For this reason the button pin
// will go LOW when the button is pressed and HIGH
// when it's release.
const int BUTTON = 5;
const long DEBOUNCE_MILLIS = 200;
int lastButtonState = LOW;
long lastToggleTime = 0;
int buttonPressCount = 0;
AdvancedRemote advancedRemote;
bool remoteEnabled = false;

//
// our handler (aka callback) implementations;
// these are what get sent data back from the iPod
// when we make requests of it.
//
void iPodNameHandler(const char *name)
{
  Serial.print("iPod Name: ");
  Serial.println(name);
}

void itemCountHandler(unsigned long count)
{
  Serial.print("Item Count: ");
  Serial.println(count);
}

void itemNameHandler(const char *name)
{
  Serial.print("Item Name: ");
  Serial.println(name);
}

// helper function to display time 
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

void timeAndStatusHandler(unsigned long length, unsigned long elapsed, AdvancedRemote::PlaybackStatus status)
{
  Serial.print("Length of track is ");
  printTime(length);

  Serial.print("Elapsed time is ");
  printTime(elapsed);

  Serial.print("Track is ");

  switch (status)
  {
    case AdvancedRemote::STATUS_STOPPED:
    Serial.println("stopped.");
    break;

    case AdvancedRemote::STATUS_PLAYING:
    Serial.println("playing.");
    break;

    case AdvancedRemote::STATUS_PAUSED:
    Serial.println("paused.");
    break;

  default:
    Serial.println("Unrecognized status?");
    break;
  }
}

void playlistPositionHandler(unsigned long position)
{
  Serial.print("Playlist position: ");
  Serial.println(position);
}

void titleHandler(const char *title)
{
  Serial.print("Title: ");
  Serial.println(title);
}

void artistHandler(const char *artist)
{
  Serial.print("Artist: ");
  Serial.println(artist);
}

void albumHandler(const char *album)
{
  Serial.print("Album: ");
  Serial.println(album);
}

void pollingHandler(unsigned long elapsed)
{
  Serial.print("Poll: ");
  printTime(elapsed);
}

void shuffleModeHandler(AdvancedRemote::ShuffleMode shuffleMode)
{
  Serial.print("Shuffle Mode: ");

  switch (shuffleMode)
  {
    case AdvancedRemote::SHUFFLE_MODE_OFF:
    Serial.println("Off");
    break;

    case AdvancedRemote::SHUFFLE_MODE_SONGS:
    Serial.println("Songs");
    break;

    case AdvancedRemote::SHUFFLE_MODE_ALBUMS:
    Serial.println("Albums");
    break;

  default:
    Serial.print("Unrecognized ");
    Serial.println(shuffleMode, DEC);
  }
}

void repeatModeHandler(AdvancedRemote::RepeatMode repeatMode)
{
  Serial.print("Repeat Mode: ");

  switch (repeatMode)
  {
    case AdvancedRemote::REPEAT_MODE_OFF:
    Serial.println("Off");
    break;

    case AdvancedRemote::REPEAT_MODE_ONE_SONG:
    Serial.println("One Song");
    break;

    case AdvancedRemote::REPEAT_MODE_ALL_SONGS:
    Serial.println("All Songs");
    break;

  default:
    Serial.print("Unrecognized ");
    Serial.println(repeatMode, DEC);
  }
}

void currentPlaylistSongCountHandler(unsigned long count)
{
  Serial.print("Current Playlist Song Count: ");
  Serial.println(count);
}

void setup()
{
  pinMode(BUTTON, INPUT);

  // enable pull-up resistor
  digitalWrite(BUTTON, HIGH);

  Serial.begin(9600);

  // direct library's debug msgs to the default serial port
  advancedRemote.setDebugPrint(Serial);

  // use Serial3 (Mega-only) to talk to the iPod
  advancedRemote.setSerial(Serial3);

  // register callback functions for the things we're going to read
  advancedRemote.setiPodNameHandler(iPodNameHandler);
  advancedRemote.setItemCountHandler(itemCountHandler);
  advancedRemote.setItemNameHandler(itemNameHandler);
  advancedRemote.setTimeAndStatusHandler(timeAndStatusHandler);
  advancedRemote.setPlaylistPositionHandler(playlistPositionHandler);
  advancedRemote.setTitleHandler(titleHandler);
  advancedRemote.setArtistHandler(artistHandler);
  advancedRemote.setAlbumHandler(albumHandler);
  advancedRemote.setPollingHandler(pollingHandler);
  advancedRemote.setShuffleModeHandler(shuffleModeHandler);
  advancedRemote.setRepeatModeHandler(repeatModeHandler);
  advancedRemote.setCurrentPlaylistSongCountHandler(currentPlaylistSongCountHandler);

  // let the library set itself up, now we've done our configuration of it
  advancedRemote.setup(); 
}

void loop()
{
  // service iPod serial. this is who will end up
  // calling our handler functions when responses
  // come back from the iPod
  advancedRemote.loop();

  const int buttonState = digitalRead(BUTTON);
  const long now = millis();

  if ((buttonState != lastButtonState) && ((now - lastToggleTime) > DEBOUNCE_MILLIS))
  {
    if (buttonState == LOW)
    {
      buttonPressCount++;

      if (buttonPressCount == 1)
      {
        advancedRemote.enable();
        delay(500);
        advancedRemote.switchToMainLibraryPlaylist();
        delay(250);
        advancedRemote.getItemCount(AdvancedRemote::ITEM_SONG);
        delay(250);
        advancedRemote.getItemNames(AdvancedRemote::ITEM_SONG, 0, 10);
        delay(250);
        advancedRemote.switchToItem(AdvancedRemote::ITEM_SONG, 2);
      }
      else if (buttonPressCount < 5)
      {
        advancedRemote.controlPlayback(AdvancedRemote::PLAYBACK_CONTROL_PLAY_PAUSE);
      }
      else
      {
        advancedRemote.disable();
      }
      /*
      // toggle advanced remote mode
       if (remoteEnabled)
       {
       advancedRemote.setPollingMode(AdvancedRemote::POLLING_STOP);
       advancedRemote.disable();
       remoteEnabled = false;
       }
       else
       {
       advancedRemote.enable();
       delay(250);
       //advancedRemote.controlPlayback(AdvancedRemote::PLAYBACK_CONTROL_PLAY_PAUSE);
       //advancedRemote.getiPodName();
       advancedRemote.setPollingMode(AdvancedRemote::POLLING_START);
       //advancedRemote.getTimeAndStatusInfo();
       //advancedRemote.getPlaylistPosition();
       remoteEnabled = true;
       }
       */
    }
    else
    {
      // don't need to do anything when user lets go of the button
    }

    lastButtonState = buttonState;
    lastToggleTime = now;
  }
}





