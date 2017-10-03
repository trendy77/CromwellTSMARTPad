//"" NODE MCU IR RECEIVER/TEMP-HUMIDITY READER
// FORWARDS RECIVED IR CODES ONTO ANOTER NODU MCU WHICH REPLICATES THAT CODE
// MQTT SENDV.....
//
// ipaddy:port/stats
// SERVER @@@ server(29876);

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>
#include <dht.h>
#include <IRrecv.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <PubSubClient.h>

// TIME, INTERNAL VARS
long lastChk, lastPrint, timeGone = 0;
// EMP AND HUMIDITY VARS
int value, temp, humid = 0;
dht DHT;
#define DHT11_PIN 14 //(D5)
char temperatureString[6];
char humidString[6];
// CONNECTIVITY
const char* ssid = "WiFi2";
const char* pass = "4328646518";
ESP8266WebServer server(29876);
//const char* ir_server = "192.168.0.187";
WiFiClient client;
PubSubClient clientIr(client);
const char* mqtt_server = "iot.eclipse.org";
// IR RECEIVER
uint16_t RECV_PIN = 4;   // IR detector @ GPIO pin (D2)
IRrecv irrecv(RECV_PIN);
decode_results results;
String response;

void handleRoot() {
  server.send(200, "text/plain", "hello from ir SENDER!");
}
void handleIr() {
  Serial.println("new IR client");
   while(!client.available()){
     delay(1);
   }
  // Read the first line of the request
   String req = client.readStringUntil('\r');
   Serial.println(req);
   client.flush();
     // Match the request
   if (req.indexOf("/ir") != -1){
  irsend.sendRaw(ir, sizeof(ir) / sizeof(ir[0]), khz);
    Serial.println("IRreq irTVpwr sent");
    }
    else {
      Serial.println("invalid request");
      client.stop();
      return;
    }
    client.flush();
    //client.print(s);
    client.print("HTTP/1.1 200 OK\r\n");
    delay(1);
    Serial.println("Client Disconnected");
    Serial.println();
  }
void handleStats() {
  String message = "";
  message += " temp=";
  message += temp;
  message += " humid=";
  message += humid;
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(200, "text/plain", message);
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
  dtostrf(temp,2,2 , temperatureString);
 dtostrf(humid,2,2 , humidString);
  client.publish("temp", temperatureString);
  client.publish("humid",humidString);
}
void handleNotFound(){
   String message = "File Not Found\n\n";
   message += "URI: ";
   message += server.uri();
   message += "\nMethod: ";
   message += (server.method() == HTTP_GET)?"GET":"POST";
   message += "\nArguments: ";
   message += server.args();
   message += "\n";
   for (uint8_t i=0; i<server.args(); i++){
     message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
   }
   server.send(404, "text/plain", message);
 }
void sendGCString(String str) {
   int16_t index;
   uint16_t count;
   // Find out how many items there are in the string.
   index = -1;
   count = 1;
   do {
     index = str.indexOf(',', index + 1);
     count++;
   } while (index != -1);
   code_array = reinterpret_cast<uint16_t*>(malloc(count * sizeof(uint16_t)));
   if (code_array == NULL) {  // malloc failed, so give up.
     Serial.printf("\nCan't allocate %d bytes. (%d bytes free)\n",
                   count * sizeof(uint16_t), ESP.getFreeHeap());
     Serial.println("Giving up & forcing a reboot.");
     ESP.restart();  // Reboot.
     delay(500);  // Wait for the restart to happen.
     return;  // Should never get here, but just in case.
   }
   // Now convert the strings to integers and place them in code_array.
   count = 0;
   uint16_t start_from = 0;
   do {
     index = str.indexOf(',', start_from);
     code_array[count] = str.substring(start_from, index).toInt();
     start_from = index + 1;
     count++;
   } while (index != -1);
  irsend.sendGC(code_array, count);  // All done. Send it.
   free(code_array);  // Free up the memory allocated.
 }

void conn2mqtt() {
   while (!clientIr.connected())
   {
     Serial.print("Attempting MQTT connection...");
     String clientId = "node189";
     clientId += String(random(0xffff), HEX);
     if (clientIr.connect(clientId.c_str()))
     {
       Serial.println("connected");
     clientIr.subscribe("/lounge/ir");
     } else {
       Serial.print("failed, rc=");
       Serial.print(clientIr.state());
     }
   }
 }
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void setup() {
  Serial.begin(115200);
  Serial.print('IR RELAY');
  setup_wifi();
clientIr.setServer(mqtt_server, 1883);
clientIr.setCallback(callback);
server.on("/", handleRoot);
  server.on("/stats", handleStats);
  server.on("/ir", handleIr);
  server.onNotFound(handleNotFound);
  server.begin();
  irrecv.enableIRIn();  // Start the receiver
}

void loop(void){
  timeGone = millis();
    server.handleClient();
  if (irrecv.decode(&results)) {
        String msg="Received value: ";
         String msg1= ;
         char * message[58];
        results.value.toCharArray(message,58);
	       //handleIr(msg1);
		       delay(100);
        irrecv.resume();  // Receive the next value
      }
     if ((timeGone - lastChk) > 30000){
    conn2mqtt();
     getTemperature();
     lastChk = millis();
   }
}
