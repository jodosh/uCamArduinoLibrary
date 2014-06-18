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
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  cam1.begin(&Serial1, &Serial);
}

void loop()
{
  delay(5000);
  if (cam1.SYNC() ==0){
	digitalWrite(13, HIGH);
  } else {
	digitalWrite(12, HIGH);
  }
  cam1.INIT();
  cam1.SNAPSHOT();
  cam1.GET();
  delay  (100000);
  return;
}
