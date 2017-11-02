#define pir1 14
#define pir2 5
int pir1prev,pir2prev = 0;   

void readPins(){
int pir1pin = digitalRead(pir1);
int pir2pin = digitalRead(pir2);
 Serial.print("1 is ");Serial.println(pir1Pin);
 Serial.print("2 is "); Serial.println(pir2Pin);
    delay(1000);
}

void setup() {
Serial.begin(115200);
  Serial.println("COMMENCING TNODE LIGHTSENSOR");
  delay(10);
pinMode(pir1, INPUT_PULLUP);
pinMode(pir2, INPUT_PULLUP);
}

void loop(){
 readPins();
 }
