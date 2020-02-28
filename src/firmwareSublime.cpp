#include <TimerThree.h>
#include <elapsedMillis.h>
#include "Wire.h"

/*=========================================================================*/
/*
* Timer for flux mode
*/
char whosAsking[5] = "done"; //instruction from master
unsigned int getDetect; //holds i from master for getDetection()
unsigned long beginning;
unsigned long ending;
uint8_t setMode;
unsigned int setDelta;
void receiveEvent(int howMany);
void requestEvent();
elapsedMillis timerr;
//volatile bool flagMe = false;
int ledMe = 13;



/*=========================================================================*/

void setup() 
{
   Serial.begin(115200);
   while(!Serial);
   Serial.println("setup");
   setupSensorCounterpart();
   Wire.begin(0x08);
   Wire.onReceive(receiveEvent);
   Wire.onRequest(requestEvent);
   Timer3.attachInterrupt(detect);
}

void loop() 
{
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
void setupSensorCounterpart()
{
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
void startDetecting()
{
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
void setDataMode(uint8_t mode, unsigned int delta)
{
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
void detect()
{
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

void clearRecordedDetections()
{
    rDetectSize = 0;
    rDetectCounter = 0;
    timerr = 0;
    
}
/*=========================================================================*/
/*
* 
*/

//---------------------------------------------------------------------------------//
void receiveEvent(int howMany) 
{
  noInterrupts();
  char instruction[5];
  int holding = 0;
  //read instruction from master
//*******************************************ask0??? ?
  if((strcmp(whosAsking,"done")==0)){
    while(Wire.available()>0) {  // loop through all the characters
      char c = Wire.read(); // receive byte as a character
      instruction[holding]=c;
      holding++;
    }
    /*--------------------clearRecordedDetections--------------------*/
    if((strcmp(holding,"clrd")) == 0){
      clearRecordedDetections();
      strcpy(whosAsking, "done");
    }
    else{
      strcpy(whosAsking,instruction);
    }
  }
  
  //proceed accordingly
  /*--------------------changeMode--------------------*/
    //*******************************************doesn't come out right, and no delta included
  else if((strcmp(whosAsking,"setm")) == 0){
    //set mode and delta
    setMode = Wire.read();
    byte bytes[4];
    bytes[0] = Wire.read();
    bytes[1] = Wire.read();
    bytes[2] = Wire.read();
    bytes[3] = Wire.read();
    setDelta = bytes[0] | ( (int)bytes[1] << 8 ) | ( (int)bytes[2] << 16 ) | ( (int)bytes[3] << 24 );
    setDataMode(setMode);
    if((strcmp(setMode,currentMode)) == 0){
      strcpy(whosAsking,"scss");
    }
    else{
      strcpy(whosAsking,"fail");
    }
    //strcpy(instruction, "done");
  }
  /*--------------------getDetection--------------------*/
  //*******************************************get the stuff + for loop example here
  else if((strcmp(whosAsking,"getd")) == 0){
    // which detection?
    byte bytes[4];
    bytes[0] = Wire.read();
    bytes[1] = Wire.read();
    bytes[2] = Wire.read();
    bytes[3] = Wire.read();
    getDetect = bytes[0] | ( (int)bytes[1] << 8 ) | ( (int)bytes[2] << 16 ) | ( (int)bytes[3] << 24 );

   }
    interrupts();
  }
//*******************************************end of receive event
void requestEvent()
{
  noInterrupts();
  /*--------------------getDetection--------------------*/
  if((strcmp(whosAsking,"getd")) == 0){
    unsigned long transferTime = recordedDetections[getDetect].time;
    uint16_t transferMag = recordedDetections[getDetect].magnitude;
  }
//*******************************************send the stuff + for loop example here (make 2 bitshift methods)
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
   }
/*--------------------getTimeSinceLastDetection--------------------*/
  else if((strcmp(whosAsking,"time")) == 0){
    double c;
    if(rDetectCounter>0){
      c = timerr-recordedDetections[rDetectCounter-1].time;
    }
    else{
      c = timerr-recordedDetections[999].time;
    }
//*******************************************bytes
    byte bytesSince[4];
    int eights = 0;
    for(int iter = 0; iter<4; iter++){
      bytesSince[iter] = ((byte)c >> eights) & 255;
      eights+=8;
    }
    for(int iter = 0;iter<4; iter++){
      Wire.write(bytesSince[iter]);
    }
    strcpy(whosAsking, "done");
  }
  /*--------------------checkDelta--------------------*/
  else if((strcmp(whosAsking,"dlta")) == 0){
   //checkDelta
   byte bytesDelta[4];
   unsigned int transferDelta = currentDelta;
   int eights = 0;
   for(int iter = 0; iter<4; iter++){
     bytesDelta[iter] = (transferDelta >> eights) & 255;
     eights+=8;
   }
   for(int iter = 0; iter<4; iter++){
     Wire.write(bytesDelta[iter]);
   }
   strcpy(whosAsking, "done");
  }
  /*--------------------checkMode--------------------*/
  else if((strcmp(whosAsking,"cmde")) == 0){
   //checkMode
   Wire.write(currentMode);
  }
  /*--------------------getDetectionsPerMinute--------------------*/
  else if((strcmp(whosAsking,"depm")) == 0){
   //getDetectionsPerMin
   float minutesProRun;
   if(rDetectCounter==999){
       minutesProRun = (timerr-(recordedDetections[0].time))/60000.0;
   }
   else{
       minutesProRun = (timerr-(recordedDetections[rDetectCounter+1].time))/60000.0;
   }
   float averageRet = rDetectSize/minutesProRun;
   byte averageRetBytes[8];
   int eights = 0;
   for(int iter = 0; iter<8; iter++){
     averageRetBytes[iter] = (averageRet >> eights) & 255;
     eights+=8;
   }
   for(int iter = 0; iter<8; iter++){
     Wire.write(averageRetBytes[iter]);
   }
   strcpy(whosAsking, "done");
  }
  /*--------------------getAvgTimeBetweenDetections--------------------*/
  else if(strcmp(whosAsking,"avgt") == 0){
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
   byte averageBWBytes[4];
   int eights = 0;
   for(int iter = 0; iter<4; iter++){
     averageBWBytes[iter] = (average >> eights) & 255;
     eights+=8;
   }
   for(int iter = 0; iter<4; iter++){
     Wire.write(averageBWBytes[iter]);
   }
   strcpy(whosAsking, "done");
  }
  /*--------------------getAvgMagnitude--------------------*/
  else if(strcmp(whosAsking,"avgm") == 0){
   float cSize = rDetectSize;
   float counting = 0;
   for(int i=0; i<cSize; i++){
       counting+=recordedDetections[i].magnitude;
   }
   float average = counting/cSize; 
   byte byteMagn[4];
   int eights = 0;
   for(int iter = 0; iter<4; iter++){
     byteMagn[iter] = (average >> eights) & 255;
     eights+=8;
   }
   for(int iter = 0; iter<4; iter++){
     Wire.write(byteMagn[iter]);
   }
   strcpy(whosAsking, "done");
 }
  /*--------------------success flag--------------------*/
  else if((strcmp(whosAsking,"scss")) == 0){
    Wire.write(true);
    strcpy(whosAsking, "done");
  }
  /*--------------------failure flag--------------------*/
  else if((strcmp(whosAsking,"fail")) == 0){
    Wire.write(false);
    strcpy(whosAsking, "done");
  }
  else{
    Wire.write(false);
  }
  interrupts();
}

