#include "Talkie.h"
#include "TalkieUtils.h"
#include "Vocab_US_Large.h"

// This demo version allows you to plug the adapter 
// cord directly into the Arduino UNO header. 
// No jumper wires needed.
//
// G = Ground = Pin 2
// M = Microphone = Pin 3
// S = Speaker = Pin 4 -- Not Used
// T = Transmit/PTT = Pin 5

// -----------------------------------------------------------------------

  
  //Adjust the delay between automatic transmissions
  const int DELAY_TIME = 10; // in seconds

  //Method to allow ground on a normally non-ground PIN
  const int GROUND_PIN = 2;
  
  //Pin connected to PTT on radio
  const int TRANSMIT_PIN = 5;

  //Talkie (Voice) pin is hard-coded to Pin 3 in the library
  
  //Variable for storing elapsed time.
  unsigned long startTime = 0;


// -----------------------------------------------------------------------


// Voice Synth
// (Talkie outputs to BOTH pins D3 and D11 : Hard Coded in Library)
Talkie voice;


// -----------------------------------------------------------------------


void setup() {

  //Pseudo ground pin
  pinMode(GROUND_PIN, OUTPUT);
  digitalWrite(GROUND_PIN, LOW);

  //PTT/Transmit pin
  pinMode(TRANSMIT_PIN, OUTPUT);
  digitalWrite(TRANSMIT_PIN, HIGH);

  //For display of transmit status (connected to 13 on most Arduinos)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(TRANSMIT_PIN, LOW);
    
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
