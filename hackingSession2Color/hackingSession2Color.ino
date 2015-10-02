//This is the code for the second Arduino Hacking Session at 
//Fjord Chicago

//Developed by Ian Wilson for Fjord Chicago
//Questions to:   ian.a.wilson@fjordnet.com
//Permi Address:  imbyde@gmail.com

//Shareable with Permission from Fjord Makeshop

//Contains open source code and libraries from Adafruit

//We use:
//Color Sensor (Adafruit TCS34725)
//A Touch strip
//Adafruit NeoPixels
//Piezo Buzzer

//To:
//Demonstrate multiple inputs and outputs and how to use libraries

char separator[] = " | ";

//Libraries
#include <Wire.h>                //Basic arduino library, default
#include "Adafruit_TCS34725.h"   //Adafruit Color Sensor Library
#include <Adafruit_NeoPixel.h>   //Adafruit NeoPixel Library

//Set-up some variables for later use
const int tonePin = 11;          //Piezo Buzzer Output
const int touchPin = A0;         //Touch Strip Input
int touchIn = 0;                 //Setting a variable to store the analog input data
int neoPin = 6;                  //Data pin for the NeoPixels
#define NUMPIXELS 8              //Define the number of NeoPixels being used

//Set-up the NeoPixel Strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, neoPin, NEO_GRB + NEO_KHZ800);


//Get the color Sensor Ready
byte gammatable[256]; // our RGB -> eye-recognized gamma color
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {

  //Start up the Serial Console and display a welcome message
  Serial.begin(115200);
  //Serial.println("Color View Test!");

  //Start up the neoPixel Strip
  strip.begin();
  strip.show();

  //Check to make sure we can find the color sensor
  if (tcs.begin()) {
    //Serial.println("Found sensor");
  } else {
    //Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  // Convert to human seeable light
  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
  }
}


void loop() {
  //Read the color sensor
  uint16_t clear, red, green, blue;

  tcs.setInterrupt(false);      // turn on LED

  delay(60);  // takes 50ms to read 
  
  tcs.getRawData(&red, &green, &blue, &clear);

  tcs.setInterrupt(true);  // turn off LED
  
  // Convert values to RGB
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;

  //Print what colors we are seeing
    
             Serial.print("Red"); //Label for the sensor
          Serial.print(":"); //Seperator between values
      Serial.print((int)r, DEC); //Actual value
  
  Serial.print(separator);//Separate different readings
  

     
          Serial.print("Green"); //Label for the sensor
          Serial.print(":"); //Seperator between values
      Serial.print((int)g, DEC); //Actual value

    Serial.print(separator);//Separate different readings
  

     
          Serial.print("Blue"); //Label for the sensor
          Serial.print(":"); //Seperator between values
      Serial.print((int)b, DEC); //Actual value
      
     Serial.println();
  //Light up the NeoPixels that color
  for (int i = 0; i < 5; i++){
    strip.setPixelColor(i, strip.Color((int)r - 30,(int)g - 30,(int)b - 30));
    strip.show();
  }

  strip.setPixelColor(7, strip.Color((int)r - 30, 0, 0));
  strip.show();

  strip.setPixelColor(6, strip.Color(0, (int)g - 30, 0));
  strip.show();

  strip.setPixelColor(5, strip.Color(0, 0, (int)b - 30));
  strip.show();

//  touchIn = analogRead(touchPin);                           //Read the AnalogSensor
//  if (touchIn > 100){                                       //Only play a sound if touched
//    //Serial.println(touchIn);                                //Show the read Value
//    int touchInMapped = map(touchIn, 400, 1000, 200, 800);  //Map the values to a sound range
//    tone(tonePin, touchInMapped, 100);                      //Play the mapped value
//  }

}

