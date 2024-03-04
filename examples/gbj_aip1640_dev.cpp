/*
  NAME:
  gbj_aip1640_dev.cpp

  DESCRIPTION:
  Basic functionality of the controller library gbj_aip1640.

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

// Time delay in miliseconds for displaying a value
const unsigned int PERIOD_TEST = 500;

gbj_aip1640 disp = gbj_aip1640(2, 3);
byte buffer[gbj_aip1640::Geometry::COLUMNS];
byte dataEven = 0xAA;
byte dataOdd = 0x55;
byte dataFull = 0xFF;

void setup()
{
  Serial.begin(9600);
  Serial.print("Platform...");
#if defined(__AVR__)
  Serial.println("Arduino");
#elif defined(ESP8266)
  Serial.println("ESP8266");
#elif defined(ESP32)
  Serial.println("ESP32");
#elif defined(PARTICLE)
  Serial.println("PARTICLE");
#endif
  Serial.println("---");
  disp.begin();
  // disp.display(dataEven);
  // disp.display(dataOdd);
  disp.displayFill(dataFull);
}

void loop()
{
  // delay(PERIOD_TEST);
  // disp.displayToggle();
}
