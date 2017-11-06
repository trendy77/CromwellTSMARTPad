// LOUNGE TEMP & HUMIDITY SENSOR +
//  UPDATES TO MQTT / THINGSPEAK CHANNEL
// https://thingspeak.com/channels/339378
  // ALSO USES IR RECEIVER TO LOG (AND FORWARD TO SENDER) ANY INFRARED SIGNALES INTERCEPTED...
//TNODE - IR REC
//      - IR WEB SONY SEND
//       TEMPN Humidity 2 cloud (thingspeak channel USING THINGSPEAK LIBRARY)
//      - hue Control (via web - http and/or tcp)

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <ESP8266WiFi.h>
#include <RestClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <dht.h>
#include <PubSubClient.h>
#include <ThingSpeak.h>

// PINS
  // IR
  uint16_t RECV_PIN = 5;   // IR detector @ GPIO pin 5 (d1)
  IRsend irsend(14);  // d6  ......  gpio 5=pin d1
  // temperature
  #define DHT11_PIN 4
  // Motion
  #define pirPin 12    //(D5)

// VARS
        // IR
      IRrecv irrecv(RECV_PIN);
      decode_results results;

        //  comands
      #define TV_OFF        0xa90
      #define HiFi_OFF        0x540a
      #define HiFi_TV        0x540a
      #define HiFi_BT        0x540a
      #define HiFi_Vup        0x240a
      #define HiFi_Vdn        0x640a
	    #define L1_ON        0xA010C	// RED
		#define L1_OFF    	 0x6010C	// GREEN
		#define L2_ON        0xE010C	// YELLOW
		#define L2_OFF       0x2010C 	// BLUE

      // HUE LIGHTING
      const char LIGHTS_ON[] = "{\"on\":true}";
      const char LIGHTS_OFF[] = "{\"on\":false}";
      const char *bridge_ip = "192.168.0.101"; // Hue Bridge IP
      long looped;  unsigned long mLastTime=0;  bool looping = false;
    // Wi-Fi / IoT
    const char ssid[] = "WiFi2";
    const char password[] = "4328646518";
    const int port = 80;
    MDNSResponder mdns;
    ESP8266WebServer server(4998);
    RestClient hue = RestClient(bridge_ip);
    int status = WL_IDLE_STATUS;
    WiFiClient client;

    // Motion PIR sensor
    unsigned long stars,down=0;
    int pirState = 0;
    int pirVal = LOW;

    // TEMP AND HUMIDITY VARS
    int value, temp, humid = 0;
    dht DHT;
    char temperatureString[6];
    char humidString[6];
    unsigned long DHTtimer = 0;

    // THINGSPEAK INFO
    unsigned long myChannelNumber = 339378;
    const char * myWriteAPIKey = "GFL52HKR8RQ44R08";


                  void sendIr(uint64_t cmd){
                    Serial.println("sending Sony");
                    irsend.sendSony(cmd, 12, 2);
                    delay(100);
                   }

            void hue_turnOff(int lightNo){
             String cmd = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
					cmd += lightNo;
					cmd += "/state/";
					hue.put(cmd, LIGHTS_OFF);
            }
            void hue_turnOn(int lightNo){
             String cmd = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
					cmd += lightNo;
					cmd += "/state/";
					hue.put(cmd, LIGHTS_ON);
            }


			void doLoop(int on, int lightNo){
			  if (on == 1){
				const char EFFECT_COLORLOOP[] = "{\"effect\":\"colorloop\"}";
					String cmd = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
					cmd += lightNo;
					cmd += "/state/";
					hue.put(cmd, EFFECT_COLORLOOP);
					looping = true;
				}
				if (on == 0){
				const char NO_COLORLOOP[] = "{\"effect\":\"none\"}";
					String cmd = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
					cmd += lightNo;
					cmd += "/state/";
					hue.put(cmd, NO_COLORLOOP);
					looping = false;
				}
			}


			void handleRoot() {
            doLoop(1,1);
            String message = "<html><head><title>NodeIR TPad</title></head><body><h1>Hello from NodeIR</h1>";
                message += "<h2>Temp =";
                message += temperatureString;
                message += "</h2>";
               String message2 = "";
                message += "<h2>Humid =";
                message += humidString;
                message += "</h2><br><h2>LoungeCommands</h2><p><a href=\"tvoff\">Send TV On/Off</a></p><p><a href=\"volup\">Send VolUP</a></p><p><a href=\"voldn\">Send VolDN</a></p><p><a href=\"tvaux\">HiFi TV AUX</a></p><p><a href=\"btaux\">HiFi BT AUX</a></p></body></html>";
              server.send(200, "text/html", message);
              }

            void volup() {
            sendIr(HiFi_Vup);
            }
            void voldn() {
            sendIr(HiFi_Vdn);
            }
            void tvaux() {
            sendIr(HiFi_TV);
            }
            void tvoff(){
              sendIr(TV_OFF);
            }
          void btaux(){
              sendIr(HiFi_BT);
          }

            void handleNotFound() {
              String message = "File Not Found\n\n";
              message += "URI: ";
              message += server.uri();
              message += "\nMethod: ";
              message += (server.method() == HTTP_GET)?"GET":"POST";
              message += "\nArguments: ";
              message += server.args();
              message += "\n";
              for (uint8_t i = 0; i < server.args(); i++)
                message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
              server.send(404, "text/plain", message);
              Serial.print(message);
            }

            void motionCheck(){
              pirVal = digitalRead(pirPin); // read input value
               if (pirVal == HIGH) {
                 if (pirState == LOW) {         // we have just turned on
                   hue_turnOn();
                   Serial.println("*NEW MOVE"); stars=0; stars++;down=0;
                    pirState = HIGH;
                    } else if (pirState == HIGH){
                   Serial.print("*"); stars++;
                    }
               } else if (pirVal == LOW){
                 if (pirState == HIGH){
                   Serial.println("_END - TURNING OFF LIGHT in 5");
                   pirState = LOW; stars = 0; down =0; down++;
                   hue_turnOff();Serial.println("_");
                 } else if (pirState == LOW){
                 down++;
                 Serial.print("_");
                 }
                 }
               delay(10);
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

            void getTemperature() {
              int chk = DHT.read11(DHT11_PIN);
                temp = DHT.temperature;
                humid = DHT.humidity;
                dtostrf(temp,2,2 , temperatureString);
                dtostrf(humid,2,2 , humidString);
                ThingSpeak.setField(1,temp);
                ThingSpeak.setField(2,humid);
                ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
            }

            void dump(decode_results *results) {
                       // Dumps out the decode_results structure.
                       // Call this after IRrecv::decode()
                       uint16_t count = results->rawlen;
                       if (results->decode_type == UNKNOWN) {
                         Serial.print("Unknown encoding: ");
                       } else if (results->decode_type == NEC) {
                         Serial.print("Decoded NEC: ");
                       } else if (results->decode_type == SONY) {
                         Serial.print("Decoded SONY: ");
                       } else if (results->decode_type == RC5) {
                         Serial.print("Decoded RC5: ");
                       } else if (results->decode_type == RC5X) {
                         Serial.print("Decoded RC5X: ");
                       } else if (results->decode_type == RC6) {
                         Serial.print("Decoded RC6: ");
                       } else if (results->decode_type == PANASONIC) {
                         Serial.print("Decoded PANASONIC - Address: ");
                         Serial.print(results->address, HEX);
                         Serial.print(" Value: ");
                       } else if (results->decode_type == LG) {
                         Serial.print("Decoded LG: ");
                       } else if (results->decode_type == JVC) {
                         Serial.print("Decoded JVC: ");
                       } else if (results->decode_type == AIWA_RC_T501) {
                         Serial.print("Decoded AIWA RC T501: ");
                       } else if (results->decode_type == WHYNTER) {
                         Serial.print("Decoded Whynter: ");
                       }
                   serialPrintUint64(results->value, 16);
                   Serial.print(" (");
                   Serial.print(results->bits, DEC);
                   Serial.println(" bits)");
                   Serial.print("Raw (");
                   Serial.print(count, DEC);
                   Serial.print("): ");
                   for (uint16_t i = 1; i < count; i++) {
                     if (i % 100 == 0)
                       yield();  // Preemptive yield every 100th entry to feed the WDT.
                     if (i & 1) {
              //         Serial.print(results.rawbuf[i] * USECPERTICK, DEC);
                     } else {
                       Serial.write('-');
                //      Serial.print((uint32_t) results.rawbuf[i] * USECPERTICK, DEC);
                     }
                     Serial.print(" ");
                   }
                   Serial.println();
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


           void brightenHue(int lightNo,int briTo, int waiting, boolean loop){
             String com = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
             com += lightNo;
             com += "/state/";
              for (int i = 1; i<briTo;i+5){
                 //const char BRI[] = "{\"bri\":\"i}";
                  // hue.put(com, LIGHTS_OFF);
                delay(waiting);
              }
              if (loop){
              doLoop();
            }
          }

          void darkenHue(int lightNo,int waiting, boolean loop){
            for (int i = 255; i>0;i-5){
               delay(waiting);
            }
            if (loop){
            doLoop();
            }
          }

















void setup() {
     Serial.begin(115200);
     setup_wifi();
       String hostNameWifi = "NODEIR";
       hostNameWifi.concat(".local");
       WiFi.hostname(hostNameWifi);
       if (mdns.begin("NODEIR", WiFi.localIP())) {
       Serial.println("MDNS responder started");
     }
      MDNS.addService("telnet", "tcp", 23);
    //   server_start(0);

       	pinMode(pirPin, INPUT);
       	pirState=LOW;
       	delay(500);
        ThingSpeak.begin(client);
       server.on("/", handleRoot);
       server.on("/volup", volup);
       server.on("/btaux", btaux);
	   server.on("/tvaux", tvaux);
       server.on("/voldn", voldn);
       server.on("/tvoff", [](){
        sendIr(TV_OFF);
        server.send(200, "text/plain", "tv OFF");
       });
       server.onNotFound(handleNotFound);
       server.begin();
         delay(100);
//     hue_turnOn();
  //       delay(500);
  //   hue_turnOff();
    //     delay(500);
  //   hue_turnOn();
    //     delay(500);
       irrecv.enableIRIn();  // Start the receiver
      // irsend.begin();
      }




void loop() {
  server.handleClient();
        if (irrecv.decode(&results)) {
          dump(&results);
		   if (results.value == L1_ON){
			hue_turnOn();
		}
		  if (results.value == L1_OFF){
hue_turnOff();
			}
		//    if (results.value == L2_OFF){

		//}
		//    if (results.value == L2_ON){

		//    }
      irrecv.resume();
       delay(100);
     }

if(looping){
looped++;
  if (looped >=5000) {
  doLoop(0,1);
  }

   if ((millis() - mLastTime) >= 30000) {
     mLastTime = millis();
   	getTemperature();
     	    //FOR DEBUG....
        //serialTemp();
      	}
   	  //motionCheck();
  }
