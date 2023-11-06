#include <stdbool.h>
#include <Arduino.h>

#define BUZZER 2
#define BTN1 3
#define BTN2 4
#define BTN3 5





// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int rs = 46, en = 42, d4 = 32, d5 = 30, d6 = 28, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);





// Rotary Encoder Inputs
#define CLK 6
#define DT 7
#define SW 8

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir="";
int lastButtonPress=0;

bool moving = true;

void poll() {
	// Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
    moving = true;
		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK) {
			counter --;
			currentDir ="CCW";
		} else {
			// Encoder is rotating CW so increment
			counter ++;
			currentDir ="CW";
		}

		//Serial.print("Direction: ");
		//Serial.print(currentDir);
		//Serial.print(" | Counter: ");
		//Serial.println(counter);
	} else {
    moving = true;
  }

	// Remember last CLK state
	lastStateCLK = currentStateCLK;

	// Read the button state
	int btnState = digitalRead(SW);

	//If we detect LOW signal, button is pressed
	if (btnState == LOW) {
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (millis() - lastButtonPress > 50) {
			Serial.println("Button pressed!");
		}

		// Remember last button press event
		lastButtonPress = millis();
	}

	// Put in a slight delay to help debounce the reading
	delay(1);
}


void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

	// Set encoder pins as inputs
	pinMode(CLK, INPUT);
	pinMode(DT, INPUT);
	pinMode(SW, INPUT_PULLUP);
	// Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  Serial.begin(9600);
}


/*
float getNoteFreq(int n) {
  return 440 * pow(2, (n-48)/12)
}*/


int getNumFromButtons(int foursPlace, int twosPlace, int onesPlace) {
  int result = 0;
  if (foursPlace == LOW) {
    result += 4;
  }
  if (twosPlace == LOW) {
    result += 2;
  }
  if (onesPlace == LOW) {
    result += 1;
  }
  return result;
}


void loop() {
  //delay(25);
  int decimalNum = getNumFromButtons(digitalRead(BTN1), digitalRead(BTN2), digitalRead(BTN3));
  //Serial.println(decimalNum);
  
  poll();
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(counter % 5);

}
