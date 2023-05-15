#include "StepperMotor.h"
#define DIR0_PIN D3   //D3
#define PUL0_PIN D4   //D4
#define HOME0_PIN D5  //D2
#define MAN_PIN D6    //D6
#define AUTO_PIN D7   // D7

StepperMotor stepper(PUL0_PIN, DIR0_PIN);

long stepsPerUnit = 40;  //pulse/mm
long target = 0;

//motor run
bool M1 = 0;  //ZRN - go home
bool M2 = 0;  //Go to position MAN
bool M3 = 0;  //Go as auto
bool M4 = 0;  //Go to 0 from auto
//motor pulse value
long P = 100;
//motor frequency
long F1 = 1000;
//motor creep frequency
long F2 = 100;
//circle
long N = 5;  //so lan chay auto
//delay motor
long T = 1000;
//position to go
long position = 200;  //chay den vi tri pos
long distance = 100;  //chay auto theo tung khoang


void setup() {
  Serial.begin(9600);
  pinMode(MAN_PIN, INPUT_PULLUP);
  pinMode(AUTO_PIN, INPUT_PULLUP);
  delay(2000);
  //set chay theo vi tri xac dinh
  // M2 = true;
  // target = position * stepsPerUnit;
  //set chay theo auto tung buoc
  M3 = true;
  target = 0;
}

void loop() {
  // Serial.println(digitalRead(HOME0_PIN));
  if (digitalRead(MAN_PIN) == 0) {
    M2 = true;
    target = position * stepsPerUnit;
  }
  if (digitalRead(AUTO_PIN) == 0) {
    M3 = true;
    target = 0;
  }
  // if (M1) {
  //   stepper.ZRN(F1, F2, HOME0_PIN);
  //   if (stepper.getExeCompleteFlag()) {
  //     M1 = false;
  //     delay(1000);
  //     Serial.println("go home");
  //   }
  // } else
  if (M2) {
    stepper.DRVA(target, F1);
    if (stepper.getExeCompleteFlag()) {
      M2 = false;
      Serial.println("go to set position");
    }
  } else if (M3) {
    stepper.DRVA(target, F1);
    if (stepper.getExeCompleteFlag()) {
      Serial.println(stepper.getCurrentPosition());
      if (!M4 && stepper.getCurrentPosition() == N * distance * stepsPerUnit) {
        Serial.println("auto go to 0");
        target = 0;
        M4 = true;
      } else if (stepper.getCurrentPosition() == 0 && M4) {
        M3 = false;
        Serial.println("auto go finish");
        M4 = false;
      } else {
        target = target + distance * stepsPerUnit;
      }
      delay(T);
    }
  }
}