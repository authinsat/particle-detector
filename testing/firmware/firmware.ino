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
elapsedMillis timerr;
volatile bool flagMe = false;
int ledMe = 13;



/*=========================================================================*/

void setup() {
   Serial.begin(115200);
  // while(!Serial);
   delay(2000);
   Serial.println("setup");
   Wire.begin(0x08);
   Wire.onReceive(receiveEvent);
   Wire.onRequest(requestEvent);
   pinMode(ledMe, OUTPUT);
   digitalWrite(ledMe,HIGH);
   delay(2000);
   digitalWrite(ledMe,LOW);
   Timer3.attachInterrupt(detect);
   startDetecting();
 

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
  //flagMe = true;
  //noInterrupts();
  char instruction[4];
  char c;
  strcpy(whosAsking,"");
  int holding = 0;
  //read instruction from master
  while(Wire.available()>4) {  // loop through all but the last
    c = Wire.read(); // receive byte as a character
    instruction[holding]=c;
    holding++;
  }
  
  //proceed accordingly
  if((strcmp(instruction,"ask")) == 0){
    int x = Wire.read();
    instruction[holding] = x;
    strcpy(whosAsking, instruction);
  }
  else if((strcmp(instruction,"set")) == 0){
    strcpy(instruction, whosAsking);
    setMode = Wire.read();
    byte bytes[2];
    bytes[0] = Wire.read();
    bytes[1] = Wire.read();
    setDelta = bytes[0] | ( (int)bytes[1] << 8 );
    setDataMode(setMode,setDelta);
  }
  //get detection
  else if((strcmp(instruction,"get")) == 0){
    strcpy(whosAsking,"ask1");
    byte bytes[4];
    bytes[0] = Wire.read();
    bytes[1] = Wire.read();
    bytes[2] = Wire.read();
    bytes[3] = Wire.read();
    getDetect = bytes[0] | ( (int)bytes[1] << 8 ) | ( (int)bytes[2] << 16 ) | ( (int)bytes[3] << 24 );
    interrupts();
  }
  else if((strcmp(instruction,"clr")) == 0){
    //clear recordedDetections
    noInterrupts();
    clearRecordedDetections();
  }
 // interrupts();
}

