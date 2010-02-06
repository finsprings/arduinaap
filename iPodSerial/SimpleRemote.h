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

#include <WProgram.h>
#include "iPodSerial.h"

class SimpleRemote : public iPodSerial
{
public:
    /**
     * Sends the Simple Remote (Mode 2) 'play/pause button being pressed' command to the iPod.
     */
    void sendPlay();

    /**
     * Sends the Simple Remote (Mode 2) 'button released' command to the iPod.
     */
    void sendRelease();


private: // attributes
    static const byte SIMPLE_REMOTE_MODE = 0x02;

    static const byte PLAY_CMD_1 = 0x00;
    static const byte PLAY_CMD_2 = 0x01;

    static const byte BUTTON_RELEASED_CMD_1 = 0x00;
    static const byte BUTTON_RELEASED_CMD_2 = 0x00;
};

#endif // SIMPLE_REMOTE
