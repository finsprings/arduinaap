#ifndef ADVANCED_REMOTE
#define ADVANCED_REMOTE
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

#include <WProgram.h>
#include "iPodSerial.h"

class AdvancedRemote : public iPodSerial
{
public: // enums
    enum ItemType
    {
        PLAYLIST = 0x01,
        ARTIST = 0x02,
        ALBUM = 0x03,
        GENRE = 0x04,
        SONG = 0x05,
        COMPOSER = 0x06
    };

    enum PlaybackStatus
    {
        STATUS_STOPPED = 0,
        STATUS_PLAYING = 1,
        STATUS_PAUSED = 2
    };

    enum PollingMode
    {
        POLLING_START = 0x01,
        POLLING_STOP = 0x00
    };

    enum PlaybackControl
    {
        PLAYBACK_CONTROL_PLAY_PAUSE = 0x01,
        PLAYBACK_CONTROL_STOP = 0x02,
        PLAYBACK_CONTROL_SKIP_FORWARD = 0x03,
        PLAYBACK_CONTROL_SKIP_BACKWARD = 0x04,
        PLAYBACK_CONTROL_FAST_FORWARD = 0x05,
        PLAYBACK_CONTROL_REVERSE = 0x06,
        PLAYBACK_CONTROL_STOP_FF_OR_REV = 0x07
    };

    enum ShuffleMode
    {
        SHUFFLE_MODE_OFF = 0x00,
        SHUFFLE_MODE_SONGS = 0x01,
        SHUFFLE_MODE_ALBUMS = 0x02
    };

    enum RepeatMode
    {
        REPEAT_MODE_OFF = 0x00,
        REPEAT_MODE_ONE_SONG = 0x01,
        REPEAT_MODE_ALL_SONGS = 0x02
    };


public: // handler definitions
    typedef void iPodNameHandler_t(const char *ipodName);
    typedef void ItemCountHandler_t(unsigned long count);
    typedef void ItemNameHandler_t(const char *itemName);
    typedef void TimeAndStatusHandler_t(unsigned long trackLengthInMilliseconds,
                                      unsigned long elapsedTimeInMilliseconds,
                                      PlaybackStatus status);
    typedef void PlaylistPositionHandler_t(unsigned long playlistPosition);
    typedef void TitleHandler_t(const char *title);
    typedef void ArtistHandler_t(const char *artist);
    typedef void AlbumHandler_t(const char *album);
    typedef void PollingHandler_t(unsigned long elapsedTimeMs);
    typedef void ShuffleModeHandler_t(ShuffleMode mode);
    typedef void RepeatModeHandler_t(RepeatMode mode);
    typedef void CurrentPlaylistSongCountHandler_t(unsigned long count);


public: // handler setting methods; you probably want to call these from init()
    void setiPodNameHandler(iPodNameHandler_t newHandler);
    void setItemCountHandler(ItemCountHandler_t newHandler);
    void setItemNameHandler(ItemNameHandler_t newHandler);
    void setTimeAndStatusHandler(TimeAndStatusHandler_t newHandler);
    void setPlaylistPositionHandler(PlaylistPositionHandler_t newHandler);
    void setTitleHandler(TitleHandler_t newHandler);
    void setArtistHandler(ArtistHandler_t newHandler);
    void setAlbumHandler(AlbumHandler_t newHandler);
    void setPollingHandler(PollingHandler_t newHandler);
    void setShuffleModeHandler(ShuffleModeHandler_t newHandler);
    void setRepeatModeHandler(RepeatModeHandler_t newHandler);
    void setCurrentPlaylistSongCountHandler(CurrentPlaylistSongCountHandler_t newHandler);


public: // methods
    AdvancedRemote();

    /**
     * Turn on Advanced Remote mode. This causes the iPod to
     * show a check mark and "OK to disconnect" on its display.
     * The iPod can only be controlled by the remote in this mode.
     *
     * You can just reset your iPod to get it out of this mode
     * if you end up with it stuck in this mode for some reason,
     * such as your sketch not calling disable().
     *
     * You must call this before calling any of the getXXX methods.
     */
    void enable();

    /**
     * disabled Advanced Remote mode.
     *
     * You should call this to get your iPod to stop saying
     * "OK to disconnect", and let you control it through its
     * own interface again.
     */
    void disable();

    /**
     * Get the name of the iPod.
     * The response will be sent to the iPodNameHandler, if one
     * has been registered (otherwise it will be ignored).
     */
    void getiPodName();

    /**
     * Switch to the Main Library playlist (playlist 0).
     */
    void switchToMainLibraryPlaylist();

    /**
     * Switch to the item identified by the type and index given.
     */
    void switchToItem(ItemType itemType, long index);

    /**
     * Get the total number of items of the specified type.
     * The response will be sent to the ItemCountHandler, if one
     * has been registered (otherwise it will be ignored).
     */
    void getItemCount(ItemType itemType);

