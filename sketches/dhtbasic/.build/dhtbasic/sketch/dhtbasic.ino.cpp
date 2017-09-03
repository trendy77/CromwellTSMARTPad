#include <Arduino.h>
#line 1 "c:\\Users\\trend\\Documents\\GitHub\\TSMARTPad\\dhtbasic\\dhtbasic.ino"
#line 1 "c:\\Users\\trend\\Documents\\GitHub\\TSMARTPad\\dhtbasic\\dhtbasic.ino"
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

#include <DHT.h>

#include <PubSubClient.h>
#define SLEEP_DELAY_IN_SECONDS  30

long lastMsg = 0;
char msg[50];

int value = 0;
dht DHT;
#define DHT11_PIN 2

const char* ssid = "WiFi2";
const char* password = "4328646518";
const char* mqtt_server = "<MQTT_BROKER_IP_ADDRESS>";
const char* mqtt_username = "<MQTT_BROKER_USERNAME>";
const char* mqtt_password = "<MQTT_BROKER_PASSWORD>";
const char* mqtt_topic = "sensors/test/temperature";
wifiClient espClient;
PubSubClient client(espClient);

char temperatureString[6];
char humidString[6];



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
   if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
int temp, humid = 0;

void getTemperature() {
   int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  temp = DHT.temperature;
  humid = DHT.humidity;
}
 

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 getTemperature();
  // convert temperature to a string with two digits before the comma and 2 digits for precision
  dtostrf(temp, 2, 2, temperatureString);
  dtostrf(humid, 2, 2, humidString);
    client.publish("lounge_temp", temperatureString);
    client.publish("lounge_humidity", humidString);
  Serial.print('sending to MQTT');
  client.disconnect();
  Serial.print('disconnected');
  WiFi.disconnect();
  delay(100);
  //Serial << "Entering deep sleep mode for " << SLEEP_DELAY_IN_SECONDS << " seconds..." << endl;
  //ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
  //ESP.deepSleep(10 * 1000, WAKE_NO_RFCAL);
  //delay(500);   // wait for deep sleep to happen
}

