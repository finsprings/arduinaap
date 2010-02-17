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
#include "AdvancedRemote.h"

AdvancedRemote::AdvancedRemote()
    : pFeedbackHandler(0),
      piPodNameHandler(0),
      pItemCountHandler(0),
      pItemNameHandler(0),
      pTimeAndStatusHandler(0),
      pPlaylistPositionHandler(0),
      pTitleHandler(0),
      pArtistHandler(0),
      pAlbumHandler(0),
      pPollingHandler(0),
      pShuffleModeHandler(0),
      pRepeatModeHandler(0),
      pCurrentPlaylistSongCountHandler(0),
      currentlyEnabled(false)
{
}

void AdvancedRemote::setFeedbackHandler(FeedbackHandler_t newHandler)
{
    pFeedbackHandler = newHandler;
}

void AdvancedRemote::setiPodNameHandler(iPodNameHandler_t newHandler)
{
    piPodNameHandler = newHandler;
}

void AdvancedRemote::setItemCountHandler(ItemCountHandler_t newHandler)
{
    pItemCountHandler = newHandler;
}

void AdvancedRemote::setItemNameHandler(ItemNameHandler_t newHandler)
{
    pItemNameHandler = newHandler;
}

void AdvancedRemote::setTimeAndStatusHandler(TimeAndStatusHandler_t newHandler)
{
    pTimeAndStatusHandler = newHandler;
}

void AdvancedRemote::setPlaylistPositionHandler(PlaylistPositionHandler_t newHandler)
{
    pPlaylistPositionHandler = newHandler;
}

void AdvancedRemote::setTitleHandler(TitleHandler_t newHandler)
{
    pTitleHandler = newHandler;
}

void AdvancedRemote::setArtistHandler(ArtistHandler_t newHandler)
{
    pArtistHandler = newHandler;
}

void AdvancedRemote::setAlbumHandler(AlbumHandler_t newHandler)
{
    pAlbumHandler = newHandler;
}

void AdvancedRemote::setPollingHandler(PollingHandler_t newHandler)
{
    pPollingHandler = newHandler;
}

void AdvancedRemote::setShuffleModeHandler(ShuffleModeHandler_t newHandler)
{
    pShuffleModeHandler = newHandler;
}

void AdvancedRemote::setRepeatModeHandler(RepeatModeHandler_t newHandler)
{
    pRepeatModeHandler = newHandler;
}

void AdvancedRemote::setCurrentPlaylistSongCountHandler(CurrentPlaylistSongCountHandler_t newHandler)
{
    pCurrentPlaylistSongCountHandler = newHandler;
}

void AdvancedRemote::enable()
{
    log("enabling advanced remote mode");
    sendCommand(MODE_SWITCHING_MODE, 0x01, ADVANCED_REMOTE_MODE);
    currentlyEnabled = true; // strictly it's not until we get feedback of success
}

void AdvancedRemote::disable()
{
    log("disabling advanced remote mode");
    sendCommand(MODE_SWITCHING_MODE, 0x01, SIMPLE_REMOTE_MODE);
    currentlyEnabled = false; // strictly it's not until we get feedback of success
}

void AdvancedRemote::getiPodName()
{
    log("getiPodName");
    sendCommand(ADVANCED_REMOTE_MODE, 0x00, CMD_GET_IPOD_NAME);
}

void AdvancedRemote::switchToMainLibraryPlaylist()
{
    log("switchToMainLibraryPlaylist");
    sendCommand(ADVANCED_REMOTE_MODE, 0x00, CMD_SWITCH_TO_MAIN_LIBRARY_PLAYLIST);
}

void AdvancedRemote::switchToItem(AdvancedRemote::ItemType itemType, long index)
{
    log("switchToItem");
    sendCommandWithOneByteAndOneNumberParam(ADVANCED_REMOTE_MODE, 0x00, CMD_SWITCH_TO_ITEM, itemType, index);
}

void AdvancedRemote::getItemCount(AdvancedRemote::ItemType itemType)
{
    log("getItemCount");
    sendCommandWithOneByteParam(ADVANCED_REMOTE_MODE, 0x00, CMD_GET_ITEM_COUNT, itemType);
}

