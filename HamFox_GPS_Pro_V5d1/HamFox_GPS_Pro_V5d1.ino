// EGG - V0.01
//
// A project by Evelyn and Ron Nelson
//
// Written: Fall of 2017
// Contact: evelyn.nelson (at) CentralTexasMakers.com  -  ron.nelson (at) CentralTexasMakers.com
//
// Target Board: Arduino Mega  ATmega2560
// Overview of Pins Used (Mega layout):
//   20 - SDA - I2C Data
//   21 - SCL - I2C Clock
//
//=================================================================================

// Libraries

// Common
#include <Arduino.h>
#include <Wire.h>

//Sending and Listening
#include <Morse.h>
#include <DTMF.h>
//Talking
#include "Talkie.h"
#include "TalkieUtils.h"
#include "Vocab_US_Large.h"

// OLED Display over I2C
#define I2C_ADDRESS 0x3C
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
// Note: I originally used the full Adafruit_SSD1306 and Adafruit-GFX-Library
//       classes. However, they are much more powerful than we needed for this
//       application. And, unfortunately, pose memory issues when running on the
//       Nano or Uno. I was able to find this slimmed down version which does
//       similar work, but only does text (no graphics).
//       https://github.com/greiman/SSD1306Ascii

// Sensors
#include <Adafruit_Sensor.h> // Adafruit sensor interface (Unified)
#include <Adafruit_BMP085_U.h> // Barometric sensor (Unified)

// Real Time Clock
#include <RTClib.h> // real time clock

// GPS
#include <TinyGPS++.h>


//=================================================================================

// Constants 

//GUID on device
const String GUID = "HAMFOX";

// Code version. Used for developer reference.
const long APP_VER = 2019090101;

// Turns on/off Serial Output debugging
const boolean DEBUG = 1;
const short BAUD = 19200;

// GPS on Serial2
const short gpsBAUD = 9600;

// 433 Mhz Radio on Serial3
const short radioSetPin = 2;
const short radioDelay = 200;
const short radioBaud = 9600;

// Ham Radio Pins and Settings
  const int TRANSMIT_PIN = 5;
  const int VOICE_PIN = A7;
  
  const byte MORSE_OUT_PIN = 6;
  const byte MORSE_SPEED_WPM = 20;
  const byte MORSE_BEEP = 1;
  const byte MORSE_LED_PIN = 8;

//=================================================================================

// Library Classes

// Voice Synth
Talkie voice;

// Morse
//morse( <pin number>, <speed WPM>, <1=beep, 0=PTT>, LEDPin
//Using Pin 6 (PWM) for output -- ganged to Pin 3 (voice output)
// MORSE PROSIGNS
// BK = Break In (I'm going to start transmitting)
// CT = Message starting / attention / voice component coming up
// AR = Out / end of transmission
Morse morse(MORSE_OUT_PIN, MORSE_SPEED_WPM, MORSE_BEEP, MORSE_LED_PIN);

// OLED Display Class
SSD1306AsciiAvrI2c oled;

// Sensors
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085); // barometric

// Real Time Clock
RTC_DS1307 rtc; // real time clock

// GPS
TinyGPSPlus gps;

//=================================================================================

// Variables

// GPS
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int16_t ax, ay, az;
int16_t gx, gy, gz;

// DTMF (tone detection for control)
  float n = 128.0;
  float sampling_rate = 8926.0;
  DTMF dtmf = DTMF(n, sampling_rate);
  float d_mags[8];
  char thischar;

// Ability to switch out users
  int user = 1;

// Automatic Transmit On/Off
  int autoTransmit = 0;
  unsigned long startTime = 0;
  unsigned long heartbeatTime = 0;

// My Stuff
boolean updateDisplay = 1;

//=================================================================================

