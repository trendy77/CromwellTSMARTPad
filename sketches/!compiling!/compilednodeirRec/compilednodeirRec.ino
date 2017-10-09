#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <ir_Argo.h>
#include <ir_Daikin.h>
#include <ir_Fujitsu.h>
#include <ir_Kelvinator.h>
#include <ir_LG.h>
#include <ir_Mitsubishi.h>
#include <ir_Trotec.h>


#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char* ssid = "WiFi2";  // Put your WIFI SSID here.
const char* password = "4328646518";  // Put your WIFI password here.

WiFiServer server(4998);  // Uses port 4998.
WiFiClient client;
uint16_t *code_array;

//IRsend irsend(4);  // An IR LED is controlled by GPIO pin 4 (D2)
void sendGCString(String str) {
  int16_t index;
  uint16_t count;
  // Find out how many items there are in the string.
  index = -1;
  count = 1;
  do {
    index = str.indexOf(',', index + 1);
    count++;
  } while (index != -1);
  code_array = reinterpret_cast<uint16_t*>(malloc(count * sizeof(uint16_t)));
  if (code_array == NULL) {  // malloc failed, so give up.
    Serial.printf("\nCan't allocate %d bytes. (%d bytes free)\n",
                  count * sizeof(uint16_t), ESP.getFreeHeap());
    Serial.println("Giving up & forcing a reboot.");
    ESP.restart();  // Reboot.
    delay(500);  // Wait for the restart to happen.
    return;  // Should never get here, but just in case.
  }
  // Now convert the strings to integers and place them in code_array.
  count = 0;
  uint16_t start_from = 0;
  do {
    index = str.indexOf(',', start_from);
    code_array[count] = str.substring(start_from, index).toInt();
    start_from = index + 1;
    count++;
  } while (index != -1);
//  irsend.sendGC(code_array, count);  // All done. Send it.
  free(code_array);  // Free up the memory allocated.
}


uint16_t RECV_PIN = 5;      // An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU)
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(115200); Serial.println("IR relay-via-IP...FWDER");
  irrecv.enableIRIn();  // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    
    serialPrintUint64(results.value, HEX);
    Serial.println(".");
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}

