/*
  uCam - Library for flashing controlling 4D uCam module.
  Created by Johnny Haddock, 03 June 2014.
  
  This example has a uCam plugged into Serial1.
  It will do all the camera setup and get a single snapshot.
*/
#include <uCam.h>

uCam cam1;
boolean camSetUp;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  cam1.begin(&Serial1, &Serial);
  camSetUp = false;
}

boolean setUpCam()
{
    if (cam1.SYNC() ==0)
    {
      digitalWrite(12,HIGH);
    } else {;
      cam1.RESET();
      return false;
    }
    cam1.INIT(&UCAM_8BIT_GREY,&UCAM_80X60);
    return true;
}

void loop()
{
  if (!camSetUp)
  {
    delay(1);
    camSetUp = setUpCam();
  } else {
    cam1.SNAPSHOT();
    cam1.GET();
    delay  (100000);
    return;
  }
}
