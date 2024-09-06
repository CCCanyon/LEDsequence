/*
IMPLEMENTATION:
 ================= Declaration ===================

#include "led_sequence.h" // include this

const byte s[20] = { LED_PTN_NULL, LED_PTN_NULL, 5, 3, 7, 2, 4, 6, 9 }; // sequence: byte array

LEDsequence led0(A3); // declare with pin

 ================= Setup =================

led0.assign(s, 9); // assign sequence, length

or use preset

led0.strobe_hard_f();

 ================= Loop =================

led0.update(dt); // call this frequently, real update frequency is 62.5ms

NUMBERS:
read the pattern defined down there, or read "numbers.png"
*/
#ifndef LEDsequence_h
#define LEDsequence_h

#define LED_inverval 62500

#define LED_PTN_NULL 10
#define LED_PTN_LOW 11
#define LED_PTN_HIGH 12
#define LED_PTN_SOFT_STROBE 13
#define LED_PTN_HARD_STROBE 14
#define LED_PTN_SOFT_STROBE_F 15
#define LED_PTN_HARD_STROBE_F 16
#define LED_PTN_STROBE_SF 17


#define LED_PTN_PRESET_LEN 1
const byte LED_PTN_PRESET_NULL[1] = { LED_PTN_NULL };
const byte LED_PTN_PRESET_HIGH[1] = { LED_PTN_HIGH };
const byte LED_PTN_PRESET_LOW[1] = { LED_PTN_LOW };
const byte LED_PTN_PRESET_SOFT_STROBE[1] = { LED_PTN_SOFT_STROBE };
const byte LED_PTN_PRESET_HARD_STROBE[1] = { LED_PTN_HARD_STROBE };
const byte LED_PTN_PRESET_SOFT_STROBE_F[1] = { LED_PTN_SOFT_STROBE_F };
const byte LED_PTN_PRESET_HARD_STROBE_F[1] = { LED_PTN_HARD_STROBE_F };
const byte LED_PTN_PRESET_STROBE_SF[1] = { LED_PTN_STROBE_SF };

class LEDsequence
{
public:
  int pin;
  unsigned long timer;
  LEDsequence(const int& _pin = LED_BUILTIN): pin(_pin), timer(0) { off(); }

  void assign(const byte* _sequence, const int& _len = LED_PTN_PRESET_LEN)
  {
    i0 = 0; // reset schedule index
    i1 = 31; // reset sequence index
    len = _len; // set schesule length
    sequence = _sequence; // point to schedule
    pinMode(pin, OUTPUT); // activate pin
  }

  void update(const unsigned long& dt)
  {
    timer += dt; // add time
    if(timer > LED_inverval) // time interval met
    {
      timer = 0; // reset timer
      //Serial.println(i1); // print pattern index (tick number)
      bool H_bit = (pattern[sequence[i0]][0] >> i1) & 1; // high bit
      bool L_bit = (pattern[sequence[i0]][1] >> i1) & 1; // low bit
      if     (H_bit && L_bit) analogWrite(pin, 255); // 100%
      else if(H_bit) analogWrite(pin, 120); // 100%
      else if(L_bit) analogWrite(pin, 40); // 50%
      else analogWrite(pin, 0); // 0%
      if(--i1 < 0) // next pattern tick (iterate pattern index)
      {
        if(++i0 >= len) i0 = 0; // sequence index loops back
        i1 = 23; // reset pattern index
      }
    }
  }
  // presets
  inline void off(){ assign(LED_PTN_PRESET_NULL); }
  inline void low(){ assign(LED_PTN_PRESET_LOW); }
  inline void high(){ assign(LED_PTN_PRESET_HIGH); }
  inline void strobe_soft(){ assign(LED_PTN_PRESET_SOFT_STROBE); }
  inline void strobe_hard(){ assign(LED_PTN_PRESET_HARD_STROBE); }
  inline void strobe_soft_f(){ assign(LED_PTN_PRESET_SOFT_STROBE_F); }
  inline void strobe_hard_f(){ assign(LED_PTN_PRESET_HARD_STROBE_F); }
  inline void strobe_sf(){ assign(LED_PTN_PRESET_STROBE_SF); }
  inline void set(const uint8_t& pwm){ analogWrite(pin, pwm); }

private:
  const byte* sequence;
  int len = 0;
  int i0 = 0; // sequence  index
  int i1 = 0; // pattern index
  // pattern
  const uint32_t pattern[18][2] = { // 24 steps for two beats, time: 2-2, 100% / 50%
    { 0b000000000000000000000000,   // [0] long low
      0b111111111111111111000000 }, // llllllllllllllllll------
    { 0b110000000000000000000000,   // [1] 1 beat 
      0b110000000000000000000000 }, // hh----------------------
    { 0b110000110000000000000000,   // [2] 2 beats 
      0b110000110000000000000000 }, // hh----hh----------------
    { 0b100000001000000000000000,   // [3] 3 beats high-low-high
      0b100010001000000000000000 }, // h---l---h---------------
    { 0b000000000000000000000000,   // [4] 4 low beats
      0b111011101110111000000000 }, // lll-lll-lll-lll---------
    { 0b000000000000100000000000,   // [5] low long ends high
      0b111111111111100000000000 }, // llllllllllllh-----------
    { 0b111100000000111100000000,   // [6] 3 beats high-low-high
      0b111100111100111100000000 }, // llll--hhhh--llll--------
    { 0b100000000000100000000000,   // [7] syncopation
      0b100000000010100000000000 }, // h---------l-h-----------
    { 0b100000001000000000000000,   // [8] 8 fast strokes
      0b101010101010101000000000 }, // h-l-l-l-h-l-l-l---------
    { 0b111110111110111110000000,   // [9] 3 long beats all high 
      0b111111111111111110000000 }, // hhhhhlhhhhhlhhhhh-------
    { 0b000000000000000000000000,   // [10] NULL
      0b000000000000000000000000 }, //
    { 0b000000000000000000000000,   // [11] LOW
      0b111111111111111111111111},  //
    { 0b111111111111111111111111,   // [12] HIGH
      0b111111111111111111111111 }, //
    { 0b000111111000000111111000,   // [13] SOFT STROBE
      0b001011110100001011110100 }, //
    { 0b000111111000000111111000,   // [14] HARD STROBE
      0b000111111000000111111000 }, //
    { 0b001111000011110000111100,   // [15] SOFT STROBE-FAST
      0b010110100101101001011010},  //
    { 0b001111000011110000111100,   // [16] HARD STROBE-FAST
      0b001111000011110000111100},  //
    { 0b000000000000000000000000,   // [11] STROBE-SUPER_FAST
      0b101010101010101010101010}   //
  };
};

#endif
