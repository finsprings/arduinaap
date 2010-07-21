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
 ******************************************************************************/
#include "iPodSerial.h"
#include <ctype.h>

static const char *STATE_NAME[] =
{
    "Waiting for Header 1",
    "Waiting for Header 2",
    "Waiting for length",
    "Waiting for data",
    "Waiting for checksum"
};

iPodSerial::iPodSerial(HardwareSerial &serial)
    : receiveState(WAITING_FOR_HEADER1),
      dataSize(0),
      pData(0),
      checksum(0),
      pSerial(&serial) // default to regular serial port as that's all most Arduinos have
#if defined(IPOD_SERIAL_DEBUG)
    ,
      pDebugPrint(0),   // default to no debug, since most Arduinos don't have a spare serial to use for debug
      pLogPrint(0)      // default to no log, since most Arduinos don't have a spare serial to use for debug
#endif
{
}

void iPodSerial::setSerial(HardwareSerial &newiPodSerial)
{
    pSerial = &newiPodSerial;
}

#if defined(IPOD_SERIAL_DEBUG)
void iPodSerial::setDebugPrint(Print &newPrint)
{
    pDebugPrint = &newPrint;
    pDebugPrint->println("Debug Print now set");
}

void iPodSerial::setLogPrint(Print &newPrint)
{
    pLogPrint = &newPrint;

    if (pDebugPrint)
    {
        pDebugPrint->println("Debug Print now set");
    }
}
#endif

void iPodSerial::setup()
{
    pSerial->begin(IPOD_SERIAL_RATE);
}

bool iPodSerial::validChecksum(byte actual)
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
#if defined(IPOD_SERIAL_DEBUG)
        if (pDebugPrint)
        {
            pDebugPrint->print("checksum mismatch: expected ");
            pDebugPrint->print(expected, HEX);
            pDebugPrint->print(" but got ");
            pDebugPrint->println(actual, HEX);
        }
#endif

        return false;
    }
}

#if defined(IPOD_SERIAL_DEBUG)
void iPodSerial::dumpReceive()
{
    if (!pDebugPrint)
    {
        return;
    }

    pDebugPrint->print("data size = ");
    pDebugPrint->println(dataSize, DEC);

    for (size_t i = 0; i < sizeof(dataBuffer); ++i)
    {
        pDebugPrint->print("dataBuffer[");
        pDebugPrint->print(i, DEC);
        pDebugPrint->print("] = ");
        pDebugPrint->println(dataBuffer[i], HEX);
    }
}
#endif