void AdvancedRemote::getItemNames(AdvancedRemote::ItemType itemType, unsigned long offset, unsigned long count)
{
    log("getItemNames");
     sendCommandWithOneByteAndTwoNumberParams(ADVANCED_REMOTE_MODE, 0x00, CMD_GET_ITEM_NAMES, itemType, offset, count);
}

void AdvancedRemote::getTimeAndStatusInfo()
{
    log("getTimeAndStatusInfo");
    sendCommand(ADVANCED_REMOTE_MODE, 0x00, CMD_GET_TIME_AND_STATUS_INFO);
}

void AdvancedRemote::getPlaylistPosition()
{
    log("getPlaylistPosition");
    sendCommand(ADVANCED_REMOTE_MODE, 0x00, CMD_GET_PLAYLIST_POSITION);
}

void AdvancedRemote::getTitle(unsigned long index)
{
    log("getTitle");
    sendCommandWithOneNumberParam(ADVANCED_REMOTE_MODE, 0x00, CMD_GET_TITLE, index);
}

void AdvancedRemote::getArtist(unsigned long index)
{
    log("getArtist");
    sendCommandWithOneNumberParam(ADVANCED_REMOTE_MODE, 0x00, CMD_GET_ARTIST, index);
}

void AdvancedRemote::getAlbum(unsigned long index)
{
    log("getAlbum");
    sendCommandWithOneNumberParam(ADVANCED_REMOTE_MODE, 0x00, CMD_GET_ALBUM, index);
}

void AdvancedRemote::setPollingMode(AdvancedRemote::PollingMode newMode)
{
    log("setPollingMode");
    sendCommandWithOneByteParam(ADVANCED_REMOTE_MODE, 0x00, CMD_POLLING_MODE, newMode);
}

void AdvancedRemote::controlPlayback(AdvancedRemote::PlaybackControl command)
{
    log("controlPlayback");
    sendCommandWithOneByteParam(ADVANCED_REMOTE_MODE, 0x00, CMD_PLAYBACK_CONTROL, command);
}

void AdvancedRemote::getShuffleMode()
{
    log("getShuffleMode");
    sendCommand(ADVANCED_REMOTE_MODE, 0x00, CMD_GET_SHUFFLE_MODE);
}

void AdvancedRemote::setShuffleMode(AdvancedRemote::ShuffleMode newMode)
{
    log("setShuffleMode");
    sendCommandWithOneByteParam(ADVANCED_REMOTE_MODE, 0x00, CMD_SET_SHUFFLE_MODE, newMode);
}

void AdvancedRemote::getRepeatMode()
{
    log("getRepeatMode");
    sendCommand(ADVANCED_REMOTE_MODE, 0x00, CMD_GET_REPEAT_MODE);
}

void AdvancedRemote::setRepeatMode(AdvancedRemote::RepeatMode newMode)
{
    log("setRepeatMode");
    sendCommandWithOneByteParam(ADVANCED_REMOTE_MODE, 0x00, CMD_SET_REPEAT_MODE, newMode);
}

void AdvancedRemote::getSongCountInCurrentPlaylist()
{
    log("getSongCountInCurrentPlaylist");
    sendCommand(ADVANCED_REMOTE_MODE, 0x00, CMD_GET_SONG_COUNT_IN_CURRENT_PLAYLIST);
}

void AdvancedRemote::jumpToSongInCurrentPlaylist(unsigned long index)
{
    log("jumpToSongInCurrentPlaylist");
    sendCommandWithOneNumberParam(ADVANCED_REMOTE_MODE, 0x00, CMD_JUMP_TO_SONG_IN_CURRENT_PLAYLIST, index);
}

