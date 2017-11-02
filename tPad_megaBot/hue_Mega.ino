#include "Arduino.h"
#include <SoftwareSerial.h>

#define bedPin 23
#define bathPin 27  
unsigned long stars,stars2=0;

#define bedDigiPin 25
#define bathDigiPin 29   
int bedState = 0;
int bathState = 0;
int bathVal = LOW;
int bedVal = LOW;
int bathLight = 9;
int bedLight = 3;


void setup()
{
  Serial.begin(115200);
Serial3.begin(9600);
	
  Serial.println("COMMENCING TNODE LIGHTSENSOR");
  delay(10);
  // We connect to a WiFi network
  Serial.print("Connecting to ");
 // Serial.println(ssid);
  //WiFi.begin(ssid, password);
  //while (WiFi.status() != WL_CONNECTED)
  
    delay(500);
    Serial.print(".");
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
pinMode(pirPin, INPUT);

delay(500);
//hue_turnOff();
}

void bedCheck(){
	bedVal = digitalRead(pirPin); // read input value
  if (bedVal == HIGH) {
    if (bedState == LOW) {         // we have just turned on
 
	digitalWrite(25, LOW);
	
	Serial3.println('bed=');
	Serial3.println('1');
   
  //    hue_turnOn();
      Serial.println("*NEW MOVE"); stars=0; stars++;down=0;
       bedState = HIGH;
       } else if (bedState == HIGH){
        if (stars>=100){
          Serial.println("*"); stars=0;
        }
      Serial.print("*"); stars++;
       }
  } else if (bedVal == LOW){
    if (bedState == HIGH)
          {
      Serial.println("_END - TURNING OFF LIGHT in 5");
      delay(5000);
  
 // tell node ...
  Serial3.println('bed=');
  Serial3.println('0');
   digitalWrite(25, HIGH);
	
  
      bedState = LOW; stars = 0; down =0; down++;
    //  hue_turnOff();
    //Serial3.println(2);

    } else if (bedState == LOW){
    if (down>=100){
          Serial.println("*"); down=0;
        }
        down++;
    Serial.print("_");
    }
    }
  delay(10);
  }

  
void bathCheck(){
	bathVal = digitalRead(pirPin); // read input value
  if (bathVal == HIGH) {
    if (bathState == LOW) {         // we have just turned on
 
 // tell node ...
  Serial3.println('bath=');
  Serial3.println('1');
   //    hue_turnOn();
  
  Serial.println("*NEW MOVE"); stars=0; stars++;down=0;
       bathState = HIGH;
       } else if (bathState == HIGH){
        if (stars>=100){
          Serial.println("*"); stars=0;
        }
      Serial.print("*"); stars++;
       }
  } else if (bathVal == LOW){
    if (bathState == HIGH)
          {
      Serial.println("_END - TURNING OFF LIGHT in 5");
      delay(5000);
  
      bathState = LOW; stars = 0; down =0; down++;
  Serial3.println('bath=');
  Serial3.println('0');
   
    //  hue_turnOff();
    //Serial3.println(2);

    } else if (bathState == LOW){
    if (down>=100){
          Serial.println("*"); down=0;
        }
        down++;
    Serial.print("_");
    }
    }
  delay(10);
  }

void loop() {
 if(Serial3.available()){
	 nodeReport();
	}
	
	bathCheck();
	
	bedCheck();
 }
 
 
 