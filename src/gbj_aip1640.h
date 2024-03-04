/*
  NAME:
  gbj_aip1640

  DESCRIPTION:
  Library for WMA451 matrix 8x16 blue LEDs matrix displays controlled by
  the driver AiP1640.
  - The communication protocol in the data sheet is dubbed as I2C, but it is not
    due to absence an address on the bus.
  - The communication is based on dedicated protocol for the controller and it
    is simple serial communication protocol with start/stop condition with only
    one-way communication, i.e., without acknowledgment, CRC, and other
    feedback measures for garanted data transmission.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3
  http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
  License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/gbj_aip1640.git
 */
#ifndef GBJ_AIP1640_H
#define GBJ_AIP1640_H

#if defined(__AVR__)
  #include <Arduino.h>
  #include <inttypes.h>
#elif defined(ESP8266) || defined(ESP32)
  #include <Arduino.h>
#elif defined(PARTICLE)
  #include <Particle.h>
#endif

class gbj_aip1640
{
public:
  enum Geometry : uint8_t
  {
    // Hardware implemented rows of leds (data byte)
    ROWS = 8,
    // Hardware implemented columns of leds
    COLUMNS = 16,
  };

  /*
    Define hardware setup.

    DESCRIPTION:
    The constructor method stores physical features of the display
    to the class instance object.

    PARAMETERS:
    pinSclk - Microcontroller pin's number utilized as a serial clock.
      - Data type: non-negative integer
      - Default value: 2
      - Limited range: 0 ~ 255 (by microcontroller datasheet)

    pinDin - Microcontroller pin's number utilized as a data input and output.
      - Data type: non-negative integer
      - Default value: 3
      - Limited range: 0 ~ 255 (by microcontroller datasheet)

    RETURN: object
  */
  inline gbj_aip1640(uint8_t pinSclk = 2, uint8_t pinDin = 3)
  {
    status_.pinSclk = pinSclk;
    status_.pinDin = pinDin;
  }

  /*
    Initialize display.

    DESCRIPTION:
    The method sets the microcontroller's pins dedicated for the driver and
    perfoms initial sequence recommended by the data sheet for the controller.
    - The method clears the display and sets it to the normal operation mode.
    - The method checks whether pins set by constructor are not equal.

    PARAMETERS: none

    RETURN: none.
  */
  inline void begin()
  {
    // Setup pins
    pinMode(status_.pinDin, OUTPUT);
    pinMode(status_.pinSclk, OUTPUT);
    digitalWrite(status_.pinSclk, LOW);
    digitalWrite(status_.pinDin, LOW);
    // Initialize controller
    displayClear();
    setContrast();
  }

  /*
    Manipulate display off or on.

    DESCRIPTION:
    Corresponding method either either turns on or off the entire display
    module or toggles its state without changing current contrast level.
    - All methods are suitable for making a display module blink.

    PARAMETERS: none

    RETURN: none
  */
  inline void displayOn()
  {
    busSend(Commands::CMD_DISP_INIT | Commands::CMD_DISP_ON | status_.contrast);
    status_.state = true;
  }
  inline void displayOff()
  {
    busSend(Commands::CMD_DISP_INIT | Commands::CMD_DISP_OFF);
    status_.state = false;
  }
  inline void displayToggle() { status_.state ? displayOff() : displayOn(); }

  inline void display(uint8_t *dataBuffer,
                      uint8_t dataBytes = Geometry::COLUMNS,
                      uint8_t address = Commands::CMD_ADDR_MIN)
  {
    dataBytes = min(dataBytes, Geometry::COLUMNS - address);
    busSend(Commands::CMD_DATA_INIT | Commands::CMD_DATA_AUTO);
    busSend(Commands::CMD_ADDR_INIT | address, dataBuffer, dataBytes);
    displayOn();
  }

  inline void display(uint8_t dataByte,
                      uint8_t address = Commands::CMD_ADDR_MIN)
  {
    busSend(Commands::CMD_DATA_INIT | Commands::CMD_DATA_FIXED);
    busSend(Commands::CMD_ADDR_INIT | address, dataByte);
    displayOn();
  }

  inline void displayFill(uint8_t dataByte)
  {
    busSend(Commands::CMD_DATA_INIT | Commands::CMD_DATA_AUTO);
    busSend(Commands::CMD_ADDR_INIT | Commands::CMD_ADDR_MIN,
            dataByte,
            Geometry::COLUMNS);
    displayOn();
  }

  inline void displayClear()
  {
    displayFill(0x00);
  }

  // Public setters

  /*
    Set contrast of the digital tubes.

    DESCRIPTION:
    The corresponding method sets contrast to respective level of all digital
    tubes and simultaniously turns display on.

    PARAMETERS:
    contrast - Level of constrast/brightness.
      - Data type: non-negative integer
      - Default value: 3
      - Limited range: 0 ~ 7

    RETURN: none
  */
  inline void setContrast(uint8_t contrast = 3)
  {
    status_.contrast = contrast & getContrastMax();
    displayOn();
  }
  inline void setContrastMin() { return setContrast(getContrastMin()); }
  inline void setContrastMax() { return setContrast(getContrastMax()); }

