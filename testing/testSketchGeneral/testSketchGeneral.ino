
#include <ParticleDetector.h>
#include <Ticker.h>

ParticleDetector PD;
Ticker debugTimer;
Ticker doub;
//ParticleDetector::Printer zero(PD.getDetection(0));

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("hellow");
  //Serial.println(PD.getDetection(0));
  //PD.printDetection(0);
  //debugTimer.attach(6,debugMe);

  
  PD.setupSensor();
  //PD.tickerTimer.attach_ms(10,testMe);
 
  doub.attach(3,debugMe);
  //PD.detect();
  //uint8_t nD = 1;
  //PD.setDataMode(nD);
  //Serial.println(PD.checkDelta());
  //PD.setDataMode(nD,15);
  //Serial.println(PD.checkDelta());

}

void loop() {
  //Serial.println(PD.getDetectionsPerMin());
  // put your main code here, to run repeatedly:

}
void debugMe(){
  
  Serial.println("detection [0]");
  //Serial<<(PD.getDetection(0));
  //Serial.println("time since");
  //Serial.println(PD.getTimeSinceLastDetection());
  
}
