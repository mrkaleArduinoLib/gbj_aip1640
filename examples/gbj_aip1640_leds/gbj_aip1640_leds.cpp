/*
  NAME:
  gbj_aip1640_leds

  DESCRIPTION:
  The sketch turns on all LEDs on the matrix subsequently.
  - In the first round LEDs are turned on one by one.
  - In the second round LEDs are turned on and off in sequence.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#if defined(__AVR__)
  #include <Arduino.h>
  #include <inttypes.h>
#elif defined(ESP8266) || defined(ESP32)
  #include <Arduino.h>
#elif defined(PARTICLE)
  #include <Particle.h>
#endif
#include "gbj_aip1640.h"

const unsigned int PERIOD_TEST = 300;

gbj_aip1640 disp = gbj_aip1640(2, 3);

void setup()
{
  disp.begin();
}

void loop()
{
  // Added LEDs
  disp.displayClear();
  for (byte col = 0; col < disp.COLUMNS; col++)
  {
    byte pattern = 0;
    for (byte led = 0; led < disp.ROWS; led++)
    {
      pattern <<= 1;
      pattern |= 1;
      disp.display(pattern, col);
      delay(PERIOD_TEST);
    }
  }
  // Running LEDs
  disp.displayClear();
  for (byte col = 0; col < disp.COLUMNS; col++)
  {
    for (byte led = 0; led < disp.ROWS; led++)
    {
      disp.display(1 << led, col);
      delay(PERIOD_TEST);
    }
    disp.display(static_cast<byte>(0x00), col);
  }
}
