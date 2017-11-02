#include <Arduino.h>
#include <restclient.h>
#include <ESP8266WiFi.h>

	// GENERAL SETUP STUFF
		long timeq=0;

	// INTERNET CONNECTIVITY
		const char ssid[] = "WiFi2";
		const char password[] = "4328646518";
		RestClient hue = RestClient(bridge_ip, port);
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

	// PHILIPS hue
		String user1 = "fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG";
		//String user2 = "gYEEPLSaIRrhV8QWrM03pzuWND-OsVYJnmPGGvMZ";
		String bathlight = "9";		// light number for BATHROOM
		String kitchenlight = "1";	// light number for KITCHEN
		const char *bridge_ip = "192.168.0.101"; // Hue Bridge IP
		const int port = 80;
		// Current LIGHT states
		bool bathOn = false;
		bool kitchOn = false;

		String hue_checkSt = "\";
		String hue_on = "{\"on\":true}";
		String hue_off = "{\"on\":false}";
				const char LIGHTS_ON[] = "{\"on\":true}";
				const char LIGHTS_OFF[] = "{\"on\":false}";

		// MotionSensor(s)
		#define pirPin3 5   // board labelled 'd5', GPIO pin #14
		#define pirPin4 6   // board labelled 'd6', GPIO pin #12
		#define pirPin1 2   // board labelled 'd2', GPIO pin #4
		//#define pirPin4    // board labelled 'd1', GPIO pin #5
		//#define pirPin5    // d7 is gpio13
		//#define pirPin6    // d8 is gpio15
		unsigned long p1on,p2on,p3on,p4on,p5on=0;

		void checkall3(){
			int pir1val = digitalRead(pirPin1);
			int pir2val = digitalRead(pirPin4);
			int pir3val = digitalRead(pirPin3); // read input value
			//	int pir3val = digitalRead(pirPin4); // read input value
				if (pir1val = 1){
				p1on++;
				}
				if (pir2val = 1){
				p2on++;
				}
				if (pir3val = 1){
				p3on++;
				}
			//	if (pir4val = 1){
			//	p4on++;
			//	}
		}

		 void tellit(){
				 Serial.print("1 is ");Serial.println(p1on);
				 Serial.print("2 is "); Serial.println(p2on);
				 Serial.print("3 is "); Serial.println(p3on);
			//	 Serial.print("4 is "); Serial.println(p4on);
		}

		void doubleCheck(){
				  if (pirVal == LOW) {
				  delay(500);
				  Serial.println("strike1");
					  pirVal = digitalRead(pirPin); // read input value
					   if (pirVal == LOW) {
						 Serial.println("strike2");
						 delay(500);
						 pirVal = digitalRead(pirPin); // read input value
						  if (pirVal == LOW) {
							Serial.println("strike3");
							delay(500);
							pirVal = digitalRead(pirPin); // read input value
							 if (pirVal == LOW) {
							   hue_turnOff();
							   pirState=LOW;
							   Serial.println("off");
							 }else{
							   return;
							 }
						   }
						  }
					   }
			}

		// philips hue FUNCTIONS
//	USING RESTCLIENT.H METHOD - HUE client
		void hue_turnOff(){
		  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state/", LIGHTS_OFF);
		}
		void hue_turnOn(){
		  hue.put("/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state/", LIGHTS_ON);
		}
		void hue_check(int light){
String c

	hue.get(, hue_checkSt);
	}

		void sendHueCheck(int light) {
		Serial.print("GET light ");Serial.println(light);
		  // Serial.print(pathGet);
		   String response = "";
		  // make the GET request to the hub:
		  String request = "";
		  request += "/lights/";
		  request += light;
		  request += "/state/";
		  String contentType = "application/text";

			String hueCmd = "/";
		    int length = hueCmd.length();
			char msgBuffer[length];
		 	 // Convert data string to character buffer
		  	hueCmd.toCharArray(msgBuffer,length+1);
		    Serial.println(request);
				hue.get(pathGet, &response);
		    	//hue.put(pathPut,msgBuffer,&response);
		    int statusCode = httpClient.responseStatusCode();
		    Serial.print('Status CODE - ');
		    Serial.println(statusCode);
		    Serial.print(response);
		}



//	AND... go.

	void setup()
	{
		  Serial.begin(115200);
		  Serial.println("COMMENCING TNODE LIGHTSENSOR");
		  delay(10);
		  setup_wifi();
		//pinMode(pirPin, INPUT_PULLUP);
		pinMode(pirPin1, INPUT);
		pinMode(pirPin2, INPUT);
		pinMode(pirPin3, INPUT);
		//pinMode(pirPin4, INPUT);
		delay(500);
	}


	void loop() {
		timeq = millis();

		// motion check
		checkall3();

		if ((timeq - lastReport)>=10000){

			tellit();
			lastReport=millis();
		}

		delay(250);

	}
