
#include "StepperMotor.h"
#define PUL_PIN 11
#define DIR_PIN 12
#define EN_PIN 13
#define RUN_PIN 10
#define ROT_PIN A0

StepperMotor stepperX(PUL_PIN, DIR_PIN);

float disPerRoundX = 360;
float gearX = 1;
int microStepX = 3200;
float stepsPerUnitX = microStepX * gearX / disPerRoundX;
float speedX = 1000;  //chinh toc do X

bool debug = true;
bool MXA = false;
bool MXB = false;

long Xa = 0;
long Xb = 180 * stepsPerUnitX;  //vi tri X den B

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(EN_PIN, OUTPUT);
  pinMode(RUN_PIN, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(RUN_PIN) == 0) {
    MXA = false;
    MXB = true;
  }

  if(!MXA && !MXB){
    speedX = 100* analogRead(ROT_PIN);
    Serial.println(speedX);
  }

  if (MXB) {
    stepperX.DRVA(Xb, speedX);
    if (stepperX.getExeCompleteFlag()) {
      MXB = false;
      MXA = true;
      if (debug) {
        Serial.print("X go to B - Position: ");        
        Serial.println(stepperX.getCurrentPosition());
        Serial.println(speedX);
      }
    }
  } else if (MXA) {
    stepperX.DRVA(Xa, speedX);
    if (stepperX.getExeCompleteFlag()) {
      MXA = false;
      if (debug) {
        Serial.print("X go to A - Position: ");
        Serial.println(stepperX.getCurrentPosition());
        Serial.println(speedX);
      }
    }
  }
}
