// Wire Master Writer
// by devyte
// based on the example of the same name by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// This example code is in the public domain.


#include <Wire.h>
#include <PolledTimeout.h>

#define SDA_PIN 2
#define SCL_PIN 14
const int16_t I2C_MASTER = 0x42;
const int16_t I2C_SLAVE = 0x08;
byte error;
unsigned int setDelta = 100;
void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(1000);
  Wire.begin(SDA_PIN, SCL_PIN); // join i2c bus (address optional for master)
  check_if_exist_I2C();
}

byte x = 0;

void loop() {
  using periodic = esp8266::polledTimeout::periodicMs;
  static periodic nextPing(1000);
  delay(1000);
  if (nextPing) {
    Wire.beginTransmission(I2C_SLAVE); // transmit to device #8
    byte bytes2[2];
    bytes2[0] = setDelta & 255;
    bytes2[1] = (setDelta >> 8) & 255;
    Wire.write(bytes2[0]);        // sends five bytes
    Wire.write(bytes2[1]);
    error = Wire.endTransmission();
    Serial.print("ir: ");
    Serial.println(error);
    setDelta++;
  }
}

bool check_if_exist_I2C() {
  Serial.println("check all the addresses");
  byte error, address;
  int nDevices;
  bool found = false;
  nDevices = 0;
  for (address = 1; address < 127; address++ )  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    //Serial.println(address);
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    Serial.print("  er: ");
    Serial.println(error);

    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      found = true;

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  } //for loop
  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("**********************************\n");
  //delay(1000);           // wait 1 seconds for next scan, did not find it necessary
  return found;
}
