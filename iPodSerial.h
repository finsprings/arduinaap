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

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
 * Helper macro for figuring out the length of command byte arrays.
 */
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

// Uncomment the line below if you want to be able to use
// setLogPrint and setDebugPrint
//#define IPOD_SERIAL_DEBUG

class iPodSerial
{
public: // attributes
    static const int IPOD_SERIAL_RATE = 19200;

public:
    iPodSerial();

    /**
     * Checks for data coming in from the iPod and processes it if there is any.
     * The library handles partial messages coming in from the iPod so it
     * will buffer those chunks until it gets a complete message. Once a complete
     * message (that the library understands) has been received it will process
     * that message in here and call any callbacks that are applicable to that
     * message and that have been configured.
     */
    void loop();

    /**
     * Sets the serial port that the library will use to communicate with the iPod.
     * This defaults to "Serial", i.e. the normal serial port.
     */
    void setSerial(Stream &newiPodSerial);

#if defined(IPOD_SERIAL_DEBUG)
    /**
     * Sets the Print object to which debug messages will be directed.
     * By default there isn't one and so debug messages are off.
     */
    void setDebugPrint(Print &newDebugSerial);

    /**
     * Sets the Print object to which log messages will be directed.
     * By default there isn't one and so log messages are off.
     */
    void setLogPrint(Print &newLogSerial);
#endif

protected: // attributes
    static const byte MODE_SWITCHING_MODE = 0x00;
    static const byte SIMPLE_REMOTE_MODE = 0x02;
    static const byte ADVANCED_REMOTE_MODE = 0x04;

    byte dataSize;
    byte dataBuffer[128]; // TODO: Why did I pick 128?
#if defined(IPOD_SERIAL_DEBUG)
    Print *pDebugPrint;
    Print *pLogPrint;
#endif

protected: // methods
#if defined(IPOD_SERIAL_DEBUG)
    /**
     * convenience function to print out a debug message
     */
    void log(const char *);

    void dumpReceive();
#endif

    /*
     * Different flavours of command-sending method to keep
     * it simple for the other classes. They're a bit silly,
     * and you wouldn't want many more than there are, but they
     * suffice for the simple and advanced remotes.
     */
    void sendCommandWithLength(size_t length, const byte *pData);
    void sendCommand(
        byte mode,
        byte cmdByte1,
        byte cmdByte2);
    void sendCommandWithOneByteParam(
        byte mode,
        byte cmdByte1,
        byte cmdByte2,
        byte param);
    void sendCommandWithOneNumberParam(
        byte mode,
        byte cmdByte1,
        byte cmdByte2,
        unsigned long param);
    void sendCommandWithOneByteAndOneNumberParam(
        byte mode,
        byte cmdByte1,
        byte cmdByte2,
        byte param1,
        unsigned long param2);
    void sendCommandWithOneByteAndTwoNumberParams(
        byte mode,
        byte cmdByte1,
        byte cmdByte2,
        byte param1,
        unsigned long param2,
        unsigned long param3);

private: // attributes
    static const byte HEADER1 = 0xFF;
    static const byte HEADER2 = 0x55;

    enum ReceiveState
    {
        WAITING_FOR_HEADER1 = 0,
        WAITING_FOR_HEADER2,
        WAITING_FOR_LENGTH,
        WAITING_FOR_DATA,
        WAITING_FOR_CHECKSUM
    };
    ReceiveState receiveState;
    byte *pData;
    byte checksum;

    Stream *pSerial;

private: // methods
    void sendHeader();
    void sendLength(size_t length);
    void sendBytes(size_t length, const byte *pData);
    void sendByte(byte b);
    void sendNumber(unsigned long n);
    void sendChecksum();
    bool validChecksum(const byte actual);
    void processResponse();

    virtual void processData();
};

#endif // IPOD_SERIAL
