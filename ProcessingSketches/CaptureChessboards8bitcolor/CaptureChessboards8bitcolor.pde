import processing.serial.*;

Serial myPort;
int loopCounter;
int[] inByte = new int[4800]; //4800 pixels in an 80x60 image each pixel is 8-bit
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
  for (int i =0; i < img.pixels.length; i++)
  {
    img.pixels[i] = color(((inByte[i]&0xE0)>>5)*36,((inByte[i]&0x1C)>>2)*36,((inByte[i]&0x03)*85));
  }
  img.updatePixels();
  img.save(fileName);
  image(img,0,0);
  
}


