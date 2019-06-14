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
#include <vector>
#include <string>
#include <elapsedMillis.h>
#include <Ticker.h>
//#include <iostream>

//#include <TimerThree.h> //dependency

//NOTES for lily
//delta ms delta t individual interrupt
//A2 magnitude sensor
//setupSensor


/*=========================================================================*/
 
class ParticleDetector{
    private:
        /*=========================================================================*/
        /* struct container for Detection object 
        * time: milliseconds since last time startDetecting() was called
        * magnitude: magnitude of detection as recorded by sensor
        */
        //template<typename T>
         struct Detection{       //: public Printable  {
            unsigned long time;
            uint16_t magnitude;
            // char theMessage[25];
            // Printer(ParticleDetector::Detection detectObj);
            // size_t printTo(Print&) const;  
            //Detection operator<<();
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
        /* struct container for Detection object
        */
        unsigned int currentDelta; //current delta time if currentMode is 1:flux
       
        /*=========================================================================*/
        /* Dynamically sized vector of detection objects
        * new detections are added to this vector
        * vector may hold up to 1000 detections at a time
        * if container is at 1000 and detect() is called, the first detection in the vector is erased to make space
        */
        //template<typename T>
        Detection recordedDetections[1000]; 
        
        /*=========================================================================*/
        /*
        * Allows save of entire recordedDetections vector
        * Call this before changing data collection modes or clearing recordedDetections to save vector data
        * @return current recordedDetection
        */
        Detection * returnRecordedDetections();
        /*=========================================================================*/


    public:
        //friend std::ostream& operator<<(std::ostream&, ParticleDetector::Detection&);
        //template<typename T>
        friend std::ostream& operator<<(std::ostream& out, const ParticleDetector::Detection& ptr);
            //const ParticleDetector::Detection *ptr);

        /*
        * Constructor
        * Creates a new instance of ParticleDetector class.
        */  
        ParticleDetector();
       /*=========================================================================*/
        /*
        * Sets up the sensor
        * Call this in setup(), before reading any sensor data.
        * @return true if setup was successful
        */
        bool setupSensor();
        /*=========================================================================*/
        /*
        * Awakes the sensor to start taking detections
        * Called in setupSensor in flux mode with default delta time of 10
        * @return none
        */
        void startDetecting();
        /*=========================================================================*/
        /*
        * Sets Particle Sensor data collection mode
        * @param mode. uint8_t value of desired data collection mode. This library handles 0: Individual and 1: Flux.
        * @param delta. desired delta time between data collections in seconds if data collection mode is 1: Flux. Required if mode is 1: Flux. Default argument = 0.
        * @return none
        */
        void setDataMode(uint8_t mode, unsigned int delta=10);
        /*=========================================================================*/
        /*
        * Takes detection measurement then pushes <Detection> object to vector recordedDetections 
        * @return none
        */
        void detect();
        /*=========================================================================*/
        /*
        * Gets recorded detections within given time period (beginning - end)
        * @param beginning. Beginning of time period in ms to get detections from.
        * @param end. End of time period in ms to get detections from.
        * @return string displaying information about detections in given time period
        */
        std::string getDetectionsPeriod(unsigned long beginning, unsigned long end);
        /*=========================================================================*/
        /*
        * Gets Detection from detection's index in recordedDetections
        * @param desIndex. Desired index of detection
        * @return desired detection as <Detection> object
        */
        ParticleDetector::Detection getDetection(int desIndex);
        /*=========================================================================*/
        /*
        * Gets Detection from detection's index in recordedDetections
        * @param desIndex. Desired index of detection
        * @return desired detection as <Detection> object
        */
        void printDetection(int desIndex);
         /*=========================================================================*/
        /*
        * Gets time since last detection (last Detection in recordedDetections vector)
        * @return time since last detection in seconds as a double
        */
        double getTimeSinceLastDetection();
        /*=========================================================================*/
        /*
        * Gets average detections per minute starting with program runtime start
        * Skips detections in the current time's minute.
        * @return average as float
        */
        unsigned long getDetectionsPerMin();
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
        * Use returnRecordedDetections() to preserve recordedDetections before use
        * @return vector of Detection objects
        */
        void clearRecordedDetections();
        /*=========================================================================*/
        /*
        * returns the current Delta time
        * Use returnRecordedDetections() to preserve recordedDetections before use
        * @return currentDelta
        */
        unsigned int checkDelta();
        /*=========================================================================*/
        /*
        * returns the current data collection mode
        * @return currentMode
        */
        uint8_t checkMode();
        /*=========================================================================*/


  // char theMessage[25];

  // void Printer(ParticleDetector::Detection detectObj);
  // size_t printTo(Print&) const;  


};

#endif