  // Public getters
  inline bool isDisplayOn() { return status_.state; }
  inline bool isDisplayOff() { return !isDisplayOn(); }
  inline uint8_t getContrast() { return status_.contrast; }
  static inline uint8_t getContrastMax() { return 7; }
  static inline uint8_t getContrastMin() { return 0; }

private:
  enum Commands : uint8_t
  {
    // Data command setting (0x40) - upper nibble
    CMD_DATA_INIT = 0b01000000,
    /* Lower nibbles */
    // Address automatically added 1 (0x0) OR-ed with data command setting
    // equal to Universal mode
    CMD_DATA_AUTO = 0b0000,
    // Fixed address (0x4) OR-ed with data command setting
    CMD_DATA_FIXED = 0b0100,
    // Test mode (0x8) OR-ed with data command setting
    CMD_DATA_TEST = 0b1000,

    // Address command setting (0xC0) - upper nibble
    CMD_ADDR_INIT = 0b11000000,
    /* Lower nibbles*/
    // Minimal start or fixed adddress (0x0) OR-ed with address command setting
    CMD_ADDR_MIN = 0b0000,
    // Maximal start or fixed adddress (0xF) OR-ed with address command setting
    CMD_ADDR_MAX = 0b1111,

    // Display control command setting (0x80) - upper nibble
    CMD_DISP_INIT = 0b10000000,
    /* Lower nibbles*/
    // Display OFF (0x0) OR-ed with display control command setting
    CMD_DISP_OFF = 0b0000,
    // Display ON (0x8) OR-ed with display control command setting
    // and OR-ed by contrast 0x0 ~ 0x7
    CMD_DISP_ON = 0b1000,
    // Minimal display contrast (0x0) OR-ed with display control command ON
    CMD_DISP_MIN = 0b000,
    // Maximal display contrast (0x7) OR-ed with display control command ON
    CMD_DISP_MAX = 0b111,
  };
  enum Timing : uint16_t
  {
    // MCU relaxing delay in microseconds after pin change
    TIMING_RELAX = 3,
  };
  // Microcontroller status features
  struct Status
  {
    // Number of serial clock pin
    uint8_t pinSclk;
    // Number of data input pin
    uint8_t pinDin;
    // Current contrast level
    uint8_t contrast;
    // Current display ON/OFF state
    bool state = true;
  } status_;
  // Write to pin with delay
  inline void pinWrite(uint8_t pin, byte value)
  {
    digitalWrite(pin, value);
    delayMicroseconds(Timing::TIMING_RELAX);
  }
  // Start condition - pull down DIN from HIGH to LOW while SCLK is HIGH
  inline void beginTransmission()
  {
    pinWrite(status_.pinSclk, LOW);
    pinWrite(status_.pinDin, HIGH);
    pinWrite(status_.pinSclk, HIGH);
    pinWrite(status_.pinDin, LOW);
  }
  // Stop condition - pull up DIN from LOW to HIGH while SCLK is HIGH
  inline void endTransmission()
  {
    pinWrite(status_.pinSclk, LOW);
    pinWrite(status_.pinDin, LOW);
    pinWrite(status_.pinSclk, HIGH);
    pinWrite(status_.pinDin, HIGH);
  }
  // Write byte to the bus
  inline void busWrite(uint8_t data)
  {
    // For active rising edge of clock pulse
    // pinWrite(status_.pinSclk, LOW);
    // shiftOut(status_.pinDin, status_.pinSclk, LSBFIRST, data);

    // Bit by bit with delays
    for (uint8_t i = 0; i < Geometry::ROWS; i++)
    {
      pinWrite(status_.pinSclk, LOW);
      pinWrite(status_.pinDin, data & 0x01);
      pinWrite(status_.pinSclk, HIGH);
      data >>= 1;
    }
  }
  // Send sole command
  inline void busSend(uint8_t command)
  {
    beginTransmission();
    busWrite(command);
    endTransmission();
  }
  // Send data at fixed address
  inline void busSend(uint8_t command, uint8_t data)
  {
    beginTransmission();
    // Send one command byte
    busWrite(command);
    // Send one data byte
    busWrite(data);
    endTransmission();
  }
  // Send byte stream at auto-increment addressing
  inline void busSend(uint8_t command, uint8_t *buffer, uint8_t bufferLen)
  {
    beginTransmission();
    busWrite(command);
    for (uint8_t bufferIndex = 0; bufferIndex < bufferLen; bufferIndex++)
    {
      busWrite(*buffer++);
    }
    endTransmission();
  }
  // Send a byte as a byte stream at auto-increment addressing
  inline void busSend(uint8_t command, uint8_t data, uint8_t length)
  {
    beginTransmission();
    busWrite(command);
    for (uint8_t i = 0; i < length; i++)
    {
      busWrite(data);
    }
    endTransmission();
  }
};
#endif
