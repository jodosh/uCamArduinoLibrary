/*
  Created by Johnny Haddock, 26 Feb 2017.
  
  This example has a uCam plugged into Serial1 and Serial2.
  It will do all the camera setup on a "S" is sent to Serial
  A snapshot is taken and transmitted each time a "P" is sent on Serial
  Once the host computer calculates heading a berring a byte is sent to move MP4GS
  Stopping once "Q" is sent
*/
#include <uCam.h>
#include <Wire.h>
#include <MD25.h>
#include <I2C.h>
#include <NewPing.h>

#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

uCam cam0;
uCam cam1;
boolean camSetUp;
boolean waitToStart;

MD25 controller;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  cam0.begin(&Serial1, &Serial);
  cam1.begin(&Serial2, &Serial);
  waitToStart = true;
  camSetUp = false;
  controller = MD25(0xB0 >> 1);
  Wire.begin();
  controller.setAccelerationRate(3);
  controller.setMode(0x00); //each motor register is litteral speed in range 0 (full reverse) 128 (stop) 255 (full forward)
  controller.disableTimeout();
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

    input_byte = Serial.read();
    switch (input_byte) {
      case 0x50:
        //"P" take Picture
        cam0.SNAPSHOT();
        cam1.SNAPSHOT();
        cam0.GET();
        cam1.GET();
        break;
      case 0x51:
        //"Q"
        blinkLED(13);
        waitToStart = true;
        break;
      case 0x46:
        //"F" left 30 deg
        turnLeft(30);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }      
        break;
      case 0x45:
        //"E" left 25 deg
        turnLeft(25);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      case 0x44:
        //"D" left 20 deg
        turnLeft(20);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      case 0x43:
        //"C" left 15 deg
        turnLeft(15);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      case 0x42:
        //"B" left 10 deg
        turnLeft(10);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      case 0x41:
        //"A" left 05 deg
        turnLeft(5);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      case 0x53:
        //"S" go forward
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      case 0x61:
        //"a" right 5 deg
        turnRight(5);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      case 0x62:
        //"b" right 10 deg
        turnRight(10);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      case 0x63:
        //"c" right 15 deg
        turnRight(15);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      case 0x64:
        //"d" right 20 deg
        turnRight(20);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      case 0x65:
        //"e" right 25 deg
        turnRight(25);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      case 0x66:
        //"f" right 30 deg
        turnRight(30);
        unsigned long distance = sonar.ping_cm();
        while (distance > 10 || distance == 0)
        {
          driveForward();
          distance = sonar.ping_cm();
        }
        break;
      
    }
    
  }
}


void driveForward()
{
  int msPerMm = 1;
  controller.setMotor1Speed(131); //left motor forward
  controller.setMotor2Speed(131); //right motor forward
  delay(25);
  //delay(distance_mm*msPerMm);
  //controller.setMotor1Speed(128); //left motor stop
  //controller.setMotor2Speed(128); //right motor stop
}

void turnLeft(int deg)
{
  int msPerDeg = 82;
  controller.setMotor1Speed(125); //left motor reverse
  controller.setMotor2Speed(131); //right motor forward
  delay(msPerDeg*deg);
  controller.setMotor1Speed(128); //left motor stop
  controller.setMotor2Speed(128); //right motor stop
}

void turnRight(int deg)
{
  int msPerDeg = 135;
  controller.setMotor1Speed(131); //left motor forward
  controller.setMotor2Speed(125); //right motor reverse
  delay(msPerDeg*deg);
  controller.setMotor1Speed(128); //left motor stop
  controller.setMotor2Speed(128); //right motor stop
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
    if (cam0.INIT(&UCAM_4BIT_GREY,&UCAM_160X120) != 0)
    {
        cam0.RESET();
        digitalWrite(13,LOW);
        cam1.RESET();
        digitalWrite(12,LOW);
        return false;
    }
    if (cam1.INIT(&UCAM_4BIT_GREY,&UCAM_160X120) != 0)
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
        digitalWrite(pinNumber,LOW);
    }
}
