#include <Arduino.h>
#include <Morse.h>
#include <DTMF.h>
#include "Talkie.h"
#include "TalkieUtils.h"
#include "Vocab_US_Large.h"

// -----------------------------------------------------------------------

const int TRANSMIT_PIN = 5;
const int VOICE_PIN = A7;

// -----------------------------------------------------------------------

// Voice Synth
Talkie voice;

// -----------------------------------------------------------------------

// Morse
//morse( <pin number>, <speed WPM>, <1=beep, 0=PTT>, LEDPin
//Using Pin 6 (PWM) for output -- ganged to Pin 3 (voice output)
Morse morse(6, 20, 1, 8);

// MORSE PROSIGNS
// BK = Break In (I'm going to start transmitting)
// CT = Message starting / attention / voice component coming up
// AR = Out / end of transmission

// -----------------------------------------------------------------------


//DTMF (tone detection for control)
float n = 128.0;
float sampling_rate = 8926.0;
DTMF dtmf = DTMF(n, sampling_rate);
float d_mags[8];
char thischar;

// -----------------------------------------------------------------------

// Ability to switch out users
int user = 1;

// -----------------------------------------------------------------------

// Automatic Transmit On/Off
int autoTransmit = 0;
unsigned long startTime = 0;

// -----------------------------------------------------------------------

void setup() {

  Serial.begin(19200); delay(500);
  Serial.println("HamFox_GMRS_V3d2_2m");

  pinMode(TRANSMIT_PIN, OUTPUT);
  digitalWrite(TRANSMIT_PIN, HIGH);
    
  pinMode(LED_BUILTIN, OUTPUT);
     
}//setup

// -----------------------------------------------------------------------

void TransmitManualResponse() {

  Serial.println("Transmit Response");

  // TODO: Wait for clear channel to send on (sample analog 7 speaker pin)

  digitalWrite(LED_BUILTIN, HIGH);

  //transmit
  digitalWrite(TRANSMIT_PIN, LOW);
  delay(750);

  //talk
  morse.sendmsg("BK");
  delay(750);
  TransmitCallsign(true);
  delay(750);
  morse.sendmsg("CT");
  delay(1000);
  voice.say(sp2_F);
  voice.say(sp2_O);
  voice.say(sp2_X);
  voice.say(sp2_H);
  voice.say(sp2_U);
  voice.say(sp2_N);
  voice.say(sp2_T);
  voice.say(sp2_REPEAT);
  voice.say(sp2_F);
  voice.say(sp2_O);
  voice.say(sp2_X);
  voice.say(sp2_H);
  voice.say(sp2_U);
  voice.say(sp2_N);
  voice.say(sp2_T);
  //TransmitFrequency();
  delay(1000);
  morse.sendmsg("AR");

  //stop transmit
  delay(750);
  digitalWrite(TRANSMIT_PIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
 
}//TransmitManualResponse


// -----------------------------------------------------------------------

void TransmitUserInfo() {

  // TODO: Wait for clear channel to send on (sample analog 7 speaker pin)

  digitalWrite(LED_BUILTIN, HIGH);

  //transmit
  digitalWrite(TRANSMIT_PIN, LOW);
  delay(750);

  //talk
  morse.sendmsg("BK");
  delay(750);
  TransmitCallsign(true);
  delay(750);
  delay(1000);
  morse.sendmsg("AR");

  //stop transmit
  delay(750);
  digitalWrite(TRANSMIT_PIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
 
}//TransmitUserInfo


// -----------------------------------------------------------------------


void TransmitCallsign(bool useVoice){

  switch (user) {
    case 1:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_ONE); delay(750);
        voice.say(sp2_ROMEO); voice.say(sp2_OSCAR); voice.say(sp2_NOVEMBER);
      }
      morse.sendmsg("DE WP4RON"); // Ron Nelson
      break;    
    case 2:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_TWO); delay(750);
        voice.say(sp2_BRAVO); voice.say(sp2_VICTOR); voice.say(sp2_QUEBEC);
      }
      morse.sendmsg("DE KI5BVQ"); // McKinley Engstrom
      break;    
    case 3:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_THREE); delay(750);
        voice.say(sp2_FOXTROT); voice.say(sp2_KILO); voice.say(sp2_TANGO);
      }
      morse.sendmsg("DE KI5FKT"); // Alex Donnell
      break;    
    case 4:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_FOUR); delay(750);
        voice.say(sp2_UNIFORM); voice.say(sp2_QUEBEC); 
      }
      morse.sendmsg("DE AG5UQ"); // Cliff Pinkerton
      break;    
    case 5:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_FIVE); delay(750);
        voice.say(sp2_BRAVO); voice.say(sp2_VICTOR); voice.say(sp2_ROMEO);
      }
      morse.sendmsg("DE KI5BVR"); // Jerald Swain
      break;    
    case 6:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_SIX); delay(750);
        voice.say(sp2_ROMEO); voice.say(sp2_OSCAR); voice.say(sp2_NOVEMBER);
      }
      morse.sendmsg("DE WP4RON"); // TBD
      break;    
    default:
      voice.say(sp2_OPERATOR); voice.say(sp2_ZERO);
      break;    
  }

}//TransmitCallsign


