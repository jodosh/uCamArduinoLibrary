/*
  uCam.cpp - Library for controlling 4D uCam module.
  Created by Johnny Haddock, 03 June 2014.
*/

#include "Arduino.h"
#include "uCam.h"

uint8_t _SYNCCMD[]={0xAA, 0x0D, 0x00, 0x00, 0x00, 0x00};
uint8_t _INIT80[]={0xAA, 0x01, 0x00, 0x02, 0x01, 0x01};
uint8_t _LIGHTCMD[]={0xAA, 0x13, 0x01, 0x00, 0x00, 0x00};
uint8_t _SNAPCMD[]={0xAA, 0x05, 0x1, 0x00, 0x00, 0x00};
uint8_t _GETCMD[]={0xAA, 0x04, 0x01, 0x00, 0x00, 0x00};
uint8_t _DATAFLAG[]={0xAA, 0x0A, 0x01}; //the next 3 bits are the size to expect
uint8_t _ACKSYNC[]={0xAA, 0x0E, 0x0D, 0x00, 0x00, 0x00};
uint8_t _ACKDATA[]={0xAA, 0x0E, 0x0A, 0x00, 0x00, 0x00};
uint8_t _RESET[]={0xAA, 0x08, 0x00, 0x00, 0x00, 0xFF};
uint8_t _ERROR;

uCam::uCam()
{
  _Serial = NULL;
  _CommsToPC = NULL;
}

void uCam::begin(HardwareSerial *SerialCam, HardwareSerial *Comms)
{
	_Serial = SerialCam;
	//_Serial->begin(115200);
	_CommsToPC = Comms;
	//_CommsToPC->begin(115200);
}

int uCam::SYNC()
{

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
  return 0;

}

int uCam::INIT()
{
  _Serial->flush();
  _Serial->write(_INIT80,6);
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
  _ERROR = _Serial->read();
  if (_ERROR != 0x0A)
  {
    return 2;
  }
  _Serial->read(); //DATA type bit
  
  uint8_t temp = _Serial->read();
  uint32_t counter = temp; //LSB
  //Serial.write(temp);
  
  temp = _Serial->read();
  uint32_t byteIn = temp; //BYTE1
  //Serial.write(temp);
  
  counter = counter+(byteIn*256);
  temp = _Serial->read();
  byteIn = temp;//MSB
  //Serial.write(temp);
  counter = counter+(byteIn*65536);//counter is now the number of bytes that the camera will send.
  _CommsToPC->println(counter);
  while (_CommsToPC->available() < 0){}
  _CommsToPC->read();
  while (counter>0)
  {
    //Serial.write (_Serial->.read());
    _CommsToPC->write (_Serial->read());
    //Serial.print (", 0x");
    counter--;
    //while (Serial.available() < 0){}
  }
  
  //send ACK that all data has been received
  
  _Serial->write(_ACKDATA,6);
  return 0;
}
