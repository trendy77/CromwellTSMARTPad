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

const char* ssid = "LAN_Time_Forgot";
const char* password = "tasmania";
MDNSResponder mdns;
int khz = 38; // 38kHz carrier frequency for both NEC and Samsung
IRsend irsend(4); //an IR led is connected to GPIO4 (pin D2 on NodeMCU)

  // Insert RAW IR signal for "TV Power"
unsigned int irTVpwr[] = {4650,4250, 700,1550, 650,1550, 700,1550, 650,450, 650,500, 600,500, 600,500, 600,550, 550,1700, 550,1650, 600,1650, 550,550, 600,500, 600,550, 550,550, 600,500, 600,550, 550,1650, 600,550, 550,550, 600,500, 600,550, 550,550, 600,500, 600,1650, 600,500, 600,1650, 550,1700, 550,1650, 600,1650, 550,1650, 600,1650, 600};  // SAMSUNG E0E040BF
  // Insert RAW IR signal for "TV Source"
unsigned int irTVsrc[] = {4600,4300, 700,1550, 650,1550, 650,1600, 650,450, 650,450, 600,550, 550,550, 600,500, 600,1650, 550,1650, 600,1650, 550,550, 600,500, 600,550, 550,550, 550,550, 600,1650, 550,550, 550,550, 600,500, 600,500, 600,550, 550,550, 600,500, 600,550, 550,1650, 550,1700, 550,1650, 600,1600, 600,1650, 600,1600, 600,1650, 550};  // SAMSUNG E0E0807F
  // Insert RAW IR signal for "TV Mute"
unsigned int irTVmute[] = {4650,4250, 700,1550, 650,1550, 700,1550, 650,450, 650,500, 600,500, 600,500, 600,500, 600,1650, 600,1600, 600,1650, 550,550, 600,500, 600,550, 550,550, 600,500, 600,1650, 550,1650, 600,1650, 550,1650, 600,550, 550,550, 550,550, 600,500, 600,550, 550,550, 550,550, 600,500, 600,1650, 550,1650, 600,1650, 550,1650, 600};  // SAMSUNG E0E0F00F
    // Insert RAW IR signal for "TV Volume Down"
unsigned int irTVvdn[] = {4650,4250, 700,1550, 650,1550, 700,1550, 650,450, 650,450, 650,450, 600,550, 550,550, 600,1650, 550,1650, 550,1650, 600,550, 550,550, 550,550, 600,500, 600,500, 600,1650, 600,1600, 600,500, 600,1650, 550,550, 600,500, 600,500, 600,550, 550,550, 600,500, 600,1650, 550,550, 550,1650, 600,1650, 550,1650, 600,1650, 550};  // SAMSUNG E0E0D02F
  
  // Insert RAW IR signal for "TV Volume Up"
unsigned int irTVvup[] = {4600,4300, 650,1600, 650,1550, 650,1600, 600,500, 600,550, 600,500, 600,550, 550,550, 550,1700, 550,1650, 600,1650, 550,550, 600,500, 600,550, 550,550, 600,500, 600,1650, 600,1650, 550,1650, 600,550, 550,550, 600,500, 600,550, 550,550, 600,500, 600,550, 550,550, 600,1600, 600,1650, 600,1650, 550,1650, 600,1650, 600};  // SAMSUNG E0E0E01F
  
  // Insert RAW IR signal for "TV Channel Up"
unsigned int irTVchup[] = {4650,4250, 700,1550, 650,1600, 650,1550, 650,500, 600,500, 600,500, 650,500, 600,500, 600,1650, 550,1650, 600,1650, 600,500, 600,500, 600,550, 550,550, 600,550, 550,550, 550,1650, 600,550, 600,500, 600,1650, 550,550, 600,500, 600,550, 550,1650, 600,550, 550,1650, 600,1650, 600,500, 600,1650, 600,1600, 600,1650, 600};  // SAMSUNG E0E048B7
  
  // Insert RAW IR signal for "TV Channel Down"
