#include <math.h>
#include "StepperMotor.h"
#define pulX 5
#define dirX 2
#define pulY 6
#define dirY 3
#define Button_Down 9
#define Button_Stop 10
#define Button_Up 11
StepperMotor stepperX(pulX, dirX);
StepperMotor stepperY(pulY, dirY);

bool debug = true;
bool MXA = false;
bool MXB = false;
bool MYA = false;
bool MYB = false;

float disPerRoundX = 360;
float gearX = 1;
int microStepX = 1600;
float stepsPerUnitX = microStepX * gearX / disPerRoundX;
float speedX = 1000;  //chinh toc do X

float disPerRoundY = 360;
float gearY = 1; // chinh ti le hop so 1:5 nhap 5
int microStepY = 1600;
float stepsPerUnitY = microStepY * gearY / disPerRoundY;
float speedY = 150;  //chinh toc do X

long Xa = 0;
long Xb = 180 * stepsPerUnitX;  //vi tri X den B
long Ya = 0;
long Yb = 100 * stepsPerUnitY;  //vi tri Y den B


void setup() {
  Serial.begin(115200);
  pinMode(Button_Down, INPUT_PULLUP);
  pinMode(Button_Stop, INPUT_PULLUP);
  pinMode(Button_Up, INPUT_PULLUP);
  delay(1000);
}

void loop() {
  if (digitalRead(Button_Up) == 0) {
    MXA = false;
    MXB = true;
    MYA = false;
    MYB = false;
  } else if (digitalRead(Button_Down) == 0) {
    MXA = false;
    MXA = false;
    MYA = true;
    MYB = false;
  } else if (digitalRead(Button_Stop) == 0) {
    MXA = false;
    MXB = false;
    MYA = false;
    MYB = false;
  }

  if (MXB) {
    stepperX.DRVA(Xb, speedX);
    if (stepperX.getExeCompleteFlag()) {
      MXB = false;
      MYB = true;
      if (debug) {
        Serial.print("X go to B - Position: ");
        Serial.println(stepperX.getCurrentPosition());
      }
    }
  } else if (MXA) {
    stepperX.DRVA(Xa, speedX);
    if (stepperX.getExeCompleteFlag()) {
      MXA = false;
      if (debug) {
        Serial.print("X go to A - Position: ");
        Serial.println(stepperX.getCurrentPosition());
      }
    }
  } else if (MYA) {
    stepperY.DRVA(Ya, speedY);
    if (stepperY.getExeCompleteFlag()) {
      MYA = false;
      MXA = true;
      if (debug) {
        Serial.print("Y go to A - Position: ");
        Serial.println(stepperY.getCurrentPosition());
      }
    }
  } else if (MYB) {
    stepperY.DRVA(Yb, speedY);
    if (stepperY.getExeCompleteFlag()) {
      MYB = false;
      if (debug) {
        Serial.print("Y go to B - Position: ");
        Serial.println(stepperY.getCurrentPosition());
      }
    }
  }
}
