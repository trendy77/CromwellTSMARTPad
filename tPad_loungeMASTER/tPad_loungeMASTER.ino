
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <ESP8266WiFi.h>
#include <RestClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// IR RECEIVER
  uint16_t RECV_PIN = D1;   // IR detector @ GPIO pin 5 (d1)
  IRrecv irrecv(RECV_PIN);
  decode_results results;
// IR SENDER
  IRsend irsend(14);  // An IR LED is controlled by GPIO pin 4 (D2) gpio5=pind1
  #define TV_OFF        0xa90
  
  
// Wi-Fi / IoT
const char ssid[] = "WiFi2";
const char password[] = "4328646518";
const char LIGHTS_ON[] = "{\"on\":true}";
const char LIGHTS_OFF[] = "{\"on\":false}";
const char *bridge_ip = "192.168.0.101"; // Hue Bridge IP
const int port = 80;
MDNSResponder mdns;
ESP8266WebServer server(80);
RestClient hue = RestClient(bridge_ip);


// Motion PIR sensor
#define pirPin D2
unsigned long stars,down=0;
int pirState = 0;
int pirVal = LOW;









void hue_turnOff(){
  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/9/state/", LIGHTS_OFF);
}
void hue_turnOn(){
  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/9/state/", LIGHTS_ON);
}

void handleRoot() {
  server.send(200, "text/html",
              "<html>" \
                "<head><title>NodeIR Demo</title></head>" \
                "<body>" \
                  "<h1>Hello from NodeIR" \
                      "send signals from here!</h1>" \
                  "<p><a href=\"ir?code=16769055\">Send 0xFFE01F</a></p>" \
                  "<p><a href=\"ir?code=16429347\">Send 0xFAB123</a></p>" \
                  "<p><a href=\"ir?code=16771222\">Send 0xFFE896</a></p>" \
                "</body>" \
              "</html>");
}

void luxon() {

}


void tvon() {

}

void luxoff(){

}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  server.send(404, "text/plain", message);
  Serial.print(message);
}



























































































#include <dht.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#define SLEEP_DELAY_IN_SECONDS  30
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtimer.h>
#include <IRutils.h>


IRsend irsend(12);  // An IR LED is controlled by GPIO pin 4 (D2)



// TIME, INTERNAL VARS
long lastChk, lastPrint, timeGone = 0;
char msg[50];
int checks = 0;

// TEMP AND HUMIDITY VARS
int value, temp, humid = 0;
dht DHT;
#define DHT11_PIN 14 //(D5)
char temperatureString[6];
char humidString[6];


WiFiServer telnetServer(23);
// Host mDNS
#define HOST_NAME "tnode189"
// IR RECEIVER
uint16_t RECV_PIN = 12;   // IR detector @ GPIO pin (D6)
IRrecv irrecv(RECV_PIN);
decode_results results;
// MQTT
const char* mqtt_server = "m12.cloudmqtt.com";
const char* mqtt_username = "node189";
const char* mqtt_password = "Joker999";
const char* mqtt_topic = "lounge_temp";
const char* mqtt_topic2 = "lounge_humid";
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}









void setup_wifi() {
  delay(10);
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
  Serial.println(WiFi.localIP());
}

void postStats() {
  while (!client.connected()) {
   Serial.print("Attempting connection...");
   if (client.connect("node189", mqtt_username, mqtt_password)) {
      Serial.println("connected");
  client.publish(mqtt_topic, temperatureString);
 client.publish(mqtt_topic2, humidString);
 Serial.print('sent to MQTT');
 client.disconnect();
 Serial.print('disconnected');
 delay(100);
 } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
        }
  }
}
 
void getTemperature() {
  int chk = DHT.read11(DHT11_PIN);
  temp = DHT.temperature;
  humid = DHT.humidity;
}

void serialTemp() {
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  Serial.println(" ");
  Serial.print("int Temperature = ");
  Serial.println(temp);
  Serial.print("int Humidity = ");
  Serial.println(humid);
  Serial.println(" ");
}































void setup() {
  Serial.begin(115200);
  setup_wifi();
    String hostNameWifi = HOST_NAME;
    hostNameWifi.concat(".local");
    WiFi.hostname(hostNameWifi);
    if (MDNS.begin(HOST_NAME)) {
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println(HOST_NAME);
    }
    MDNS.addService("telnet", "tcp", 23);
    server_start(0);
    Serial.println("* Arduino RemoteDebug Library");
    Serial.println("*");
    Serial.print("* WiFI connected. IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("*");
    irrecv.enableIRIn();  // Start the receiver
    irsend.begin();
   }

void loop() {
  timeGone = millis();
  
 if (irrecv.decode(&results)) {
    serialPrintUint64(results.value, HEX);
    Serial.println("IR");
    irrecv.resume();  
    // Receive the next value
  delay(100);
  }

    if ((millis() - mLastTime) >= 5000) {
        mLastTime = millis();
		getTemperature();
		serialTemp();
		mqttConnect();
		}
	
   } 