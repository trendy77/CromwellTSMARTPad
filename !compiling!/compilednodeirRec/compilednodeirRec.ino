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

/*
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Example circuit diagram:
 *  https://github.com/markszabo/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.2 June, 2017
 *     Changed GPIO pin to the same as other examples.
 *     Used our own method for printing a uint64_t.
 *     Changed the baud rate to 115200.
 *   Version 0.1 Sept, 2015
 *     Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
 */
/*
 * IRremoteESP8266: IRGCTCPServer - send Global Cache-formatted codes via TCP.
 * An IR emitter must be connected to GPIO pin 4.
 * Version 0.2  May, 2017
 * Copyright 2016 Hisham Khalifa, http://www.hishamkhalifa.com
 * Copyright 2017 David Conran
 *
 * Example command - Samsung TV power toggle: 38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,63,20,63,20,63,20,63,20,1798\r\n
 * For more codes, visit: https://irdb.globalcache.com/
 *
 * How to use this program:
 *   1) Update "ssid" and "password" below for your WIFI network.
 *   2) Compile and upload the sketch to your ESP8266 module.
 *   3) (Optional) Use the serial connection to confirm it started and get the
 *      IP address.
 *   4) From a client machine, connect to port 4998 on the ESP8266, using
 *      'telnet', 'nc' (netcat), 'putty' or similar command, etc.
 *      You may need to install one.
 *   Unix/OSX:
 *     Start a shell. Then type:
 *       telnet <esp8266deviceIPaddress> 4998
 *   Windows:
 *     Start a new CMD window, then type:
 *       telnet <esp8266deviceIPaddress> 4998
 *
 *   5) Enter a Global Cache-formatted code, starting at the frequency,
 *      and then a return/enter at the end. No spaces. e.g.:
 *
 *   38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,63,20,63,20,63,20,63,20,1798
 *
 *   To exit the 'telnet' command:
 *     press <control> + <]> at the same time, then press 'q', and then <return>.
 *   or:
 *     <control> + <d> might work.
 *
 * This program will display the ESP's IP address on the serial console, or you
 * can check your wifi router for it's address.
 */

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char* ssid = "wifi2";  // Put your WIFI SSID here.
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
  // Now we know how many there are, allocate the memory to store them all.
  code_array = reinterpret_cast<uint16_t*>(malloc(count * sizeof(uint16_t)));
  // Check we malloc'ed successfully.
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

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU)
uint16_t RECV_PIN = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(115200); Serial.println("115200");
  irrecv.enableIRIn();  // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, HEX);
    Serial.println(".");
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}

