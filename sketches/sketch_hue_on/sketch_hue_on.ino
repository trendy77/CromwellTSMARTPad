
// Libraries
#include <restclient.h>
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#include <ESP8266mDNS.h>

// Remote debug over telnet - not recommended for production, only for development

#include "RemoteDebug.h" //https://github.com/JoaoLopesF/RemoteDebug

RemoteDebug Debug;

int dots = 0;
bool hueOn;    // on/off
int hueBri;    // brightness value
long hueHue;   // hue value
String hueCmd; // Hue command
const char statePoint[] = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/1/state";
int pirState = LOW;
unsigned long timer = 0;
int val = LOW;
// Wifi Settings
//const char* ssid = "LAN_Time_Forgot";
//const char* password = "tasmania";
const char ssid[] = "LordProtectorOfTheWiFi";
const char password[] = "4328646518";

WiFiServer telnetServer(23);
WiFiClient serverClient;
unsigned long startTime = millis();

// Hue Settings
const char *bridge_ip = "192.168.0.101"; // Hue Bridge IP
const int port = 80;
String user1 = "fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG";
restclient hue(bridge_ip, port);
unsigned long lastMove = 0;

// MotionSensor(s)
#define pirPin D1 // Input for PIR AS312
int pirValue;     // Place to store read PIR Value

// Host mDNS
#define HOST_NAME "tnode188"
// Hue Settings


// Time
uint32_t mLastTime = 0;
uint32_t mTimeSeconds = 0;

// Buildin Led ON ?
boolean mLedON = false;
boolean motion = false;
////// Setup

void setup()
{

    // Initialize the Serial (educattional use only, not need in production)

    Serial.begin(115200);

    // Buildin led off ESP8266

    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, LOW);
    pinMode(pirPin, INPUT);
    val = LOW;pirValue = LOW;
    // WiFi connection
  WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Register host name in WiFi and mDNS

    String hostNameWifi = HOST_NAME;
    hostNameWifi.concat(".local");

    WiFi.hostname(hostNameWifi);

    if (MDNS.begin(HOST_NAME))
    {
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println(HOST_NAME);
    }

    MDNS.addService("telnet", "tcp", 23);

    // Initialize the telnet server of RemoteDebug

    Debug.begin(HOST_NAME); // Initiaze the telnet server

    Debug.setResetCmdEnabled(true); // Enable the reset command

    //Debug.showTime(true); // To show time

    // Debug.showProfiler(true); // To show profiler - time between messages of Debug
    // Good to "begin ...." and "end ...." messages

    // This sample (serial -> educattional use only, not need in production)

    Serial.println("* Arduino RemoteDebug Library");
    Serial.println("*");
    Serial.print("* WiFI connected. IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("*");
    Serial.println("* Please use the telnet client (telnet for Mac/Unix or putty and others for Windows)");
    Serial.println("*");
    Serial.println("* This sample will send messages of debug in all levels.");
    Serial.println("*");
    Serial.println("* Please try change debug level in telnet, to see how it works");
    Serial.println("*");
}

void loop()
{


  doDebug();
}

void doDebug(){
if ((millis() - mLastTime) >= 1000)
{

    // Time

    mLastTime = millis();

    mTimeSeconds++;

    // Blink the led

    mLedON = !mLedON;
    digitalWrite(BUILTIN_LED, (mLedON) ? LOW : HIGH);

    // Debug the time (verbose level)

    if (Debug.isActive(Debug.VERBOSE))
    {
        Debug.printf("* Time: %u seconds (VERBOSE)\n", mTimeSeconds);
    }

    if (mTimeSeconds % 5 == 0)
    { // Each 5 seconds

        // Debug levels

        if (Debug.isActive(Debug.VERBOSE))
        {
            Debug.println("* This is a message of debug level VERBOSE");
        }
        if (Debug.isActive(Debug.DEBUG))
        {
            Debug.println("* This is a message of debug level DEBUG");
        }
        if (Debug.isActive(Debug.INFO))
        {
            Debug.println("* This is a message of debug level INFO");
        }
        if (Debug.isActive(Debug.WARNING))
        {
            Debug.println("* This is a message of debug level WARNING");
        }
        if (Debug.isActive(Debug.ERROR))
        {
            Debug.println("* This is a message of debug level ERROR");
        }
    }
}

// Remote debug over telnet

Debug.handle();

// Give a time for ESP8266

yield();
}
