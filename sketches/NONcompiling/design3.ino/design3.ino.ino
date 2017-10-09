#include <RemoteDebug.h>

#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#include <WiFiClient.h>
WiFiClient client;
int pirPin = D1;
int pirState = LOW;

#define HOST_NAME "tnode188"        // Host mDNS
RemoteDebug Debug;
int status = WL_IDLE_STATUS;      // the Wifi radio's status
char bridge_ip[] = "192.168.0.101";  // IP address of the HUE bridge
const char *pathGet = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/5"; 
const char *pathPut = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/5/state"; 
      // make a wifi instance and a HttpClient instance:
const char ssid[] = "LordProtectorOfTheWiFi";
const char pass[] = "4328646518";
const int port = 80;
#define pirPin D1 // Input for PIR AS312
int pirValue;     // Place to store read PIR Value

void setup() {
  // Initialize the Serial (educattional use only, not need in production)
  Serial.begin(115200);
  // Buildin led off ESP8266
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  pinMode(pirPin, INPUT);
  pirValue = LOW;
  // WiFi connection
  WiFi.begin(ssid, pass);    Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)    {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");    Serial.print("Connected to ");    Serial.println(ssid);
  Serial.print("IP address: ");    Serial.println(WiFi.localIP());

  // Register host name in WiFi and mDNS
  String hostNameWifi = HOST_NAME;
  hostNameWifi.concat(".local");
  WiFi.hostname(hostNameWifi);
  
 // MDNS.addService("telnet", "tcp", 23);
  // Initialize the telnet server of RemoteDebug
  Debug.begin(HOST_NAME); // Initiaze the telnet server
  Debug.setResetCmdEnabled(true); // Enable the reset command
  Debug.showTime(true); // To show time
  Serial.println("");    Serial.print("Connected to ");    Serial.println(ssid);
  Serial.print("IP address: ");    Serial.println(WiFi.localIP());

  
  hostNameWifi.concat(".local");
  WiFi.hostname(hostNameWifi);
  // Initialize the telnet server of RemoteDebug
  Debug.begin(HOST_NAME); // Initiaze the telnet server
  Debug.setResetCmdEnabled(true); // Enable the reset command
  Debug.showTime(true); // To show time
    
  if (!client.connect(bridge_ip, port)) {
    if (Debug.isActive(Debug.WARNING))   {
      Debug.println("('CONNECTION FAILED')");
    } return;
  }
  }
 
void loop() {
if (Debug.isActive(Debug.INFO))   {
    Debug.print(Serial.read());
  }  
  delay(2000);
  int inputState = digitalRead(pirPin);
  String response = "";
   String response1 = "";
   int status = client.get(pathGet, &response);  
 Serial.print(response);
 Debug.println(response);
  delay(2000);
  int status1 = client.put(pathPut, &response1);   // turn light on
 delay(2000);
  
  delay(2000);
}

void sendRequest(int light, String cmd, String value) {
    // make a String for the HTTP request path:
  String request = "/api/" + hueUserName;
  request += "/lights/";
  request += light;
  request += "/state/";
  String contentType = "application/json";
    // make a string for the JSON command:
  String hueCmd = "{\"" + cmd;
  hueCmd += "\":";
  hueCmd += value;
  hueCmd += "}";
    // see what you assembled to send:
  Serial.print("PUT request to server: ");
  Serial.println(request);
  Serial.print("JSON command to server: ");
    // make the PUT request to the hub:
  client.println("PUT "+ request);
 client.println(contentType);
  client.println("");
  client.println(hueCmd);
  // read the status code and body of the response
  //int statusCode = httpClient.responseStatusCode();
    Serial.println(hueCmd);
  while (client.available()) {
   	String line = client.readStringUntil('\r');
			Serial.print(line);
		}
}



void sendCheck(int light) {
  Serial.print("GET light state");
   Serial.println(pathGet);
   String response = "";
      // make the GET request to the hub:
 client.get(pathGet, &response);
  int statusCode = httpClient.responseStatusCode();
     while (client.available()) {
     String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }




