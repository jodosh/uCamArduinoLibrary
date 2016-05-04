/*
  uCam - Library for flashing controlling 4D uCam module.
  Created by Johnny Haddock, 03 June 2014.
  
  This example has a uCam plugged into Serial1 and Serial2.
  It will do all the camera setup and get a snapshot for each time
  a byte is sent to the arduino via Serial. Stopping once "Q" is sent
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
    //Check the byte that was sent if "Q" we have all the chessboards we need
      
    if (Serial.read() == 0x51){
       blinkLED(13);
       return;
    }
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
    if (cam0.INIT(&UCAM_8BIT_GREY,&UCAM_80X60) != 0)
    {
        cam0.RESET();
        digitalWrite(13,LOW);
        cam1.RESET();
        digitalWrite(12,LOW);
        return false;
    }
    if (cam1.INIT(&UCAM_8BIT_GREY,&UCAM_80X60) != 0)
    {
        cam0.RESET();
        digitalWrite(13,LOW);
        cam1.RESET();
        digitalWrite(12,LOW);
        return false;
    }
    return true;
}


void blinkLED(int pinNumber)
{
    for (int i=0; i<10; i++)
    {
        digitalWrite(pinNumber,LOW);
        delay(150);
        digitalWrite(pinNumber,HIGH);
        delay(150);
    }
}
