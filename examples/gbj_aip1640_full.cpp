/*
  NAME:
  gbj_aip1640_full.cpp

  DESCRIPTION:
  The sketch turns on all LEDs on the matrix and cycle all contrast levels.
  - It is suitable to check functionality of all LEDs.

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

const unsigned int PERIOD_TEST = 500;

gbj_aip1640 disp = gbj_aip1640(2, 3);

void setup()
{
  disp.begin();
}

void loop()
{
  for (byte contrast = disp.getContrastMin(); contrast < disp.getContrastMax();
       contrast++)
  {
    disp.setContrast(contrast);
    disp.displayFill(0xFF);
    delay(PERIOD_TEST);
  }
}
