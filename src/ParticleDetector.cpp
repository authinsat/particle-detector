/*

    This is a library for a particle detector

    Functions include
    -      I2C communication
    -      

*/


#include "ParticleDetector.h"

int particleDeviceAddress = 0x08;

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
    //transmit to Particle Detector instructions for getDetection and desIndex
    Wire.begin();
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("get");
    byte bytes2[4];
    bytes2[0] = desIndex & 255;
    bytes2[1] = (desIndex >> 8) & 255;
    bytes2[2] = (desIndex >> 16) & 255;
    bytes2[3] = (desIndex >> 24) & 255;
    for(int iter = 0;iter<4;iter++){
        Wire.write(bytes2[iter]);
    }
    Wire.endTransmission();
    //request information on the requested detection
    Wire.requestFrom(particleDeviceAddress, 6);
    //read in the detection's time
    
    byte bytesTime[4];    
    for(int iter = 0; iter<4; iter++){
        bytesTime[iter] = Wire.read();
    }
    
    int eights = 8;
    unsigned long timeTr = bytesTime[0];
    for(int iter = 1; iter<4; iter++){
        timeTr = timeTr | ( (int)bytesTime[iter] << eights ); 
        eights+=8;
    }
   //read in the detection's magnitude
    byte bytesMag[2];
    bytesMag[0] = Wire.read();
    bytesMag[1] = Wire.read();
    uint16_t timeMg = bytesMag[0] | ( (int)bytesMag[1] << 8 );
    //reassemble into a detection object
    ParticleDetector::Detection myDetect;
    myDetect.time = timeTr;
    myDetect.magnitude = timeMg;
    return myDetect;
}

double  ParticleDetector::getTimeSinceLastDetection() {
    double myTime;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask0");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 8);
    
    byte bytesSince[4];
    for(int iter = 0; iter<4; iter++){
        bytesSince[iter] = Wire.read();
    }

    int eights = 8;
    myTime = bytesSince[0];
    for(int iter = 1; iter<4; iter++){
        myTime = (byte)myTime | ( (byte)bytesSince[iter] << eights ); 
        eights+=8;
    }
    return myTime;
}


unsigned int ParticleDetector::checkDelta(){
    unsigned int myDelta;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask2");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 4);
    byte bytesDelta[4];
    for(int iter = 0; iter<4; iter++){
        bytesDelta[iter] = Wire.read();
    }
    int eights = 8;
    myDelta = bytesDelta[0];
    for(int iter = 1; iter<4; iter++){
        myDelta = myDelta | ( (int)bytesDelta[iter] << eights ); 
        eights+=8;
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
    byte bytesPerMin[4];    
    for(int iter = 0; iter<4; iter++){
        bytesPerMin[iter] = Wire.read();
    }
    
    int eights = 8;
    myPerMin = bytesPerMin[0];
    for(int iter = 1; iter<4; iter++){
        myPerMin = myPerMin | ( (int)bytesPerMin[iter] << eights ); 
        eights+=8;
    }
    return myPerMin;
}


float  ParticleDetector::getAvgTimeBetweenDetections() {
    float myAvgTime;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask5");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 4);
    
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
