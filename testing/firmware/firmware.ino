#include <TimerThree.h>
#include <elapsedMillis.h>
#include "Wire.h"
#include "xCore.h"
#include <array>
/*=========================================================================*/
/*
* Timer for flux mode
*/
char whosAsking[5];
int getDetect;
unsigned long beginning;
unsigned long ending;
uint8_t setMode;
unsigned int setDelta;
void receiveEvent(int howMany);
void requestEvent();



/*=========================================================================*/

void setup() {
   Serial.begin(115200);
  // while(!Serial);
   delay(2000);
  // Serial.println("setup");
   Wire.begin(0x08);
   Wire.onReceive(receiveEvent);
   Wire.onRequest(requestEvent);
   pinMode(13, OUTPUT);
   Timer3.attachInterrupt(detect);
   startDetecting();

}

void loop() {
  // put your main code here, to run repeatedly:

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
 elapsedMillis timerr;
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
void setDataMode(uint8_t mode, unsigned int delta=10){
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
    rDetectCounter++;
    if(rDetectCounter>999){
        rDetectCounter = 0;
        rDetectSize--;
    }
    recordedDetections[rDetectCounter] = newDetection;
    if(rDetectSize>=1000){
        rDetectSize=1000;
    }
    else{
        rDetectSize++;
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
    
}
/*=========================================================================*/
/*
* 
*/
void receiveEvent(int howMany) {
  noInterrupts();
  char instruction[4];
  char c;
  int holding = 0;

  //read instruction from master
  while(Wire.available()>4) {  // loop through all but the last
    c = Wire.read(); // receive byte as a character
    instruction[holding]=c;
    holding++;
  }
  
  //proceed accordingly
  if(strcmp(instruction,"ask") == 0){
    int x = Wire.read();
    instruction[holding] = x;
    strcpy(whosAsking, instruction);
  }
  else if(strcmp(instruction,"set") == 0){
    strcpy(instruction, whosAsking);
    while (4 < Wire.available()){
      setMode = Wire.read();
     }
    setDelta = Wire.read();
  }
  //get detection
  else if(strcmp(instruction,"get") == 0){
    strcpy(whosAsking,"ask1");
    byte bytes[4];
    bytes[0] = Wire.read();
    bytes[1] = Wire.read();
    bytes[2] = Wire.read();
    bytes[3] = Wire.read();
    getDetect = bytes[0] | ( (int)bytes[1] << 8 ) | ( (int)bytes[2] << 16 ) | ( (int)bytes[3] << 24 );
    interrupts();
  }
}
void requestEvent(){
    noInterrupts();
    unsigned long transferTime = recordedDetections[getDetect].time;
    uint16_t transferMag = recordedDetections[getDetect].magnitude;
    
    byte bytesTime[4];
    bytesTime[0] = transferTime & 255;
    bytesTime[1] = (transferTime >> 8) & 255;
    bytesTime[2] = (transferTime >> 16) & 255;
    bytesTime[3] = (transferTime >> 24) & 255;
    Wire.write(bytesTime[0]);
    Wire.write(bytesTime[1]);
    Wire.write(bytesTime[2]);
    Wire.write(bytesTime[3]);
    
    byte bytesMag[2];
    bytesMag[0] = transferMag & 255;
    bytesMag[1] = (transferMag >> 8) & 255;
    Wire.write(bytesMag[0]);
    Wire.write(bytesMag[1]);

//  for(int g = 0;g<5;g++){
//    Wire.write(whosAsking[g]);
  //}
//    Wire.write("w");
//    Wire.write("w");
//    Wire.write("w");
//    Wire.write("w");
    interrupts();
  //getDetect = 900;
  //-----------------------------

  // }
  //-----------------------------
  
//  if(!(whosAsking=="ask0")){
//    //getTimeSinceLastDetection
//    double c = timerr-recordedDetections[rDetectCounter].time;
//    Wire.write(static_cast<byte>(c));
//  }
//  else if(!(whosAsking=="ask1")){
//    //getDetection
//    //Wire.write(recordedDetections[getDetect].time);
//    //Wire.write(recordedDetections[getDetect].magnitude);
//    //getDetect = 567;
//    }
    
//  else if(!(whosAsking=="ask2")){
//    //checkDelta
//    Wire.write(currentDelta);
//  }
//  else if(!(whosAsking=="ask3")){
//    //checkMode
//    Wire.write(currentMode);
//  }
//  else if(!(whosAsking=="ask4")){
//    //getDetectionsPerMin
//    float minutesProRun;
//    if(rDetectCounter==999){
//        minutesProRun = (timerr-(recordedDetections[0].time))/60000.0;
//    }
//    else{
//        minutesProRun = (timerr-(recordedDetections[rDetectCounter+1].time))/60000.0;
//    }
//    float averageRet = rDetectSize/minutesProRun;
//    Wire.write(static_cast<byte>(averageRet));
//  }
//  else if(!(whosAsking=="ask5")){
//    //getAvgTimeBetweenDetections
//    int cSize = rDetectSize;
//    float counting = 0;
//    for(int i=0; i<cSize-1; i++){
//        if(i!=rDetectCounter){
//            counting+=(recordedDetections[i+1].time-recordedDetections[i].time);
//        }
//    }
//    float average = counting/cSize; 
//    Wire.write(static_cast<byte>(average));
//  }
//  else if(!(whosAsking=="ask6")){
//    //getAvgMagnitude
//    float cSize = rDetectSize;
//    float counting = 0;
//    for(int i=0; i<cSize; i++){
//        counting+=recordedDetections[i].magnitude;
//    }
//    float average = counting/cSize; 
//    Wire.write(static_cast<byte>(average));
//  }
//  else if(!(whosAsking=="set")){
//    setDataMode(setMode,setDelta);
//    Wire.write(true);
//  }
//  else if(!(whosAsking=="clr")){
//    clearRecordedDetections();
//    Wire.write(true);
//  }
//  else{
//    Wire.write(false);
//  }

}
