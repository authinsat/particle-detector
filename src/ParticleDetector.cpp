/*

    This is a library for a particle detector

    Functions include
    -      I2C communication
    -      Set data collection mode (individual (True) or flux(False))


*/


#include "ParticleDetector.h"

class Detection; // forward declaration

ParticleDetector::ParticleDetector() {
    class Detection(){
    };
}

bool ParticleDetector::setupSensor() {
    return false;
}

String ParticleDetector::getDetectionsPeriod(time_t beginning, time_t end) {

    return "";
}
//Detection 
int ParticleDetector::getDetection(int desIndex){
}

void ParticleDetector::detect(){

}

//std::vector<Detection> 
int ParticleDetector::clearRecordedDetections(){

}

double  ParticleDetector::getTimeSinceLastDetection() {
    time_t current_time;
    current_time = time(NULL);
    //return difftime(recordedDetections.back().time,current_time);
}

float  ParticleDetector::getDetectionsPerMin() { 

    return 0;
}

float  ParticleDetector::getAvgTimeBetweenDetections() {

    return 0;
}

float  ParticleDetector::getAvgMagnitude() {

    return 0;
}

void   ParticleDetector::setDataMode(uint8_t mode, unsigned int delta=10){
    //Individual mode = 1
    //Flux mode = 0
    //int mode : requested mode
    //int currentMode : current mode; default on program start is flux (False)
    // if(mode!=currentMode){
    //     currentMode = mode;
    // }
}