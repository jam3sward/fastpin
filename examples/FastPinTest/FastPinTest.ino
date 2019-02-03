#include <FastPin.h>

// This is a simple test program to compare the
// performance of FastPin against digitalWrite
// with a logic analyser or oscilloscope.
// It outputs a square wave on an Arduino pin.
// James Ward, 3rd February 2019

// We define a pin number here, so we can test the
// same pin with both FastPin and digitalWrite
const int TEST_PIN = 7;

// This initialises a pin as an output
FastPin pin(TEST_PIN, OUTPUT);

void setup() {
  // Nothing to do here: the pin is initialised by
  // the constructor above
}

void loop() {
  int i;

  // This takes about 172us on Arduino Uno,
  // about 6x faster than digitalWrite below
  // Each write takes about 1us ~ 2us
  for (i=0;i<100;i++) {
    pin.set();
    pin.clear();
  }

  // This takes about 1042us on Arduino Uno
  // Each write takes about 5us ~ 6us
  for (i=0;i<100;i++) {
    digitalWrite(TEST_PIN, HIGH);
    digitalWrite(TEST_PIN, LOW);
  } 
}
