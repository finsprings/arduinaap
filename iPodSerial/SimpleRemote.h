#ifndef SIMPLE_REMOTE
#define SIMPLE_REMOTE
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

/**
 * Issue Simple Remote (AAP Mode 2) commands.
 */
class SimpleRemote : public iPodSerial
{
public:
    /**
     * Send this command when the user lets go of a button.
     * So, for example, if you want to simulate the user pressing
     * Play, you should call sendPlay() when the button becomes
     * depressed, then call sendButtonReleasd() when the button
     * is let go. The same goes for all the other buttons.
     * The time between send<SomeButton>() and sendButtonReleased()
     * is significant - it can trigger press-and-hold behaviour
     * and so on.
     */
    void sendButtonReleased();

    void sendPlay();
    void sendVolPlus();
    void sendVolMinus();
    void sendSkipForward();
    void sendSkipBackward();
    void sendNextAlbum();
    void sendPreviousAlbum();
    void sendStop();
    void sendJustPlay();
    void sendJustPause();
    void sendToggleMute();
    void sendNextPlaylist();
    void sendPreviousPlaylist();
    void sendToggleShuffle();
    void sendToggleRepeat();
    void sendiPodOff();
    void sendiPodOn();
    void sendMenuButton();
    void sendOkSelectButton();
    void sendScrollUp();
    void sendScrollDown();
};

#endif // SIMPLE_REMOTE
