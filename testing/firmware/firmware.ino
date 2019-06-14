#include <Ticker.h>
#include <elapsedMillis.h>
Ticker tickerTimer;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}


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
unsigned int currentDelta;
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
void startDetecting(){
    tickerTimer.attach_ms(currentDelta, detect);
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
        tickerTimer.attach_ms(currentDelta, detect);
    }

    if(mode!=currentMode){
        currentMode = mode;
        clearRecordedDetections();
        if(mode==1){
            tickerTimer.attach_ms(currentDelta, detect);
        }
        else{
            tickerTimer.detach();
        }
        
    }
}

/*=========================================================================*/
/*
* Takes detection measurement then pushes <Detection> object to vector recordedDetections 
* @return none
*/
void detect(){
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
