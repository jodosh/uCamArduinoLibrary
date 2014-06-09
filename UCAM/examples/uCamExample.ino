/*
  uCam - Library for flashing controlling 4D uCam module.
  Created by Johnny Haddock, 03 June 2014.
  
  This example has a uCam plugged into Serial1.
  It will do all the camera setup and get a single snapshot.
*/
#include <uCam.h>

uCam cam1;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  cam1.begin(&Serial1, &Serial);
}

void loop()
{
  delay(5000);
  cam1.SYNC();
  delay(500);
  cam1.INIT();
  cam1.SNAPSHOT();
  cam1.GET();
  delay  (10000);
  return;
}
