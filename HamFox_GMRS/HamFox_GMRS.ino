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

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    
    pinMode(LED_BUILTIN, OUTPUT);
    
    voice.say(sp2_START);
    voice.say(spPAUSE2);
    
}
void loop() {

    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(2, LOW);
    voice.say(sp2_W);
    voice.say(sp2_R);
    voice.say(sp2_D);
    voice.say(sp2_G);
    voice.say(sp2_TWO);
    voice.say(sp2_NINE);
    voice.say(sp2_THREE);
    voice.say(sp2_AUTOMATIC);
    voice.say(sp2_F);
    voice.say(sp2_O);
    voice.say(sp2_X);
    voice.say(spPAUSE2);
    voice.say(sp2_REPEAT);
    voice.say(spPAUSE1);
    voice.say(sp2_WHISKY);
    voice.say(sp2_ROMEO);
    voice.say(sp2_DELTA);
    voice.say(sp2_GOLF);
    voice.say(sp2_TWO);
    voice.say(sp2_NINE);
    voice.say(sp2_THREE);
    voice.say(sp2_AUTOMATIC);
    voice.say(sp2_F);
    voice.say(sp2_O);
    voice.say(sp2_X);
     digitalWrite(2, HIGH);
   digitalWrite(LED_BUILTIN, LOW);
    
    delay(10000);
  
}
