/*

    This is a library for a particle detector

    Functions include
    -      I2C communication
    -      Set data collection mode (individual (True) or flux(False))

*/


#include "ParticleDetector.h"
Ticker tickerTimer;

ParticleDetector::ParticleDetector() {
    currentMode=1;
    currentDelta=10;

}
//HERE_---------------_
bool ParticleDetector::setupSensor() {
    //test me
    if(true){
        //tickerTimer.attach_ms(currentDelta, [this](){ this->detect(); });
        rDetectSize = 0;
        rDetectCounter = 0;
        ParticleDetector::startDetecting();
        return true;
    }
    if(false){return false;}
}

void ParticleDetector::startDetecting(){
    tickerTimer.attach_ms(currentDelta, [this](){ this->detect(); });
    timerr = 0;
}

ParticleDetector::Detection ParticleDetector::getDetection(int desIndex){
    return recordedDetections[desIndex];
}

void ParticleDetector::detect(){
    // Serial.println("size of recordedDetections");
    // Serial.println(rDetectSize);
    // Serial.println("rDetectCounter");
    // Serial.println(rDetectCounter);
    // Serial.println("----------------------------------im detecting");
    ParticleDetector::Detection newDetection;
    newDetection.time = timerr; 
    newDetection.magnitude = 3.367;
    rDetectCounter++;
    if(rDetectCounter>20){
        rDetectCounter = 0;
        rDetectSize--;
        //Serial.println("greater than 20");
    }
    recordedDetections[rDetectCounter] = newDetection;
    if(rDetectSize>=20){
        rDetectSize=20;
    }
    else{
        rDetectSize++;
    }  
    
    //Serial.println("----------------------------------end of detect");
}

void ParticleDetector::clearRecordedDetections(){
    //std::vector<ParticleDetector::Detection> recordedDetectionsCopy = recordedDetections;
    rDetectSize = 0;
    rDetectCounter = 0;
    
}

double  ParticleDetector::getTimeSinceLastDetection() {
    unsigned long current_time;
    current_time = time(NULL);
    return (recordedDetections[rDetectSize].time,current_time);
}
ParticleDetector::Detection * ParticleDetector::returnRecordedDetections(){
    return recordedDetections;
}
void ParticleDetector::setDataMode(uint8_t mode, unsigned int delta){
    if(currentDelta!=delta){
        currentDelta = delta;
        ParticleDetector::clearRecordedDetections();
        tickerTimer.attach_ms(currentDelta, [this](){ this->detect(); });

    }
    
    if(mode!=currentMode){
        currentMode = mode;
        ParticleDetector::clearRecordedDetections();
        if(mode==1){
            tickerTimer.attach_ms(currentDelta, [this](){ this->detect(); });
        }
        else{
            tickerTimer.detach();
        }
        
    }
}
unsigned int ParticleDetector::checkDelta(){
    return currentDelta;
}
uint8_t ParticleDetector::checkMode(){
    return currentMode;
}

float  ParticleDetector::getDetectionsPerMin() {
    int perMin = millis()/60; 
    float averageRet = rDetectSize/perMin;
    return averageRet;
}

float  ParticleDetector::getAvgTimeBetweenDetections() {
    int cSize = rDetectSize-1;
    float counting = 0;
    for(int i=1; i<=cSize; i++){
        counting+=recordedDetections[i].time-recordedDetections[i-1].time;
    }
    float average = counting/cSize; 
    return average;
}

float  ParticleDetector::getAvgMagnitude() {
    int cSize = rDetectSize;
    float counting = 0;
    for(int i=0; i<=cSize; i++){
        counting+=recordedDetections[i].magnitude;
    }
    float average = counting/cSize; 
    return average;
}

std::string ParticleDetector::getDetectionsPeriod(unsigned long beginning, unsigned long end) {
    std::string returnString = "";
    int counter = 0;
    while(recordedDetections[counter].time>beginning){
        counter++;
    }
    while((recordedDetections[counter].time)<=end){
        char msg[200];
        unsigned long noUseCaseBecauseWHYWOULDYOU = recordedDetections[counter].time;
        sprintf(msg,"Detection: %d \nTime Recorded: %d \nMagnitude: %d \n",counter,noUseCaseBecauseWHYWOULDYOU,recordedDetections[counter].magnitude);
        returnString+=msg;
        counter++;
        }
    return returnString;
} 