// -----------------------------------------------------------------------


void TransmitFrequency() {

/*  
  //Frequency -- 145.670
  voice.say(sp2_ON);
  voice.say(sp2_ONE);voice.say(sp2_FOUR);voice.say(sp2_FIVE);
  voice.say(sp2_POINT);
  voice.say(sp2_SIX);voice.say(sp2_SEVEN);voice.say(sp2_ZERO);
  voice.say(sp2_MEGA); voice.say(sp2_HERTZ);
*/

}//TransmitFrequency


// -----------------------------------------------------------------------


void TransmitToggle() {

  // TODO: Wait for clear channel to send on (sample analog 7 speaker pin)

  digitalWrite(LED_BUILTIN, HIGH);

  //transmit
  digitalWrite(TRANSMIT_PIN, LOW);
  delay(750);

  //talk
  morse.sendmsg("BK"); 
  delay(750);
  morse.sendmsg("CT");
  delay(1000);

  voice.say(sp2_AUTOMATIC);
  if (autoTransmit) {
    voice.say(sp2_ON);
    startTime = millis();    
  }
  else {
    voice.say(sp2_OFF);
    startTime = 0;    
  }

  delay(1000);  
  TransmitCallsign(false);
  delay(1000);  
  morse.sendmsg("AR");

  //stop transmit
  delay(750);
  digitalWrite(TRANSMIT_PIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);

  
}//TransmitToggle


// -----------------------------------------------------------------------

void TransmitFoxMessage() {

  // TODO: Wait for clear channel to send on (sample analog 7 speaker pin)

  digitalWrite(LED_BUILTIN, HIGH);

  //transmit
  digitalWrite(TRANSMIT_PIN, LOW);
  delay(750);

  //automatic morse message on foxhunt -- no voice
  morse.sendmsg("BK");
  delay(1000);
  TransmitCallsign(false);
  delay(500);
  morse.sendmsg("/FOX");
  delay(1000);
  morse.sendmsg("AR");

  //stop transmit
  delay(750);
  digitalWrite(TRANSMIT_PIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);

  //Reset timer
  startTime = millis(); 
  
}//TransmitFoxMessage

// -----------------------------------------------------------------------

void loop() {


  //Serial.println(analogRead(VOICE_PIN));
  
  dtmf.sample(VOICE_PIN);
  dtmf.detect(d_mags, 506);
  thischar = dtmf.button(d_mags, 1800.);
 
  if (thischar) {
    Serial.print("Received: "); Serial.println(thischar);
    switch (thischar) {
      case '1':
        user = 1; // switch user
        TransmitUserInfo();
        break;
      case '2':
        user = 2; // switch user
        TransmitUserInfo();
        break;
      case '3':
        user = 3; // switch user
        TransmitUserInfo();
        break;
      case '4':
        user = 4; // switch user
        TransmitUserInfo();
        break;
      case '5':
        user = 5; // switch user
        TransmitUserInfo();
        break;
      case '6':
        user = 6; // switch user
        TransmitUserInfo();
        break;
      case '#':
        //Toggle auto transmit
        autoTransmit = !autoTransmit;
        //Radio responds with setting (on or off)
        TransmitToggle();
        break;
      case '*':
        //Allows user to manually trigger response 
        TransmitManualResponse();
        break;
      default:
        break;
    }//switch
  }//if dtmf received

  delay(100); // Short pause in main loop

  //If auto transmit is on, and sixty seconds have passed, transmit the fox message & reset timer
  if (autoTransmit) {
    if (millis() - startTime >= 60000) {
      TransmitFoxMessage();
    }
  }//if autoTransmit


}//loop
