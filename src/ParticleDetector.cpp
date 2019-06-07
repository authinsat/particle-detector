/*

    This is a library for a particle detector

    Functions include
    -      I2C communication
    -      Set data collection mode (individual (True) or flux(False))

*/


#include "ParticleDetector.h"


ParticleDetector::ParticleDetector() {
    currentMode=1;
    currentDelta=10;

}
//HERE_---------------_
bool ParticleDetector::setupSensor() {
    //test me
    if(true){
        tickerTimer.attach_ms(currentDelta, ParticleDetector::detect);
        ParticleDetector::startDetecting();
        return true;
    }
    if(false){return false;}
}

void ParticleDetector::startDetecting(){
    tickerTimer.attach_ms(currentDelta, ParticleDetector::detect);
    timerr = 0;
}

ParticleDetector::Detection ParticleDetector::getDetection(int desIndex){
    return recordedDetections.at(desIndex);
}

void ParticleDetector::detect(){
    ParticleDetector::Detection newDetection;
    newDetection.time = timerr; 
    newDetection.magnitude = 3.367;
    while(recordedDetections.size()>=3000){
        recordedDetections.erase(recordedDetections.begin());
    }
    recordedDetections.push_back(newDetection);
}

void ParticleDetector::clearRecordedDetections(){
    std::vector<ParticleDetector::Detection> recordedDetectionsCopy = recordedDetections;
    recordedDetections.clear();
}

double  ParticleDetector::getTimeSinceLastDetection() {
    unsigned long current_time;
    current_time = time(NULL);
    return difftime(recordedDetections.back().time,current_time);
}
std::vector<ParticleDetector::Detection> ParticleDetector::returnRecordedDetections(){
    return recordedDetections;
}
void ParticleDetector::setDataMode(uint8_t mode, unsigned int delta){
    if(currentDelta!=delta){
        currentDelta = delta;
        ParticleDetector::clearRecordedDetections();
        tickerTimer.attach_ms(currentDelta, ParticleDetector::detect);

    }
    
    if(mode!=currentMode){
        currentMode = mode;
        ParticleDetector::clearRecordedDetections();
        if(mode==1){
            tickerTimer.attach_ms(currentDelta, ParticleDetector::detect);
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
    float averageRet = recordedDetections.size()/perMin;
    return averageRet;
}

float  ParticleDetector::getAvgTimeBetweenDetections() {
    int cSize = recordedDetections.size()-1;
    float counting = 0;
    for(int i=1; i<=cSize; i++){
        counting+=recordedDetections.at(i).time-recordedDetections.at(i-1).time;
    }
    float average = counting/cSize; 
    return average;
}

float  ParticleDetector::getAvgMagnitude() {
    int cSize = recordedDetections.size();
    float counting = 0;
    for(int i=0; i<=cSize; i++){
        counting+=recordedDetections.at(i).magnitude;
    }
    float average = counting/cSize; 
    return average;
}

std::string ParticleDetector::getDetectionsPeriod(unsigned long beginning, unsigned long end) {
    std::string returnString = "";
    int counter = 0;
    while(recordedDetections.at(counter).time>beginning){
        counter++;
    }
    while((recordedDetections.at(counter).time)<=end){
        char msg[200];
        unsigned long noUseCaseBecauseWHYWOULDYOU = recordedDetections.at(counter).time;
        sprintf(msg,"Detection: %d \nTime Recorded: %d \nMagnitude: %d \n",counter,noUseCaseBecauseWHYWOULDYOU,recordedDetections.at(counter).magnitude);
        returnString+=msg;
        counter++;
        }
    return returnString;
} 