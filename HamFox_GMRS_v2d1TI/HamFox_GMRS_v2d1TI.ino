// Sound is output on digital pin 3

#include <Arduino.h>
#include "Talkie.h"
#include "Vocab_US_TI99.h"

Talkie voice;

// On the nano, ground to a ground pin.
// Audio to Digital 3
// Transmit to Digital 5 (or as defined)

const int TRANSMIT_PIN = 5;


void setup() {

    pinMode(TRANSMIT_PIN, OUTPUT);
    digitalWrite(TRANSMIT_PIN, HIGH);
    
    pinMode(LED_BUILTIN, OUTPUT);

    TransmitXTimes();
      
}

void TransmitXTimes() {
  
   for (int i=1; i <= 60 ; i++){

  digitalWrite(LED_BUILTIN, HIGH);

  //transmit
  digitalWrite(TRANSMIT_PIN, LOW);
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
   digitalWrite(TRANSMIT_PIN, HIGH);
   digitalWrite(LED_BUILTIN, LOW);

  //pause
  delay(60000);
 }
 
}

void loop() {

}
