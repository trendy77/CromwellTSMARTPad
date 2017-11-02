#include <RestClient.h>
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>


const char ssid[] = "WiFi2";
const char password[] = "4328646518";
const char LIGHTS_ON[] = "{\"on\":true}";
const char LIGHTS_OFF[] = "{\"on\":false}";
const char *bridge_ip = "192.168.0.101"; // Hue Bridge IP
const int port = 80;
RestClient hue = RestClient(bridge_ip);
WiFiServer server(80);


void hue_turnOff(int light, const char cmd){
	const char addy[] = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/" + light + "/state/"
hue.put(addy, cmd);
}
void hue_turnOn(){
  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state/", LIGHTS_ON);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("COMMENCING TNODE LIGHTSENSOR");
 
  delay(10);
  
  // We connect to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED)
 {
    delay(500);
    Serial.print(".");
 }
 Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
    if (!MDNS.begin("esp8266")) {
    Serial.println("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
    }
server.begin();
  Serial.println("TCP server started");
  
  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
delay(500);
hue_turnOff();
}
 int x =0;
 String ss = "";
void hue_bue(String ss){
 
  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start1 = ss.indexOf('hue=');
  int addr_end1 = ss.indexOf('&', addr_start1 + 1);
  String hue = ss.substring(addr_start1 + 1, addr_end1);

  int addr_start3 = ss.indexOf('bri=');
  int addr_end3 = ss.indexOf('&', addr_start3 + 1);
String bri= ss.substring(addr_start3 + 1, addr_end3);

int addr_start2 = ss.indexOf('sat=');
  int addr_end2 = ss.indexOf('&', addr_start2 + 1);
String sat= ss.substring(addr_start2 + 1, addr_end2);

  Serial.print("Hue: ");
  Serial.println(hue);
  
  Serial.print("bra: ");
  Serial.println(bri);
  
  Serial.print("Sat: ");
  Serial.println(sat);
  
}

void deal(){
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("");
  
  // Wait for data from client to become available
  while(client.connected() && !client.available()){
    delay(1);
  }
  
  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');
  
  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
    Serial.print("Invalid request: ");
    Serial.println(req);
    return;
  }
  req = req.substring(addr_start + 1, addr_end);
  String s;
  ss=req;
  Serial.print("Request: ");
  Serial.println(req);
  client.flush();
  
  if (req == "/")
  {
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
    s += ipStr;
    s += "</html>\r\n\r\n";
    Serial.println("Sending 200");
  
  }
 if (req == "/1")
   {
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
    s += ipStr;
    s += "</html>\r\n\r\n";
    Serial.println("Sending 200");
  hue_turnOn();
  }
  if (req.indexOf("/hue")!=-1){
    hue_bue(ss);
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
    s += ipStr;
    s += "</html>\r\n\r\n";
    Serial.println("Sending 200");
  }
   if (req == "/2")
   {
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
    s += ipStr;
    s += "</html>\r\n\r\n";
    Serial.println("Sending 200");
  hue_turnOff();
  }
  else
  {
    s = "HTTP/1.1 404 Not Found\r\n\r\n";
    Serial.println("Sending 404");
  }
  client.print(s);
  
  client.print(x);
  
  Serial.println("Done with client");
  }

 
void loop() {
 deal();
 if(Serial.available())
    {
      String str = Serial.readStringUntil('\n');
    x = Serial.parseInt();
        if (x>0){

       if (x==1){
       hue_turnOn();
    }
    if (x==2){
  hue_turnOff();
    }
    }
  delay(10);
 
    }
}