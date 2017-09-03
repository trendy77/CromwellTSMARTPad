#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "WiFi2";
const char* password = "4328646518";
String  command =""; // Command received from Android device
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3); // RX, TX

void setup()
{
  Serial.begin(115200);
mySerial.begin(4800); 
 mySerial.println("Hello, world?"); 
connectWiFi();
  server.begin();
}

void loop()
{
    client = server.available();
    if (!client) return; 
    command = checkClient ();

    if (command == "on" || command == "frente" || command == "a frente")  forwardMotor();
    else if (command == "off" || command == "reverso"|| command == "voltar") reverseMotor();
    else if (command == "blue"    || command == "esquerda") leftMotor();    
    else if (command == "red"   || command == "direita") rightMotor();     
    else if (command == "green"    || command == "pare" || command == "parar" || command == "parando")     stopMotor();     
     
    
	 if (mySerial.available()) {
    Serial.write(mySerial.read());
  int word = mySerial.read();
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }

  
    command = "";   
} 

/* command motor forward */
void forwardMotor(void)   
{

}

/* command motor backward */
void reverseMotor(void)   
{
 
}

/* command motor turn left */
void leftMotor(void)   
{
  
}

/* command motor turn right */
void rightMotor(void)   
{
 
}

/* command motor stop */
void stopMotor(void)   
{
  
}


void connectWiFi(){
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
}

String checkClient (void){
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}

void sendBackEcho(String echo){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println(echo);
  client.println("</html>");
  client.stop();
  delay(1);
}