unsigned int irTVchdn[] = {4600,4350, 650,1550, 650,1600, 650,1600, 600,500, 600,500, 600,550, 550,550, 600,550, 550,1650, 600,1650, 550,1700, 550,550, 550,550, 600,500, 600,550, 550,550, 600,500, 600,550, 550,550, 550,550, 600,1650, 600,500, 600,500, 600,550, 550,1650, 600,1650, 600,1650, 550,1650, 600,550, 550,1650, 600,1650, 600,1650, 550};  // SAMSUNG E0E008F7
  
  // Insert RAW IR signal for "Receiver Power"
unsigned int irRECpwr[] = {9050,4350, 650,500, 600,1600, 600,500, 650,500, 600,1600, 600,550, 600,1600, 600,1650, 550,550, 600,500, 600,1600, 650,1600, 600,500, 600,1650, 600,1600, 600,500, 600,1650, 600,1600, 600,550, 600,1600, 600,500, 600,550, 600,1600, 600,1600, 650,500, 600,500, 600,1600, 650,500, 600,1600, 600,1650, 600,500, 600,500, 600};  // NEC 4B36D32C

  // Insert RAW IR signal for "Receiver Power On"
unsigned int irRECpwrON[] = {9000,4400, 600,550, 600,1600, 600,500, 600,550, 600,1600, 600,500, 600,1600, 650,1600, 600,1600, 600,500, 650,1600, 600,1600, 600,500, 650,1600, 600,1600, 600,500, 600,550, 600,500, 600,1600, 600,550, 600,500, 600,500, 650,500, 600,500, 600,1600, 650,1600, 600,500, 600,1600, 650,1600, 600,1600, 600,1600, 600,1600, 650};  // NEC 4BB620DF
  
  // Insert RAW IR signal for "Receiver Power Off"
unsigned int irRECpwrOFF[] = {9000,4400, 600,550, 550,1650, 600,550, 550,550, 600,1650, 550,550, 600,1650, 550,1650, 600,550, 550,550, 550,1650, 600,1650, 600,550, 550,1650, 600,1650, 550,550, 600,1650, 550,1650, 600,1650, 600,500, 600,550, 550,550, 600,1650, 550,550, 600,500, 600,550, 550,550, 550,1700, 550,1650, 600,1650, 550,550, 600,1650, 550};  // NEC 4B36E21D
  // Insert RAW IR signal for "Receiver Mute"
unsigned int irRECmute[] = {9000,4400, 650,450, 650,1600, 600,500, 600,500, 650,1600, 600,500, 600,1650, 600,1600, 600,1600, 650,500, 600,1600, 650,1600, 600,500, 600,1600, 650,1600, 600,500, 600,1650, 600,500, 600,1600, 650,500, 600,500, 600,500, 600,500, 650,500, 600,500, 600,1600, 650,500, 600,1600, 600,1600, 650,1600, 600,1650, 600,1600, 600};  // NEC 4BB6A05F
 // Insert RAW IR signal for "Receiver Volume Down"
unsigned int irRECvdn[] = {9150,4250, 750,350, 700,1550, 700,400, 700,450, 650,1550, 700,450, 600,1600, 650,1600, 600,1650, 600,500, 600,1650, 600,1600, 600,550, 600,1600, 600,1650, 600,500, 600,1650, 600,1600, 650,500, 600,500, 600,500, 650,500, 600,500, 600,500, 600,550, 600,500, 600,1650, 600,1600, 600,1650, 600,1650, 600,1600, 600,1650, 600};  // NEC 4BB6C03F
 // Insert RAW IR signal for "Receiver Volume Up"
unsigned int irRECvup[] = {9050,4400, 650,500, 600,1600, 600,550, 600,500, 600,1650, 600,500, 600,1600, 650,1600, 600,1600, 600,550, 600,1600, 600,1600, 650,500, 600,1600, 650,1600, 600,500, 600,550, 600,1600, 600,550, 600,500, 600,550, 600,500, 600,550, 600,500, 600,1600, 650,500, 600,1600, 600,1650, 600,1600, 600,1650, 600,1600, 600,1600, 600};  // NEC 4BB640BF
  // Insert RAW IR signal for "Receiver Source CBL/SAT"
