/*

    This is a library for a particle detector

    Functions include
    -      I2C communication
    -      

*/


#include "ParticleDetector.h"

int particleDeviceAddress = 8;


ParticleDetector::ParticleDetector() {

}


bool ParticleDetector::setupSensor() {
    //test me
    if(true){
        //tickerTimer.attach_ms(currentDelta, [this](){ this->detect(); });
        Wire.begin();

        //Wire.onReceive([this](){ this->receiveEvent(5); });
        //
        //ParticleDetector::startDetecting();
        return true;
    }
    if(false){return false;}
}


ParticleDetector::Detection ParticleDetector::getDetection(int desIndex){
    Serial.println("inside");
    unsigned long theTime;
    uint16_t theMagnitude;
    Wire.beginTransmission(particleDeviceAddress);
    char transmit[] = "get";
    Wire.write(transmit);
    Wire.write(desIndex);
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 6);
    //4 time 2 mag
    while (2 > Wire.available()) {
        theTime = Wire.read();
    }
    theMagnitude = Wire.read();
    ParticleDetector::Detection myDetect;
    myDetect.time = theTime;
    myDetect.magnitude = theMagnitude;
    return myDetect;
}

double  ParticleDetector::getTimeSinceLastDetection() {
    double myTime;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask0");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 8);
    while (Wire.available()) {
        myTime = Wire.read();
    }
    return myTime;
}


unsigned int ParticleDetector::checkDelta(){
    unsigned int myDelta;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask2");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 4);
    while (Wire.available()) { 
        myDelta = Wire.read();
    }
    return myDelta;
}


uint8_t ParticleDetector::checkMode(){
    uint8_t myMode;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask3");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 1);
    while (Wire.available()) { 
        myMode = Wire.read();
    }
    return myMode;
}

bool changeMode(uint8_t mode, unsigned int delta){
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("set");
    Wire.write(mode);
    Wire.write(delta);
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 1);
    bool checkMeMode = Wire.read();
    return checkMeMode;
}

bool ParticleDetector::clearRecordedDetections(){
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("clr");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 1);
    bool checkMeClear = Wire.read();
    return checkMeClear;
}

unsigned long  ParticleDetector::getDetectionsPerMin() {
    unsigned long myPerMin;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask4");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 4);
    while (Wire.available()) { 
        myPerMin = Wire.read();
    }
    return myPerMin;
}


float  ParticleDetector::getAvgTimeBetweenDetections() {
    float myAvgTime;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask5");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 4);
    while (Wire.available()) { 
        myAvgTime = Wire.read();
    }
    return myAvgTime;
}


float  ParticleDetector::getAvgMagnitude() {
    float myAvgMag;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask6");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 4);
    while (Wire.available()) { 
        myAvgMag = Wire.read();
    }
    return myAvgMag;
}



void ParticleDetector::receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}
