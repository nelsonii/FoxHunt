// Talkie library
// Copyright 2011 Peter Knight
// This code is released under GPLv2 license.

//
// Welcome to the Talkie library examples.
//
// Talkie is a speech synthesiser that works from a fixed vocabulary.
//
// There are hundreds of words in the 'Vocabulary' examples.
//
// Sound is output on digital pin 3 and/or 11. It can drive headphones directly, or add a simple audio amplifier to drive a loudspeaker.

#include <Arduino.h>

#include "Talkie.h"
#include "TalkieUtils.h"
#include "Vocab_US_Large.h"

Talkie voice;

void setup() {
//    voice.doNotUseUseInvertedOutput();
#if defined(CORE_TEENSY)
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH); //Enable Amplified PROP shield
#endif
    
    voice.say(sp2_START);
    voice.say(spPAUSE2);
}
void loop() {

    voice.say(sp2_W);
    voice.say(sp2_P);
    voice.say(sp2_FOUR);
    voice.say(sp2_R);
    voice.say(sp2_O);
    voice.say(sp2_N);
    voice.say(sp2_IS);
    voice.say(sp2_AUTOMATIC);
    voice.say(sp2_F);
    voice.say(sp2_O);
    voice.say(sp2_X);
    voice.say(sp2_DEVICE);
    voice.say(spPAUSE2);
    voice.say(sp2_WHISKY);
    voice.say(sp2_PAPA);
    voice.say(sp2_FOUR);
    voice.say(sp2_ROMEO);
    voice.say(sp2_OSCAR);
    voice.say(sp2_NOVEMBER);
    voice.say(sp2_IS);
    voice.say(sp2_AUTOMATIC);
    voice.say(sp2_F);
    voice.say(sp2_O);
    voice.say(sp2_X);
    voice.say(sp2_DEVICE);
    voice.say(spPAUSE2);

    delay(10000);
  
}
