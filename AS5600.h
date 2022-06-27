#pragma once
//
//    FILE: AS5600.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.0
// PURPOSE: Arduino library for AS5600 magnetic rotation meter
//    DATE: 2022-05-28
//     URL: https://github.com/RobTillaart/AS5600


#include "Arduino.h"
#include "Wire.h"


#define AS5600_LIB_VERSION              (F("0.2.0"))

//  setDirection
const uint8_t AS5600_CLOCK_WISE         = 0;  //  LOW
const uint8_t AS5600_COUNTERCLOCK_WISE  = 1;  //  HIGH

//  0.0879120879120879121;
const float   AS5600_RAW_TO_DEGREES     = 360.0 / 4095.0;
//  0.00153435538636864138630654133494;
const float   AS5600_RAW_TO_RADIANS     = PI * 2.0 / 4095.0;

//  getAngularSpeed
const uint8_t AS5600_MODE_DEGREES       = 0;
const uint8_t AS5600_MODE_RADIANS       = 1;

//  CONFIGURE CONSTANTS
//  check datasheet for details

//  setOutputMode
const uint8_t AS5600_OUTMODE_ANALOG_100 = 0;
const uint8_t AS5600_OUTMODE_ANALOG_90  = 1;
const uint8_t AS5600_OUTMODE_PWM        = 2;

//  setPowerMode
const uint8_t AS5600_POWERMODE_NOMINAL  = 0;
const uint8_t AS5600_POWERMODE_LOW1     = 1;
const uint8_t AS5600_POWERMODE_LOW2     = 2;
const uint8_t AS5600_POWERMODE_LOW3     = 3;

//  setPWMFrequency
const uint8_t AS5600_PWM_115            = 0;
const uint8_t AS5600_PWM_230            = 1;
const uint8_t AS5600_PWM_460            = 2;
const uint8_t AS5600_PWM_920            = 3;

//  setHysteresis
const uint8_t AS5600_HYST_OFF           = 0;
const uint8_t AS5600_HYST_LSB1          = 1;
const uint8_t AS5600_HYST_LSB2          = 2;
const uint8_t AS5600_HYST_LSB3          = 3;

//  setSlowFilter
const uint8_t AS5600_SLOW_FILT_16X      = 0;
const uint8_t AS5600_SLOW_FILT_8X       = 1;
const uint8_t AS5600_SLOW_FILT_4X       = 2;
const uint8_t AS5600_SLOW_FILT_2X       = 3;

//  setFastFilter
const uint8_t AS5600_FAST_FILT_NONE     = 0;
const uint8_t AS5600_FAST_FILT_LSB6     = 1;
const uint8_t AS5600_FAST_FILT_LSB7     = 2;
const uint8_t AS5600_FAST_FILT_LSB9     = 3;
const uint8_t AS5600_FAST_FILT_LSB18    = 4;
const uint8_t AS5600_FAST_FILT_LSB21    = 5;
const uint8_t AS5600_FAST_FILT_LSB24    = 6;
const uint8_t AS5600_FAST_FILT_LSB10    = 7;

//  setWatchDog
const uint8_t AS5600_WATCHDOG_OFF       = 0;
const uint8_t AS5600_WATCHDOG_ON        = 1;



class AS5600
{
public:
  AS5600(TwoWire *wire = &Wire);

#if defined (ESP8266) || defined(ESP32)
           //  255 is software controlled direction pin
  bool     begin(int sda, int scl, uint8_t directionPin = 255);
#endif
           //  255 is software controlled direction pin
  bool     begin(uint8_t directionPin = 255);
  bool     isConnected();

  uint8_t  getAddress() { return _address; };  //  0x36


  //  SET CONFIGURE REGISTERS
  //  read datasheet first
  void     setDirection(uint8_t direction = AS5600_CLOCK_WISE);
  uint8_t  getDirection();

  uint8_t  getZMCO();

  void     setZPosition(uint16_t value);
  uint16_t getZPosition();

  void     setMPosition(uint16_t value);
  uint16_t getMPosition();

  void     setMaxAngle(uint16_t value);
  uint16_t getMaxAngle();

  //  access the whole configuration register
  //  check datasheet for bit fields
  void     setConfigure(uint16_t value);
  uint16_t getConfigure();

  //  access details of the configuration register
  //  0 = Normal
  //  1,2,3 are low power mode - check datasheet
  void     setPowerMode(uint8_t powerMode);
  uint8_t  getPowerMode();

  //  0 = off    1 = lsb1    2 = lsb2    3 = lsb3
  void     setHysteresis(uint8_t hysteresis);
  uint8_t  getHysteresis();

  //  0 = analog 0-100%
  //  1 = analog 10-90%
  //  2 = PWM
  void     setOutputMode(uint8_t outputMode);
  uint8_t  getOutputMode();

  //  0 = 115    1 = 230    2 = 460    3 = 920 (Hz)
  void     setPWMFrequency(uint8_t pwmFreq);
  uint8_t  getPWMFrequency();

  //  0 = 16x    1 = 8x     2 = 4x     3 = 2x
  void     setSlowFilter(uint8_t mask);
  uint8_t  getSlowFilter();

  //  0 = none   1 = LSB6   2 = LSB7   3 = LSB9
  //  4 = LSB18  5 = LSB21  6 = LSB24  7 = LSB10
  void     setFastFilter(uint8_t mask);
  uint8_t  getFastFilter();

  //  0 = OFF
  //  1 = ON   (auto low power mode)
  void     setWatchDog(uint8_t mask);
  uint8_t  getWatchDog();


  //  READ OUTPUT REGISTERS
  uint16_t rawAngle();
  uint16_t readAngle();


  //  READ STATUS REGISTERS
  uint8_t  readStatus();
  uint8_t  readAGC();
  uint16_t readMagnitude();

  //  access detail status register
  bool     detectMagnet();


  //  BURN COMMANDS
  //  DO NOT UNCOMMENT - USE AT OWN RISK - READ DATASHEET
  //  void burnAngle();
  //  void burnSetting();

  //  experimental 0.1.2 - to be tested.
  //  approximation of the angular speed in rotations per second.
  //  mode == 1: radians /second
  //  mode == 0: degrees /second  (default)
  float    getAngularSpeed(uint8_t mode = AS5600_MODE_DEGREES);


private:
  uint8_t  readReg(uint8_t reg);
  uint16_t readReg2(uint8_t reg);
  uint8_t  writeReg(uint8_t reg, uint8_t value);
  uint8_t  writeReg2(uint8_t reg, uint16_t value);

  const uint8_t _address = 0x36;
  uint8_t  _directionPin = 255;
  uint8_t  _direction    = AS5600_CLOCK_WISE;
  uint8_t  _error        = 0;

  TwoWire*  _wire;

  uint32_t _lastMeasurement = 0;
  uint16_t _lastAngle       = 0;
};


//  -- END OF FILE --


