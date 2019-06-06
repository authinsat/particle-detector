/*

    This is a library for a particle detector

    Functions include
    -      I2C communication
    -      Set data collection mode (individual (True) or flux(False))

*/


#include "ParticleDetector.h"

//struct Detection; // forward declaration

ParticleDetector::ParticleDetector() {
    currentMode=0;
    currentDelta=10;
}

bool ParticleDetector::setupSensor() {
    if(true){
        return true;
    }
    if(false){return false;}
}

ParticleDetector::Detection ParticleDetector::getDetection(int desIndex){
    return recordedDetections.at(desIndex);
}

void ParticleDetector::detect(){
    ParticleDetector::Detection newDetection;
    newDetection.modeType = currentMode;
    time_t current_time;
    current_time = time(NULL);
    newDetection.time = current_time; 
    newDetection.magnitude = 3.367;
    recordedDetections.push_back(newDetection);
}

std::vector<ParticleDetector::Detection> ParticleDetector::clearRecordedDetections(){
    std::vector<ParticleDetector::Detection> recordedDetectionsCopy = recordedDetections;
    recordedDetections.clear();
    return recordedDetectionsCopy;
}

double  ParticleDetector::getTimeSinceLastDetection() {
    time_t current_time;
    current_time = time(NULL);
    return difftime(recordedDetections.back().time,current_time);
}

void   ParticleDetector::setDataMode(uint8_t mode, unsigned int delta){
    if(mode!=currentMode){
         currentMode = mode;
    }
    currentDelta = delta;
}
unsigned int ParticleDetector::checkDelta(){
    return currentDelta;
}
uint8_t ParticleDetector::checkMode(){
    return currentMode;
}




float  ParticleDetector::getDetectionsPerMin() { 
    //float
    double startingTime = static_cast<double>(recordedDetections.at(0).time);
    double endTime = static_cast<double>(recordedDetections.back().time);
    double seconds = endTime-startingTime;
    double minutes = (seconds)/60;
    double secStartTime = 0;
    double secEndTime = 0;
    std::vector<double> holdAverages;
    double iterDetections=0;
    double counter = 0;
    floor(minutes);
    for(double i = 0; i<minutes; i++){
        double vectorVolCounter = 0;
        secEndTime+=60;
        while(secStartTime<=(recordedDetections.at(iterDetections).time-startingTime)<secEndTime){
            vectorVolCounter++;
            iterDetections++;
        }
        holdAverages.push_back(vectorVolCounter);
        secStartTime+=60;
    }
    double averageRet=0;
    for(int j=0; j < holdAverages.size(); j++){
        averageRet+=holdAverages[j];
    }
    averageRet/=holdAverages.size();
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
String ParticleDetector::getDetectionsPeriod(time_t beginning, time_t end) {

    return "";
} 