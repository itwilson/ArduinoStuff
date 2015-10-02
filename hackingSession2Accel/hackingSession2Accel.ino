// I2C device class (I2Cdev) demonstration Arduino sketch for MPU9150
// 1/4/2013 original by Jeff Rowberg <jeff@rowberg.net> at https://github.com/jrowberg/i2cdevlib
//          modified by Aaron Weiss <aaron@sparkfun.com>
//
// Changelog:
//     2011-10-07 - initial release
//     2013-1-4 - added raw magnetometer output

/* ============================================
I2Cdev device library code is placed under the MIT license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"


#include <Adafruit_NeoPixel.h>
#define PIN            6
#define NUMPIXELS      8
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

#define LED_PIN 13
bool blinkState = false;

//////////////
//////////////   Calibration Ints
//////////////

int axFuzzMax = -2000;
int axFuzzMin = 2000;
int axScaler = 30;

int ayFuzzMax = -2000;
int ayFuzzMin = 2000;
int ayScaler = 25;

int azFuzzMax = -20000;
int azFuzzMin = 20000;
int azScaler = 50;

int colorHoldTime = 0;


//////////////
//////////////
//////////////

//////////////
//////////////   Multigraph support
//////////////

char separator[] = " | ";

//////////////
//////////////
//////////////


void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    strip.begin();
    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(115200);

    // initialize device
    //Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    //Serial.println("Testing device connections...");
    //Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    while(millis() < 5000){
        accelgyro.getAcceleration(&ax, &ay, &az);
        
        //Set ax Fuzz
        if (ax < axFuzzMin){
          axFuzzMin = ax;
        }else if (ax > axFuzzMax){
          axFuzzMax = ax;
        }

        //Set ay Fuzz
        if (ay < ayFuzzMin){
          ayFuzzMin = ay;
        }else if (ay > ayFuzzMax){
          ayFuzzMax = ay;
        }

        //Set az Fuzz
        if (az < azFuzzMin){
          azFuzzMin = az;
        }else if (az > azFuzzMax){
          azFuzzMax = az;
        }
    }
    
    // read raw accel/gyro measurements from device
    //accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    // these methods (and a few others) are also available
    accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    // display tab-separated accel/gyro x/y/z values
    //Serial.print("a/g/m:\t");
    

    
    Serial.print("Accel X"); //Label for the sensor
    Serial.print(":"); //Seperator between values
    if (millis() > colorHoldTime){
      Serial.print(0);
      
        for (int i = 0; i < NUMPIXELS; i++){
          strip.setPixelColor(i, strip.Color(0,0,0)); // Moderately bright green color.
          strip.show(); // This sends the updated pixel color to the hardware.
          
      }
    }if (ax > axFuzzMax + 1000){
      Serial.print((ax - axFuzzMax) / axScaler);
      
        for (int i = 4; i < NUMPIXELS; i++){
          strip.setPixelColor(i, strip.Color(0,150,150)); // Moderately bright green color.
          strip.show(); // This sends the updated pixel color to the hardware.
          
      }
      colorHoldTime = millis() + 500;
    } else if (ax < axFuzzMin - 1000) {
      Serial.print((ax + axFuzzMin) / axScaler);
      
      for (int i = 3; i >= 0; i--){
         strip.setPixelColor(i, strip.Color(150,0,150)); 
         strip.show(); 
      
      }
      colorHoldTime = millis() + 500;
    }
    Serial.print(separator);//Separate different readings

    Serial.print("Accel Y"); //Label for the sensor
    Serial.print(":"); //Seperator between values
    if (ay < ayFuzzMax && ay > ayFuzzMin){
      Serial.print(0);
    } else if (ay > ayFuzzMax){
      Serial.print((ay - ayFuzzMax) / ayScaler);
    } else {
      Serial.print((ay + ayFuzzMin) / ayScaler);
    }
    Serial.print(separator);//Separate different readings

     Serial.print("Accel Z"); //Label for the sensor
    Serial.print(":"); //Seperator between values
    if (az < azFuzzMax && az > azFuzzMin){
      Serial.print(0);
    } else if (az > azFuzzMax){
      Serial.print((az - azFuzzMax) / azScaler);
    } else {
      Serial.print((az - azFuzzMin) / azScaler);
    }

Serial.println();
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);

    
}
