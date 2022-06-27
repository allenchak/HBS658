/*
HBS658.h - Library for HBS658.
*/

#ifndef HBS658_h
#define HBS658_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#if !defined(max)
// min, max are no macro in ESP core 2.3.9 libraries, see https://github.com/esp8266/Arduino/issues/398
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#define HBS658_CMD_DATA_AUTO 0x40
#define HBS658_CMD_DATA_READ 0x42 // command to read data used on two wire interfaces
#define HBS658_CMD_DATA_FIXED 0x44
#define HBS658_CMD_DISPLAY 0x80
#define HBS658_CMD_ADDRESS 0xC0

#define HBS658_MAX_POS 5

#include "HBS658Fonts.h"

class HBS658
{
public:
  HBS658(byte dataPin, byte clockPin, byte numDigits = 5, boolean activateDisplay = true, byte intensity = 7);
  void setupDisplay(boolean active, byte intensity);
  void clearDisplay();
  void setSegments(byte segments, byte position);
  void sendChar(byte pos, byte data, boolean dot);
  void setDisplayDigit(byte digit, byte pos, boolean dot);
  void setDisplayToDecNumber(long nNumber, byte bDots);
  void clearDisplayDigit(byte pos, boolean dot);
  void setDisplay(const byte values[], byte size);

protected:
  void bitDelay();
  void start();
  void stop();
  void send(byte data);
  void sendCommand(byte cmd);
  void sendData(byte address, byte data);

  byte _maxDisplays = 5; // maximum number of digits
  byte _maxSegments = 7; // maximum number of segments

  byte digits; // number of digits in the display
  byte dataPin;
  byte clockPin;
};

#endif
