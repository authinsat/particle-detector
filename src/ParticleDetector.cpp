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
        Wire.onReceive([this](){ this->receiveEvent(); });
        //
        //ParticleDetector::startDetecting();
        return true;
    }
    if(false){return false;}
}




ParticleDetector::Detection ParticleDetector::getDetection(int desIndex){
    return recordedDetections[desIndex];
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

//CHECKED
double  ParticleDetector::getTimeSinceLastDetection() {
    return (timerr-recordedDetections[rDetectCounter].time);
}

//CHECKED
ParticleDetector::Detection * ParticleDetector::returnRecordedDetections(){
    return recordedDetections;
}


//CHECKED
unsigned int ParticleDetector::checkDelta(){
    return currentDelta;
}

//CHECKED
uint8_t ParticleDetector::checkMode(){
    return currentMode;
}

//CHECKED
unsigned long  ParticleDetector::getDetectionsPerMin() {
    float minutesProRun;
    if(rDetectCounter==999){
        minutesProRun = (timerr-(recordedDetections[0].time))/60000.0;
    }
    else{
        minutesProRun = (timerr-(recordedDetections[rDetectCounter+1].time))/60000.0;
    }
    float averageRet = rDetectSize/minutesProRun;
    return averageRet;
}
//CHECKED
float  ParticleDetector::getAvgTimeBetweenDetections() {
    int cSize = rDetectSize;
    float counting = 0;
    for(int i=0; i<cSize-1; i++){
        if(i!=rDetectCounter){
            counting+=(recordedDetections[i+1].time-recordedDetections[i].time);
        }
    }
    float average = counting/cSize; 
    return average;
}

//CHECKED
float  ParticleDetector::getAvgMagnitude() {
    float cSize = rDetectSize;
    float counting = 0;
    for(int i=0; i<cSize; i++){
        counting+=recordedDetections[i].magnitude;
    }
    float average = counting/cSize; 
    return average;
}

std::string ParticleDetector::getDetectionsPeriod(unsigned long beginning, unsigned long end) {
    std::string returnString = "";
    int counter = 0;
    while(recordedDetections[counter].time>beginning){
        counter++;
    }
    while((recordedDetections[counter].time)<=end){
        char msg[200];
        unsigned long noUseCaseBecauseWHYWOULDYOU = recordedDetections[counter].time;
        sprintf(msg,"Detection: %d \nTime Recorded: %d \nMagnitude: %d \n",counter,noUseCaseBecauseWHYWOULDYOU,recordedDetections[counter].magnitude);
        returnString+=msg;
        counter++;
        }
    return returnString;
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


