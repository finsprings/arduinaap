// Example of Simple Remote that exposes an ethernet command interface.
// Look at the clientLoop() function to see what it understands.

#include <Ethernet.h>
#include <SimpleRemote.h>

SimpleRemote sr;

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

  if (strcmp(cmd, "play") == 0)
  {
    sr.sendPlay();
    c.println("sent play");
  }
  else if (strcmp(cmd, "vol-plus") == 0)
  {
    sr.sendVolPlus();
    c.println("sent vol plus");
  }
  else if (strcmp(cmd, "vol-minus") == 0)
  {
    sr.sendVolMinus();
    c.println("sent vol minus");
  }
  else if (strcmp(cmd, "skip-forward") == 0)
  {
    sr.sendSkipForward();
    c.println("sent skip forward");
  }
  else if (strcmp(cmd, "skip-backward") == 0)
  {
    sr.sendSkipBackward();
    c.println("sent skip backward");
  }
  else if (strcmp(cmd, "next-album") == 0)
  {
    sr.sendNextAlbum();
    c.println("sent next album");
  }
  else if (strcmp(cmd, "previous-album") == 0)
  {
    sr.sendPreviousAlbum();
    c.println("sent previous album");
  }
  else if (strcmp(cmd, "stop") == 0)
  {
    sr.sendStop();
    c.println("sent stop");
  }
  else if (strcmp(cmd, "just-play") == 0)
  {
    sr.sendJustPlay();
    c.println("sent just play");
  }
  else if (strcmp(cmd, "just-pause") == 0)
  {
    sr.sendJustPause();
    c.println("sent just pause");
  }
  else if (strcmp(cmd, "toggle-mute") == 0)
  {
    sr.sendToggleMute();
    c.println("sent toggle mute");
  }
  else if (strcmp(cmd, "next-playlist") == 0)
  {
    sr.sendNextPlaylist();
    c.println("sent next playlist");
  }
  else if (strcmp(cmd, "previous-playlist") == 0)
  {
    sr.sendPreviousPlaylist();
    c.println("sent previous playlist");
  }
  else if (strcmp(cmd, "toggle-shuffle") == 0)
  {
    sr.sendToggleShuffle();
    c.println("sent toggle shuffle");
  }
  else if (strcmp(cmd, "toggle-repeat") == 0)
  {
    sr.sendToggleRepeat();
    c.println("sent toggle repeat");
  }
  else if (strcmp(cmd, "ipod-off") == 0)
  {
    sr.sendiPodOff();
    c.println("sent ipod off");
  }
  else if (strcmp(cmd, "ipod-on") == 0)
  {
    sr.sendiPodOn();
    c.println("sent ipod on");
  }
  else if (strcmp(cmd, "menu-button") == 0)
  {
    sr.sendMenuButton();
    c.println("sent menu button");
  }
  else if (strcmp(cmd, "ok-select-button") == 0)
  {
    sr.sendOkSelectButton();
    c.println("sent ok select button");
  }
  else if (strcmp(cmd, "scroll-up") == 0)
  {
    sr.sendScrollUp();
    c.println("sent scroll up");
  }
  else if (strcmp(cmd, "scroll-down") == 0)
  {
    sr.sendScrollDown();
    c.println("sent scroll down");
  }
  else
  {
    c.println("?");
  }

  // for now just do one press of each button
  sr.sendButtonReleased();
}

void setup()
{
  Ethernet.begin(MAC_ADDRESS, IP_ADDRESS);
  server.begin();

  // let the library set itself up, now we've done our configuration of it
  sr.setup();
}

void loop()
{
  // service iPod
  sr.loop();

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
