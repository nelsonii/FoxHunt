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
#include "Vocab_US_TI99.h"

Talkie voice;


void setup() {

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    
    pinMode(LED_BUILTIN, OUTPUT);

    TransmitXTimes();
      
}

void TransmitXTimes() {
  
   for (int i=1; i <= 2 ; i++){

  digitalWrite(LED_BUILTIN, HIGH);

  //transmit
  digitalWrite(2, LOW);
  delay(750);

  //talk
    voice.say(spt_WHAT);
    voice.say(spt_IS);
    voice.say(spt_YOUR);
    voice.say(spt_LAST);
    voice.say(spt_POSITION);
    voice.say(spt_OVER);
   

  //stop transmit
    delay(750);
   digitalWrite(2, HIGH);
   digitalWrite(LED_BUILTIN, LOW);

  //pause
  delay(60000);
 }
 
}

void loop() {

}
