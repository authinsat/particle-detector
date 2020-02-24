#include <TimerThree.h>
#include <elapsedMillis.h>
#include "Wire.h"

/*=========================================================================*/
/*
* Timer for flux mode
*/
char whosAsking[5] = "done"; //instruction from master
int getDetect; //holds i from master for getDetection()
unsigned long beginning;
unsigned long ending;
uint8_t setMode;
unsigned int setDelta;
//void receiveEvent(int howMany);
//void requestEvent();
elapsedMillis timerr;
//volatile bool flagMe = false;
int ledMe = 13;



/*=========================================================================*/

void setup() {
   Serial.begin(115200);
   while(!Serial);
   // delay(2000);
   Serial.println("setup");
   //setupSensorCounterpart();
   Wire.begin(0x08);
   Wire.onReceive(receiveEvent);
   Wire.onRequest(requestEvent);
   Timer3.attachInterrupt(detect);
 

}

void loop() {
  delay(1000);
}

/*=========================================================================*/
/*------------------------------DETECTOR METHODS---------------------------*/
/*=========================================================================*/

/* struct container for Detection object 
* time: milliseconds since last time startDetecting() was called
* magnitude: magnitude of detection as recorded by sensor
*/
struct Detection{
  unsigned long time;
  uint16_t magnitude;
};

/*=========================================================================*/
//counter for recordedDetections array
int rDetectCounter; 
/*=========================================================================*/
//number of detections in recordedDetections
int rDetectSize; 
/*=========================================================================*/
//current data collection mode. This library handles 0: Individual and 1: Flux. Default is 1:Flux.
uint8_t currentMode; 
/*=========================================================================*/
//time in ms since last time startDetecting() was called

/*=========================================================================*/
// current delta time if currentMode is 1:flux
unsigned int currentDelta=10;
/*=========================================================================*/
/* Array of detection objects
* new detections are added to this vector
* array may hold up to 1000 detections at a time
* if container is at 1000 and detect() is called, detections overwrite starting at [0]
*/
Detection recordedDetections[1000];
/*=========================================================================*/
/*
* Awakes the sensor to start taking detections
* Called in setupSensor in flux mode with default delta time of 10
* @return none
*/
void setupSensorCounterpart(){
    currentMode=1;
    currentDelta=10;
    rDetectSize = 0;
    rDetectCounter = 0;
    startDetecting();
}
/*=========================================================================*/
/*
* Awakes the sensor to start taking detections
* Called in setupSensor in flux mode with default delta time of 10
* @return none
*/
void startDetecting(){
    Timer3.initialize(currentDelta);
    timerr = 0;
}
/*=========================================================================*/
/*
* Sets Particle Sensor data collection mode
* @param mode. uint8_t value of desired data collection mode. This library handles 0: Individual and 1: Flux.
* @param delta. desired delta time between data collections in seconds if data collection mode is 1: Flux. Required if mode is 1: Flux. Default argument = 0.
* @return none
*/
void setDataMode(uint8_t mode, unsigned int delta){
    if(currentDelta!=delta){
        currentDelta = delta;
        clearRecordedDetections();
        Timer3.initialize(currentDelta);
    }

    if(mode!=currentMode){
        currentMode = mode;
        clearRecordedDetections();
        if(mode==1){
           interrupts();
           Timer3.initialize(currentDelta);
           Timer3.attachInterrupt(detect);
        }
        else{
            noInterrupts();
        }
        
    }
}

/*=========================================================================*/
/*
* Takes detection measurement then pushes <Detection> object to vector recordedDetections 
* @return none
*/
void detect(){
    noInterrupts();
    Detection newDetection;
    newDetection.time = timerr;
    newDetection.magnitude = 4; 
    recordedDetections[rDetectCounter] = newDetection;
    if(rDetectSize>=1000){
        rDetectSize=1000;
    }
    else{
        rDetectSize++;
    }
    rDetectCounter++;
    if(rDetectCounter>999){
        rDetectCounter = 0;
    }
 
    interrupts();
}
/*=========================================================================*/
/*
* Clears the vector recordedDetections and returns a copy of that vector before clear
* Use returnRecordedDetections() to preserve recordedDetections before use
* @return vector of Detection objects
*/

void clearRecordedDetections(){
    rDetectSize = 0;
    rDetectCounter = 0;
    timerr = 0;
    
}
/*=========================================================================*/
/*
* 
*/

//---------------------------------------------------------------------------------//
void receiveEvent(int howMany) {
//  Serial.println("receiving");
  noInterrupts();
  
  char instruction[5];
  int holding = 0;
  byte bytes[2];
//  unsigned int twenty = 20;
//  byte bytes2[2];
//  bytes2[0] = twenty & 255;
//  bytes2[1] = (twenty >> 8) & 255;
//  Serial.println("twenty");
//  Serial.println(bytes2[0]);
//  Serial.println(bytes2[1]);

//  Serial.println("reading");
  bytes[0] = Wire.read();
//  Serial.println(bytes[0]);
  bytes[1] = Wire.read();
//  Serial.println(bytes[1]);
  
  setDelta = bytes[0] | ( (int)bytes[1] << 8 );
  Serial.println(setDelta); //unsigned int
  interrupts();
  }

 void requestEvent()
{
}
