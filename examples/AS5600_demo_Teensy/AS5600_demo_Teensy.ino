//
//    FILE: AS5600_demo_Teensy.ino
//  AUTHOR: Rob Tillaart, SquadQuiz
// PURPOSE: demo
// 
//  tested compilation with Teensy 4.1

#include "AS5600.h"
#include "Wire.h"

AS5600L as5600(AS5600_DEFAULT_ADDRESS);   //  use default Wire


void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("AS5600_LIB_VERSION: ");
  Serial.println(AS5600_LIB_VERSION);

  Wire.begin();

  as5600.begin(4);  //  set direction pin.
  as5600.setDirection(AS5600_CLOCK_WISE);  //  default, just be explicit.
  int b = as5600.isConnected();
  Serial.print("Connect: ");
  Serial.println(b);
  delay(1000);
}


void loop()
{
  //  Serial.print(millis());
  //  Serial.print("\t");
  Serial.print(as5600.readAngle());
  Serial.print("\t");
  Serial.println(as5600.rawAngle());
  //  Serial.println(as5600.rawAngle() * AS5600_RAW_TO_DEGREES);

  delay(1000);
}


//  -- END OF FILE --
