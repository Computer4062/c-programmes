/*
Voltmeter that shows the accurate voltage across a circuit with Arduino
Interface looks like an analog speedometer with a circle and a line at the center
*/

import processing.serial.*;

int fixedPosX = 250;      // first X position of the line (center of the circle)
int fixedPosY = 250;      // first Y position of the line (center of the circle)
float posX;               // second X position of the line
float posY;               // second Y position of the line

int diameter = 450;       // radius of the meter
int lineLen  = 450/2;     // length of the line on the meter (radius of the circular meter)

float voltage = 0.00;     // test value
float count   = 0.00;     // for animations

int CAMBRIDGE_BLUE  = #A3C1AD;
int ARTICHOKE_GREEN = #4B6F44;
int BRIGHT_GREEN    = #66FF00;

Serial serialPort;
boolean connected = false;
int serialBytesArray[] = new int[3];
char byteCount = 0;

void setup()
{
  // Surface parameters
  size(500, 600);  
  surface.setTitle("VOLTMETER");
  surface.setLocation(100, 100);
  
  // Setup serial connection with voltmeter
  println(Serial.list());                         // for debugging purposes
  String portName =  "COM5";                      // change the name to the corresponding port
  
  // Check if port is available
  boolean portConnected = false;
  String[] portList = Serial.list();
  while(!portConnected)                                  // stay in while loop while voltmeter is not connected
  {
    if(portList.length > 0)                              // check if any ports were picked up
    {
      for(int i = 0; i < portList.length; i++)           // iterate through the exsisting ports to look for the specified port
      {
        if(portList[i].equals(portName))
        {
          portConnected = true;
          break;
        }
      }
      
      // warn user if port cannot be found
      if(false == portConnected)
      {
        println(portName + " cannot be found");
      }
    }    
    else
    {
      println("No ports detected");
    }
    
    // repeat process every second
    delay(1000);
  }
  
  serialPort = new Serial(this, portName, 9600);  // instantiate the serial communication mode
}

void draw()
{
  // define some graphical parameters
  background(CAMBRIDGE_BLUE);
  noStroke();
  
  // draw a circle at the center of the screen
  fill(ARTICHOKE_GREEN);
  stroke(BRIGHT_GREEN);
  strokeWeight(2);
  ellipse(250, 250, diameter, diameter);
  
  // draw an interior circle for style
  noFill();
  stroke(BRIGHT_GREEN);
  strokeWeight(3.1);
  ellipse(250, 250, 10, 10);
  
  // add the labels onto the meter
  labelMeter();
  
  // draw the line
  stroke(BRIGHT_GREEN);
  strokeWeight(2);
  float angle = radians(count * 72) + HALF_PI;
  float posX = fixedPosX + (cos(angle) * lineLen);
  float posY = fixedPosY + (sin(angle) * lineLen);
  
  // create an animated like effect
  if ((count - 0.1) < voltage && (count + 0.1) > voltage)
  {
    count = voltage;
  }
  else if(count < voltage)
  {
    count += 0.1;
  }
  else if(count > voltage)
  {
    count -= 0.1;
  }
  
  line(fixedPosX, fixedPosY, posX, posY);
}

// label the circle with values of voltages (V)
void labelMeter()
{
  fill(BRIGHT_GREEN);
  textSize(20);
    
  int v_count = 0;
  
  // find the co-ordinates and draw the labels
  for(int a = 0; a < 360; a += 1)
  {
    // find the co-ordinates of labels
    float angle = radians(a) + HALF_PI;          // find angle for point
    float x = fixedPosX + (cos(angle) * 200);    // find X co-ordinate of point
    float y = fixedPosY + (sin(angle) * 200);    // find Y co-ordinate of point
      
    if((a % 72) == 0)                            // every 5th position write the number   
    {      
      // draw in the label   
      text(str(v_count), x, y);
      v_count++;                                 // increment v_count by 1
    }
    else if ((a % 7) == 0)                       // Every 10th position put a marker
    {
      point(x, y);
    }
  }
  
  // Display voltage
  textSize(40);
  text(str(voltage) + "V", 200, 540);
  
  textSize(20);
  text("Max Voltage: 5V", 190, 560);
  
  // Display connection
  text("Connection: ", 190, 580);
  
  if(connected)                       // display green circle
  {
    noStroke();
    fill(0, 255, 0);
    ellipse(300, 575, 15, 15);
  }
  else                                // display red circle
  {
    noStroke();
    fill(255, 0, 0);
    ellipse(300, 575, 15, 15);
  }
}

void serialEvent(Serial myPort)
{
  int inByte = myPort.read();
  
  println(inByte);
  
  // Check if voltmeter is connected properly
  if(false == connected)                              // if not connected, connect
  {  
    if('H' == inByte)
    {
      myPort.clear();
      connected = true;
      myPort.write('H');
    }
  }
  else                                                 // if connected read incomming data
  {
    serialBytesArray[byteCount] = inByte;
    byteCount++;
    
    // If message 3 bytes in length
    if(byteCount > 2)
    {
      int n1 = ASCII_TO_INT(serialBytesArray[0]);
      int n2 = ASCII_TO_INT(serialBytesArray[1]);
      int n3 = ASCII_TO_INT(serialBytesArray[2]);
      
      println(str(n1) + "\t" + str(n2) + "\t" + str(n3));         // debugging purposes
      
      String voltageValue = n1 + "." + n2 + n3;
      voltage = float(voltageValue);
      
      myPort.write('H');
      byteCount = 0;
    }
  }
}

int ASCII_TO_INT(int n)
{
  int number = 0;
  
  switch(n)
  {
    case 48:
      number = 0;
      break;
     
     case 49:
       number = 1;
       break;
       
     case 50:
       number = 2;
      
     case 51:
       number = 3;
       break;
        
     case 52:
       number = 4;
       break;
         
     case 53:
       number = 5;
       break;
       
     case 54:
       number = 6;
       break;
       
      case 55:
        number = 7;
        break;
        
      case 56:
        number = 8;
        break;
        
      case 57:
        number = 9;
        break;
  }
  
  return number;
}
