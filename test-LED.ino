#include "LEDsequence.h"

unsigned long dt = 0;
unsigned long then = 0;

const byte s[18] = { LED_PTN_NULL, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, LED_PTN_NULL, LED_PTN_LOW, LED_PTN_HIGH, LED_PTN_SOFT_STROBE, LED_PTN_HARD_STROBE, LED_PTN_SOFT_STROBE_F, LED_PTN_HARD_STROBE_F };

LEDsequence led0(A3); // declare with pin

void setup()
{
  led0.assign(s, 18); // assign schedule, length
  //led0.off();
  //led0.low();
  //led0.high();
  //led0.strobe_soft();
  //led0.strobe_hard();
  //led0.strobe_soft_f();
  //led0.strobe_hard_f();

  then = micros();
}

void loop()
{
  // timing
  dt = micros() - then;
  then += dt;


  led0.update(dt);



  delay(10); // change this delay is to see it handle different timings
}