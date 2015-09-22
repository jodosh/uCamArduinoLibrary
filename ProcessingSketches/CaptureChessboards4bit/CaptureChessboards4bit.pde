import processing.serial.*;

Serial myPort;
int loopCounter;
int[] inByte = new int[4800]; //4800 pixels in an 80x60 image each pixel is 4-bit
long counter;
int[] counterArr = new int[3];
int tempByte;
boolean picReq;

void setup() {
  myPort = new Serial(this, "COM9", 115200);
  picReq = false;
  loopCounter = 0;
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
      getImage("0"+loopCounter+"Left.bmp");
    } else{
      getImage(loopCounter+"Left.bmp");
    }
  
    while ( myPort.available() <3) {
      delay(1);
    }
    setCounter();
    if (loopCounter < 10)
    {  
      getImage("0"+loopCounter+"Right.bmp");
      loopCounter++;
    } else{
      getImage(loopCounter+"Right.bmp");
      loopCounter++;
    }
    delay(5000); //give time to move chessboard
  }
 
}

void setCounter(){
  counterArr[0] = myPort.read();
  counterArr[1] = myPort.read();
  counterArr[2] = myPort.read();
  
  counter = counterArr[0]+(counterArr[1]*256)+(counterArr[2]*65536);
}

void getImage(String fileName){
    
  
  int position = 0; 
  for (int in=0; in < counter; in++)
  {
   if (myPort.available() > 0) {
     //each byte is 2 values
     tempByte = myPort.read();
     inByte[position++] = ((tempByte & 0xF0) >> 4)*17;
     inByte[position++] = (tempByte & 0x0F)*17;
   }else{
     in--;
   }    
  }
  
  
  
  PImage img = createImage(80, 60, RGB);
  img.loadPixels();
  for (int i =0; i < img.pixels.length; i++)
  {
    //img.pixels[i] = color(inByte[i],inByte[i],inByte[i]);
    img.pixels[i] = color(inByte[i]);
  }
  img.updatePixels();
  img.save(fileName);
  image(img,0,0);
  
}


