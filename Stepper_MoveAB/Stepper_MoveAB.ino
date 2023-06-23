#include <math.h>
#include "StepperMotor.h"
#define PUL1_PIN 5
#define DIR1_PIN 2
#define LimitX 9
StepperMotor motor_01(PUL1_PIN, DIR1_PIN);

bool M1 = false;
long D200 = 200;                  // micro step
long D202 = 1;                     // gear
float D204 = D202 * D200 / 360.0;  // pulse/deg


long D0 = 200;  //motor frequency
long D2 = 0;     //Position A - deg
long D4 = 180;    //Position B - deg


long D10 = 0;          //target position
long D12 = D2 * D204;  //Position A - pulse
long D14 = D4 * D204;  //Position B - pulse


void setup() {
  Serial.begin(115200);
  Serial.println(D204);
  Serial.println(D12);
  Serial.println(D14);

  pinMode(LimitX, INPUT_PULLUP);

  delay(1000);
  D10 = D12;
}

void loop() {
  if (!M1) {
    D10 = D10 == D12 ? D14 : D12;
    M1 = true;
  }
  if (M1) {
    motor_01.DRVA(D10, D0);
    if (motor_01.getExeCompleteFlag()) {
      M1 = false;
      Serial.println(D10);
      delay(1000);
    }
  }
}
