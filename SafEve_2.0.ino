//SafEve, aimed at enhancing women safety in India, was
//conceptualised during National Innovation Drive 2017
//at PDPM IIITD&M, Jabalpur
//by Ms. Amanat Mishra and Mr. Ajwad Shaikh

//SafEve 2.0 was released at Technovation, Abhikalpan 2018
//at PDPM IIITD&M Jabalpur

#include "Adafruit_FONA.h"

#define EVE_RX 10 // Digital pin of Arduino that is mapped to receive from SIM Module
#define EVE_TX 11 // Digital pin of Arduino that is mapped to transmit to SIM Module
#define EVE_RST 12 // Digital pin of Arduino sends a reset pulse to initialize the SIM Module
#define button 9 // Connected to the trigger to listen to distress
#define button2 8 // Set 'LOW' to make 'button' low on trigger 
unsigned long previousTenthSecondMillis = 0L;
long tenthSecond = 100UL;
byte buttonStillDown = 0;

char replybuffer[255];

#include <SoftwareSerial.h>
#include <AltSoftSerial.h>
SoftwareSerial eveSS = SoftwareSerial(EVE_TX, EVE_RX);
//AltSoftSerial eveSS = AltSoftSerial(EVE_TX, EVE_RX);
SoftwareSerial *eveSerial = &eveSS;
//AltSoftSerial *eveSerial = &eveSS;
//HardwareSerial *eveSerial = &Serial;
Adafruit_FONA eve = Adafruit_FONA(EVE_RST);
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
//uint8_t type;

void setup() {
  while (!Serial);

  Serial.begin(9600);
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  eveSerial->begin(4800);
  if (! eve.begin(*eveSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  //type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  
  // Print module IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = eve.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); 
    Serial.println(imei);
  }
  pinMode(button, INPUT_PULLUP);
  pinMode(button2, OUTPUT);
  digitalWrite(button2, LOW);
}

void loop() {
  if (millis() - previousTenthSecondMillis >= tenthSecond) {
    
    // check the button (pin 9 --> button --> gnd (pin 8) )
    if (!digitalRead(button)) {
      if ((buttonStillDown++ > 0 ) && (buttonStillDown < 3)) {

        // do the single click thing
        Serial.println("Clicked");
 
      } else {
        if (buttonStillDown > 30 ) {

          // do the held down thing
          Serial.println("Held");
          triggered();
          buttonStillDown = 0;
        }
      }
    } else {
      buttonStillDown = 0;
    }

    previousTenthSecondMillis += tenthSecond;
  }
}

void flushSerial() {
  while (Serial.available())
    Serial.read();
}

void triggered()
{
  char message[141];
  const char sendto[] = "98xxxxxxxx"; // Number to which the SMS is to be delivered.
  // Change the above char string to a valid number before running the code.
  flushSerial();
  Serial.print(F("Send to #"));
  //readline(sendto, 20);
  Serial.println(sendto);
  //delay(1500);
  if (!eve.enableGPRS(true))
  Serial.println(F("Failed to turn on"));
  uint16_t returncode;
  float latitude, longitude;
  float* plat = &latitude;
  float* plong = &longitude;
  String finalmsg = "<SafEveAlert>";
  if (!eve.getGSMLoc(plat, plong))
  Serial.println(F("Failed!"));
  if (returncode == 0) {
  finalmsg+="\nMaps Link : \nhttps://maps.google.com/?api=1&q=";
  finalmsg+=latitude;
  finalmsg+=",";
  finalmsg+=longitude;
  finalmsg+="\n</SafEveAlert>";
  } else {
  Serial.print(F("Fail code #")); Serial.println(returncode);
  }
  finalmsg.toCharArray(message,141);
  //Serial.print(F("Type out one-line message (140 char): "));
  //readline(message, 140);
  Serial.println(message);
  //delay(1500);
  if (!eve.sendSMS(sendto, message)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("Sent!"));
  }
  delay(1500);
  if (!eve.enableGPRS(false))
  Serial.println(F("Failed to turn off"));
}

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout) {
  uint16_t buffidx = 0;
  boolean timeoutvalid = true;
  if (timeout == 0) timeoutvalid = false;

  while (true) {
    if (buffidx > maxbuff) {
      //Serial.println(F("SPACE"));
      break;
    }

    while (Serial.available()) {
      char c =  Serial.read();

      //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);

      if (c == '\r') continue;
      if (c == 0xA) {
        if (buffidx == 0)   // the first 0x0A is ignored
          continue;

        timeout = 0;         // the second 0x0A is the end of the line
        timeoutvalid = true;
        break;
      }
      buff[buffidx] = c;
      buffidx++;
    }

    if (timeoutvalid && timeout == 0) {
      //Serial.println(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  buff[buffidx] = 0;  // null term
  return buffidx;
}
