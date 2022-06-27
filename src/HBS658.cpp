/*
HBS658.cpp - Library implementation for HBS658.
*/

#include "HBS658.h"

HBS658::HBS658(byte dataPin, byte clockPin, byte numDigits, boolean activateDisplay, byte intensity)
{
    this->dataPin = dataPin;
    this->clockPin = clockPin;
    this->_maxDisplays = HBS658_MAX_POS;
    this->digits = numDigits;

    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);

    digitalWrite(dataPin, HIGH);
    digitalWrite(clockPin, HIGH);
    clearDisplay();
    setupDisplay(activateDisplay, intensity);
    // delay(1); // NOTE: using delay in constructor (called prior to Setup) may hang the MCU!
}

void HBS658::setupDisplay(boolean active, byte intensity)
{
    sendCommand(HBS658_CMD_DISPLAY | (active ? 8 : 0) | min(7, intensity));
}

void HBS658::clearDisplay()
{
    // Clear all data registers. The number of registers depends on the chip.
    sendCommand(HBS658_CMD_DATA_AUTO); // set auto increment addressing mode

    // send the address followed by bulk-sending of the data to clear the display memory
    start();
    send(HBS658_CMD_ADDRESS);
    for (int i = 0; i < _maxDisplays; i++)
    {
        send(0x00);
    }
    stop();
}

void HBS658::setSegments(byte segments, byte position)
{
    if (position < _maxDisplays)
        sendData(position, segments);
}

void HBS658::sendChar(byte pos, byte data, boolean dot)
{
    setSegments(data | (dot ? 0b10000000 : 0), pos);
}

void HBS658::setDisplayDigit(byte digit, byte pos, boolean dot)
{
    const byte *numberFont = HBS658_NUMBER_FONT;
    sendChar(pos, pgm_read_byte_near(numberFont + (digit & 0xF)), dot);
}

void HBS658::setDisplayToDecNumber(long nNumber, byte bDots)
{
    // byte bDots=0
    // Function to display a decimal number on a n-digit clock display.
    // Kept simple to fit in ATtiny44A
    for (byte nPos = 1; nPos <= digits; nPos++)
    {
        setDisplayDigit(nNumber % 10, digits - nPos, bDots & _BV(nPos));
        nNumber /= 10;
    }
}

void HBS658::clearDisplayDigit(byte pos, boolean dot)
{
    sendChar(pos, 0, dot);
}

void HBS658::setDisplay(const byte values[], byte size)
{ // send an array of values to the display
    for (byte i = 0; i < size; i++)
    {
        sendChar(i, pgm_read_byte_near(values + i), 0);
    }
}

void HBS658::bitDelay()
{
    delayMicroseconds(5);
}

void HBS658::start()
{
    digitalWrite(dataPin, LOW);
    bitDelay();
}

void HBS658::stop()
{
    digitalWrite(dataPin, LOW);
    digitalWrite(clockPin, LOW);
    bitDelay();
    digitalWrite(clockPin, HIGH);
    // bitDelay();
    digitalWrite(dataPin, HIGH);
    bitDelay();
}

void HBS658::send(byte data)
{
    // shiftOut(dataPin, clockPin, LSBFIRST, data);
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(clockPin, LOW);
        bitDelay();
        digitalWrite(dataPin, data & 1 ? HIGH : LOW);
        bitDelay();
        data >>= 1;
        digitalWrite(clockPin, HIGH);
        bitDelay();
    }
    bitDelay();

    // read the acknowledgement
    // TODO? return the ack?
    digitalWrite(clockPin, LOW);
    pinMode(dataPin, INPUT);
    bitDelay();
    digitalWrite(clockPin, HIGH);
    bitDelay();
    uint8_t ack = digitalRead(dataPin);
    if (ack == 0)
        digitalWrite(dataPin, LOW);
    pinMode(dataPin, OUTPUT);
}

void HBS658::sendCommand(byte cmd)
{
    start();
    send(cmd);
    stop();
}

void HBS658::sendData(byte address, byte data)
{
    sendCommand(HBS658_CMD_DATA_FIXED); // use fixed addressing for data
    start();
    send(HBS658_CMD_ADDRESS | address); // address command + address
    send(data);
    stop();
}