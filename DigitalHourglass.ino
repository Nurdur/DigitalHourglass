/*
  Debounced Digital LED Hourglass 

  Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
  press), the next LED is lit. There's a minimum delay between toggles to 
  debounce the circuit (i.e. to ignore noise). As time elapses (ledDelay) the 
  LEDs are unlit in reverse. When LED 0 is unlit, all 5 LED's flash on and off 
  continuously. More time can be added at any time by pressing the button.

  The circuit:
  - LEDs attached from pins 7,6,5,4,3 to ground through 220 ohm resistors
  - pushbutton attached from pin 2 to +5V
  - 10 kilohm pulldown resistor attached from pin 2 to ground

  created  21 Nov 2006 by David A. Mellis
  modified 30 Aug 2011 by Limor Fried
  modified 28 Dec 2012 by Mike Walters
  modified 30 Aug 2016 by Arturo Guadalupi
  modified 22 Sep 2017 by Milan Keser

  This program was based on example code in the public domain.
  http://www.arduino.cc/en/Tutorial/Debounce

*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;                 // the number of the pushbutton pin
const int leds[] = {3, 4, 5, 6, 7};      // the number of the LED pin

// Variables will change:
int buttonState;                         // the current reading from the input pin
int lastButtonState = LOW;               // the previous reading from the input pin
int lastLED = 0;                         // Points at which LED to light next
boolean flashing = false;                // Once time runs out, flash all 5 led's

// Time, measured in milliseconds, will quickly become too big to be stored in an int.
unsigned long lastDebounceTime = 0;      // the last time the output pin was toggled
unsigned long debounceDelay = 50;        // the debounce delay; increase if the output flickers
unsigned long ledDelay = 300000;         // 300k ms = 5 minutes per LED / button press
unsigned long flasher = 0;               // timestamp for LED flash on/off
unsigned long flash_time = 800;          // flash speed (on/off time)

void setup() {
  pinMode(buttonPin, INPUT);             // Set pin I/O modes
  for(int i=0;i<5;i++) {
    pinMode(leds[i], OUTPUT);
  }
  for(int i=0;i<5;i++) {                 // Turn off all LEDs
    digitalWrite(leds[i], LOW);          // (should already be off, but just in case)
  }
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // If the switch has changed state, due to noise or user input:
  if (reading != lastButtonState) {
    lastDebounceTime = millis();    // reset the debouncing timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the reading has been there for longer than the debounce delay, take it as the current state
    if (reading != buttonState) {           // If the button state has changed
      buttonState = reading;                // Remember the new state
      if (buttonState == HIGH) {            // If the new button state is HIGH (pressed)  
        digitalWrite(leds[lastLED], HIGH);  // Light lastLED. Lower LEDs should already be lit
        flashing = false;                   // Make sure the LEDs are not flashing
        if (lastLED < 5) {                            
          lastLED++;                        // Increment lastLED unless at 5 already
          for(int i=lastLED;i<5;i++) {      // Unlight any higher LEDs (in case they were flashed on)
            digitalWrite(leds[i], LOW);
          }
        }
      }
    }
    else {                                                // if the button state has not changed
      if (((millis() - lastDebounceTime) > ledDelay) && !flashing) {  // And we're not currently flashing   
        if (lastLED > 0) {                                // If something is lit
          lastLED--;                                      //  Decrement the 'pointer'
          digitalWrite(leds[lastLED], LOW);               //  And unlight that highest lit LED
        }
        if (lastLED == 0) {                               // if no LEDs are lit
          flashing = true;                                // flash the LEDs
          flasher = millis();                             // And remember when the current flash began
        }
        lastDebounceTime = millis();                      // reset the debounce timer  
      }
    }
  }

  // Light / Unlight the LEDs by setting the pin states
  if((flashing) && (millis() - flasher) > flash_time) { // When the current on or off flash time is up
    for(int i=0;i<5;i++) {                              // Toggle the LED states on->off or off->on
      digitalWrite(leds[i],digitalRead(leds[i]) ? LOW : HIGH);
    }
    flasher = millis();                                 // Reset the current on/off timer
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