unsigned int irRECsrc[] = {8950,4450, 600,500, 600,1650, 600,500, 600,500, 600,1650, 600,500, 600,1600, 600,1650, 600,1600, 600,550, 600,1600, 600,1650, 600,500, 600,1600, 600,1650, 600,500, 600,500, 600,1650, 600,1600, 600,1650, 600,500, 600,500, 600,500, 650,500, 600,1600, 600,500, 600,550, 600,500, 600,1600, 600,1650, 600,1600, 600,1650, 600};  // NEC 4BB6708F

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


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
  else if (req.indexOf("/irTVsrc") != -1){
      irsend.sendRaw(irTVsrc, sizeof(irTVsrc) / sizeof(irTVsrc[0]), khz);   
      Serial.println("IRreq irTVsrc sent");
  }
  else if (req.indexOf("/irTVmute") != -1){
      irsend.sendRaw(irTVmute, sizeof(irTVmute) / sizeof(irTVmute[0]), khz);   
      Serial.println("IRreq irTVmute sent");
  }
  else if (req.indexOf("/irTVvdn") != -1){
      irsend.sendRaw(irTVvdn, sizeof(irTVvdn) / sizeof(irTVvdn[0]), khz);   
      Serial.println("IRreq irTVvdn sent");
  }
  else if (req.indexOf("/irTVvup") != -1){
      irsend.sendRaw(irTVvup, sizeof(irTVvup) / sizeof(irTVvup[0]), khz);   
      Serial.println("IRreq irTVvup sent");
  }
  else if (req.indexOf("/irTVchup") != -1){
      irsend.sendRaw(irTVchup, sizeof(irTVchup) / sizeof(irTVchup[0]), khz);   
      Serial.println("IRreq irTVchup sent");
  }
  else if (req.indexOf("/irTVchdn") != -1){
      irsend.sendRaw(irTVchdn, sizeof(irTVchdn) / sizeof(irTVchdn[0]), khz);   
      Serial.println("IRreq irTVchdn sent");
  }
  else if (req.indexOf("/irALLpwr") != -1){
      irsend.sendRaw(irRECpwrON, sizeof(irRECpwrON) / sizeof(irRECpwrON[0]), khz);   
      irsend.sendRaw(irTVpwr, sizeof(irTVpwr) / sizeof(irTVpwr[0]), khz);   
      delay(2000);
      irsend.sendRaw(irRECsrc, sizeof(irRECsrc) / sizeof(irRECsrc[0]), khz);         
      Serial.println("IRreq irALLpwr sent");
  }
  else if (req.indexOf("/irRECpwr") != -1){
      irsend.sendRaw(irRECpwr, sizeof(irRECpwr) / sizeof(irRECpwr[0]), khz);   
      Serial.println("IRreq irRECpwr sent");
  }
  else if (req.indexOf("/irRECpwrON") != -1){
      irsend.sendRaw(irRECpwrON, sizeof(irRECpwrON) / sizeof(irRECpwrON[0]), khz);   
      Serial.println("IRreq irRECpwrON sent");
  }
  else if (req.indexOf("/irRECpwrOFF") != -1){
      irsend.sendRaw(irRECpwrOFF, sizeof(irRECpwrOFF) / sizeof(irRECpwrOFF[0]), khz);   
      Serial.println("IRreq irRECpwrOFF sent");
  }
  else if (req.indexOf("/irRECmute") != -1){
      irsend.sendRaw(irRECmute, sizeof(irRECmute) / sizeof(irRECmute[0]), khz);   
      Serial.println("IRreq irRECmute sent");
  }  
  else if (req.indexOf("/irRECvdn") != -1){
      irsend.sendRaw(irRECvdn, sizeof(irRECvdn) / sizeof(irRECvdn[0]), khz);   
      Serial.println("IRreq irRECvdn sent");
  }
  else if (req.indexOf("/irRECvup") != -1){
      irsend.sendRaw(irRECvup, sizeof(irRECvup) / sizeof(irRECvup[0]), khz);   
      Serial.println("IRreq irRECvup sent");
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
