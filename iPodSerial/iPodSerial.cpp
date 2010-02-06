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
#include "iPodSerial.h"
#include <ctype.h>

iPodSerial::iPodSerial()
    : receiveState(WAITING_FOR_HEADER1),
      dataSize(0),
      pData(0),
      checksum(0),
      pSerial(&Serial), // default to regular serial port as that's all most Arduinos have
      pPrint(0)        // default to no debug, since most Arduinos don't have a spare serial to use for debug
{
}

void iPodSerial::setSerial(HardwareSerial &newiPodSerial)
{
    pSerial = &newiPodSerial;
}

void iPodSerial::setDebugPrint(Print &newPrint)
{
    pPrint = &newPrint;
    pPrint->println("Debug Print now set");
}

void iPodSerial::setup()
{
    pSerial->begin(IPOD_SERIAL_RATE);
}

bool iPodSerial::validChecksum(const byte actual)
{
    int expected = dataSize;
    for (int i = 0; i < dataSize; ++i)
    {
        expected += dataBuffer[i];
    }

    expected = (0x100 - expected) & 0xFF;

    if (expected == actual)
    {
        return true;
    }
    else
    {
        if (pPrint)
        {
            pPrint->print("checksum mismatch: expected ");
            pPrint->print(expected, HEX);
            pPrint->print(" but got ");
            pPrint->println(actual, HEX);
        }

        return false;
    }
}

void iPodSerial::dumpReceive()
{
    if (!pPrint)
    {
        return;
    }

    pPrint->print("data size = ");
    pPrint->println(dataSize, DEC);

    for (size_t i = 0; i < sizeof(dataBuffer); ++i)
    {
        pPrint->print("dataBuffer[");
        pPrint->print(i, DEC);
        pPrint->print("] = ");
        pPrint->println(dataBuffer[i], HEX);
    }
}

void iPodSerial::processResponse()
{
    const int avail = pSerial->available();
    if (avail <= 0)
    {
        return;
    }

    // read a single byte from the iPod
    const int b = pSerial->read();
    if (pPrint)
    {
        pPrint->print(b, HEX);
        pPrint->print(" ");

        if (isprint(b))
        {
            pPrint->print(b, BYTE);
        }
        pPrint->println();

        pPrint->print("Receive State now ");
        pPrint->println(receiveState, DEC);
    }

    switch (receiveState)
    {
    case WAITING_FOR_HEADER1:
        if (b == HEADER1)
        {
            receiveState = WAITING_FOR_HEADER2;
        }
        break;

    case WAITING_FOR_HEADER2:
        if (b == HEADER2)
        {
            receiveState = WAITING_FOR_LENGTH;
        }
        break;

    case WAITING_FOR_LENGTH:
        dataSize = b;
        pData = dataBuffer;
        receiveState = WAITING_FOR_DATA;
        if (pPrint)
        {
            pPrint->print("Data length is ");
            pPrint->println(dataSize);
        }
        break;

    case WAITING_FOR_DATA:
        *pData++ = b;

        if ((pData - dataBuffer) == dataSize)
        {
            receiveState = WAITING_FOR_CHECKSUM;
        }
        else
        {
            if (pPrint)
            {
                pPrint->print("Waiting for ");
                pPrint->println(dataSize - (pData - dataBuffer));
            }
        }
        break;

    case WAITING_FOR_CHECKSUM:
        if (validChecksum(b))
        {
            processData();
        }
        receiveState = WAITING_FOR_HEADER1;
        memset(dataBuffer, 0, sizeof(dataBuffer));
        break;
    }
}

void iPodSerial::sendCommand(
    const byte mode,
    const byte cmdByte1,
    const byte cmdByte2)
{
    // header
    pSerial->print(HEADER1, BYTE);
    pSerial->print(HEADER2, BYTE);

    // length (mode + command bytes)
    pSerial->print(3, BYTE);
    checksum = 3;

    // mode
    pSerial->print(mode, BYTE);
    checksum += mode;

    // command
    pSerial->print(cmdByte1, BYTE);
    checksum += cmdByte1;

    pSerial->print(cmdByte2, BYTE);
    checksum += cmdByte2;

    pSerial->print((0x100 - checksum) & 0xFF, BYTE);
}

void iPodSerial::sendCommandWithParam(
    const byte mode,
    const byte cmdByte1,
    const byte cmdByte2,
    const unsigned long param)
{
    if (pPrint)
    {
        pPrint->print("sending command ");
        pPrint->println(cmdByte2, HEX);
    }

    // header
    pSerial->print(HEADER1, BYTE);
    pSerial->print(HEADER2, BYTE);

    // length (mode + command bytes + param)
    pSerial->print(7, BYTE);
    checksum = 7;

    // mode
    pSerial->print(mode, BYTE);
    checksum += mode;

    // command
    pSerial->print(cmdByte1, BYTE);
    checksum += cmdByte1;

    pSerial->print(cmdByte2, BYTE);
    checksum += cmdByte2;

    // parameter (4-byte int sent big-endian)
    byte b = (param >> 3) & 0xFF;
    pSerial->print(b, BYTE);
    checksum += b;

    b = (param >> 2) & 0xFF;
    pSerial->print(b, BYTE);
    checksum += b;

    b = (param >> 1) & 0xFF;
    pSerial->print(b, BYTE);
    checksum += b;

    b = (param >> 0) & 0xFF;
    pSerial->print(b, BYTE);
    checksum += b;

    pSerial->print((0x100 - checksum) & 0xFF, BYTE);
}

void iPodSerial::loop()
{
    if (pSerial->available() > 0)
    {
        processResponse();
    }
}

void iPodSerial::processData()
{
    if (pPrint)
    {
        pPrint->println("Ignoring data from iPod:");
        dumpReceive();
    }
}
