
#include <IRLib.h>


#define MY_PROTOCOL 	SONY
#define UP_ARROW      	0x1E108 //	INCREASE TILT TIME +1SEC
#define SUBTITLE		0x88108
#define DOWN_ARROW    	0x9E108 //DECREASE TILT TIME -1SEC
#define RIGHT_ARROW   	0xDE108 //Move All tilt FWD
#define LEFT_ARROW    	0x5E108 //Move All tilt BKD
#define SELECT_BUTTON 	0x3E108 //	BUZZER!!
#define BUTTON_0 		0x90108  			//  ACTIVATE SOLENOID @ DOOR		0xA010C?
#define BUTTON_1		0x108 			 // KITCHENT F
#define BUTTON_2 		0x80108		 // KITCHENT B
#define BUTTON_3 		0x40108		 // WINT F
#define BUTTON_4 		0xC0108		// WINT B
#define BUTTON_5 		0x20108		// DWT F
#define BUTTON_6 		0xa0108 		// DWT B
#define BUTTON_7		0x60108		 // LT F 
#define BUTTON_8 		0xe0108 		// LT B
#define BUTTON_9 		0x10108		 // 
#define PRESET_PREV 	0xC108 		//  LOWER LOUNGE BLIND
#define PRESET_NEXT 	0x8C108		//  RAISE LOUNGE BLIND  
#define RED				0xA010C	// close kitchen window
#define GREEN 			0x6010C	// open kitchen windwo
#define YELLOW 			0xE010C	// Close loungewin
#define BLUE 			0x2010c		// // Open loungewindow
#define SOUND_PREV 		0x7B0B			// LOWER BOB
#define SOUND_NEXT 		0x3B0B 	 // RAISE BOB
#define TOP_MENU 		0x98108
#define POPUP_MENU 		0x58108	
#define PAUSE 			0x400B	
#define STOP 			0xB
#define PLAY 			0x200B		
#define FAST_FWD		0x20C108
#define FAST_RWD		0xCC108


IRTYPES codeType;      unsigned long codeValue; int codeBits; unsigned int rawCodes[RAWBUF]; int rawCount; bool GotOne, GotNew;
IRdecode My_Decoder;

IRrecv My_Receiver(23);


void setup()
{
	Serial.begin(115200);
	Serial.println("IR INITIALISING...");
	My_Receiver.enableIRIn();
}

void loop()
{

	if (My_Receiver.GetResults(&My_Decoder)) {
	IRDetected();
		delay(100);
		My_Receiver.resume();
	}
	serialcomms();
}





void IRDetected() {
	My_Decoder.decode(); GotOne = true;
	GotNew = true;  codeType = My_Decoder.decode_type;  if (codeType == UNKNOWN) {
		Serial.println("RUC @ IR");
		rawCount = My_Decoder.rawlen - 1;
	}
	else {
		Serial.print(F("Received "));    Serial.print(Pnames(codeType));
		if (My_Decoder.value == REPEAT) { Serial.println(F("repeat; ignoring.")); }
		else {
			codeValue = My_Decoder.value; codeBits = My_Decoder.bits;
		} 	Serial.print(F(" Value:0x")); Serial.println(My_Decoder.value, HEX);
		if (My_Decoder.decode_type == MY_PROTOCOL) {
Serial.println("IR COMMAND DETECTED - DECODING...");
		switch (My_Decoder.value) {
			case SELECT_BUTTON: 		break;
			case SOUND_PREV:	 break;	// raise room bob
			case SOUND_NEXT: break; 	 // lower room bob
			case SUBTITLE: break;
			case BUTTON_0:  break;
			case BUTTON_9:  break;
		case BUTTON_0: break; case BUTTON_9: break;
		}
	}
}

void serialcomms() {
	if (Serial.available()) {
		int command = Serial.read();
		switch (command) {
		case '0':
			Serial.println(" 1/q-  KitchenTILT 5 SEC ");
			Serial.println(" 2/w-  WindowTILT 5 SEC ");
			Serial.println(" 3/e- DoorWindowTILT 5 SEC ");
			Serial.println(" 4/r- LoungeTilt 5 SEC ");
			Serial.println(" **5/t- ALL TILT 6 SEC");
			Serial.println(" 6/y- KITCHEN WINDOW OPEN/CLOSE");
			Serial.println("   P = POTMODE ALL TILTSPEED ** ");
			Serial.println(" z/x/c/v- relays 1-4; m to reset ");
			Serial.println(" 8- Lux Lumen Info");
			Serial.println(" 9-  SEND2SERVER ");
			delay(100);
			break;

		case '1':
			break;
		case 'z':
			break;
			//case 'x':      sendValueToLatch(2);      break;
			//case 'c':      sendValueToLatch(4); break;
			//case 'v':     sendValueToLatch(8);      break;
			//case 'm':      sendValueToLatch(0);      break;
			//case 'q':
				//Serial.println(" Direction: BACKWARD - KitchenTilt ... ");
		}
	}