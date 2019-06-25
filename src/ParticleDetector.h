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
#include "math.h"
#include <vector>
#include <string>
//#include <iostream>

//#include <TimerThree.h> //dependency

//NOTES for lily
//delta ms delta t individual interrupt
//A2 magnitude sensor
//setupSensor


//change 1000 / 999 in firmware to variable


/*=========================================================================*/

class ParticleDetector{
    private:
        /*=========================================================================*/
        /* struct container for Detection object 
        * time: milliseconds since last time startDetecting() was called
        * magnitude: magnitude of detection as recorded by sensor
        */

        //template<typename T>
         // struct Detection{       //: public Printable  {
         //    unsigned long time;
         //    uint16_t magnitude;
         //    // char theMessage[25];
         //    // Printer(ParticleDetector::Detection detectObj);
         //    // size_t printTo(Print&) const;  
         //    //Detection operator<<();
         //    };




        /*
        * Allows save of entire recordedDetections vector
        * Call this before changing data collection modes or clearing recordedDetections to save vector data
        * @return current recordedDetection
        */
        //Detection * returnRecordedDetections();
        /*=========================================================================*/


    public:
        struct Detection{
            unsigned long time;
            uint16_t magnitude;
        };
        //friend std::ostream& operator<<(std::ostream&, ParticleDetector::Detection&);
        //template<typename T>
        //friend std::ostream& operator<<(std::ostream& out, const ParticleDetector::Detection& ptr);
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
        * Gets recorded detections within given time period (beginning - end)
        * @param beginning. Beginning of time period in ms to get detections from.
        * @param end. End of time period in ms to get detections from.
        * @return string displaying information about detections in given time period
        */
        //std::string getDetectionsPeriod(unsigned long beginning, unsigned long end);
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
        //void printDetection(int desIndex);
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
        bool clearRecordedDetections();
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
        /*
        * changes the current data collection mode and/or delta
        * @return none
        */
        bool changeMode(uint8_t mode, unsigned int delta=10);
        /*=========================================================================*/


  // char theMessage[25];

  // void Printer(ParticleDetector::Detection detectObj);
  // size_t printTo(Print&) const;  
void receiveEvent(int howMany);

};


#endif