void setup() {

  if (DEBUG) {
      Serial.begin(BAUD);
      Serial.print("HAMFox_GPS_Pro_V5d1 ");
      Serial.println(APP_VER);
    }

  pinMode(LED_BUILTIN, OUTPUT);

  //Ham output pins
  if (DEBUG) {Serial.println("Setting Ham transmit pins...");}
  pinMode(TRANSMIT_PIN, OUTPUT);
  digitalWrite(TRANSMIT_PIN, HIGH);

  // Start real time clock
  if (DEBUG) {Serial.println("Starting Real Time Clock...");}
  if (! rtc.begin()) {Serial.println("Real Time Clock not found!");}

  // If clock isn't running (set to 0:0:0) then adjust the date/time on RTC to compile date/time.
  if (! rtc.isrunning()) {
    Serial.println("RTC not running. Adjusting to Compile Date/Time");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Setup OLED screen/display message
  if (DEBUG) {Serial.println("Starting OLED...");}
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(Arial14);

  // Start the GPS
  if (DEBUG) {Serial.println("Starting GPS...");}
  Serial2.begin(gpsBAUD);

  // Start the Radio
  if (DEBUG) {Serial.println("Starting Radio...");}
  Serial3.begin(radioBaud); // radio
  pinMode(radioSetPin, OUTPUT); // radio set ("program") mode pin
  digitalWrite(radioSetPin, LOW); // SET Mode -- High = normal, Low = set 
  delay(radioDelay);
  Serial3.println("AT+RX");
  delay(radioDelay);
  while (Serial3.available()) {           // If HC-12 has data (the AT Command response)
      Serial.write(Serial3.read());         // Send the data to Serial monitor
    }
  digitalWrite(radioSetPin, HIGH); // RUN mode -- High = normal, Low = set 
  delay(radioDelay);

  //Heartbeat
  heartbeatTime = millis();

   if (DEBUG) {Serial.println("Setup Complete");}


} //setup

//=================================================================================

void TransmitManualResponse() {

  if (DEBUG) {Serial.println("TransmitManualResponse");}

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

  if (DEBUG) {Serial.println("TransmitUserInfo");}

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
  morse.sendmsg("AR");

  //stop transmit
  delay(750);
  digitalWrite(TRANSMIT_PIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
 
}//TransmitUserInfo


// -----------------------------------------------------------------------


// -----------------------------------------------------------------------


void TransmitCallsign(bool useVoice){

  if (DEBUG) {Serial.println("TransmitCallsign");}

  switch (user) {
    case 1:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_ONE); delay(750);
        voice.say(sp2_ROMEO); voice.say(sp2_OSCAR); voice.say(sp2_NOVEMBER);delay(750);
      }
      morse.sendmsg("DE WP4RON"); // Ron Nelson
      break;    
    case 2:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_TWO); delay(750);
        voice.say(sp2_BRAVO); voice.say(sp2_VICTOR); voice.say(sp2_QUEBEC);delay(750);
      }
      morse.sendmsg("DE KI5BVQ"); // McKinley Engstrom
      break;    
    case 3:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_THREE); delay(750);
        voice.say(sp2_FOXTROT); voice.say(sp2_KILO); voice.say(sp2_TANGO);delay(750);
      }
      morse.sendmsg("DE KI5FKT"); // Alex Donnell
      break;    
    case 4:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_FOUR); delay(750);delay(750);
        voice.say(sp2_UNIFORM); voice.say(sp2_QUEBEC); 
      }
      morse.sendmsg("DE AG5UQ"); // Cliff Pinkerton
      break;    
    case 5:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_FIVE); delay(750);
        voice.say(sp2_BRAVO); voice.say(sp2_VICTOR); voice.say(sp2_ROMEO);delay(750);
      }
      morse.sendmsg("DE KI5BVR"); // Jerald Swain
      break;    
    case 6:
      if (useVoice) {
        voice.say(sp2_OPERATOR); voice.say(sp2_SIX); delay(750);
        voice.say(sp2_ROMEO); voice.say(sp2_OSCAR); voice.say(sp2_NOVEMBER);delay(750);
      }
      morse.sendmsg("DE WP4RON"); // TBD
      break;    
    default:
      voice.say(sp2_OPERATOR); voice.say(sp2_ZERO);
      break;    
  }

}//TransmitCallsign


void TransmitFrequency() {

  if (DEBUG) {Serial.println("TransmitFrequency");}

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

  if (DEBUG) {Serial.println("TransmitToggle");}

  // TODO: Wait for clear channel to send on (sample analog 7 speaker pin)

  digitalWrite(LED_BUILTIN, HIGH);

  //transmit
  digitalWrite(TRANSMIT_PIN, LOW);
  delay(750);

  //talk
  morse.sendmsg("BK"); 
  delay(750);
  morse.sendmsg("CT");
  delay(750);

  voice.say(sp2_AUTOMATIC);
  if (autoTransmit) {
    voice.say(sp2_ON);
    startTime = millis();    
  }
  else {
    voice.say(sp2_OFF);
    startTime = 0;    
  }

  delay(750);  
  TransmitCallsign(false);
  delay(750);  
  morse.sendmsg("AR");

  //stop transmit
  delay(750);
  digitalWrite(TRANSMIT_PIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);

  
}//TransmitToggle


