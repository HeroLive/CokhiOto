#include <math.h>
#include "StepperMotor.h"
#define pulX 5
#define dirX 2
#define pulY 6
#define dirY 3
#define Button_Up A3    //D3 - CoolEn
#define Button_Stop A2  //D2 - Resume
#define Button_Down A1  //D1 - Hold

#define LimitA 10  //Y+ Y-
#define LimitB 11  //Z+ Z-
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
float speedX = 1400;  //chinh toc do X

float disPerRoundY = 360;
float gearY = 1;  // chinh ti le hop so 1:5 nhap 5
int microStepY = 1600;
float stepsPerUnitY = microStepY * gearY / disPerRoundY;
float speedY = 100;  //chinh toc do Y

long Xa = 0;
long Xb = 270 * stepsPerUnitX;  //vi tri X den B
long Ya = 0;
long Yb = 300 * stepsPerUnitY;  //Khoang chay Y tu A-B


void setup() {
  Serial.begin(115200);
  pinMode(Button_Down, INPUT_PULLUP);
  // pinMode(Button_Stop, INPUT_PULLUP);
  pinMode(Button_Up, INPUT_PULLUP);
  pinMode(LimitA, INPUT_PULLUP);
  pinMode(LimitB, INPUT_PULLUP);
  delay(1000);
}

void loop() {
  // Serial.println(digitalRead(Button_Up));
  if (analogRead(Button_Up) > 1000) {
    MXA = false;
    MXB = true;
    MYA = false;
    MYB = false;
  } else if (analogRead(Button_Down) > 1000) {
    MXA = false;
    MXA = false;
    MYA = true;
    MYB = false;
  } else if (analogRead(Button_Stop) > 1000) {
    MXA = false;
    MXB = false;
    MYA = false;
    MYB = false;
  } else if (MYB && digitalRead(LimitB) == 0) {
    MXA = false;
    MXB = false;
    MYA = false;
    MYB = false;
  } else if (MYA && digitalRead(LimitA) == 0) {
    MXA = true;
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
  } else if (MYA && digitalRead(LimitA) == 1) {
    stepperY.DRVI(-Yb, speedY);
    if (stepperY.getExeCompleteFlag()) {
      MYA = false;
      MXA = true;
      if (debug) {
        Serial.print("Y go to A - Position: ");
        Serial.println(stepperY.getCurrentPosition());
      }
    }
  } else if (MYB) {
    stepperY.DRVI(Yb, speedY);
    if (stepperY.getExeCompleteFlag()) {
      MYB = false;
      if (debug) {
        Serial.print("Y go to B - Position: ");
        Serial.println(stepperY.getCurrentPosition());
      }
    }
  }
}
