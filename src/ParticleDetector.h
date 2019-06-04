/*

    This is a library for a particle detector 
    
    The board uses I2C for communication.
    
    The board communicates with [?] I2C devices:

*/

// Include File Headers
#include "xCore.h"


#ifndef ParticleDetector_h
#define ParticleDetector_h

#pragma once

#include <Arduino.h>
#include "inttypes.h"
#include "Wire.h"
#include "xCore.h"
#include "math.h"

class ParticleDetector {
    public:
        ParticleDetector(void);
        void setDataMode(uint8_t);
        int getDetections();
        float getTimeSinceLastDetection();
        float getDetectionsPerMin();
        float getAvgTimeBetweenDetections();
        float getAvgMagnitude();

    private:
        int detections;
        uint8_t currentMode;

};


#endif