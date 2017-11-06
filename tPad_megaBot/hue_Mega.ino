#include <Arduino.h>
//#include <IRLib.h>

#define bedPin 25
#define bathPin 27
unsigned long stars, downBa,down, stars2 = 0;

#define bedDigiPin 28
#define bathDigiPin 29
int bedState = 0;
int bathState = 0;
int bathVal = LOW;
int bedVal = LOW;
int bathLight = 9;
int bedLight = 3;

#include <IRremote.h>

IRsend irsend;
#define L1_ON        0xA010C  // RED
#define L1_OFF       0x6010C  // GREEN
#define L2_ON        0xE010C  // YELLOW
#define L2_OFF       0x2010C  // BLUE

void sendIr(uint64_t cmd) {
  Serial.println("sending Sony");
  for (int i = 0; i < 3; i++) {
    irsend.sendSony(0xa90, 12);
    delay(40);
  }
  delay(100);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("COMMENCING TNODE LIGHTSENSOR");
  delay(10);
  pinMode(bedPin, INPUT);
  pinMode(bathPin, INPUT);
  delay(500);
  //pinMode(SEND_PIN, OUTPUT);
}

void bedCheck() {
  bedVal = digitalRead(bedPin); // read input value
  if (bedVal == HIGH) {
   if (bedState == LOW) {         // we have just turned on
      sendIr(L1_ON); Serial.println("*NEW MOVE"); stars = 0; stars++; down = 0; bedState = HIGH;
    } else if (bedState == HIGH) {
      Serial.print("*");
      }
    stars++;
    } else if (bedVal == LOW) {
    if (bedState == HIGH) {
       if (bathState == LOW){
       delay(2000);
      Serial.println("_END");
      sendIr(L1_OFF);stars = 0; down = 0; down++;
    }
    } else if (bedState == LOW) {
       Serial.print("_");
      }
      Serial.print("_");
      bedState = LOW;  
    }
    delay(10);
}

void bathCheck() {
  bathVal = digitalRead(bathPin); 
  if (bathVal == HIGH) {
    if (bathState == LOW) {         // we have just turned on
      sendIr(L2_ON);
      Serial.println("*NEW MOVE"); stars2 = 0; stars2++; downBa = 0;
      bathState = HIGH;
    } else if (bathState == HIGH) {
      if (stars2 >= 100) {
        Serial.println("*");
      }
      Serial.print("*"); 
      stars2++;
    }
  } else if (bathVal == LOW) {
    if (bathState == HIGH) {
      Serial.println("_END");
      delay(2000);
      stars2 = 0; downBa = 0;
      sendIr(L2_OFF);
    } else if (bathState == LOW) {
      if (downBa >= 100) {
        Serial.println("_");  
      }
    }
      bathState = LOW; 
      downBa++;
      Serial.print("_");
    }
    delay(10);
}

void loop() {
  bathCheck();
  delay(200);
  bedCheck();
  delay(200);
}
