#include "AS5600.h"
#include "Wire.h"

AS5600L as5600;   //  use default Wire

void setup()
{
  Serial.begin(115200);
  as5600.begin(14, 15); //ESP32
  as5600.setAddress(0x40);

  as5600.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
  delay(1000);
  as5600.resetCumulativePosition(777); //  NEW CODE to set the initial value to 777
}


void loop()
{
  static uint32_t lastTime = 0;

  //  set initial position
  as5600.getCumulativePosition();

  //  update every 100 ms
  if (millis() - lastTime >= 100)
  {
    lastTime = millis();
    Serial.println(as5600.getCumulativePosition());
  }
}
