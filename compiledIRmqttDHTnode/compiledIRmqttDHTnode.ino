#include <ESP8266WiFi.h>
#include <dht.h>
#include <PubSubClient.h>
#define SLEEP_DELAY_IN_SECONDS  30
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <ir_Argo.h>
#include <ir_Daikin.h>
#include <ir_Fujitsu.h>
#include <ir_Kelvinator.h>
#include <ir_LG.h>
#include <ir_Mitsubishi.h>
#include <ir_Trotec.h>

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

// CONNECTIVITY
const char* ssid = "WiFi2";
const char* password = "4328646518";
WiFiClient espClient;
PubSubClient client(espClient);

// IR RECEIVER
uint16_t RECV_PIN = 4;   // IR detector @ GPIO pin (D2)
IRrecv irrecv(RECV_PIN);
decode_results results;

// MQTT
const char* mqtt_server = "m12.cloudmqtt.com";
const char* mqtt_username = "yklkyogj";
const char* mqtt_password = "fe0xxxhDMx2t";
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

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 14303);
  //client.setServer(mqtt_server2, 1883);
  client.setCallback(callback);
  irrecv.enableIRIn();  // Start the receiver
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
   if (client.connect("node189", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    mqttTemp();
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

void loop() {
  timeGone = millis();
 if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, HEX);
    Serial.println("");
    irrecv.resume();  // Receive the next value
  }
  delay(100);
    if ((timeGone - lastChk) > 30){
    getTemperature();
    lastChk = millis();
    checks++;
      if (checks > 2){
      serialTemp();
      checks =0;
      }
    }
//if (!client.connected()) {
  //  reconnect();
 // }
 //client.loop();
}

void mqttTemp(){
 dtostrf(temp, 2, 2, temperatureString);
 dtostrf(humid, 2, 2, humidString);
 client.publish("lounge_temp", temperatureString);
 client.publish("lounge_humid", humidString);
 Serial.print('sent to MQTT');
 client.disconnect();
 Serial.print('disconnected');
 WiFi.disconnect();
 delay(100);
}
