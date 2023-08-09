
#include "StepperMotor.h"
#define PUL_PIN 11
#define DIR_PIN 12
#define EN_PIN 13
#define Run 10
#define Up 9
#define Down 8
#define ROT_PIN A0

StepperMotor stepperX(PUL_PIN, DIR_PIN);

float disPerRoundX = 360;
float gearX = 1;
int microStepX = 800;
float stepsPerUnitX = microStepX * gearX / disPerRoundX;
long speedX = 1000;  //chinh toc do X

bool debug = true;
bool MXA = false;
bool MXB = false;
long t = millis();
long t_delay = 100;  //thoi gian dung ms

long Xa = 0;
long Xb = 360 * stepsPerUnitX;  //vi tri X den B

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(Run, INPUT_PULLUP);
  pinMode(Up, INPUT_PULLUP);
  pinMode(Down, INPUT_PULLUP);

  pinMode(EN_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!MXA && !MXB) {

    /*
    ********chinh toc do bang bien tro***********
    speedX = 8 * long(analogRead(ROT_PIN));
    // Serial.println(analogRead(ROT_PIN));
    //***************************************
    */

    //*********chinh toc do bang nut nhan Up Down**********
    if (!digitalRead(Up)) {
      t = millis();
      while (!digitalRead(Up)) {
        if (millis() - t > 1000) {
          speedX++;
          delay(5);
          if (debug) {
            Serial.println(speedX);
          }
        }
      }
      speedX++;
      if (debug) {
        Serial.println(speedX);
      }
    }
    if (!digitalRead(Down)) {
      t = millis();
      while (!digitalRead(Down)) {
        if (millis() - t > 1000) {
          if (speedX > 0) {
            speedX--;
          }

          delay(5);
          if (debug) {
            Serial.println(speedX);
          }
        }
      }
      if (speedX > 0) {
        speedX--;
      }
      if (debug) {
        Serial.println(speedX);
      }
    }
    //***************************************
    if (!digitalRead(Run)) {
      t = millis();
      while (!digitalRead(Run)) {
        if (debug) {
          Serial.println("Press Run button ");
        }
      }
      if (millis() - t > 10) {
        MXA = false;
        MXB = true;
      }
    }
  } else {
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
        delay(t_delay);
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
}
