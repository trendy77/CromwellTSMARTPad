#include <restclient.h>
#include <ESP8266WiFi.h>

const char LIGHTS_ON[] = "{\"on\":true}";			// LENGTH OF 11
const char LIGHTS_OFF[] = "{\"on\":false}";			// LENGTH OF 12
const char EFFECT_COLORLOOP[] = "{\"effect\":\"colorloop\"}";		// LENGTH OF 22?
const char EFFECT_NONE[] = "{\"effect\":\"none\"}";				LENGTH OF 17?

int dots = 0;
bool hueOn;  // on/off
int hueBri;  // brightness value
long hueHue;  // hue value
String hueCmd;  // Hue command
const char statePoint[] = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state";
int pirState = LOW;
unsigned long timer=0;
int val = LOW;
// Wifi Settings
//const char* ssid = "LAN_Time_Forgot";
//const char* password = "tasmania";
const char ssid[] = "LordProtectorOfTheWiFi";
const char password[] = "4328646518";

WiFiServer telnetServer(23);
WiFiClient serverClient;
unsigned long startTime = millis();

// Hue Settings
const char* bridge_ip = "192.168.0.101"; // Hue Bridge IP
const int port = 80;
String user1 = "fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG";
restclient hue(bridge_ip, port);
unsigned long lastMove=0;

// MotionSensor(s)
#define pirPin D1 // Input for PIR AS312
int pirValue; // Place to store read PIR Value


void hue_stateCheck() {
  Serial.print("Connecting to ");
  Serial.println(bridge_ip);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(bridge_ip, port)) {
    Serial.println("Connection failed");
    return;
  }
  // This will send the request to the server
  client.println("GET /api/" + user1 + "/lights/1");
  client.println("Host: " + String(bridge_ip) + ":" + String(port));
  client.println("User-Agent: ESP8266/1.0");
  client.println("Connection: close");
  client.println("Content-type: text/xml; charset=\"utf-8\"");
  client.println("Content-length: 0");
  delay(10);
  while (client.available()) {
   if (client.available())
      {
        client.findUntil("\"on\":", "\0");
        hueOn = (client.readStringUntil(',') == "true");  // if light is on, set variable to true
        client.findUntil("\"bri\":", "\0");
        hueBri = client.readStringUntil(',').toInt();  // set variable to brightness value
        client.findUntil("\"hue\":", "\0");
        hueHue = client.readStringUntil(',').toInt();  // set variable to hue value
 break; // not capturing other light attributes yet
      }
    }
  }
int dwn, stars;

void getPirValue(void){
pirValue = digitalRead(pirPin);
  if (pirValue == HIGH) {
   Serial.print("*^");  dots++; stars++;
      if (val == HIGH){
         if (stars=100){hue_turnOn();}
          if (dots>=100){Serial.println(".");dots=0;}
      if (val == LOW){
    Serial.println("*NEW Motion*");stars=0;val = HIGH;
} else  if (pirValue == LOW) {
 Serial.print("_");  dots++; dwn++;
     if (val == LOW) {
 if (dwn=19000){hue_turnOff();}
      if (dots>=100){Serial.println(".");dots=0;}
    if (val == HIGH){
  Serial.println("T==> Motion END"); dwn=0;val=LOW;
    }
     }
  }
      }
  }
}
void hue_turnOff(){
hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state", LIGHTS_OFF); 
}

void hue_turnOn()
{
  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state", LIGHTS_ON);
}

void setup() {
  Serial.begin(115200);
  Serial.println("COMMENCING TNODE LIGHTSENSOR");
  delay(10);
    // We connect to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  delay(500);
  hue_turnOff();  delay(500);
  pinMode(pirPin, INPUT);
  val = LOW;
}
bool motion;

  void loop()
  {
  getPirValue(); 
    delay(10);
  
    if (motion){
        hue_turnOn();
    lastMove = millis(); 
      } else {
    unsigned long timeSnMove = (timer - lastMove);
    if (timeSnMove >= 30000){
    hue_turnOff();
    }
    }
     delay(10);
   