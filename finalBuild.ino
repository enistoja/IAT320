//code for sensor adapted from https://learn.sparkfun.com/tutorials/apds-9960-rgb-and-gesture-sensor-hookup-guide/all
//code for memory card adapted from https://learn.sparkfun.com/tutorials/microsd-shield-and-sd-breakout-hookup-guide#sd-card-breakout-boards
//code for playing audio adapted from https://maker.pro/arduino/projects/arduino-audio-player
//libraries
#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <SPI.h>
#include <SD.h>

#include "TMRpcm.h"

//interrupt pin, 
#define APDS9960_INT    7 // Needs to be an interrupt pin

//define CS pin
#define SD_ChipSelectPin 3

//define Speaker pin
#define mySpeakerPin 6

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;
TMRpcm tmrpcm;

void setup() {

   // Initialize Serial port
  Serial.begin(9600);

  CircuitPlayground.begin();
  CircuitPlayground.clearPixels();
  
  // Initialize interrupt service routine. First parameter is the interrupt pin, second ISR (function that is gonna be called), mode defines what makes the code change, in this case going from HIGH to LOW
  //right now it is RX0, recommended to use attachInterrupt() function if this will be ran in other hardware. It is not likely to, so we do Unwise Coding Decisions
  attachInterrupt(0, interruptRoutine, FALLING);

  //check to see if sensor initializes
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  //check if sensor works
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }

  //speaker code
  
  //tmrpcm.speakerPin = mySpeakerPin;
  //tmrpcm.setVolume(6);

  //SD code
  if (!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }
  else
  {
    Serial.println("CARD WORKS");
  }

  

}

void loop() {

    //if not interruptible
  if( isr_flag == 1 ) {
    Serial.println("sensor fire!");
    //remove interruption
    detachInterrupt(0);
    //do gesture
    handleGesture();
    //make interruptible
    isr_flag = 0;
    //reatach interrupt
    attachInterrupt(0, interruptRoutine, FALLING);
  }
  

  
}

//reset interrupt flag
void interruptRoutine() {
  isr_flag = 1;
}


//set up gesture actions, this is where we'll read the files we have
void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("UP");
        //sound file!
        tmrpcm.play("coldNotes.mp3");
        CircuitPlayground.clearPixels();
        for (int i = 0; i<10; i++)
        {
          CircuitPlayground.setPixelColor(i, 255, 255, 255);
        }
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        //sound file!
        tmrpcm.play("forestSound.mp3");
        CircuitPlayground.clearPixels();
        for (int i = 0; i<10; i++)
        {
          CircuitPlayground.setPixelColor(i, 0, 255, 0);
        }
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        //sound file!
        tmrpcm.play("waterSound.mp3");
        CircuitPlayground.clearPixels();
        for (int i = 0; i<10; i++)
        {
          CircuitPlayground.setPixelColor(i, 0, 0, 255);
        }
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        //sound file!
        tmrpcm.play("sunSound.mp3");
        CircuitPlayground.clearPixels();
        for (int i = 0; i<10; i++)
        {
          CircuitPlayground.setPixelColor(i, 232, 187, 72);
        }
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        //sound file!
        tmrpcm.play("thunderSound.mp3");
        CircuitPlayground.clearPixels();
        for (int i = 0; i<10; i++)
        {
          CircuitPlayground.setPixelColor(i, 255, 0, 255);
        }
        break;
      case DIR_FAR:
        Serial.println("FAR");
        //sound file!
        tmrpcm.play("energeticSound.mp3");
        CircuitPlayground.clearPixels();
        for (int i = 0; i<10; i++)
        {
          CircuitPlayground.setPixelColor(i, 255, 0, 0);
        }
        break;
      default:
        Serial.println("NONE");
    }
  }
}