// -----------------------------------------------------------------------

// -----------------------------------------------------------------------

void TransmitFoxMessage() {

  if (DEBUG) {Serial.println("TransmitFoxMessage");}

  // TODO: Wait for clear channel to send on (sample analog 7 speaker pin)

  digitalWrite(LED_BUILTIN, HIGH);

  //transmit
  digitalWrite(TRANSMIT_PIN, LOW);
  delay(750);

  //automatic morse message on foxhunt -- no voice
  morse.sendmsg("BK");
  delay(750);
  TransmitCallsign(false);
  morse.sendmsg("/FOX");
  delay(750);
  morse.sendmsg("AR");

  //stop transmit
  delay(750);
  digitalWrite(TRANSMIT_PIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);

}//TransmitFoxMessage

// -----------------------------------------------------------------------


void HandleGPSandPacketRadio() {

  char msgTIME[18];
  String msgGPS;
  String msgPAYLOAD;

  // Real time clock
  DateTime now = rtc.now();
  sprintf( msgTIME, "%02hhu:%02hhu:%02hhu:%02hhu:%02hhu:%02hhu", now.year(), now.month(), now.day(),now.hour(), now.minute(), now.second() );


   if (gps.location.isUpdated())
    {
      msgGPS = "";
      msgGPS.concat(gps.satellites.value());
      msgGPS.concat(":");
      msgGPS.concat(gps.location.age());
      msgGPS.concat(":");
      msgGPS.concat(gps.location.lat());
      msgGPS.concat(":");
      msgGPS.concat(gps.location.lng());
      msgGPS.concat(":");
      msgGPS.concat(gps.altitude.feet()) ;

      oled.clear();
      oled.println("() () ()    EGG    () () ()");
      oled.print(F(" Lat="));
      oled.println(gps.location.lat(), 6);
      oled.print(F(" Long="));
      oled.println(gps.location.lng(), 6);
      oled.print(F(" Altitude="));
      oled.print(gps.altitude.feet());
      oled.println(F(" ft "));
    }
    else {
    
      Serial.println("NO GPS Signal");
      msgGPS = "0:0:0:0:0";

      oled.clear();
      oled.println("() () ()    EGG    () () ()");
      oled.println();
      oled.println(F(" NO GPS Signal"));
    }

  // Packet Radio

  Serial.println("Radio Transmitting:");
  //Build payload
  msgPAYLOAD = "{";
  msgPAYLOAD.concat(GUID);
  msgPAYLOAD.concat("|");
  msgPAYLOAD.concat(msgTIME);
  msgPAYLOAD.concat("|");
  msgPAYLOAD.concat(msgGPS);
  msgPAYLOAD.concat("}");
  //Send payload
  Serial.println(msgPAYLOAD);
  Serial3.println(msgPAYLOAD);
  
  Serial.println("Radio Transmission Complete");

  Serial.println("Radio listening...");
  if(Serial3.available() > 1){//Read from HC-12 and send to serial monitor
    String input = Serial3.readString();
    Serial.println("RECEIVED:" + input);    
  }
  else{
    Serial.println("No data received.");
  }
  delay(radioDelay);

}//HandleGPSandPacketRadio


//=================================================================================


void loop() {


  dtmf.sample(VOICE_PIN);
  dtmf.detect(d_mags, 506);
  thischar = dtmf.button(d_mags, 1800.);
 
  if (thischar) {
    if (DEBUG) {Serial.print("DTMF Received: "); Serial.println(thischar);}
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
        //do nothing
        break;
    }//switch
    
    //When we get a tone, update GPS
    HandleGPSandPacketRadio();
    
  }//if dtmf received


  //If auto transmit is on, and sixty seconds have passed, transmit the fox message & reset timer
  if (autoTransmit) {
    if (millis() - startTime >= 60000) {
      TransmitFoxMessage();
      startTime = millis();  //Reset timer
    }
  }//if autoTransmit


  //Heartbeat every 10 seconds, just to make sure code is running
  if (millis() - heartbeatTime >= 10000) {
    HandleGPSandPacketRadio();
    heartbeatTime = millis(); // reset
    if (DEBUG) { Serial.println("Heartbeat");}
  }

  //Serial.println("Pausing for 5 seconds.");
  //smartDelay(5000);
  
} //loop 


//=================================================================================


static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial2.available())
      gps.encode(Serial2.read());
  } while (millis() - start < ms);
}

//=================================================================================
