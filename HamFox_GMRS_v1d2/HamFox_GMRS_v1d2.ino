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

    TransmitSixtyTimes();
      
}

void TransmitSixtyTimes() {
  
   for (int i=1; i <= 60; i++){

  digitalWrite(LED_BUILTIN, HIGH);

  //transmit
  digitalWrite(2, LOW);
  delay(750);

  //talk
    voice.say(sp2_BREAK);
    voice.say(spPAUSE2); voice.say(spPAUSE2);
    
    voice.say(sp2_AUTOMATIC);
    voice.say(sp4_FOXTROT);
    voice.say(sp2_OPERATOR);
    voice.say(spPAUSE2); voice.say(spPAUSE2);

    voice.say(sp2_ON);
    sayQNumber(&voice, 432); voice.sayQ(spPAUSE1); while (voice.isTalking()) {;}
    voice.say(sp3_POINT);
    voice.say(sp2_FOUR);
    voice.say(spPAUSE2); voice.say(spPAUSE2);

    voice.say(sp2_WHISKY);
    voice.say(sp2_PAPA);
    voice.say(sp2_FOUR);
    voice.say(sp2_ROMEO);
    voice.say(sp2_OSCAR);
    voice.say(sp2_NOVEMBER);
    voice.say(spPAUSE2); voice.say(spPAUSE2);

    voice.say(sp4_INFORMATION);
    voice.say(spPAUSE2);
    sayQNumber(&voice, i); voice.sayQ(spPAUSE1); while (voice.isTalking()) {;}
    voice.say(sp2_MINUTES);
    voice.say(spPAUSE2);
    voice.say(sp3_FOR);
    voice.say(sp3_SIXTY);
    voice.say(sp2_MINUTES);
    voice.say(sp2_TIMER);
    voice.say(spPAUSE2); voice.say(spPAUSE2);

    voice.say(sp2_AUTOMATIC);
    voice.say(sp4_FOXTROT);
    voice.say(sp2_OPERATOR);
    voice.say(spPAUSE2); voice.say(spPAUSE2);

    voice.say(sp2_WHISKY);
    voice.say(sp2_PAPA);
    voice.say(sp2_FOUR);
    voice.say(sp2_ROMEO);
    voice.say(sp2_OSCAR);
    voice.say(sp2_NOVEMBER);
    voice.say(spPAUSE2); voice.say(spPAUSE2);

    voice.say(sp3_CLEAR);
    voice.say(spPAUSE2); voice.say(spPAUSE2);

  delay(500);

  //stop transmit
   digitalWrite(2, HIGH);
   digitalWrite(LED_BUILTIN, LOW);

  //pause a minute  
  delay(60000);
 }
 
}

void loop() {

}
