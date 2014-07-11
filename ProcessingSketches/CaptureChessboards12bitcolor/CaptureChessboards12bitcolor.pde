import processing.serial.*;

Serial myPort;
int loopCounter;
int[] inByte = new int[7200]; //4800 pixels in an 80x60 image each pixel is 12-bit
long counter;
int[] counterArr = new int[3];
boolean picReq;

void setup() {
  myPort = new Serial(this, "COM9", 115200);
  loopCounter = 0;
  picReq = false;
  delay(8000);
}

void draw(){
  
  if (!picReq){
    myPort.write(65); //wite A out of serial port
    picReq = true;
  }
  
  if ( myPort.available() >2) {
      println("Press Anykey to take a picture."); 
      println("Already Captured "+loopCounter+" images");
      picReq = false;
  
    setCounter();
    if (loopCounter < 10)
    {  
      getImage("0"+loopCounter+"First.bmp");
    } else{
      getImage(loopCounter+"First.bmp");
    }
  
    while ( myPort.available() <3) {
      delay(1);
    }
    setCounter();
    if (loopCounter < 10)
    {  
      getImage("0"+loopCounter+"Second.bmp");
      loopCounter++;
    } else{
      getImage(loopCounter+"Second.bmp");
      loopCounter++;
    }
    delay(5000); //give time to move chessboard
  }
  //delay(100);
 
}

void setCounter(){
  counterArr[0] = myPort.read();
  counterArr[1] = myPort.read();
  counterArr[2] = myPort.read();
  
  counter = counterArr[0]+(counterArr[1]*256)+(counterArr[2]*65536);
}

void getImage(String fileName){
    
  
   
  for (int in=0; in < counter; in++)
  {
   if (myPort.available() > 0) {
     inByte[in] = myPort.read(); //save the int values for the HEX of each pixel
   }else{
     in--;
   }    
  }
  
  
  
  PImage img = createImage(80, 60, RGB);
  img.loadPixels();
  println("img.pixels.length: "+img.pixels.length);
  int in = 0;
  for (int i =0; i < img.pixels.length; i=i+2)
  {
    img.pixels[in] = color(((inByte[in]&0xF0)>>4)*17,(inByte[in]&0x0F)*17,(((inByte[in+1]&0xF0)>>4)*17));
    img.pixels[in+1] = color((inByte[in+1]&0x0F)*17,((inByte[in+2]&0xF0)>>4)*17,((inByte[in+2]&0x0F)*17));
    in=in+3;
  }
  img.updatePixels();
  img.save(fileName);
  image(img,0,0);
  
}


