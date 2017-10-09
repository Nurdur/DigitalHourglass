This is the code for an Arduino-based timer. I use it to time my sauna sessions,
but it also makes a nice Pomodoro timer. Every time I press the button on the
device, the next led in a series of 5 is lit, and a new countdown begins. I
like 5 minutes per LED, making this a 25-minute timer. The device can be viewed
from a distance (as in through my glass sauna door) and the number of remaining
LEDs give a decent indication of the time remaining. When the time runs out,
the LEDs flash on and off repeatedly. (Easy to change this behavior, of course.)

The circuit is pretty simple: 
  Arduino Uno
  Pins 3-7 each go through an LED, then a resistor, to ground. Set to OUTPUT.
    (I used red LEDs and 220 ohm resisstors, ymmv)
  Pin 2 goes to a pushbutton switch, grounded with a 10k pulldown. Set to INPUT.
