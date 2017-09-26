/*
  Debounced Digital LED Hourglass -- Now in GIT for practice

  Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
  press), the next LED is lit. There's a minimum delay between toggles to 
  debounce the circuit (i.e. to ignore noise). As time elapses (ledDelay) the 
  LEDs are unlit in reverse. When LED 0 is unlit, all 5 LED's flash on and off 
  5 times. More time can be added at any time by pressing the button, lighting
  the next higher LED.

  The circuit:
  - LEDs attached from pins 7,6,5,4,3 to ground through 220 ohm resistors
  - pushbutton attached from pin 2 to +5V
  - 10 kilohm resistor attached from pin 2 to ground

  created 21 Nov 2006
  by David A. Mellis
  modified 30 Aug 2011
  by Limor Fried
  modified 28 Dec 2012
  by Mike Walters
  modified 30 Aug 2016
  by Arturo Guadalupi
  modified 22 Sep 2017
  by Milan Keser

  This program was based on example code in the public domain.

  http://www.arduino.cc/en/Tutorial/Debounce
*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;           // the number of the pushbutton pin
const int leds[] = {3, 4, 5, 6, 7};  // the number of the LED pin

// Variables will change:
int ledState[] = {LOW,LOW,LOW,LOW,LOW};  // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int lastLED = 0;
boolean flashed = false;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long ledDelay = 300000;     // 5 minutes per LED / button press

void setup() {
  pinMode(buttonPin, INPUT);
  for(int i=0;i<5;i++) {
    pinMode(leds[i], OUTPUT);
  }
  // set initial LED state
  for(int i=0;i<5;i++) {
    digitalWrite(leds[i], ledState[i]);
  }
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        if (ledState[lastLED] == LOW) {       // If the "current" LED is off:
          ledState[lastLED] = HIGH;           //  Light it
          flashed = false;                    //  and make sure they'll all flash when time runs out again
          if (lastLED < 5) {                  // If we just lit LED 5, there is no 6th, so don't increment
            lastLED++;       
          }
        }
      }
    }
    else {                                                // if the button state has not changed
      if ((millis() - lastDebounceTime) > ledDelay) {     // for longer than the preset LED delay time
        if (lastLED > 0) {                                // And if something is lit
          lastLED--;                                      //  Fix the 'pointer'
          ledState[lastLED] = LOW;                        //  And unlight that highest lit LED
        }
        if ((lastLED == 0) && (!flashed)) {               // if we just unlit the final LED
          for(int j=0;j<5;j++) {                          // flash all 5 LEDs 5 times
            for (int i=0;i<5;i++) {
              digitalWrite(leds[i], HIGH);
            }
            delay(300);
            for (int i=0;i<5;i++) {
              digitalWrite(leds[i], LOW);
            }
            delay(300);
          }
          flashed = true;                                 // And only do that once, unless we add more time again
        }
      lastDebounceTime = millis();                        // reset the timer  
      }
    }
  }

  // light/unlight the LEDs: (MK: I could just move the writes up into the loop. This refreshes them every pass,
  //   which seems like a lot of extra work for nothing. Inherited from example. Might fix sometime, however-
  //   keeping track of states using ledState[] lets us do logic/tests of the state. Otherwise, we'd have to\
  //   query the output pin itself. Might be possible, don't know.
    for(int i=0;i<5;i++) {
      digitalWrite(leds[i], ledState[i]);
    }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
