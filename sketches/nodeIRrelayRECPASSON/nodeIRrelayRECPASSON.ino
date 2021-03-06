#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
#include <PubSubClient.h>
#include <dht.h>
#include <ESP8266WiFi.h>

#define PubNub_BASE_CLIENT = client;
const static char pubkey[] = "pub-c-7f535266-ef71-47d8-80be-bd9d391e6661";         // your publish key
const static char subkey[] = "sub-c-901180e0-8cfa-11e7-91ed-aa3b4df5deac";         // your subscribe key
const static char channel[] = "humidity"; // channel to use
const static char channel2[] = "temperature"; // channel to use

#define SLEEP_DELAY_IN_SECONDS  30
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtimer.h>
#include <IRutils.h>

// TIME, INTERNAL VARS
long lastChk, lastPrint, timeGone = 0;
char msg[50];
int checks = 0;
// EMP AND HUMIDITY VARS
int value, temp, humid = 0;
dht DHT;
#define DHT11_PIN 14 //(D5)
char temperatureString[6];
char humidString[6];
#define PubNub_BASE_CLIENT WiFiClient
PubNub.begin(pubkey, subkey);
// CONNECTIVITY
WiFiClient client;
const char* ssid = "WiFi2";
const char* pass = "4328646518";
MDNSResponder mdns;
// IR RECEIVER
uint16_t RECV_PIN = 4;   // IR detector @ GPIO pin (D2)
IRrecv irrecv(RECV_PIN);
decode_results results;

void getTemperature() {
  int chk = DHT.read11(DHT11_PIN);
  temp = DHT.temperature;
  humid = DHT.humidity;
Serial.print("int Temperature = ");
  Serial.println(temp);
  Serial.print("int Humidity = ");
  Serial.println(humid);
  Serial.println(" ");
 WiFiClient *client;
    client = PubNub.publish(channel, temp);
delay(500);
client = PubNub.publish(channel2, humid);
  if (!client) {
    Serial.println("publishing error");
    delay(1000);
    return;
  }
  if (PubNub.get_last_http_status_code_class() != PubNub::http_scc_success) {
    Serial.print("Got HTTP status code error from PubNub, class: ");
    Serial.print(PubNub.get_last_http_status_code_class(), DEC);
  }
  while (client->available()) {
    Serial.write(client->read());
  }
  client->stop();
  Serial.println("---");

}

void getTemperature() {
  int chk = DHT.read11(DHT11_PIN);
  temp = DHT.temperature;
  humid = DHT.humidity;
Serial.print("int Temperature = ");
  Serial.println(temp);
  Serial.print("int Humidity = ");
  Serial.println(humid);
  Serial.println(" ");
 WiFiClient *client;
    client = PubNub.publish(channel, temp);
delay(500);
client = PubNub.publish(channel2, humid);
  if (!client) {
    Serial.println("publishing error");
    delay(1000);
    return;
  }
  if (PubNub.get_last_http_status_code_class() != PubNub::http_scc_success) {
    Serial.print("Got HTTP status code error from PubNub, class: ");
    Serial.print(PubNub.get_last_http_status_code_class(), DEC);
  }
  while (client->available()) {
    Serial.write(client->read());
  }
  client->stop();
  Serial.println("---");
}

void setup(void){
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Serial.println("");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);  Serial.print(".");
    }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
    if (mdns.begin("nodeIRsendr", WiFi.localIP())) {
      Serial.println("MDNS responder started");
    }


}




void loop(void){
  timeGone = millis();
  if (irrecv.decode(&results)) {
    serialPrintUint64(results.value, 16);
        dump(&results);
        irrecv.resume();  // Receive the next value
      }
     if ((timeGone - lastChk) > 30000){
     getTemperature();
   }
}
