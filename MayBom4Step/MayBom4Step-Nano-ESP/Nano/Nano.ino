#include <Wire.h>
#include "StepperMotor.h"
#define PUL1_PIN 4
#define DIR1_PIN 13

#define I2C_SLAVE_ADDRESS 99


StepperMotor motor_01(PUL1_PIN, DIR1_PIN);

int state = 0;
bool debug = false;

bool M1 = false;
bool M2 = false;
bool M3 = false;
bool M4 = false;
//motor pulse value
long P[] = { 6400, 200, 300, 400 };
//motor frequency
long F[] = { 1000, 2000, 3000, 4000 };

void setup() {
  Wire.begin(I2C_SLAVE_ADDRESS);
  Serial.begin(9600);
  Serial.print("I am Slave with I2C ");
  Serial.println(I2C_SLAVE_ADDRESS);

  Wire.onRequest(requestEvents);
  Wire.onReceive(receiveEvents);
}

void loop() {
  if (M1) {
    if (debug) Serial.println(P[0]);
    motor_01.DRVI(P[0], F[0]);
    if (motor_01.getExeCompleteFlag()) {
      M1 = false;
    }
  }
}

void requestEvents()
{
  Serial.println(F("---> recieved request"));
  Serial.print(F("sending value : "));
  String msg = String(I2C_SLAVE_ADDRESS) + 'T';
  Serial.println(msg);
  Wire.write(msg.c_str());
}

void receiveEvents(int numBytes) {
  //  Serial.println(F("---> recieved events"));
  String data = "";
  while (Wire.available()) {
    data += (char)Wire.read();
  }
  Serial.print("Received data ");
  Serial.println(data);
}
