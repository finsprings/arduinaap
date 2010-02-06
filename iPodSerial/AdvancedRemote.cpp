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
#include "AdvancedRemote.h"

AdvancedRemote::AdvancedRemote()
  : pCurrentPositionHandler(0),
    pSongTitleHandler(0),
    pTimeAndStatusHandler(0)
{
}
void AdvancedRemote::setSongTitleHandler(SongTitleHandler newSongTitleHandler)
{
    pSongTitleHandler = newSongTitleHandler;
}

void AdvancedRemote::setTimeAndStatusHandler(TimeAndStatusHandler newTimeAndStatusHandler)
{
    pTimeAndStatusHandler = newTimeAndStatusHandler;
}

void AdvancedRemote::setCurrentPositionHandler(CurrentPositionHandler newCurrentPositionHandler)
{
    pCurrentPositionHandler = newCurrentPositionHandler;
}

void AdvancedRemote::processData()
{
    if (dataBuffer[0] != ADVANCED_REMOTE_MODE)
    {
        if (pPrint)
        {
            pPrint->println("response not for adv mode so ignoring");
            dumpReceive();
        }

        return;
    }

    if (dataBuffer[1] != 0x00)
    {
        if (pPrint)
        {
            pPrint->println("1st cmd byte in response not 0x00 so ignoring");
            dumpReceive();
        }

        return;
    }

    // -1 because response number is always cmd number + 1
    switch (dataBuffer[2]-1)
    {
    case CMD_CURRENT_POSITION:
        if (pCurrentPositionHandler)
        {
            pCurrentPositionHandler(endianConvert(&dataBuffer[3]));
        }
        break;

    case CMD_SONG_TITLE:
        if (pSongTitleHandler)
        {
            pSongTitleHandler((char*) &dataBuffer[3]);
        }
        break;

    case CMD_TIME_AND_STATUS_INFO:
        if (pTimeAndStatusHandler)
        {
            pTimeAndStatusHandler(
                endianConvert(&dataBuffer[3]),
                endianConvert(&dataBuffer[7]),
                (Status) dataBuffer[11]);
        }
        break;

    default:
        if (pPrint)
        {
            pPrint->print("unsupported response for cmd: ");
            pPrint->println(dataBuffer[2], HEX);
            dumpReceive();
        }
        break;
    }
}

/*
 * iPod is big endian and arduino is little endian,
 * so we must byte swap the iPod's 4-byte integers
 * before we can use them on the arduino.
 * note that on arduino int is 16-bit and long is 32-bit.
 * TODO: does that ever change from arduino board to board?
 */
unsigned long AdvancedRemote::endianConvert(const byte *p)
{
    return
        (((unsigned long) p[3]) << 0)  |
        (((unsigned long) p[2]) << 8)  |
        (((unsigned long) p[1]) << 16) |
        (((unsigned long) p[0]) << 24);
}

void AdvancedRemote::askForPos()
{
    if (pPrint)
    {
        pPrint->println("switch to adv");
    }

    sendCommand(MODE_SWITCHING_MODE, 0x01, ADVANCED_REMOTE_MODE);
    delay(250);
    sendCommandWithParam(ADVANCED_REMOTE_MODE, 0x00, 0x20, 1);
    delay(250);
    sendCommand(ADVANCED_REMOTE_MODE, 0x00, 0x1C);
    delay(250);
    sendCommand(ADVANCED_REMOTE_MODE, 0x00, 0x1E);
    delay(250);
}