void AdvancedRemote::processData()
{
    const byte mode = dataBuffer[0];
    if (mode != ADVANCED_REMOTE_MODE)
    {
        if (pDebugPrint)
        {
            pDebugPrint->println("response not for adv mode so ignoring");
            dumpReceive();
        }

        return;
    }

    const byte firstCommandByte = dataBuffer[1];
    if (firstCommandByte != 0x00)
    {
        if (pDebugPrint)
        {
            pDebugPrint->println("1st cmd byte in response not 0x00 so ignoring");
            dumpReceive();
        }

        return;
    }

    const byte secondCommandByte = dataBuffer[2];
    switch (secondCommandByte)
    {
    case RESPONSE_BAD:
        if (pDebugPrint)
        {
            pDebugPrint->print("BAD Response: Result=0x");
            pDebugPrint->print(dataBuffer[3], HEX);
            pDebugPrint->print(", Command=0x");
            pDebugPrint->print(dataBuffer[4], HEX);
            pDebugPrint->print(", 0x");
            pDebugPrint->println(dataBuffer[5], HEX);
        }
        return;

    case RESPONSE_FEEDBACK:
        /*
         * Result:
         * 0=success,
         * 2=failure,
         * 4=you exceeded the limit of whatever you were requesting/wrong parameter-count,
         * 5=sent an iPod Response instead of a command
         */
        if (pDebugPrint)
        {
            pDebugPrint->print("Feedback Response: Result=0x");
            pDebugPrint->print(dataBuffer[3], HEX);
            pDebugPrint->print(", Command=0x");
            pDebugPrint->print(dataBuffer[4], HEX);
            pDebugPrint->print(", 0x");
            pDebugPrint->println(dataBuffer[5], HEX);
        }

        if (pFeedbackHandler)
        {
            const Feedback feedback = (Feedback) dataBuffer[3];
            pFeedbackHandler(feedback, dataBuffer[5]);
        }
        return;
    }
    // if we made it past that, hopefully this is a response
    // to a command that we sent

    const byte commandThisIsAResponseFor = secondCommandByte - 1;
    // -1 because response number is always cmd number + 1

    const byte *pData = &dataBuffer[3];
    switch (commandThisIsAResponseFor)
    {
    case CMD_GET_IPOD_NAME:
        if (piPodNameHandler)   
        {
            piPodNameHandler((const char *) pData);
        }
        break;

    case CMD_GET_ITEM_COUNT:
        if (pItemCountHandler)
        {
            pItemCountHandler(endianConvert(pData));
        }
        break;

    case CMD_GET_ITEM_NAMES:
        if (pItemNameHandler)
        {
            const unsigned long itemOffset = endianConvert(pData);
            const char *itemName = (const char *) (pData + 4);
            pItemNameHandler(itemOffset, itemName);
        }
        break;

    case CMD_GET_TIME_AND_STATUS_INFO:
        if (pTimeAndStatusHandler)
        {
            const unsigned long trackLength = endianConvert(pData);
            const unsigned long elapsedTime = endianConvert(pData + 4);
            PlaybackStatus playbackStatus = (PlaybackStatus) *(pData + 8);

            pTimeAndStatusHandler(trackLength, elapsedTime, playbackStatus);

        }
        break;

    case CMD_GET_PLAYLIST_POSITION:
        if (pPlaylistPositionHandler)
        {
            pPlaylistPositionHandler(endianConvert(pData));
        }
        break;

    case CMD_GET_TITLE:
        if (pTitleHandler)
        {
            pTitleHandler((const char *) pData);
        }
        break;


    case CMD_GET_ARTIST:
        if (pArtistHandler)
        {
            pArtistHandler((const char *) pData);
        }
        break;

    case CMD_GET_ALBUM:
        if (pAlbumHandler)
        {
            pAlbumHandler((const char *) pData);
        }
        break;

    case CMD_POLLING_MODE:
        if (pPollingHandler)
        {
            pPollingHandler(endianConvert(pData));
        }
        break;

    case CMD_GET_SHUFFLE_MODE:
        if (pShuffleModeHandler)
        {
            pShuffleModeHandler((ShuffleMode) *pData);
        }
        break;

    case CMD_GET_REPEAT_MODE:
        if (pRepeatModeHandler)
        {
            pRepeatModeHandler((RepeatMode) *pData);
        }
        break;

    case CMD_GET_SONG_COUNT_IN_CURRENT_PLAYLIST:
        if (pCurrentPlaylistSongCountHandler)
        {
            pCurrentPlaylistSongCountHandler(endianConvert(pData));
        }
        break;

    default:
        if (pDebugPrint)
        {
            pDebugPrint->print("unsupported response: ");
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
 * TODO: Is that true of all the micrcontrollers on
 * arduino boards?
 */
unsigned long AdvancedRemote::endianConvert(const byte *p)
{
    return
        (((unsigned long) p[3]) << 0)  |
        (((unsigned long) p[2]) << 8)  |
        (((unsigned long) p[1]) << 16) |
        (((unsigned long) p[0]) << 24);
}

bool AdvancedRemote::isCurrentlyEnabled()
{
    return currentlyEnabled;
}
