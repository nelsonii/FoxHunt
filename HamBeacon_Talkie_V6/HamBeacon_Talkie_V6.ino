#include "Talkie.h"
#include "TalkieUtils.h"
#include "Vocab_US_Large.h"


// -----------------------------------------------------------------------

  
  //Adjust the delay between automatic transmissions
  const int DELAY_TIME = 10; // in seconds
  
  //Pin connected to PTT on radio
  const int TRANSMIT_PIN = 5;
  
  //Variable for storing elapsed time.
  unsigned long startTime = 0;


// -----------------------------------------------------------------------


// Voice Synth
// Talkie outputs to BOTH pins D3 and D6
Talkie voice;


// -----------------------------------------------------------------------


void setup() {

  pinMode(TRANSMIT_PIN, OUTPUT);
  digitalWrite(TRANSMIT_PIN, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);
    
}//setup


// -----------------------------------------------------------------------


void TransmitBeaconMessage() {

  //visual indicator of transmission -- on
  digitalWrite(LED_BUILTIN, HIGH);

  //transmit
  digitalWrite(TRANSMIT_PIN, LOW);
  delay(750);

  voice.say(sp2_F);
  voice.say(sp2_O);
  voice.say(sp2_X);
  voice.say(sp2_H);
  voice.say(sp2_U);
  voice.say(sp2_N);
  voice.say(sp2_T);

  //stop transmit
  delay(750);
  digitalWrite(TRANSMIT_PIN, HIGH);

  //visual indicator of transmission -- off
  digitalWrite(LED_BUILTIN, LOW);

  //Reset timer
  startTime = millis(); 
  
}//TransmitBeaconMessage


// -----------------------------------------------------------------------


void loop() {

    // Run every x seconds (as defined by DELAY_TIME)
    if (millis() - startTime >= (DELAY_TIME * 1000)) {
      TransmitBeaconMessage();
    }

}//loop
