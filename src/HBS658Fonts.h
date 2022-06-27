/*
HBS658Fonts.h - Font definition for HBS658.

Segment labels
 -- A --
|       |
F       B
 -- G --
E       C
|       |
 -- D --  .DP

The bits are displayed by the mapping below:
 -- 0 --
|       |
5       1
 -- 6 --
4       2
|       |
 -- 3 --  .7

*/

#ifndef HBS658Fonts_h
#define HBS658Fonts_h

// definition for standard hexadecimal numbers
const PROGMEM byte HBS658_NUMBER_FONT[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001  // F
};

const PROGMEM byte MINUS = 0b01000000;

#endif
