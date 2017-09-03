#include <Arduino.h>
#line 1 "c:\\Users\\trend\\Documents\\GitHub\\TNodeHue1.0\\hue-nodemcu-smarthome-sensor\\kitchTmotion\\kitchTmotion.ino"
#line 1 "c:\\Users\\trend\\Documents\\GitHub\\TNodeHue1.0\\hue-nodemcu-smarthome-sensor\\kitchTmotion\\kitchTmotion.ino"
#include <restclient.h>
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
int RECV_PIN = D2; //an IR detector/demodulatord is connected to GPIO pin 2
IRrecv irrecv(RECV_PIN);
decode_results results;

const char LIGHTS_ON[] = "{\"on\":true}";
const char LIGHTS_OFF[] = "{\"on\":false}";
const char EFFECT_COLORLOOP[] = "{\"effect\":\"colorloop\"}";
const char EFFECT_NONE[] = "{\"effect\":\"none\"}";
int HUEINED,BRIED,SATED=0; 
const char HUEEDIT[] = "{\"on\": true,\"hue\": " +HUEINED+ ",\"sat\":255,\"bri\":255,\"transitiontime\":20}";
const char BRIEDIT[] = "{\"on\": true,\"hue\": " + BRIED+ ",\"sat\":255,\"bri\":255,\"transitiontime\":25}";
const char SATEDIT[] = "{\"hue\": 65280,\"sat\":255,\"bri\":255,\"transitiontime\":15}";
const char TRANSIT[] = "{\"hue\": 50100,\"sat\":255,\"bri\":255,\"transitiontime\":20}";

bool looping = false;
bool firstM, secondM = false;
//  Hue variables
bool hueOn;    // on/off
int hueBri;    // brightness value
long hueHue;   // hue value
String hueCmd; // Hue command
const char statePoint[] = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state";
bool motion = false;
unsigned long timer = 0;

// Wifi Settings
//const char* ssid = "LAN_Time_Forgot";
//const char* password = "tasmania";
const char ssid[] = "LordProtectorOfTheWiFi";
const char password[] = "4328646518";

// declare telnet server (do NOT put in setup())
WiFiServer telnetServer(2388);
WiFiClient serverClient;

unsigned long startTime = millis();

// Hue Settings
const char *bridge_ip = "192.168.0.101"; // Hue Bridge IP
const int port = 80;
String user1 = "fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG";
restclient hue(bridge_ip, port);
unsigned long lastMove = 0;

// MotionSensor(s)
#define pirPin D1 // Input for PIR AS312
int pirValue;     // Place to store read PIR Value
long looper = 0;

void hue_stateCheck()
{
  Serial.print("Connecting to ");
  Serial.println(bridge_ip);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(bridge_ip, port))
  {
    Serial.println("Connection failed");
    return;
  }
  // This will send the request to the server
  client.println("GET /api/" + user1 + "/lights/1");
  client.println("Host: " + String(bridge_ip) + ":" + String(port));
  client.println("User-Agent: ESP8266/1.0");
  client.println("Connection: close");
  client.println("Content-type: text/xml; charset=\"utf-8\"");
  delay(10);
  while (client.available())
  {
    if (client.available())
    {
 String oupt= client.readStringUntil('/n');
      Serial.println(outp);
                    
    }
  }

void getPirValue(void){
  pirValue = digitalRead(pirPin);
  if (pirValue == HIGH)  {
   motion = true;  Serial.print("*");
   } else if (firstM == true){
      Serial.print("2M");
       secondM = true;
  
  }
} else {
  motion = false;

}
void hue_loop()
{
  if (looping == true){
  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state", EFFECT_NONE);
  looping = false;
}else if (looping ==false){
  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state", EFFECT_COLORLOOP);
  looping = true;
}
}

void hue_turnOff(){
  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state", LIGHTS_OFF);
}

void hue_turnOn(){
  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state", LIGHTS_ON);
}
void hue_do(){
  hue.put(statePoint, FIRST);
  hue.put(statePoint, SECOND);
}
void setup()
{
  Serial.begin(115200);
  Serial.println("COMMENCING TNODE LIGHTSENSOR");
  delay(10);
  // We connect to a WiFi network
  Serial.println();
  Serial.println();
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
  hue_stateCheck();
  delay(10);
  Serial.print("KitchenHue currently:");
  Serial.println(hueOn);
  telnetServer.begin();
  telnetServer.setNoDelay(true);
  Serial.println("Please connect Telnet Client, exit with ^] and 'quit'");
  Serial.print("Free Heap[B]: ");
  Serial.println(ESP.getFreeHeap());
  delay(500);
    hue_turnOff();
    delay(500);
    irrecv.enableIRIn(); // Start the receiver
}

unsigned long stateCheck = 0;
int r,g,b=0;
void loop(){
  timer = millis();
 b=255;b--;r++;g=g-(g/2);g++;
 
  getPirValue();
  if (irrecv.decode(&results))  {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
    }
  }
   if (millis() - startTime > 3000)  { // run every 2000 ms
    hue_stateCheck();
    stateCheck = millis();
      }
  Serial.println(hueOn);
  delay(10);
  if (telnetServer.hasClient())  {
    if (!serverClient || !serverClient.connected())    {
      if (serverClient)      {
        serverClient.stop();
        Serial.println("Telnet Client Stop");
      }
      serverClient = telnetServer.available();
      Serial.println("New Telnet client");
      serverClient.flush(); // clear input buffer, else you get strange characters
    }
  }
  while (serverClient.available())
  { // get data from Client
    Serial.write(serverClient.read());
  }
  while (Serial.available())
  { // get data from Client
    serverClient.write(Serial.read());
  }
  if (millis() - startTime > 2000)  { // run every 2000 ms
    startTime = millis();
    if (serverClient && serverClient.connected())
    { // send data to Client
      serverClient.print("KitchenMotion, millis: ");
      serverClient.println(millis());
      serverClient.print("LastMotion: ");
      serverClient.print("Free Heap RAM: ");
      serverClient.println(ESP.getFreeHeap());
      serverClient.print("Light: ");
      serverClient.println(hueOn);
      }
  }
  delay(10); // to avoid strange characters left in buffer
}
