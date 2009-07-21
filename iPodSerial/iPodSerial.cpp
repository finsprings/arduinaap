#include "iPodSerial.h"
#include <ctype.h>

iPodSerial::iPodSerial()
    : receiveState(WAITING_FOR_HEADER1),
      dataSize(0),
      pData(0),
      checksum(0),
      pSerial(&Serial), // default to regular serial port as that's all most Arduinos have
      pPrint(0),        // default to no debug, since most Arduinos don't have a spare serial to use for debug
      pCurrentPositionHandler(0),
      pSongTitleHandler(0),
      pTimeAndStatusHandler(0)
{
}

void iPodSerial::setSerial(HardwareSerial &newiPodSerial)
{
    pSerial = &newiPodSerial;    
}

void iPodSerial::setDebugPrint(Print &newPrint)
{
    pPrint = &newPrint;
}

void iPodSerial::setSongTitleHandler(SongTitleHandler newSongTitleHandler)
{
    pSongTitleHandler = newSongTitleHandler;
}

void iPodSerial::setTimeAndStatusHandler(TimeAndStatusHandler newTimeAndStatusHandler)
{
    pTimeAndStatusHandler = newTimeAndStatusHandler;
}

void iPodSerial::setCurrentPositionHandler(CurrentPositionHandler newCurrentPositionHandler)
{
    pCurrentPositionHandler = newCurrentPositionHandler;
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

/*
 * iPod is big endian and arduino is little endian,
 * so we must byte swap the iPod's 4-byte integers
 * before we can use them on the arduino.
 * note that on arduino int is 16-bit and long is 32-bit.
 * TODO: does that ever change from arduino board to board?
 */
unsigned long iPodSerial::endianConvert(const byte *p)
{
    return
        (((unsigned long) p[3]) << 0)  |
        (((unsigned long) p[2]) << 8)  |
        (((unsigned long) p[1]) << 16) |
        (((unsigned long) p[0]) << 24);
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

void iPodSerial::processData()
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

void iPodSerial::processResponse()
{
    const int avail = pSerial->available();
    if (avail <= 0)
    {
        return; 
    }
    
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

void iPodSerial::sendPlay()
{
    if (pPrint)
    {
        pPrint->println("Sending play");
    }
    
    sendCommand(SIMPLE_REMOTE_MODE, PLAY_CMD_1, PLAY_CMD_2);
}

void iPodSerial::sendRelease()
{
    if (pPrint)
    {
        pPrint->println("Sending release");
    }
    
    sendCommand(SIMPLE_REMOTE_MODE, BUTTON_RELEASED_CMD_1, BUTTON_RELEASED_CMD_2);
}

// TODO: This is too wrapped-up. Expose the individual parts of this to the user instead.
void iPodSerial::askForPos()
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

void iPodSerial::loop()
{
    if (pSerial->available() > 0)
    {
        processResponse();
    }
}