//---------------------------------------------------------------------------------//
void requestEvent(){
   // noInterrupts();

  if((strcmp(whosAsking,"ask0")) == 0){
    //getTimeSinceLastDetection
    double c;
    if(rDetectCounter>0){
      c = timerr-recordedDetections[rDetectCounter-1].time;
    }
    else{
      c = timerr-recordedDetections[999].time;
    }
    byte bytesSince[4];
    int eights = 0;
    for(int iter = 0; iter<4; iter++){
      bytesSince[iter] = ((byte)c >> eights) & 255;
      eights+=8;
    }
    for(int iter = 0;iter<4; iter++){
      Wire.write(bytesSince[iter]);
    }
  }
  else if((strcmp(whosAsking,"ask1")) == 0){
    //getDetection
    unsigned long transferTime = recordedDetections[getDetect].time;
    uint16_t transferMag = recordedDetections[getDetect].magnitude;

    
    byte bytesTime[4];
    int eights = 0;
    for(int iter = 0; iter<4; iter++){
      bytesTime[iter] = (transferTime >> eights) & 255;
      eights+=8;
    }
    for(int iter = 0; iter<4; iter++){
      Wire.write(bytesTime[iter]);
    }
    
    byte bytesMag[2];
    bytesMag[0] = transferMag & 255;
    bytesMag[1] = (transferMag >> 8) & 255;
    Wire.write(bytesMag[0]);
    Wire.write(bytesMag[1]);

//
//    eights = 8;
//    unsigned long timeTr = bytesTime[0];
//    for(int iter = 1; iter<4; iter++){
//        timeTr = timeTr | ( (int)bytesTime[iter] << eights ); 
//        eights+=8;
//    }
//
//
//    uint16_t timeMg = bytesMag[0] | ( (int)bytesMag[1] << 8 );
//
//
//    Serial.println(timeTr);
//    Serial.println(timeMg);





   }
   
 else if((strcmp(whosAsking,"ask2")) == 0){
   //checkDelta
   byte bytesDelta[4];
   unsigned int transferDelta = currentDelta;
   bytesDelta[0] = transferDelta & 255;
   bytesDelta[1] = (transferDelta >> 8) & 255;
   Wire.write(bytesDelta[0]);
   Wire.write(bytesDelta[1]);

 }
 else if((strcmp(whosAsking,"ask3")) == 0){
   //checkMode
   Wire.write(currentMode);
 }
 else if((strcmp(whosAsking,"ask4")) == 0){
   //getDetectionsPerMin
   float minutesProRun;
   if(rDetectCounter==999){
       minutesProRun = (timerr-(recordedDetections[0].time))/60000.0;
   }
   else{
       minutesProRun = (timerr-(recordedDetections[rDetectCounter+1].time))/60000.0;
   }
   float averageRet = rDetectSize/minutesProRun;
   byte byteAsk4[4];
   byteAsk4[0] = (byte)averageRet & 255;
   byteAsk4[1] = ((byte)averageRet >> 8) & 255;
   byteAsk4[2] = ((byte)averageRet >> 16) & 255;
   byteAsk4[3] = ((byte)averageRet >> 24) & 255;
   Wire.write(byteAsk4[0]);
   Wire.write(byteAsk4[1]);
   Wire.write(byteAsk4[2]);
   Wire.write(byteAsk4[3]);
   

 }
 else if(strcmp(whosAsking,"ask5") == 0){
   noInterrupts();
   //getAvgTimeBetweenDetections
   int cSize = rDetectSize;
   float counting = 0;
   for(int i=0; i<cSize-1; i++){
       if(i!=rDetectCounter){
           counting+=(recordedDetections[i+1].time-recordedDetections[i].time);
       }
   }
   float average = counting/cSize; 
   byte byteAsk5[4];
   byteAsk5[0] = (byte)average & 255;
   byteAsk5[1] = ((byte)average >> 8) & 255;
   byteAsk5[2] = ((byte)average >> 16) & 255;
   byteAsk5[3] = ((byte)average >> 24) & 255;
   Wire.write(byteAsk5[0]);
   Wire.write(byteAsk5[1]);
   Wire.write(byteAsk5[2]);
   Wire.write(byteAsk5[3]);
   interrupts();
 }
 else if(strcmp(whosAsking,"ask6") == 0){
   //getAvgMagnitude
   float cSize = rDetectSize;
   float counting = 0;
   for(int i=0; i<cSize; i++){
       counting+=recordedDetections[i].magnitude;
   }
     float average = counting/cSize; 
     byte byteAsk6[4];
   byteAsk6[0] = (byte)average & 255;
   byteAsk6[1] = ((byte)average >> 8) & 255;
   byteAsk6[2] = ((byte)average >> 16) & 255;
   byteAsk6[3] = ((byte)average >> 24) & 255;
   Wire.write(byteAsk6[0]);
   Wire.write(byteAsk6[1]);
   Wire.write(byteAsk6[2]);
   Wire.write(byteAsk6[3]);
 }
 else if(!(whosAsking=="set")){
   if(setMode==currentMode && setDelta==currentDelta){
     Wire.write(true);
   }
   else{
     Wire.write(false);
   }
 }
 else if(!(whosAsking=="clr")){
   if(rDetectSize==0){
     Wire.write(true);
    }
    else{
     Wire.write(false);
    }
 }
 else{
   Wire.write(false);
 }
  //interrupts();
}

void callDuck(){
  Serial.println(recordedDetections[0].time);
  Serial.println(recordedDetections[0].magnitude);
  Serial.println(recordedDetections[100].time);
  Serial.println(recordedDetections[100].magnitude);
  
}
