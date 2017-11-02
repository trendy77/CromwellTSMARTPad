
//TNODE - IR SONY SEND

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

IRsend irsend(4);  // An IR LED is controlled by GPIO pin 4 (D2)
#define TV_OFF        0xa90

void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}

void loop() {
  Serial.println("Sony");
  irsend.sendSony(TV_OFF, 12, 2);
  delay(2000);
 }
