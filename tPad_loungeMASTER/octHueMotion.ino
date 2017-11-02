#include <RestClient.h>

// working, basic PIR motion - turns kitchenHue on/off (w. 5sec delay on the off...)

#include "Arduino.h"

#include <ESP8266WiFi.h>

#define pirPin D2   //
unsigned long stars=0;
unsigned long down=0;
int pirState = 0;
int pirVal = LOW;
int lightNo = 1;
const char ssid[] = "WiFi2";
const char password[] = "4328646518";
const char LIGHTS_ON[] = "{\"on\":true}";
const char LIGHTS_OFF[] = "{\"on\":false}";
const char *bridge_ip = "192.168.0.101"; // Hue Bridge IP
const int port = 80;
RestClient hue = RestClient(bridge_ip);

void hue_turnOff(){
  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state/", LIGHTS_OFF);
}
void hue_turnOn(){
  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state/", LIGHTS_ON);
}
void hue_do(){
//  hue.put(statePoint, FIRST);
//  hue.put(statePoint, SECOND);
}
void setup()
{
  Serial.begin(115200);
  Serial.println("COMMENCING TNODE LIGHTSENSOR");
  delay(10);
  // We connect to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
pinMode(pirPin, INPUT);
pirState=LOW;
delay(500);
hue_turnOff();
}

void loop() {
 pirVal = digitalRead(pirPin); // read input value
  if (pirVal == HIGH) {
    if (pirState == LOW) {         // we have just turned on
      hue_turnOn();
      Serial.println("*NEW MOVE"); stars=0; stars++;down=0;
       pirState = HIGH;
       } else if (pirState == HIGH){
      Serial.print("*"); stars++;
       }
  } else if (pirVal == LOW){
    if (pirState == HIGH)
          {
      Serial.println("_END - TURNING OFF LIGHT in 5");
      delay(5000);
      pirState = LOW; stars = 0; down =0; down++;
      hue_turnOff();
    } else if (pirState == LOW){
    down++;
    Serial.print("_");
    }
    }
  delay(10);
 }
