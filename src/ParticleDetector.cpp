/*

    This is a library for a particle detector

    Functions include
    -      I2C communication
    -      Set data collection mode (individual (True) or flux(False))

*/


#include "ParticleDetector.h"
Ticker tickerTimer;

ParticleDetector::ParticleDetector() {
    currentMode=1;
    currentDelta=10;

}


bool ParticleDetector::setupSensor() {
    //test me
    if(true){
        //tickerTimer.attach_ms(currentDelta, [this](){ this->detect(); });
        rDetectSize = 0;
        rDetectCounter = 0;
        ParticleDetector::startDetecting();
        return true;
    }
    if(false){return false;}
}

//CHECKED
void ParticleDetector::startDetecting(){
    tickerTimer.attach_ms(currentDelta, [this](){ this->detect(); });
    timerr = 0;
}


ParticleDetector::Detection ParticleDetector::getDetection(int desIndex){
    return recordedDetections[desIndex];
}

void ParticleDetector::printDetection(int desIndex){
    ParticleDetector::Printer zero(getDetection(desIndex));
    Serial.println(zero);
}


//ParticleDetector::Printer zero(PD.getDetection(0));
//CHECKED
void ParticleDetector::detect(){
    ParticleDetector::Detection newDetection;
    newDetection.time = timerr;
    newDetection.magnitude = 4;
    rDetectCounter++;
    if(rDetectCounter>999){
        rDetectCounter = 0;
        rDetectSize--;
    }
    recordedDetections[rDetectCounter] = newDetection;
    if(rDetectSize>=1000){
        rDetectSize=1000;
    }
    else{
        rDetectSize++;
    }  
    
}

void ParticleDetector::clearRecordedDetections(){
    rDetectSize = 0;
    rDetectCounter = 0;
    
}

//CHECKED
double  ParticleDetector::getTimeSinceLastDetection() {
    return (timerr-recordedDetections[rDetectCounter].time);
}

//CHECKED
ParticleDetector::Detection * ParticleDetector::returnRecordedDetections(){
    return recordedDetections;
}

//CHECKED
void ParticleDetector::setDataMode(uint8_t mode, unsigned int delta){
    if(currentDelta!=delta){
        currentDelta = delta;
        ParticleDetector::clearRecordedDetections();
        tickerTimer.attach_ms(currentDelta, [this](){ this->detect(); });
    }

    if(mode!=currentMode){
        currentMode = mode;
        ParticleDetector::clearRecordedDetections();
        if(mode==1){
            tickerTimer.attach_ms(currentDelta, [this](){ this->detect(); });
        }
        else{
            tickerTimer.detach();
        }
        
    }
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


ParticleDetector::Printer::Printer(ParticleDetector::Detection detectObj){
    strcpy(theMessage, "Nothingness");
}


size_t ParticleDetector::Printer::printTo(Print& p) const{
  return p.print(theMessage);
}


