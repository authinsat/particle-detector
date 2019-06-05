/*

    This is a library for a particle detector

    Functions include
    -      I2C communication
    -      Set data collection mode (individual (True) or flux(False))


*/

#include "ParticleDetector.h"

ParticleDetector::ParticleDetector(void) {

}

int ParticleDetector::getDetections() {

    return 0;
}

float  ParticleDetector::getTimeSinceLastDetection() {

    return 0;
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

void   setDataMode(uint8_t mode){
    //Individual mode = 1
    //Flux mode = 0
    //int mode : requested mode
    //int currentMode : current mode; default on program start is flux (False)
    if(mode!=currentMode){
        currentMode = mode;
    }
}