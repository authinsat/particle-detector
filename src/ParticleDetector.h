/*

    This is a library for a particle detector 
    
    The board uses I2C for communication.
    
    The board communicates with [?] I2C devices:

*/

#ifndef ParticleDetector_h
#define ParticleDetector_h

#pragma once

// System Includes
#include "xCore.h"
#include <Arduino.h>
#include "inttypes.h"
#include "Wire.h"
#include "xCore.h"
#include "math.h"
#include <elapsedMillis.h>

//NOTES for lily
//delta ms delta t individual interrupt
//clear recordedDetections between mode switches

/*=========================================================================*/
 
class ParticleDetector {
    public:
        /**
        * Constructor
        * Creates a new instance of ParticleDetector class.
        */  
        ParticleDetector(void);
       /*=========================================================================*/
        /*
        * Sets up the sensor
        * Call this in setup(), before reading any sensor data.
        * @return true if setup was successful.
        */
        bool setupSensor();
        /*=========================================================================*/
        /*
        * Sets Particle Sensor data collection mode
        * @param mode. uint8_t value of desired data collection mode. This library handles 0: Individual and 1: Flux.
        * @param delta. desired delta time between data collections in seconds if data collection mode is 1: Flux. Required if mode is 1: Flux. Default argument = 0.
        * @return none
        */
        void setDataMode(uint8_t mode, unsigned int delta=0);
        /*=========================================================================*/
        /*
        * Takes detection measurement then pushes <Detection> object to vector recordedDetections 
        * @return none
        */
        void detect();
        /*=========================================================================*/
        /*
        * Gets recorded detections within given time period (beginning - end)
        * @param beginning. Beginning of time period to get detections from.
        * @param end. End of time period to get detections from.
        * @return string displaying information about detections in given time period
        */
        String getDetectionsPeriod(time_t beginning, time_t end);
        /*=========================================================================*/
        /*
        * Gets Detection from detection's index in recordedDetections
        * @param desIndex. Desired index of detection
        * @return desired detection as <Detection> object
        */
        <Detection> getDetection(int desIndex);
        /*=========================================================================*/
        /*
        * Gets time since last detection (last Detection in recordedDetections vector)
        * @return time since last detection in seconds as a double
        */
        double getTimeSinceLastDetection();
        /*=========================================================================*/
        /*
        * Gets average detections per minute starting with first detection.
        * Skips detections in the current time's minute.
        * @return average as float
        */
        float getDetectionsPerMin();
        /*=========================================================================*/
        /*
        * Gets average time in seconds between detections.
        * Includes all detections in recordedDetections
        * @return average as float
        */
        float getAvgTimeBetweenDetections();
        /*=========================================================================*/
        /*
        * Gets average magnitude of all detections.
        * Includes all detections in recordedDetections
        * @return average as float
        */
        float getAvgMagnitude();
        /*=========================================================================*/
        /*
        * Clears the vector recordedDetections and returns a copy of that vector before clear
        * @return vector of Detection objects
        */
        std::vector<Detection> clearRecordedDetections();
        /*=========================================================================*/

    private:
        class Detection {
            public:
                /**
                * Constructor
                * Creates a new instance of ParticleDetector class.
                */ 
                ParticleDetector(void);
                /*=========================================================================*/
            private:
                uint8_t modeType; //what mode was measurement taken in
                time_t time; //time measurement was taken as time object
                float magnitude; //magnitude of detection

        };
        uint8_t currentMode; //current data collection mode
        unsigned int currentDelta; //current delta time if currentMode is 1:flux
        std::vector<Detection> recordedDetections; //dynamically sized vector of detection objects; new detections are added to this vector


};


#endif