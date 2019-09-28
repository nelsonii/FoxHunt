#include <Arduino.h>
#include <Morse.h>
#include <DTMF.h>
#include "Talkie.h"
#include "TalkieUtils.h"
#include "Vocab_US_Large.h"


const int TRANSMIT_PIN = 5;
const int VOICE_PIN = A7;


// Voice Synth
Talkie voice;

// Morse
//morse( <pin number>, <speed WPM>, <1=beep, 0=PTT>
//Using Pin 6 (PWM) for output -- ganged to Pin 3 (voice output)
Morse morse(6, 20, 1);


//DTMF (tone detection for control)
float n = 128.0;
float sampling_rate = 8926.0;
DTMF dtmf = DTMF(n, sampling_rate);
float d_mags[8];
char thischar;


void setup() {

  Serial.begin(19200); delay(500);
  Serial.println("HamFox_GMRS_V3d1");

  pinMode(TRANSMIT_PIN, OUTPUT);
  digitalWrite(TRANSMIT_PIN, HIGH);
    
  pinMode(LED_BUILTIN, OUTPUT);
     
}//setup


void TransmitResponse() {

  Serial.println("Transmit Response");

  // TODO: Wait for clear channel to send on (sample analog 7 speaker pin)

  digitalWrite(LED_BUILTIN, HIGH);

  //transmit
  digitalWrite(TRANSMIT_PIN, LOW);
  delay(750);

  //talk
  morse.sendmsg("BK");
  delay(750);
  morse.sendmsg("DE WP4RON");
  delay(750);
  
  morse.sendmsg("CT");
  delay(1000);
  voice.say(sp2_RANGE);
  voice.say(sp2_TEST);
  //Location (ZIP)
  voice.say(sp2_FROM);
  voice.say(sp2_SEVEN);
  voice.say(sp2_SIX);
  voice.say(sp2_FIVE);
  voice.say(sp2_TWO);
  voice.say(sp2_TWO);
  //Frequency
  voice.say(sp2_ON);
  voice.say(sp2_FOUR); voice.say(sp2_THIR_); voice.say(sp2_T); voice.say(sp2_ONE);
  voice.say(sp2_POINT);
  voice.say(sp2_TWO); voice.say(sp2_FIVE); voice.say(sp2_ZERO);
  voice.say(sp2_MEGA); voice.say(sp2_HERTZ);
  delay(1000);

  morse.sendmsg("DE WP4RON");
  delay(750);
  morse.sendmsg("AR");

  //stop transmit
  delay(750);
  digitalWrite(TRANSMIT_PIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
 
}//TransmitResponse


void loop() {

  //Serial.println(analogRead(VOICE_PIN));
  
  dtmf.sample(VOICE_PIN);
  dtmf.detect(d_mags, 506);
  thischar = dtmf.button(d_mags, 1800.);
 
  if (thischar) {
    switch (thischar) {
      case '*':
        Serial.print("Received: "); Serial.println(thischar);
        TransmitResponse();
        break;
      default:
        Serial.print("Received Unknown DTMF: "); Serial.println(thischar);
        break;
    }//switch
  }//if dtmf received

  delay(100);

}//loop
