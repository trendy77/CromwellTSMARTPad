#include <ESP8266WiFi.h>

// Wifi Settings

const char* ssid = "LAN_Time_Forgot";
const char* password = "tasmania";

// Hue Settings

const char* bridge_ip = "192.168.0.101"; // Hue Bridge IP
const int port = 80;
String user1 = "fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG";
//String user2 = "gYEEPLSaIRrhV8QWrM03pzuWND-OsVYJnmPGGvMZ";
String bathlight = "9";		// light number for BATHROOM
							//String kitchenlight = "1";	// light number for KITCHEN

							// Current States
bool bathOn = false;
bool kitchOn = false;

// Commands
//String hue_check = "\";
String hue_on = "{\"on\":true}";
String hue_off = "{\"on\":false}";
String hue_check = "setup";

// MotionSensor(s)
#define pirPin D1 // Input for PIR AS312
int pirValue; // Place to store read PIR Value

void setup() {
	Serial.begin(115200);
	delay(10);
	pinMode(pirPin, INPUT);

	// We connect to a WiFi network

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
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	hue_set();

}


void hue_set() {
	Serial.print("Connecting to ");
	Serial.println(bridge_ip);
	// Use WiFiClient class to create TCP connections
	WiFiClient client;
	if (!client.connect(bridge_ip, port)) {
		Serial.println("Connection failed");
		return;
	}
	// This will send the request to the server
	client.println("PUT /api/" + user1 + "/lights/" + bathlight + "");
	client.println("Host: " + String(bridge_ip) + ":" + String(port));
	client.println("User-Agent: ESP8266/1.0");
	client.println("Connection: close");
	client.println("Content-type: text/xml; charset=\"utf-8\"");
	client.print("Content-Length: ");
	//ient.println(hue_check.length()); // PUT COMMAND HERE
	client.println();
	//client.println(hue_check); // PUT COMMAND HERE
	delay(10);

	// Read all the lines of the reply from server and print them to Serial
	while (client.available()) {
		String line = client.readStringUntil('\r');
		Serial.print(line);
	}
	Serial.println();
	Serial.println("Closing connection");
}

void turnlightOn() {
	Serial.print("Turning Light On, Connecting to ");
	Serial.println(bridge_ip);
	// Use WiFiClient class to create TCP connections
	WiFiClient client;
	if (!client.connect(bridge_ip, port)) {
		Serial.println("Connection failed");
		return;
	}
	// This will send the request to the server
	client.println("PUT /api/" + user1 + "/lights/" + bathlight + "");
	client.println("Host: " + String(bridge_ip) + ":" + String(port));
	client.println("User-Agent: ESP8266/1.0");
	client.println("Connection: close");
	client.println("Content-type: text/xml; charset=\"utf-8\"");
	client.print("Content-Length: ");
	//ient.println(hue_check.length()); // PUT COMMAND HERE
	client.println();
	//client.println(hue_check); // PUT COMMAND HERE
	delay(10);

	// Read all the lines of the reply from server and print them to Serial
	while (client.available()) {
		String line = client.readStringUntil('\r');
		Serial.print(line);
	}
	Serial.println();
	Serial.println("Closing connection");
}


bool getPirValue()
{
	pirValue = digitalRead(pirPin);
	if (pirValue)
	{
		
		Serial.println("T==> Motion detected");
	}
}


void loop() {

	delay(10);

	if (!getPirValue) {
		turnLightOn();
	}

}
