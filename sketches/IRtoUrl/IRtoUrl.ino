/*
 * WiFi IR Blaster by Buddy Crotty
 *  Use an ESP8266 module or dev board to receive HTTP GET request
 *  and then send IR codes to an attached IR LED based on those requests.
 *  This works best with another web server acting as a front end that 
 *  sends cURL requests based on which buttons are pressed. 
 *  cURL format: http://ESP8266/IRcode
 */
#include <IRsend.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;
int khz = 38; // 38kHz carrier frequency for both NEC and Samsung
IRsend irsend(4); //an IR led is connected to GPIO4 (pin D2 on NodeMCU)

unsigned int irTVpwr[] = {"0000 0068 0000 000d 0060 0018 0030 0018 0030 0018 0030 0018 0030 0018 0018 0018 0030 0018 0018 0018 0030 0018 0018 0018 0018 0018 0018 0018 0018 03de"};  // SAMSUNG E0E040BF
  
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
const char* ssid = "WiFi2";
const char* pass = "4328646518";

void setup() {
  Serial.begin(115200);
  delay(10);

  irsend.begin();
  
  // Connect to WiFi network
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
  Serial.println("WiFi Connected");

  // Start the server
  server.begin();
  Serial.println("HTTP Server Started");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("IRBlasterLR", WiFi.localIP())) {
    Serial.println("MDNS Responder Started");
  }

  Serial.println();
  Serial.println();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  if (req.indexOf("/irTVpwr") != -1){
      irsend.sendRaw(irTVpwr, sizeof(irTVpwr) / sizeof(irTVpwr[0]), khz);   
      Serial.println("IRreq irTVpwr sent");
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  
  client.flush();
   
  // Send the response to the client
  //client.print(s);
  client.print("HTTP/1.1 200 OK\r\n");
  delay(1);
  Serial.println("Client Disconnected");
  Serial.println();
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
