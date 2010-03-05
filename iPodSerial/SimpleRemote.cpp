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
#include "SimpleRemote.h"

void SimpleRemote::sendButtonReleased()
{
    static const byte button_released[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending button_released");
#endif
    sendCommandWithLength(ARRAY_LEN(button_released), button_released);
}

void SimpleRemote::sendPlay()
{
    static const byte play[] = {SIMPLE_REMOTE_MODE, 0x00, 0x01};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending play");
#endif
    sendCommandWithLength(ARRAY_LEN(play), play);
}

void SimpleRemote::sendVolPlus()
{
    static const byte vol_plus[] = {SIMPLE_REMOTE_MODE, 0x00, 0x02};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending vol_plus");
#endif
    sendCommandWithLength(ARRAY_LEN(vol_plus), vol_plus);
}

void SimpleRemote::sendVolMinus()
{
    static const byte vol_minus[] = {SIMPLE_REMOTE_MODE, 0x00, 0x04};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending vol_minus");
#endif
    sendCommandWithLength(ARRAY_LEN(vol_minus), vol_minus);
}

void SimpleRemote::sendSkipForward()
{
    static const byte skip_forward[] = {SIMPLE_REMOTE_MODE, 0x00, 0x08};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending skip_forward");
#endif
    sendCommandWithLength(ARRAY_LEN(skip_forward), skip_forward);
}

void SimpleRemote::sendSkipBackward()
{
    static const byte skip_backward[] = {SIMPLE_REMOTE_MODE, 0x00, 0x10};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending skip_backward");
#endif
    sendCommandWithLength(ARRAY_LEN(skip_backward), skip_backward);
}

void SimpleRemote::sendNextAlbum()
{
    static const byte next_album[] = {SIMPLE_REMOTE_MODE, 0x00, 0x20};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending next_album");
#endif
    sendCommandWithLength(ARRAY_LEN(next_album), next_album);
}

void SimpleRemote::sendPreviousAlbum()
{
    static const byte previous_album[] = {SIMPLE_REMOTE_MODE, 0x00, 0x40};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending previous_album");
#endif
    sendCommandWithLength(ARRAY_LEN(previous_album), previous_album);
}

void SimpleRemote::sendStop()
{
    static const byte stop[] = {SIMPLE_REMOTE_MODE, 0x00, 0x80};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending stop");
#endif
    sendCommandWithLength(ARRAY_LEN(stop), stop);
}

void SimpleRemote::sendJustPlay()
{
    static const byte just_play[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x01};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending just_play");
#endif
    sendCommandWithLength(ARRAY_LEN(just_play), just_play);
}

void SimpleRemote::sendJustPause()
{
    static const byte just_pause[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x02};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending just_pause");
#endif
    sendCommandWithLength(ARRAY_LEN(just_pause), just_pause);
}

void SimpleRemote::sendToggleMute()
{
    static const byte toggle_mute[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x04};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending toggle_mute");
#endif
    sendCommandWithLength(ARRAY_LEN(toggle_mute), toggle_mute);
}

void SimpleRemote::sendNextPlaylist()
{
    static const byte next_playlist[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x20};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending next_playlist");
#endif
    sendCommandWithLength(ARRAY_LEN(next_playlist), next_playlist);
}

void SimpleRemote::sendPreviousPlaylist()
{
    static const byte previous_playlist[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x40};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending previous_playlist");
#endif
    sendCommandWithLength(ARRAY_LEN(previous_playlist), previous_playlist);
}

void SimpleRemote::sendToggleShuffle()
{
    static const byte toggle_shuffle[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x80};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending toggle_shuffle");
#endif
    sendCommandWithLength(ARRAY_LEN(toggle_shuffle), toggle_shuffle);
}

void SimpleRemote::sendToggleRepeat()
{
    static const byte toggle_repeat[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x00, 0x01};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending toggle_repeat");
#endif
    sendCommandWithLength(ARRAY_LEN(toggle_repeat), toggle_repeat);
}

void SimpleRemote::sendiPodOff()
{
    static const byte ipod_off[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x00, 0x04};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending ipod_off");
#endif
    sendCommandWithLength(ARRAY_LEN(ipod_off), ipod_off);
}

void SimpleRemote::sendiPodOn()
{
    static const byte ipod_on[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x00, 0x08};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending ipod_on");
#endif
    sendCommandWithLength(ARRAY_LEN(ipod_on), ipod_on);
}

void SimpleRemote::sendMenuButton()
{
    static const byte menu_button[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x00, 0x40};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending menu_button");
#endif
    sendCommandWithLength(ARRAY_LEN(menu_button), menu_button);
}

void SimpleRemote::sendOkSelectButton()
{
    static const byte ok_select_button[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x00, 0x80};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending ok_select_button");
#endif
    sendCommandWithLength(ARRAY_LEN(ok_select_button), ok_select_button);
}

void SimpleRemote::sendScrollUp()
{
    static const byte scroll_up[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x00, 0x00, 0x01};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending scroll_up");
#endif
    sendCommandWithLength(ARRAY_LEN(scroll_up), scroll_up);
}

void SimpleRemote::sendScrollDown()
{
    static const byte scroll_down[] = {SIMPLE_REMOTE_MODE, 0x00, 0x00, 0x00, 0x00, 0x02};

#if defined(IPOD_SERIAL_DEBUG)
    log("Sending scroll_down");
#endif
    sendCommandWithLength(ARRAY_LEN(scroll_down), scroll_down);
}