    /**
     * Get the names of a range of items.
     * offset is the starting item (starts at 0, not 1)
     */
    void getItemNames(ItemType itemType, unsigned long offset, unsigned long count);

    /**
     * Ask the iPod for time and playback status information.
     * The response will be sent to the TimeAndStatusHandler, if one
     * has been registered (otherwise it will be ignored).
     */
    void getTimeAndStatusInfo();

    /**
     * Ask the iPod for the current position in the current playlist.
     * The response will be sent to the PlaylistPositionHandler, if one
     * has been registered (otherwise it will be ignored).
     */
    void getPlaylistPosition();

    /**
     * ask the iPod for the title of a track.
     * The response will be sent to the TitleHandler, if one
     * has been registered (otherwise it will be ignored).
     */
    void getTitle(unsigned long index);

    /**
     * ask the iPod for the artist for a track.
     * The response will be sent to the ArtistHandler, if one
     * has been registered (otherwise it will be ignored).
     */
    void getArtist(unsigned long index);

    /**
     * ask the iPod for the album for a track.
     * The response will be sent to the AlbumHandler, if one
     * has been registered (otherwise it will be ignored).
     */
    void getAlbum(unsigned long index);

    /**
     * Start or stop polling mode.
     * Polling mode causes the iPod to return the time elapsed every 500 milliseconds.
     * These responses will be sent to the PollingHandler, if one
     * has been registered (otherwise they will be ignored).
     */
    void setPollingMode(PollingMode newMode);

    /**
     * Control playback (play/pause, etc)
     */
    void controlPlayback(PlaybackControl command);

    /**
     * Ask the iPod for the current shuffle mode.
     * The response will be sent to the ShuffleModeHandler, if one
     * has been registered (otherwise it will be ignored).
     */
    void getShuffleMode();

    /**
     * Set the shuffle mode.
     */
    void setShuffleMode(ShuffleMode newMode);

    /**
     * Ask the iPod for the current repeat mode.
     * The response will be sent to the RepeatModeHandler, if one
     * has been registered (otherwise it will be ignored).
     */
    void getRepeatMode();

    /**
     * Set the repeat mode.
     */
    void setRepeatMode(RepeatMode newMode);

    /**
     * Get the count of songs in the current playlist.
     * The response will be sent to the CurrentPlaylistSongCountHandler, if one
     * has been registered (otherwise it will be ignored).
     */
    void getSongCountInCurrentPlaylist();

    /**
     * Jump to the specified song in the current playlist.
     */
    void jumpToSongInCurrentPlaylist(unsigned long index);


private: // attributes
    static const byte RESPONSE_BAD = 0x00;
    static const byte RESPONSE_FEEDBACK = 0x01;

    static const byte CMD_GET_IPOD_NAME = 0x14;
    static const byte CMD_SWITCH_TO_MAIN_LIBRARY_PLAYLIST = 0x16;
    static const byte CMD_SWITCH_TO_ITEM = 0x17;
    static const byte CMD_GET_ITEM_COUNT = 0x18;
    static const byte CMD_GET_ITEM_NAMES = 0x1A;
    static const byte CMD_GET_TIME_AND_STATUS_INFO = 0x1C;
    static const byte CMD_GET_PLAYLIST_POSITION = 0x1E;
    static const byte CMD_GET_TITLE = 0x20;
    static const byte CMD_GET_ARTIST = 0x22;
    static const byte CMD_GET_ALBUM = 0x24;
    static const byte CMD_POLLING_MODE = 0x26;
    static const byte CMD_PLAYBACK_CONTROL = 0x29;
    static const byte CMD_GET_SHUFFLE_MODE = 0x2C;
    static const byte CMD_SET_SHUFFLE_MODE = 0x2E;
    static const byte CMD_GET_REPEAT_MODE = 0x2F;
    static const byte CMD_SET_REPEAT_MODE = 0x31;
    static const byte CMD_GET_SONG_COUNT_IN_CURRENT_PLAYLIST = 0x35;
    static const byte CMD_JUMP_TO_SONG_IN_CURRENT_PLAYLIST = 0x37;


    iPodNameHandler_t *piPodNameHandler;
    ItemCountHandler_t *pItemCountHandler;
    ItemNameHandler_t *pItemNameHandler;
    TimeAndStatusHandler_t *pTimeAndStatusHandler;
    PlaylistPositionHandler_t *pPlaylistPositionHandler;
    TitleHandler_t *pTitleHandler;
    ArtistHandler_t *pArtistHandler;
    AlbumHandler_t *pAlbumHandler;
    PollingHandler_t *pPollingHandler;
    ShuffleModeHandler_t *pShuffleModeHandler;
    RepeatModeHandler_t *pRepeatModeHandler;
    CurrentPlaylistSongCountHandler_t *pCurrentPlaylistSongCountHandler;


private: // methods
    virtual void processData();
    unsigned long endianConvert(const byte *p);
};

#endif // ADVANCED_REMOTE
