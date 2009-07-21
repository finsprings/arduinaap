#ifndef IPOD_SERIAL
#define IPOD_SERIAL

#include <WProgram.h>

class iPodSerial
{
public:
    iPodSerial();
    
    void setSerial(HardwareSerial &newiPodSerial);
    void setDebugPrint(Print &newDebugSerial);

    typedef void SongTitleHandler(const char *title);
    void setSongTitleHandler(SongTitleHandler newSongTitleHandler);

    enum Status
    {
        STATUS_STOP = 0,
        STATUS_PLAYING = 1,
        STATUS_PAUSED = 2
    };
    typedef void TimeAndStatusHandler(unsigned long length, unsigned long elapsed, Status status);
    void setTimeAndStatusHandler(TimeAndStatusHandler newTimeAndStatusHandler);

    typedef void CurrentPositionHandler(unsigned long position);
    void setCurrentPositionHandler(CurrentPositionHandler newCurrentPositionHandler);

    void sendPlay();
    void sendRelease();
    void askForPos();

    void setup();
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
