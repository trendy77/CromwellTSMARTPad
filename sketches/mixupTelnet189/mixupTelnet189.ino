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
#include <ir_Argo.h>
#include <ir_Daikin.h>
#include <ir_Fujitsu.h>
#include <ir_Kelvinator.h>
#include <ir_LG.h>
#include <ir_Mitsubishi.h>
#include <ir_Trotec.h>

IRsend irsend(12);  // An IR LED is controlled by GPIO pin 4 (D2)

// Host mDNS
#define HOST_NAME "tnode189"

// Time
uint32_t mLastTime = 0;
uint32_t mTimeSeconds = 0;
uint32_t mtLastTime = 0;
uint32_t mtTimeSeconds = 0;
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
WiFiServer telnetServer(23);
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
   
    Serial.println("* Arduino RemoteDebug Library");
    Serial.println("*");
    Serial.print("* WiFI connected. IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("*");
   
    irrecv.enableIRIn();  // Start the receiver
        irsend.begin();
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

 void send (){
        Serial.println("NEC");
  irsend.sendNEC(0x00FFE01FUL, 32);
 Serial.println("Sony");
  irsend.sendSony(0xa90, 12, 2);
  delay(100);
  irrecv.enableIRIn();  // Start the receiver
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
    irrecv.resume();  
    // Receive the next value
  delay(100);
  }

    if ((millis() - mLastTime) >= 5000) {

        // Time

        mLastTime = millis();

getTemperature();
 serialTemp();
//    if (!client.connected()) {
 
 //}
  
          
}
   
        if (mTimeSeconds % 5 == 0) { // Each 5 seconds
      
   mqttConnect();
     }
} 
