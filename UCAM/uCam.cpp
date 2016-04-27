/*
  uCam.cpp - Library for controlling 4D uCam module.
  Created by Johnny Haddock, 03 June 2014.
*/

#include "Arduino.h"
#include "uCam.h"



uint8_t _SYNCCMD[]={0xAA, 0x0D, 0x00, 0x00, 0x00, 0x00};
uint8_t _INIT[]={0xAA, 0x01, 0x00}; //the next 3 bytes are passed in
uint8_t _LIGHTCMD[]={0xAA, 0x13, 0x01, 0x00, 0x00, 0x00};
uint8_t _SNAPCMD[]={0xAA, 0x05, 0x1, 0x00, 0x00, 0x00};
uint8_t _GETCMD[]={0xAA, 0x04, 0x01, 0x00, 0x00, 0x00};
uint8_t _DATAFLAG[]={0xAA, 0x0A, 0x01}; //the next 3 bytes are the size to expect
uint8_t _ACKSYNC[]={0xAA, 0x0E, 0x0D, 0x00, 0x00, 0x00};
uint8_t _ACKDATA[]={0xAA, 0x0E, 0x0A, 0x00, 0x00, 0x00};
uint8_t _RESET[]={0xAA, 0x08, 0x00, 0x00, 0x00, 0xFF};

uCam::uCam()
{
  _Serial = NULL;
  _CommsToPC = NULL;
}

void uCam::begin(HardwareSerial *SerialCam, HardwareSerial *Comms)
{
	_Serial = SerialCam;
	_CommsToPC = Comms;
}

void uCam::RESET()
{
	_Serial->write(_RESET,6);
	delay(300);
}

int uCam::SYNC()
{
  //resetting the camera first aids in SYNC when the MCU is reset
  _Serial->write(_RESET,6);
  delay(1500);

  while (_Serial->available() <6)
  {
	_Serial->write(_SYNCCMD,6);
    delay(10);
  }   
 
  _Serial->read();
  if (_Serial->read() != 0x0E)//check if this is ACK
  {
    return 1;
  }  
  if (_Serial->read() != 0x0D)//check that the ACK if for SYNC
  {
    return 1;
  }
  _Serial->read();
  _Serial->read();
  _Serial->read();//last byte of ACK response
  delay(2);
  _Serial->read();//First byte of SYNC from camera
  if (_Serial->read() != 0x0D)//make sure that command from cam is sync
  {
    return 2;
  }
  _Serial->read();
  _Serial->read();
  _Serial->read();
  _Serial->read(); //last byte of SYNC
  _Serial->flush();
  _Serial->write(_ACKSYNC,6);
  delay(1500); //per datasheet allow 1-2 seconds after sync before pic is captured
  return 0;

}

int uCam::INIT(uint8_t *Color, uint8_t *RawResolution)
{
  _Serial->flush();
  _Serial->write(_INIT,3);
  _Serial->write(Color,1);
  _Serial->write(RawResolution,1);
  _Serial->write(RawResolution,1); //TODO This will be changed with value checking if JPEG mode is added
  while (_Serial->available() <6){}
  _Serial->read();


  if (_Serial->read() != 0x0E)
  {
    return 1;
  }
  if (_Serial->read() != 0x01)
  {
    return 2;
  }
  _Serial->read();
  _Serial->read();
  _Serial->read();
  _Serial->flush();
  return 0;
}

int uCam::SNAPSHOT()
{
  _Serial->flush();
  _Serial->write(_SNAPCMD,6);
  while (_Serial->available() < 6) {}
  
  _Serial->read();
  if (_Serial->read() != 0x0E)
  {
    return 1;
  }
  if (_Serial->read() != 0x05)
  {
    return 2;
  }
  _Serial->read();
  _Serial->read();
  _Serial->read();  
  _Serial->flush();
 return 0; 
}

int uCam::GET()
{
  //local vars
  uint8_t counterArr[3];
  uint32_t counter;

  
  _Serial->flush();
  _Serial->write(_GETCMD,6);
  while ( _Serial->available() <6) {}
  _Serial->read();
  if (_Serial->read() != 0x0E)
  {
    return 1;
  }
  _Serial->read();
  _Serial->read(); //4byte
  _Serial->read(); //5byte
  _Serial->read(); //6byte
  while (_Serial->available() < 6 ){}
  _Serial->read(); //1 byte DATA
  if (_Serial->read() != 0x0A)
  {
    return 2;
  }
  _Serial->read(); //DATA type bit
  
  counterArr[0] = _Serial->read();  
  counterArr[1] = _Serial->read();
  counterArr[2] = _Serial->read();
  
  counter = counterArr[0]+(counterArr[1]*256)+(counterArr[2]*65536);
    
  _CommsToPC->write(counterArr,3);
  
  while (counter>0)
  {
    if (_Serial->available() > 0){
		_CommsToPC->write (_Serial->read());
		counter--;
		if (counter%1000==0){
			delay(1);       //this is a work around to provet buffer overflow when using bluetooth
		}
	}
  }
  
  //send ACK that all data has been received  
  _Serial->write(_ACKDATA,6);
  return 0;
}
