/*
  uCam.h - Library for controlling 4D uCam module.
  Created by Johnny Haddock, 03 June 2014.
*/
#ifndef uCam_h
#define uCam_h

#include "Arduino.h"

class uCam
{
  public:
    uCam();
	void begin(HardwareSerial *SerialCam, HardwareSerial *CommsToPC);
    int SYNC();
	int INIT();
	int SNAPSHOT();
	int GET();
  private:
	HardwareSerial *_Serial;
	HardwareSerial *_CommsToPC;
};

#endif 