void iPodSerial::processResponse()
{
    const int avail = pSerial->available();
    if (avail <= 0)
    {
        return;
    }

    // read a single byte from the iPod
    const int b = pSerial->read();

#if defined(IPOD_SERIAL_DEBUG)
    if (pDebugPrint)
    {
        pDebugPrint->print("Receive Status: ");
        pDebugPrint->println(STATE_NAME[receiveState]);

        pDebugPrint->print(b, HEX);
        pDebugPrint->print(" ");

        if (isprint(b))
        {
            pDebugPrint->print(b, BYTE);
        }
        pDebugPrint->println();
    }
#endif

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
#if defined(IPOD_SERIAL_DEBUG)
        if (pDebugPrint)
        {
            pDebugPrint->print("Data length is ");
            pDebugPrint->println(dataSize, DEC);
        }
#endif
        break;

    case WAITING_FOR_DATA:
        *pData++ = b;

        if ((pData - dataBuffer) == dataSize)
        {
            receiveState = WAITING_FOR_CHECKSUM;
        }
#if defined(IPOD_SERIAL_DEBUG)
        else
        {
            if (pDebugPrint)
            {
                pDebugPrint->print("Waiting for ");
                pDebugPrint->println(dataSize - (pData - dataBuffer));
            }
        }
#endif
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

void iPodSerial::sendCommandWithLength(
    size_t length,
    const byte *pData)
{
#if defined(IPOD_SERIAL_DEBUG)
    if (pDebugPrint)
    {
        pDebugPrint->print("Sending command of length: ");
        pDebugPrint->println(length, DEC);
    }
#endif

    sendHeader();
    sendLength(length);
    sendBytes(length, pData);
    sendChecksum();
}

void iPodSerial::sendCommand(
    byte mode,
    byte cmdByte1,
    byte cmdByte2)
{
    sendHeader();
    sendLength(1 + 1 + 1);
    sendByte(mode);
    sendByte(cmdByte1);
    sendByte(cmdByte2);
    sendChecksum();
}

void iPodSerial::sendCommandWithOneByteParam(
    byte mode,
    byte cmdByte1,
    byte cmdByte2,
    byte byteParam)
{
    sendHeader();
    sendLength(1 + 1 + 1 + 1);
    sendByte(mode);
    sendByte(cmdByte1);
    sendByte(cmdByte2);
    sendByte(byteParam);
    sendChecksum();
}

void iPodSerial::sendCommandWithOneNumberParam(
    byte mode,
    byte cmdByte1,
    byte cmdByte2,
    unsigned long numberParam)
{
    sendHeader();
    sendLength(1 + 1 + 1 + 4);
    sendByte(mode);
    sendByte(cmdByte1);
    sendByte(cmdByte2);
    sendNumber(numberParam);
    sendChecksum();
}

void iPodSerial::sendCommandWithOneByteAndOneNumberParam(
    byte mode,
    byte cmdByte1,
    byte cmdByte2,
    byte byteParam1,
    unsigned long numberParam2)
{
    sendHeader();
    sendLength(1 + 1 + 1 + 1 + (1 * 4));
    sendByte(mode);
    sendByte(cmdByte1);
    sendByte(cmdByte2);
    sendByte(byteParam1);
    sendNumber(numberParam2);
    sendChecksum();
}

void iPodSerial::sendCommandWithOneByteAndTwoNumberParams(
    byte mode,
    byte cmdByte1,
    byte cmdByte2,
    byte byteParam1,
    unsigned long numberParam2,
    unsigned long numberParam3)
{
    sendHeader();
    sendLength(1 + 1 + 1 + 1 + (2 * 4));
    sendByte(mode);
    sendByte(cmdByte1);
    sendByte(cmdByte2);
    sendByte(byteParam1);
    sendNumber(numberParam2);
    sendNumber(numberParam3);
    sendChecksum();
}

void iPodSerial::sendHeader()
{
    sendByte(HEADER1);
    sendByte(HEADER2);
}

void iPodSerial::sendLength(size_t length) // length is mode+command+parameters in bytes
{
    sendByte(length);
    checksum = length;
}

void iPodSerial::sendBytes(size_t length, const byte *pData)
{
    for (size_t i = 0; i < length; i++)
    {
        sendByte(pData[i]);
    }
}

void iPodSerial::sendByte(byte b)
{
    pSerial->write(b);
    checksum += b;

#if defined(IPOD_SERIAL_DEBUG)
    // likely to slow stuff down!
    if (pDebugPrint)
    {
        pDebugPrint->print("sent byte ");
        pDebugPrint->println(b, HEX);
    }
#endif
}

void iPodSerial::sendNumber(unsigned long n)
{
    // parameter (4-byte int sent big-endian)
    sendByte((n & 0xFF000000) >> 24);
    sendByte((n & 0x00FF0000) >> 16);
    sendByte((n & 0x0000FF00) >> 8);
    sendByte((n & 0x000000FF) >> 0);
}

void iPodSerial::sendChecksum()
{
    sendByte((0x100 - checksum) & 0xFF);
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
#if defined(IPOD_SERIAL_DEBUG)
    if (pDebugPrint)
    {
        pDebugPrint->println("Ignoring data from iPod:");
        dumpReceive();
    }
#endif
}

#if defined(IPOD_SERIAL_DEBUG)
void iPodSerial::log(const char *message)
{
    if (pLogPrint)
    {
        pLogPrint->println(message);
    }
}
#endif
