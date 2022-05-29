//
//    FILE: AS56000.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: Arduino library for AS5600 magnetic rotation meter
//    DATE: 2022-05-28
//     URL: https://github.com/RobTillaart/AS5600



  //  Power-up time  1 minute
  //  check  Timing Characteristics



#include "AS5600.h"


//  CONFIGURATION REGISTERS
#define AS5600_ZMCO                     0x00
#define AS5600_ZPOS                     0x01  // + 0x02
#define AS5600_MPOS                     0x03  // + 0x04
#define AS5600_MANG                     0x05  // + 0x06
#define AS5600_CONF                     0x07  // + 0x08

//  unknown 0x09-0x0A

//  OUTPUT REGISTERS
#define AS5600_RAW_ANGLE                0x0C  // + 0x0D
#define AS5600_ANGLE                    0x0E  // + 0x0F

//  STATUS REGISTERS
#define AS5600_STATUS                   0x0B
#define AS5600_AGC                      0x1A
#define AS5600_MAGNITUDE                0x1B  // + 0x1C
#define AS5600_BURN                     0xFF


AS5600::AS5600(TwoWire *wire)
{
  _wire       = wire;
}


#if defined (ESP8266) || defined(ESP32)
bool AS5600::begin(int dataPin, int clockPin, int directionPin)
{
  _directionPin = directionPin;
  pinMode(_directionPin, OUTPUT);
  setDirection(AS5600_CLOCK_WISE);

  _wire      = &Wire;
  if ((dataPin < 255) && (clockPin < 255))
  {
    _wire->begin(dataPin, clockPin);
  } else {
    _wire->begin();
  }
  if (! isConnected()) return false;
  return true;
}
#endif


bool AS5600::begin(int directionPin)
{
  _directionPin = directionPin;
  pinMode(_directionPin, OUTPUT);
  setDirection(AS5600_CLOCK_WISE);

  _wire->begin();
  if (! isConnected()) return false;
  return true;
}


bool AS5600::isConnected()
{
  _wire->beginTransmission(_address);
  return ( _wire->endTransmission() == 0);
}


/////////////////////////////////////////////////////////
//
//  CONFIGURATION REGISTERS + direction pin
//
void AS5600::setDirection(uint8_t direction)
{
  digitalWrite(_directionPin, direction);
}


uint8_t AS5600::getDirection()
{
  return digitalRead(_directionPin);
}


uint8_t AS5600::getZMCO()
{
  uint8_t value = readReg(AS5600_ZMCO);
  return value;
}


void AS5600::setZPosition(uint16_t value)
{
  writeReg(AS5600_ZPOS, (value >> 8) & 0x0F);
  writeReg(AS5600_ZPOS + 1, value & 0xFF);
}


uint16_t AS5600::getZPosition()
{
  uint16_t value = (readReg(AS5600_ZPOS) & 0x0F) >> 8;
  value += readReg(AS5600_ZPOS + 1);
  return value;
}


void AS5600::setMPosition(uint16_t value)
{
  writeReg(AS5600_MPOS, (value >> 8) & 0x0F);
  writeReg(AS5600_MPOS + 1, value & 0xFF);
}


uint16_t AS5600::getMPosition()
{
  uint16_t value = (readReg(AS5600_MPOS) & 0x0F) >> 8;
  value += readReg(AS5600_MPOS + 1);
  return value;
}


void AS5600::setMaxAngle(uint16_t value)
{
  writeReg(AS5600_MANG, (value >> 8) & 0x0F);
  writeReg(AS5600_MANG + 1, value & 0xFF);
}


uint16_t AS5600::getMaxAngle()
{
  uint16_t value = (readReg(AS5600_MANG) & 0x0F) >> 8;
  value += readReg(AS5600_MANG + 1);
  return value;
}


void AS5600::setConfigure(uint16_t value)
{
  writeReg(AS5600_CONF, (value >> 8) & 0x0F);
  writeReg(AS5600_CONF + 1, value & 0xFF);
}


uint16_t AS5600::getConfigure()
{
  uint16_t value = (readReg(AS5600_CONF) & 0x2F) >> 8;
  value += readReg(AS5600_CONF + 1);
  return value;
}


/////////////////////////////////////////////////////////
//
//  OUTPUT REGISTERS
//
uint16_t AS5600::rawAngle()
{
  uint16_t value = (readReg(AS5600_RAW_ANGLE) & 0x0F) << 8;
  value += readReg(AS5600_RAW_ANGLE + 1);
  return value;
}


uint16_t AS5600::readAngle()
{
  uint16_t value = (readReg(AS5600_ANGLE) & 0x0F) << 8;
  value += readReg(AS5600_ANGLE + 1);
  return value;
}


/////////////////////////////////////////////////////////
//
//  STATUS REGISTERS
//
uint8_t AS5600::readStatus()
{
  uint8_t value = readReg(AS5600_STATUS);
  return value;
}


uint8_t AS5600::readAGC()
{
  uint8_t value = readReg(AS5600_AGC);
  return value;
}


uint16_t AS5600::readMagnitude()
{
  uint16_t value = (readReg(AS5600_MAGNITUDE) & 0x0F) << 8;
  value += readReg(AS5600_MAGNITUDE + 1);
  return value;
}


/////////////////////////////////////////////////////////
//
//  BURN COMMANDS
//
//  DO NOT UNCOMMENT - USE AT OWN RISK - READ DATASHEET
//
//  void AS5600::burnAngle()
//  {
//    writeReg(AS5600_BURN, x0x80);
//  }
//
//
//  void AS5600::burnSetting()
//  {
//    writeReg(AS5600_BURN, x0x40);
//  }



/////////////////////////////////////////////////////////
//
//  PRIVATE
//
uint8_t AS5600::readReg(uint8_t reg)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _error = _wire->endTransmission();

  _wire->requestFrom(_address, (uint8_t)1);
  uint8_t _data = _wire->read();
  return _data;
}


uint8_t AS5600::writeReg(uint8_t reg, uint8_t value)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->write(value);
  _error = _wire->endTransmission();
  return _error;
}


//  -- END OF FILE --


