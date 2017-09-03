#include <command.h>
#include <ESP8266AVRISP.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <restclient.h>
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>

//int RECV_PIN = D2; //an IR detector/demodulatord is connected to GPIO pin 2
//IRrecv irrecv(RECV_PIN);
//decode_results results;

const char *LIGHTS_ON = "{\"on\":true}";
const char *LIGHTS_OFF= "{\"on\":false}";

unsigned long startTime = 0;
String turnonhue = "{\"on\":true}";
String turnoffhue = "{\"on\":false}";
const char *endpoint = "GET /api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/9 HTTP/1.1";
const char *state ="PUT /api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/9/state HTTP/1.1";
long stillOff =0;
const char *hueFirst = "{\"on\": true,\"hue\": 50100,\"sat\":255,\"bri\":100,\"transitiontime\":20}";
const char *hueSec = "{\"on\": true,\"hue\": 65280,\"sat\":255,\"bri\":200,\"transitiontime\":25}";
const char *hueThrd = "{\"hue\": 65280,\"sat\":255,\"bri\":255,\"transitiontime\":5}";
const char *hueFth = "{\"hue\": 50100,\"sat\":255,\"bri\":255,\"transitiontime\":20}";
int status = WL_IDLE_STATUS; // the Wifi radio's status

int pirState = LOW; // we start, assuming no motion detected
int pirVal = LOW;        // variable for reading the pin status
int hueBri;    // brightness value
int hueBriToSet;    // target number
long hueHue;   // hue value
long hueToHue;
String hueCmd; // Hue command
bool hueOn = false;
    // Wifi Settings
const char *ssid = "LordProtectorOfTheWiFi";
const char *password = "4328646518";
WiFiServer telnetServer(23);
WiFiClient serverClient;
WiFiClient client;

const char *bridge_ip = "192.168.0.101"; 
const int port = 80;
String user1 = "fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG";
restclient hue(bridge_ip, port);
#define pirPin D1 // Input for PIR AS312

static bool clientParseBool(char delim){
  bool val = false;
  uint8_t count = 0; // number of chars we've read for the value
  while (status == WL_CONNECTED)  {
    if (client.available())    {
      char c = client.read();
      if (c == delim)        break; // done!
      if ((c == ' ') || (count++ > 0))
        continue; // skip spaces and anything after the first non-space char
      if (c == 't'){
        val = true;
    }
    // Debug prints
  Serial.print(F("bool "));
  Serial.println(val);
  return val;
}
  }
}
//-------------------------------------------
// Utility function to extract one 'unsigned 16-bit int' value from the client stream.
//   No buffers are used.  Each character is handled as it is received.
//   (you may want/need to add a timeout)
static uint16_t clientParseUint(char delim){
  uint16_t val = 0;
  while (status == WL_CONNECTED)
  {
    if (client.available())
    {
      char c = client.read();
      if (c == delim)
        break; // done!
      if (c == ' ')
        continue; // skip spaces
      if (isdigit(c))
        val = val * 10 + (c - '0'); // accumulate one digit of the bri value
    }
  }
  // Debug prints
  Serial.print(F("int "));
  Serial.println(val);
  return val;
}

/* A helper function in case your logic depends on the current state of the light. 
 * This sets a number of global variables which you can check to find out if a light is currently on or not
 * and the hue etc. Not needed just to send out commands
 */
bool getHue(int lightNum){
  if (client.connect(bridge_ip, 80))  {
  Serial.print("getHue: ");
  Serial.println(endpoint);
  
  client.print(String(endpoint) +"\r\n" +
               "Host: " + String(bridge_ip) + "\r\n" + 
               "Connection: close\r\n" + 
               "Content-Length: 0\r\n" + 
               "\r\n");

  delay(100);
    while(client.available()){
       if (status == WL_CONNECTED)    {
        hueOn = clientParseBool(','); 
         if (status == WL_CONNECTED)    {
      client.findUntil("\"bri\":", "\0");
     }
        hueBri = clientParseUint(',');  
        if (status == WL_CONNECTED)    {
        client.findUntil("\"hue\":", "\0");
          hueHue = clientParseUint(',');
          }
        client.flush();
    client.stop();
    printHueInfo();
  return true;
   }else{
    return false;
    }
}
  }
}

void printHueInfo(){
  Serial.print(F("Hue is: "));
  Serial.println(hueOn);
  Serial.print(F("HueBri is: "));
  Serial.println(hueBri);
  Serial.print(F("hueHue is: "));
  Serial.println(hueHue);
  }

void printWifiData(){
  //WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);
}
void printCurrentNet(){
  //SSID of the network you're attached to:
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());
}

void setup(){
  Serial.begin(115200);
  Serial.println("COMMENCING TNODE LIGHTSENSOR");
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)  {
  delay(500);
  Serial.print(".");
  }
  Serial.println("WiFi connected");  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(pirPin, INPUT);
  delay(10);
  telnetServer.begin();  telnetServer.setNoDelay(true);
delay(10);
getHue(9);
}
unsigned long lastTime=0; int stars =0; int down = 0;
unsigned long timer; unsigned long offDelay=0;

void motionCheck(){
  pirVal = digitalRead(pirPin); // read input value
  if (pirVal == HIGH) {        
    if (pirState == LOW) {         // we have just turned on
      Serial.print("*NEW MOVE"); stars++;down =0;
      // We only want to print on the output change, not state
      pirState = HIGH; Serial.print("*");
    } else if (pirState == HIGH){
      stars++;Serial.print("*");
       } 
  } else if (pirVal == LOW) {
     if (pirState == HIGH) {
      Serial.println("_END - TURNING OFF LIGHT IN ");
      int waiting = (stars*100);Serial.print("_");
      Serial.print(waiting); Serial.print(" seconds");
     offDelay=millis();  pirState = LOW; stars = 0; down++;  
    } else if (pirState == LOW){
    down++;Serial.print("_");
  }
}
if((down == 100)||(stars == 100)){
  Serial.println(".");down=0;stars=0;
}
}

void telnetMe(){
if (telnetServer.hasClient())  {
    if (!serverClient || !serverClient.connected())    {
      if (serverClient)      {
        serverClient.stop();
        Serial.println("Telnet Client Gone");
      }
      serverClient = telnetServer.available();
      Serial.println("New Telnet client");
      serverClient.flush(); // clear input buffer, else you get strange characters
    }
  }
  while (serverClient.available()) {
    Serial.write(serverClient.read());
  }
 while (Serial.available()){
    serverClient.write(Serial.read());
  }
    if (serverClient && serverClient.connected())    { // send data to Client
      serverClient.print("LIGHT IS ? ");
      serverClient.println(hueOn);
    if (hueHue!=0){  
      serverClient.print("HUEs Hue?");
      serverClient.println(hueHue);
    }
    if (hueBri!=0){
 serverClient.print("BRI ");
      serverClient.println(hueBri);
    }
  }
  lastTime = millis();
  delay(10); // to avoid strange characters left in buffer
 }


  //while (client.available()) {
  // if (client.available())   {
      //}


void hue_turnOff(){
hue.put(endpoint, LIGHTS_OFF); 
}

void hue_turnOn(){
 
  hue.put(state, LIGHTS_ON);
}
unsigned long onOffTest = 0;

void loop(){
timer = millis();
if (timer > (5000 + onOffTest)){
hue.put(state,hueFirst);
onOffTest=millis();
} 
stillOff = (timer-offDelay);
if (stillOff>10000){
hue_turnOff();
}
    
  telnetMe();
} 

