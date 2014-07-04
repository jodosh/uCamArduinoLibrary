/*
  uCam.h - Library for controlling 4D uCam module.
  Created by Johnny Haddock, 03 June 2014.
*/
#ifndef uCam_h
#define uCam_h

#include "Arduino.h"

//bytes for color init
static uint8_t UCAM_2BIT_GREY = 0x01;
static uint8_t UCAM_4BIT_GREY = 0x02;
static uint8_t UCAM_8BIT_GREY = 0x03;
static uint8_t UCAM_8BIT_COLOR = 0x04;
static uint8_t UCAM_12BIT_COLOR = 0x05;
static uint8_t UCAM_16BIT_COLOR = 0x06;
static uint8_t UCAM_JPEG = 0x07;

//bytes for resolution
static uint8_t UCAM_80X60 = 0x01;
static uint8_t UCAM_160X120 = 0x03;
static uint8_t UCAM_320X240 = 0x05;
static uint8_t UCAM_640X480 = 0x07;
static uint8_t UCAM_128X128_RAW = 0x09;
static uint8_t UCAM_128X96_RAW = 0x0B;

class uCam
{
  public:
    uCam();
	void begin(HardwareSerial *SerialCam, HardwareSerial *CommsToPC);
	void RESET();
    int SYNC();
	int INIT(uint8_t *Color, uint8_t *RawResolution);
	int SNAPSHOT();
	int GET();
  private:
	HardwareSerial *_Serial;
	HardwareSerial *_CommsToPC;
};

#endif 
