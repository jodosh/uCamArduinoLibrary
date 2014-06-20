import processing.serial.*;

Serial myPort;

void setup() {
  myPort = new Serial(this, "COM10", 115200);
}

int GET(){
  myPort.clear(); //clears the buffer
  delay(100);
  while ( myPort.available() <6) {}
  myPort.read();
  if (myPort.read() != 14)//(int)14 == 0x0E
  {
    return 1;
  }
  myPort.read();
  myPort.read(); //4byte
  myPort.read(); //5byte
  myPort.read(); //6byte
  while (myPort.available() < 6 ){}
  myPort.read(); //1 byte DATA
  int ERR = myPort.read();
  if (ERR != 10)//(int)10 == 0x0A
  {
    return 2;
  }
  myPort.read(); //DATA type bit
  long counter = myPort.read(); //LSB
  long byteIn = myPort.read(); //BYTE1
  counter = counter+(byteIn*256);
  byteIn = myPort.read();//MSB
  counter = counter+(byteIn*65536);//counter is now the number of bytes that the camera will send.
  
  int[] inByte = new int[2399]; //4800 pixels in an 80x60 image each pixel is 4-bit 
  for (int in=0; in < counter; in++)
  {
    while (myPort.available() < 0) {} //make sure buffer has data
    
    inByte[in] = myPort.read(); //save the int values for the HEX of each pixel
    
  }
  
  
  
  PImage img = createImage(80, 60, RGB);
  img.loadPixels();
  for (int i =0; i < img.pixels.length; i++)
  {
    img.pixels[i] = inByte[i];
  }
  img.updatePixels();
  
  //send ACK that all data has been received
  
  //Serial1.write(ACKDATA,6);
  return 0;
}
