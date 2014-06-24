/*
  uCam - Library for flashing controlling 4D uCam module.
  Created by Johnny Haddock, 03 June 2014.
  
  This example has a uCam plugged into Serial1 and Serial2.
  It will do all the camera setup and get a snapshot for each time
  a byte is sent to the arduino via Serial.
*/
#include <uCam.h>

uCam cam0;
uCam cam1;
boolean camSetUp;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  cam0.begin(&Serial1, &Serial);
  cam1.begin(&Serial2, &Serial);
  camSetUp = false;
}

void loop()
{
  if (!camSetUp)
  {
    camSetUp = setUpCam();
  }
  
  if (camSetUp && Serial.available() > 0 )
  {
    Serial.read(); //clear the byte that was set for picture
    delay(150); //wait just a bit so that pressing button doesn't cause the chessboard to be in motion
    cam0.SNAPSHOT();
    cam1.SNAPSHOT();
    cam0.GET();
    cam1.GET();
  }
}

boolean setUpCam()
{
  if (cam0.SYNC() == 0)
    {
      digitalWrite(13,HIGH);  
    } else {
      cam0.RESET();
      return false;
    }
    if (cam1.SYNC() ==0)
    {
      digitalWrite(12,HIGH);
    } else {
      cam0.RESET();
      digitalWrite(13,LOW);
      cam1.RESET();
      return false;
    }
    cam0.INIT();
    cam1.INIT();
    return true;
}
