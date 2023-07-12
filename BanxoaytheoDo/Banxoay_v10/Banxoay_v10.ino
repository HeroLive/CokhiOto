#include <math.h>
#include "StepperMotor.h"
#define pulX 11
#define dirX 12
#define enX 13

#define Run 5
#define Home 6
#define Home_Limit 7

StepperMotor stepperX(pulX, dirX);

bool debug = true;
bool MXA = false;
bool MH = false;

float disPerRoundX = 360;
float gearX = 1;
int microStepX = 10000;
float stepsPerUnitX = microStepX * gearX / disPerRoundX;
float speedX = 3000;  //chinh toc do X

long Xd = -30;
long Xa = 0;
long X = 0;


void setup() {
  Serial.begin(115200);

  pinMode(enX, OUTPUT);
  pinMode(Run, INPUT_PULLUP);
  pinMode(Home, INPUT_PULLUP);
  pinMode(Home_Limit, INPUT_PULLUP);

  digitalWrite(enX, LOW);
  delay(1000);
}

void loop() {
  // Serial.println(digitalRead(Button_Up));
  if (digitalRead(Run) == 0 && !MXA) {
    while (digitalRead(Run) == 0) {
    }
    MXA = true;
    Xa = Xa + Xd;
    X = Xa * stepsPerUnitX;
    Serial.print("XA: ");
    Serial.println(Xa);
  } else if (digitalRead(Home) == 0) {
    while (digitalRead(Home) == 0) {
    }
    MH = true;
    Xa = 0;
  }

  if (MXA) {
    stepperX.DRVA(X, speedX);
    if (stepperX.getExeCompleteFlag()) {
      MXA = false;
      if (debug) {
        Serial.print("X go to A: ");
        Serial.println(X);
        Serial.print("Position: ");
        Serial.println(stepperX.getCurrentPosition());
      }
    }
  }
  if (MH) {
    stepperX.ZRN(speedX, speedX / 15, Home_Limit);
    if (stepperX.getExeCompleteFlag()) {
      MH = false;
      if (debug) {
        Serial.print("Come to home ");
        Serial.println(stepperX.getCurrentPosition());
      }
    }
  }
}
