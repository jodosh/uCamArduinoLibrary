import processing.serial.*;

Serial myPort;
int[] inByte = new int[4800]; //4800 pixels in an 80x60 image each pixel is 8-bit
long counter;
int[] counterArr = new int[3];

void setup() {
  myPort = new Serial(this, "COM10", 115200);
}

void draw() {
  while ( myPort.available() <3) {
    delay(1);
  }
  setCounter(); 
   getImage("test.bmp");
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
    img.pixels[i] = color(inByte[i],inByte[i],inByte[i]);
  }
  img.updatePixels();
  img.save(fileName);
  image(img,0,0);
  
}
