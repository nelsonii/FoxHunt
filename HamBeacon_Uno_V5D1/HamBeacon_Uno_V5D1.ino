#include <Arduino.h>
#include <Morse.h>

// -----------------------------------------------------------------------
  
  // Change this CALL_SIGN to your Amateur Radio callsign
  const char CALL_SIGN = "WP4RON";
  
  //Adjust the delay between automatic transmissions
  const int DELAY_TIME = 20; // in seconds
  
  //Pin connected to PTT on radio
  const int TRANSMIT_PIN = 5;
  
  //Audio output of morse connected on the MIC on radio
  const byte MORSE_OUT_PIN = 6;
  //Flashes this pin / LED based on morse being sent
  const byte MORSE_LED_PIN = LED_BUILTIN;
  //Speed of morse sending. Recommend keeping it fast (20 WPM).
  const byte MORSE_SPEED_WPM = 20;
  //Beep out the morse, default. No need to change
  const byte MORSE_BEEP = 1;
  
  //Variable for storing elapsed time.
  unsigned long startTime = 0;


// -----------------------------------------------------------------------


// Morse
//morse( <pin number>, <speed WPM>, <1=beep, 0=PTT>, <LEDPin> )
Morse morse(MORSE_OUT_PIN, MORSE_SPEED_WPM, MORSE_BEEP, MORSE_LED_PIN);


// -----------------------------------------------------------------------


void setup() {

  pinMode(TRANSMIT_PIN, OUTPUT);
  digitalWrite(TRANSMIT_PIN, HIGH);
    
  pinMode(MORSE_LED_PIN, OUTPUT);

}//setup


// -----------------------------------------------------------------------


void TransmitBeaconMessage() {

  //transmit
  digitalWrite(TRANSMIT_PIN, LOW);
  delay(750);

  //automatic morse message on beacon -- no voice
  morse.sendmsg("BK");
  delay(1000);
  morse.sendmsg("DE");
  morse.sendmsg(CALL_SIGN);
  delay(1000);
  morse.sendmsg("/BEACON");
  delay(1000);
  morse.sendmsg("AR");

  //stop transmit
  delay(750);
  digitalWrite(TRANSMIT_PIN, HIGH);

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
