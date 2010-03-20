// NOTE: This sketch is still a work-in-progress.

// Example of Advanced Remote that exposes an ethernet command interface.
// Look at the clientLoop() function to see what it understands.
//
// If your iPod ends up stuck with the "OK to disconnect" message on its display,
// reset the Arduino. There's a called to AdvancedRemote::disable() in the setup()
// function which should put the iPod back to its normal mode. If that doesn't
// work, or you are unable to reset your Arduino for some reason, resetting the
// iPod will put it back to its normal mode.

#include <Ethernet.h>
#include <AdvancedRemote.h>

AdvancedRemote ar;

byte MAC_ADDRESS[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte IP_ADDRESS[] = { 
  192, 168, 13, 2 };

// Don't use 23 or telnet client will tend to do telnet negotiation (WILL/WONT/DO/DONT etc)
byte PORT = 80;
Server server = Server(PORT);
Client c(255);

char buffer[32];
int len = 0;

/*
// for debug only, but right now using it causes the connection to reset for some reason
 class ClientPrint : 
 public Print
 {
 public:
 virtual void write(uint8_t b)
 {
 if (c)
 {
 c.write(b);
 }
 }
 };
 ClientPrint clientPrint;
 */

//
// our handler (aka callback) implementations;
// these are what get sent data back from the iPod
// when we make requests of it.
//

void feedbackHandler(AdvancedRemote::Feedback feedback, byte cmd)
{
  c.print("{\"feedback\":{\"cmd\": ");
  c.print(cmd, DEC);
  c.print(", \"feedback\": ");
  c.print(feedback, DEC);
  c.println("}}");
}

void iPodNameHandler(const char *name)
{
  c.print("{\"ipod-name\": \"");
  c.print(name);
  c.println("\"}");
}

void titleHandler(const char *name)
{
  c.print("{\"title\": \"");
  c.print(name);
  c.println("\"}");
}

void artistHandler(const char *name)
{
  c.print("{\"artist\": \"");
  c.print(name);
  c.println("\"}");
}

void albumHandler(const char *name)
{
  c.print("{\"album\": \"");
  c.print(name);
  c.println("\"}");
}

void currentPlaylistSongCountHandler(unsigned long count)
{
  c.print("{\"pl-song-count\": ");
  c.print(count, DEC);
  c.println("}");
}

void pollingHandler(unsigned long elapsedTimeMs)
{
  c.print("{\"poll\": ");
  c.print(elapsedTimeMs, DEC);
  c.println("}");
}

void playlistPositionHandler(unsigned long index)
{
  c.print("{\"pl-position\": ");
  c.print(index, DEC);
  c.println("}");
}

void itemCountHandler(unsigned long count)
{
  c.print("{\"item-count\": ");
  c.print(count, DEC);
  c.println("}");
}

void itemNameHandler(unsigned long offset, const char *name)
{
  c.print("{\"item\": {\"offset\": ");
  c.print(offset, DEC);
  c.print(", \"name\": \"");
  c.print(name);
  c.println("\"}}");
}

void timeAndStatusHandler(unsigned long trackLenMs, unsigned long elapsedTimeMs, AdvancedRemote::PlaybackStatus status)
{
  c.print("{\"status\": {\"track-length\" ");
  c.print(trackLenMs, DEC);
  c.print(", \"elapsed-time\": ");
  c.print(elapsedTimeMs);
  c.println("}}");
}

const char *tryAndReadALine()
{
  int b = -1;
  if (c.available())
  {
    b = c.read();
    if ((b != -1) && (b != '\r') && (b != '\n'))
    {
      buffer[len++] = b;
    }
  };

  if (b == '\n')
  {
    buffer[len] = '\0';
    len = 0;
    return buffer;
  }
  else
  {
    return NULL;
  }
}

// extract the number at the end of a string, e.g. the 1 in "foo1"
long getIndex(const char *cmd, const char *prefix)
{
  const size_t prefixLen = strlen(prefix);

  if (strncmp(cmd, prefix, prefixLen) == 0)
  {
    return atol(cmd + prefixLen);
  }
  else
  {
    return -1;
  }
}

// see if we got a full command from the client, and if so process it
void clientLoop()
{
  const char *cmd = tryAndReadALine();
  if (cmd == NULL)
  {
    // not a complete command yet
    return;
  }

  long index;

  if (strcmp(cmd, "enable") == 0)
  {
    ar.enable();
    // iPod has no response for this, so fake one
    c.println("{\"enabled\": 1}");
  }
  else if (strcmp(cmd, "disable") == 0)
  {
    ar.disable();
    // iPod has no response for this, so fake one
    c.println("{\"enabled\": 0}");
  }
  else if (strcmp(cmd, "ipod-name") == 0)
  {
    ar.getiPodName();
  }
  /*
  else if (strcmp(cmd, "s-main-pl") == 0)
  {
    ar.switchToMainLibraryPlaylist();
  } 
  */  
  //
  // item switchers
  //
  else if ((index = getIndex(cmd, "s-pl-")) != -1)
  {
    ar.switchToItem(AdvancedRemote::ITEM_PLAYLIST, index);
  }
  else if ((index = getIndex(cmd, "s-artist-")) != -1)
  {
    ar.switchToItem(AdvancedRemote::ITEM_ARTIST, index);
  }
  else if ((index = getIndex(cmd, "s-album-")) != -1)
  {
    ar.switchToItem(AdvancedRemote::ITEM_ALBUM, index);
  }
  else if ((index = getIndex(cmd, "s-genre-")) != -1)
  {
    ar.switchToItem(AdvancedRemote::ITEM_GENRE, index);
  }
  else if ((index = getIndex(cmd, "s-song-")) != -1)
  {
    ar.switchToItem(AdvancedRemote::ITEM_SONG, index);
  }
  else if ((index = getIndex(cmd, "s-composer-")) != -1)
  {
    ar.switchToItem(AdvancedRemote::ITEM_COMPOSER, index);
  }
  //
  // item counts
  //
  else if (strcmp(cmd, "g-pl-count") == 0)
  {
    ar.getItemCount(AdvancedRemote::ITEM_PLAYLIST);
  }    
  else if (strcmp(cmd, "g-artist-count") == 0)
  {
    ar.getItemCount(AdvancedRemote::ITEM_ARTIST);
  }    
  else if (strcmp(cmd, "g-album-count") == 0)
  {
    ar.getItemCount(AdvancedRemote::ITEM_ALBUM);
  }    
  else if (strcmp(cmd, "g-genre-count") == 0)
  {
    ar.getItemCount(AdvancedRemote::ITEM_GENRE);
  }    
  else if (strcmp(cmd, "g-song-count") == 0)
  {
    ar.getItemCount(AdvancedRemote::ITEM_SONG);
  }    
  else if (strcmp(cmd, "g-composer-count") == 0)
  {
    ar.getItemCount(AdvancedRemote::ITEM_COMPOSER);
  }
  //
  // item names
  //
  else if ((index = getIndex(cmd, "g-pl-name-")) != -1)
  {
    ar.getItemNames(AdvancedRemote::ITEM_PLAYLIST, index, 1);
  }
  else if ((index = getIndex(cmd, "g-artist-name-")) != -1)
  {
    ar.getItemNames(AdvancedRemote::ITEM_ARTIST, index, 1);
  }
  else if ((index = getIndex(cmd, "g-album-name-")) != -1)
  {
    ar.getItemNames(AdvancedRemote::ITEM_ALBUM, index, 1);
  }
  else if ((index = getIndex(cmd, "g-genre-name-")) != -1)
  {
    ar.getItemNames(AdvancedRemote::ITEM_GENRE, index, 1);
  }
  else if ((index = getIndex(cmd, "g-song-name-")) != -1)
  {
    ar.getItemNames(AdvancedRemote::ITEM_SONG, index, 1);
  }
  else if ((index = getIndex(cmd, "g-composer-name-")) != -1)
  {
    ar.getItemNames(AdvancedRemote::ITEM_COMPOSER, index, 1);
  }
  //
  // Misc
  //
  else if (strcmp(cmd, "g-status") == 0)
  {
    ar.getTimeAndStatusInfo();
  }    
  else if (strcmp(cmd, "g-pl-song-count") == 0)
  {
    ar.getSongCountInCurrentPlaylist();
  }    
  else if (strcmp(cmd, "g-pl-position") == 0)
  {
    ar.getPlaylistPosition();
  }    
  else if ((index = getIndex(cmd, "jump-to-song-")) != -1)
  {
    ar.jumpToSongInCurrentPlaylist(index);
  }    
  else if (strcmp(cmd, "play") == 0)
  {
    ar.controlPlayback(AdvancedRemote::PLAYBACK_CONTROL_PLAY_PAUSE);
  }    
  else if (strcmp(cmd, "stop") == 0)
  {
    ar.controlPlayback(AdvancedRemote::PLAYBACK_CONTROL_STOP);
  }    
  else if (strcmp(cmd, "poll-start") == 0)
  {
    ar.setPollingMode(AdvancedRemote::POLLING_START);
  }    
  else if (strcmp(cmd, "poll-stop") == 0)
  {
    ar.setPollingMode(AdvancedRemote::POLLING_STOP);
  }    
  else if ((index = getIndex(cmd, "switch-")) != -1)
  {
    ar.executeSwitch(index);
  }
  else if (strcmp(cmd, "switch") == 0)
  {
    ar.executeSwitch(0xFFFFFFFF);
  }
  else if ((index = getIndex(cmd, "pl-info-")) != -1)
  {
    ar.getItemNames(AdvancedRemote::ITEM_PLAYLIST, index, 1);
    ar.switchToItem(AdvancedRemote::ITEM_PLAYLIST, index);
    ar.getSongCountInCurrentPlaylist();
    ar.getPlaylistPosition();
    ar.getItemCount(AdvancedRemote::ITEM_ARTIST);
    ar.getItemCount(AdvancedRemote::ITEM_ALBUM);
    ar.getItemCount(AdvancedRemote::ITEM_SONG);
    ar.getItemCount(AdvancedRemote::ITEM_GENRE);
    ar.getItemCount(AdvancedRemote::ITEM_COMPOSER);
  }
  else
  {
    c.println("{\"huh\"}");
  }

}

void setup()
{
  Ethernet.begin(MAC_ADDRESS, IP_ADDRESS);
  server.begin();

  // register callback functions for the things we're going to read
  ar.setFeedbackHandler(feedbackHandler);
  ar.setiPodNameHandler(iPodNameHandler);
  ar.setTitleHandler(titleHandler);
  ar.setArtistHandler(artistHandler);
  ar.setAlbumHandler(albumHandler);
  ar.setPollingHandler(pollingHandler);
  ar.setCurrentPlaylistSongCountHandler(currentPlaylistSongCountHandler);
  ar.setItemCountHandler(itemCountHandler);
  ar.setItemNameHandler(itemNameHandler);
  ar.setTimeAndStatusHandler(timeAndStatusHandler);
  ar.setPlaylistPositionHandler(playlistPositionHandler);

  /*
  // non-working debug stuff
   ar.setLogPrint(clientPrint);
   ar.setDebugPrint(clientPrint);
   */

  // let the library set itself up, now we've done our configuration of it
  ar.setup();

  // start in simple remote mode
  ar.disable();
}

void loop()
{

  // service iPod
  ar.loop();

  if (c && c.connected())
  {
    // service network client
    clientLoop();
  }
  else
  {
    // look for a new network client
    Client newClient = server.available();
    if (newClient)
    {
      if (c)
      {
        c.stop();
      }

      c = newClient;
      len = 0;
    }
  }
}






















