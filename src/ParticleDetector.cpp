/*

    This is a library for a particle detector

    Functions include
    -      I2C communication
    -      

*/


#include "ParticleDetector.h"



ParticleDetector::ParticleDetector() {
    // currentMode=1;
    // currentDelta=10;

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




// void ParticleDetector::printDetection(int desIndex){
//     ParticleDetector::Printer zero(getDetection(desIndex));
//     Serial.println(zero);
// }


//ParticleDetector::Printer zero(PD.getDetection(0));
//CHECKED


// void ParticleDetector::clearRecordedDetections(){
//     rDetectSize = 0;
//     rDetectCounter = 0;
    
// }
ParticleDetector::Detection ParticleDetector::getDetection(int desIndex){
    ParticleDetector::Detection c;
    Wire.beginTransmission(particleDeviceAddress);
    char transmit[] = "get";
    transmit+=desIndex;
    Wire.write(transmit);
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 8);
    while (Wire.available()) {
        c = Wire.read();
    }
    return c;
}

double  ParticleDetector::getTimeSinceLastDetection() {
    double c;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask0");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 8);
    while (Wire.available()) {
        c = Wire.read();
    }
    return c;
}


ParticleDetector::Detection * ParticleDetector::returnRecordedDetections(){
    ParticleDetector::Detection c[1000];
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask1");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 6000);
    while (Wire.available()) { 
        c = Wire.read();
    }
    return c;
}


unsigned int ParticleDetector::checkDelta(){
    unsigned int c;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask2");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 4);
    while (Wire.available()) { 
        c = Wire.read();
    }
    return c;
}


uint8_t ParticleDetector::checkMode(){
    uint8_t c;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask3");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 1);
    while (Wire.available()) { 
        c = Wire.read();
    }
    return c;
}


unsigned long  ParticleDetector::getDetectionsPerMin() {
    unsigned long c;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask4");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 4);
    while (Wire.available()) { 
        c = Wire.read();
    }
    return c;
}


float  ParticleDetector::getAvgTimeBetweenDetections() {
    float c;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask5");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 4);
    while (Wire.available()) { 
        c = Wire.read();
    }
    return c;
}


float  ParticleDetector::getAvgMagnitude() {
    float c;
    Wire.beginTransmission(particleDeviceAddress);
    Wire.write("ask6");
    Wire.endTransmission();
    Wire.requestFrom(particleDeviceAddress, 4);
    while (Wire.available()) { 
        c = Wire.read();
    }
    return c;
}



void ParticleDetector::receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}




//template<typename T>
// std::ostream& operator<<(std::ostream& out, const ParticleDetector::Detection& ptr){
//     //out << (*ptr).magnitude;
//     //cout << "test" <<ptr.magnitude;
//     std::string test = "test";
//     out << test;
//     // cout << ptr.magnitude;
//     //cout << ptr.*magnitude;   //member selection through pointer to member
//     //uint16_t test = (*ptr).magnitude;
//     //return out << ptr.magnitude;
//     return out;
// }

// std::ostream& operator<<(std::ostream& os, const ParticleDetector::Detection& a)
// {
//   return os << std::to_string(a.magnitude);

// ParticleDetector::Detection::Printer(){
//     strcpy(theMessage, "Nothingness2");
// }
// ParticleDetector::Detection::Printer(ParticleDetector::Detection detectObj){
//     strcpy(theMessage, "Nothingness");
// }


// size_t ParticleDetector::Detection::printTo(Print& p) const{
//   return p.print(theMessage);
// }



//------------------------------------------------------------------------
//void setDataMode(uint8_t mode, unsigned int delta){
//--------------------------------------------------------------------------
// std::string ParticleDetector::getDetectionsPeriod(unsigned long beginning, unsigned long end) {
//     std::string returnString = "";
//     int counter = 0;
//     while(recordedDetections[counter].time>beginning){
//         counter++;
//     }
//     while((recordedDetections[counter].time)<=end){
//         char msg[200];
//         unsigned long noUseCaseBecauseWHYWOULDYOU = recordedDetections[counter].time;
//         sprintf(msg,"Detection: %d \nTime Recorded: %d \nMagnitude: %d \n",counter,noUseCaseBecauseWHYWOULDYOU,recordedDetections[counter].magnitude);
//         returnString+=msg;
//         counter++;
//         }
//     return returnString;
// } 