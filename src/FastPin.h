/*
  FastPin.h - Library for faster reading/writing of Arduino pins.
  Created by James Ward, 2nd February 2019.
  Released into the public domain.
 */

#ifndef __FastPin_h
#define __FastPin_h

#include "Arduino.h"
#include <assert.h>

/// FastPin allows faster reading/writing of Arduino pins, compared
/// to the existing digitalRead/digitalWrite functions. It achieves
/// this by doing some of the more time consuming preparation work
/// at construction time, so that the read/write functions can be
/// much simpler. It also provides convenient functions to access
/// the pins (e.g. set high, set low, pulse high, pulse low, wait
/// for the pin to transition to high or low)
///
/// James Ward 02/02/19

///----------------------------------------------------------------

/// The FastPin class allows faster reading/writing of Arduino pins
/// compared to the existing digitalRead/digitalWrite functions
///
class FastPin {
public:
  /// The constructor requires the Arduino pin number
  FastPin(byte pin) {
    byte port = digitalPinToPort(pin);
    assert(port != NOT_A_PIN);
    m_pin = pin;
    m_bit = digitalPinToBitMask(pin);
    m_in  = portInputRegister(port);
    m_out = portOutputRegister(port);
  }

  /// This constructor allows the pin and mode to be set
  FastPin(byte pin, byte mode)
    :FastPin(pin)
  {
    ::pinMode(pin, mode);
  }

  /// This constructor allows the pin, mode and initial
  /// output state to be set (for outputs). This is only
  /// useful when (mode==OUTPUT). The advantage of this
  /// is that it will execute a digitalWrite which does
  /// the normal Arduino initialisation, for example by
  /// ensuring PWM is disabled for the pin
  FastPin(byte pin, byte mode, byte value)
    :FastPin(pin, mode)
  {
    if (mode == OUTPUT) ::digitalWrite(pin, value);
  }

  /// Allows the pin mode to be changed at any time
  void pinMode(byte mode) {
    ::pinMode(m_pin, mode);
  }

  /// Read the state of the pin (returns HIGH or LOW)
  byte read() const {
    if (*m_in & m_bit)
      return HIGH;
    else
      return LOW;
  }

  /// Returns true if the pin is low
  bool isLow() const {
    return (*m_in & m_bit) == 0;
  }

  /// Returns true if the pin is high
  bool isHigh() const {
    return (*m_in & m_bit) != 0;
  }

  /// Wait until the pin goes low
  void waitLow() {
    while (isHigh());
  }

  /// Wait until the pin goes high
  void waitHigh() {
    while (isLow());
  }

  /// Wait for rising edge
  void waitRisingEdge() {
    waitLow();
    waitHigh();
  }

  /// Wait for falling edge
  void waitFallingEdge() {
    waitHigh();
    waitLow();
  }

  /// Write to the pin, setting it HIGH or LOW
  void write(byte value) {
    if (value == LOW)
      clear();
    else
      set();
  }

  /// Set the pin to high
  void set() {
    *m_out |= m_bit;
  }

  /// Clear the pin to low
  void clear() {
    *m_out &= ~m_bit;
  }

  /// Pulse the pin high (assuming it's already low)
  void pulseHigh() {
    set();
    clear();
  }

  /// Pulse the pin low (assuming it's already high)
  void pulseLow() {
    clear();
    set();
  }

private:
  byte m_pin;           ///< the Arduino pin number
  byte m_bit;           ///< the bit mask
  volatile byte *m_in;  ///< the input port
  volatile byte *m_out; ///< the output port
};

///----------------------------------------------------------------

#endif//__FastPin_h

