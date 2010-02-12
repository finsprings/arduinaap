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
 ** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ********************************************************************************/

// Simple example of using iPodSerial library to send
// Simple Remote Play command on the press of a button.
// This will alternate between Play and Pause on the iPod.
//
// Uses the Arduino Bounce library for debouncing.
// You can get this from http://arduino.cc/playground/code/Bounce
#include <SimpleRemote.h>
#include <Bounce.h>

const byte BUTTON_PIN = 5;
const unsigned long DEBOUNCE_MS = 20;

Bounce button(BUTTON_PIN, DEBOUNCE_MS);
SimpleRemote simpleRemote;

void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  
  // enable pull-up
  digitalWrite(BUTTON_PIN, HIGH);
  
  // if you're using a Mega you could talk to the iPod
  // on Serial3 and turn on debugging messages
  // to Serial (i.e. back to the Arduino IDE) with
  // the following three lines. Obviously for this to
  // work you'll actually need to wire the iPod serial
  // lines up to Serial3...
  //
  //simpleRemote.setSerial(Serial3);
  //Serial.begin(9600);
  //simpleRemote.setDebugPrint(Serial);

  simpleRemote.setup();
}

void loop()
{
  simpleRemote.loop();

  if (button.update())
  {
    if (button.read() == LOW)
    {
      simpleRemote.sendPlay();
    }
    else
    {
      simpleRemote.sendButtonReleased();
    }
  }
}

