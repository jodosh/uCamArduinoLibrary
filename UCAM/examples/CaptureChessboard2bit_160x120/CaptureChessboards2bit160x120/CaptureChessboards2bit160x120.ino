/*
  uCam - Library for controlling 4D uCam module.
  Created by Johnny Haddock, 02 Feb 2017.
  
  This example has a uCam plugged into Serial1 and Serial2.
  It will do all the camera setup and get a snapshot for each time
  a byte is sent to the arduino via Serial. Stopping once "Q" is sent
*/
#include <uCam.h>

uCam cam0;
uCam cam1;
boolean camSetUp;
boolean waitToStart;
boolean cam0SetSync;
boolean cam1SetSync;
boolean cam0SetInit;
boolean cam1SetInit;

void setup()
{
  Serial.begin(115200);
  //Serial.begin(14400);
  Serial1.begin(14400);
  Serial2.begin(14400);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(32, OUTPUT);
  cam0.begin(&Serial1, &Serial);
  cam1.begin(&Serial2, &Serial);
  cam0SetSync = false;
  cam1SetSync = false;
  cam0SetInit = false;
  cam1SetInit = false;
  waitToStart = true;
  camSetUp = false;
  digitalWrite(30,LOW);
  //delay(5000);  //wait  seconds for camera stabilization
}

void loop()
{
  while (waitToStart){
    if (Serial.available() >0){
      //check for "S" as signal that serial is set up, this allows the camera circuits to stabelize before trying a sync
      if (Serial.read() == 0x53){
        waitToStart=false;
      }
    }
    
  }
  
  if (!camSetUp)
  {
    blinkLED(13);
    camSetUp = setUpCam();
    
  }
  
  if (camSetUp && Serial.available() > 0 )
  {
    //Check the byte that was sent if "Q" we have all the chessboards we need
      
    if (Serial.read() == 0x51){
       waitToStart=true;
    }
    delay(150); //wait just a bit so that pressing button doesn't cause the chessboard to be in motion
    cam0.SNAPSHOT();
    cam1.SNAPSHOT();
    if (cam0.GET() !=0)
    {
      blinkLED(13);
      blinkLED(13);
      blinkLED(13);
      blinkLED(13);
      waitToStart=true;
    }
    if (cam1.GET() != 0)
    {
      blinkLED(13);
      blinkLED(13);
      blinkLED(13);
      blinkLED(13);
      waitToStart=true;
    }
  }
}

boolean setUpCam()
{
  if (!cam0SetSync)
  {
    if (cam0.SYNC() == 0)
    {
      digitalWrite(22,HIGH);
      cam0SetSync = true;  
    } else {
      cam0.RESET();
      return false;
    }
  }

  
  if (!cam1SetSync)
  {
    if (cam1.SYNC() ==0)
    {
      digitalWrite(24,HIGH);
      cam1SetSync = true;
    } else {
      //cam0.RESET();
      digitalWrite(22,LOW);
      //cam1.RESET();
      return false;
    }
  }

  
  if (!cam0SetInit && cam0SetSync)
  {
    if (cam0.INIT(&UCAM_4BIT_GREY,&UCAM_160X120) != 0)
    {
        //cam0.RESET();
        digitalWrite(22,LOW);
        //cam1.RESET();
        digitalWrite(24,LOW);
        return false;
    }
    cam0SetInit = true;
    digitalWrite(26,HIGH);
  }

  
  if (!cam1SetInit && cam1SetSync)
  {
  if (cam1.INIT(&UCAM_4BIT_GREY,&UCAM_160X120) != 0)
    {
        //cam0.RESET();
        digitalWrite(22,LOW);
        //cam1.RESET();
        digitalWrite(24,LOW);
        return false;
    }
    digitalWrite(13,HIGH);
    cam1SetInit = true;
  }
  if (cam0SetSync && cam1SetSync && cam0SetInit && cam1SetInit)
  {
    delay(2000);
    return true;
  }
  return false;
}


void blinkLED(int pinNumber)
{
    for (int i=0; i<3; i++)
    {
        digitalWrite(pinNumber,LOW);
        delay(150);
        digitalWrite(pinNumber,HIGH);
        delay(150);
        digitalWrite(pinNumber,LOW);
    }
}
