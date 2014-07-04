/*
  uCam - Library for flashing controlling 4D uCam module.
  Created by Johnny Haddock, 03 June 2014.
  
  This example has a uCam plugged into Serial1 and Serial2.
  It will do all the camera setup and get a single snapshot.
*/
#include <uCam.h>

uCam cam0;
uCam cam1;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  cam0.begin(&Serial1, &Serial);
  cam1.begin(&Serial2, &Serial);
}

void loop()
{
  delay(5000);
  cam0.SYNC();
  cam1.SYNC();
  cam0.INIT(&UCAM_8BIT_GREY,&UCAM_80X60);
  cam1.INIT(&UCAM_8BIT_GREY,&UCAM_80X60);
  cam0.SNAPSHOT();
  cam1.SNAPSHOT();
  cam0.GET();
  cam1.GET();
  delay  (100000);
  return;
}
