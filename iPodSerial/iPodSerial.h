#ifndef IPOD_SERIAL
#define IPOD_SERIAL
/*******************************************************************************
* Copyright (c) 2009 David Findlay
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*    - Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*    - Redistributions in binary form must reproduce the above copyright
**      notice, this list of conditions and the following disclaimer in the
**      documentation and/or other materials provided with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
********************************************************************************/

#include <WProgram.h>

class iPodSerial
{
public:
    iPodSerial();
    
    /**
     * Sets the serial port that the library will use to communicate with the iPod.
     * This defaults to "Serial", i.e. the normal serial port.
     */
    void setSerial(HardwareSerial &newiPodSerial);
    
    /**
     * Sets the Print object to which debug messages will be directed.
     * By default there isn't one ando debug messages are off.
     */
    void setDebugPrint(Print &newDebugSerial);

    /**
     * Type of the callback that can receive song title information.
     * If you call this function mySongTitleHandler your function
     * definition might look like:
     *
     * void mySongTitleHandler(const char *title)
     * {
     *   // do something with the title we just got
     * }
     */
    typedef void SongTitleHandler(const char *title);
    
    /**
     * Sets the callback that the library will call when it
     * receives song title information back from the iPod.
     * (Currently this would happen after you called askForPos().)
     * Setting it overrides any previous handler.
     */
    void setSongTitleHandler(SongTitleHandler newSongTitleHandler);

    /**
     * Play status of the iPod.
     * Used by TimeAndStatusHandler
     */
    enum Status
    {
        STATUS_STOP = 0,
        STATUS_PLAYING = 1,
        STATUS_PAUSED = 2
    };
    
    /**
     * Type of the callback that can receive time and status information.
     * If you call this function myTimeAndStatusHandler your function
     * definition might look like:
     *
     * void myTimeAndStatusHandler(unsigned long length, unsigned long elapsed, Status status)
     * {
     *   // do something with the information we just got
     * }
     */
    typedef void TimeAndStatusHandler(unsigned long length, unsigned long elapsed, Status status);
    
    /**
     * Sets the callback that the library will call when it
     * receives time and status information back from the iPod.
     * (Currently this would happen after you called askForPos().)
     * Setting it overrides any previous handler.
     */
    void setTimeAndStatusHandler(TimeAndStatusHandler newTimeAndStatusHandler);

    /**
     * Type of the callback that can receive the current position in the
     * current playlist.
     * If you call this function myCurrentPositionHandler your function
     * definition might look like:
     *
     * void myCurrentPositionHandler(unsigned long position)
     * {
     *   // do something with the position information we just got
     * }
     *
     * position is the index of the currently-playing track in the current playlist
     */
    typedef void CurrentPositionHandler(unsigned long position);
    
    /**
     * Sets the callback that the library will call when it
     * receives current position information back from the iPod.
     * (Currently this would happen after you called askForPos().)
     * Setting it overrides any previous handler.
     */
    void setCurrentPositionHandler(CurrentPositionHandler newCurrentPositionHandler);

    /**
     * Sends the Simple Remote (Mode 2) 'play/pause button being pressed' command to the iPod.
     */
    void sendPlay();
    
    /**
     * Sends the Simple Remote (Mode 2) 'button released' command to the iPod.
     */
    void sendRelease();
    
    /**
     * Switches the iPod to Advanced Remote mode (Mode 4) and issues
     * the following commands:
     *
     * get-title (0x20)
     * get-time-and-status-info (0x1C)
     * get-current-position-in-playlist (0x1E)
     *
     * If no callbacks have been registered for the responses for
     * these commands, then nothing will happen when those responses
     * come in: the library will parse them when its loop() method
     * is called, and it will generate debug messages if debugging
     * has been enabled, but the responses will otherwise be
     * ignored.
     *
     * This function is really a proof-of-concept for Advanced Remote
     * mode support. If you look at http://nuxx.net/wiki/Apple_Accessory_Protocol
     * you will see there are many other commands that could/should be
     * supported.
     *
     * Also, note that placing your iPod in Advanced Remote mode
     * causes its display to change to the green check with the
     * "OK to disconnect" message. If you disconnect your connector
     * cable the iPod will stay in this mode, but it should go back
     * to its normal mode after 30 seconds or so. If not you can
     * power it off and back on to get it back into normal mode.
     * There's a change-mode command but I haven't implemented it yet.
     */
    void askForPos();

    /**
     * Initializes the library.
     * This method must be called in your sketch's setup() function.
     * You should *not* call begin on the serial port that your
     * iPod is connected to: the library will do that here since it
     * knows what baud rate it wants to talk at.
     */
    void setup();
    
    /**
     * Checks for data coming in from the iPod and processes it if there is any.
     * The library handles partial messages coming in from the iPod so it
     * will buffer those chunks until it gets a complete message. Once a complete
     * message (that the library understands) has been received it will process
     * that message in here and call any callbacks that are applicable to that
     * message and that have been configured.
     */
    void loop();

private: // attributes
    static const byte HEADER1 = 0xFF;
    static const byte HEADER2 = 0x55;

    static const byte MODE_SWITCHING_MODE = 0x00;
    static const byte SIMPLE_REMOTE_MODE = 0x02;
    static const byte ADVANCED_REMOTE_MODE = 0x04;
    static const byte PLAY_CMD_1 = 0x00;
    static const byte PLAY_CMD_2 = 0x01;
    static const byte BUTTON_RELEASED_CMD_1 = 0x00;
    static const byte BUTTON_RELEASED_CMD_2 = 0x00;
    static const int IPOD_SERIAL_RATE = 19200;
    static const byte CMD_SONG_TITLE = 0x20;
    static const byte CMD_TIME_AND_STATUS_INFO = 0x1C;
    static const byte CMD_CURRENT_POSITION = 0x1E;

    enum ReceiveState
    {
        WAITING_FOR_HEADER1,
        WAITING_FOR_HEADER2,
        WAITING_FOR_LENGTH,
        WAITING_FOR_DATA,
        WAITING_FOR_CHECKSUM
    };
    
    ReceiveState receiveState;
    byte dataSize;
    byte dataBuffer[128];
    byte *pData;
    byte checksum;
    
    HardwareSerial *pSerial;
    Print *pPrint;

    CurrentPositionHandler *pCurrentPositionHandler;
    SongTitleHandler *pSongTitleHandler;
    TimeAndStatusHandler *pTimeAndStatusHandler;
    
private: // methods
    bool validChecksum(const byte actual);
    unsigned long endianConvert(const byte *p);
    void displayTime(const char *prefix, const unsigned long ms);
    void dumpReceive();
    void processData();
    void processResponse();
    void sendCommand(
        const byte mode,
        const byte cmdByte1,
        const byte cmdByte2);
    void sendCommandWithParam(
        const byte mode,
        const byte cmdByte1,
        const byte cmdByte2,
        const unsigned long param);
};

#endif // IPOD_SERIAL
