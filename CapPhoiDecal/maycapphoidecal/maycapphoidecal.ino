#include <math.h>
#include "StepperMotor.h"

#include <Servo.h>
// Create a new servo object:
Servo myservo;

//driver for the axis 1 - X
#define PUL1_PIN 2
#define DIR1_PIN 5
//driver for the axis 2 - Y
#define PUL2_PIN 3
#define DIR2_PIN 6
//enable pin for the axis 1,2
#define EN_PIN 8

#define X0_PIN 9     //map to X limit
#define Y0_PIN 10    //map to Y limit
#define Y2_PIN 11    // map to Z limit
#define Servo_PIN 4  // map to step Z
#define START_PIN 12     //map to SpnEn
#define Pump_PIN A3  //map to CoolEn

#define STATE_STARTUP 0
#define STATE_WAIT_START 1
#define STATE_CHECK_START 2
#define STATE_HOME_X 3
#define STATE_HOME_Y 4
#define STATE_MOVE_Y0 5
#define STATE_MOVE_Y2 6
#define STATE_MOVING 7

byte currentState = STATE_STARTUP;

StepperMotor stepperX(PUL1_PIN, DIR1_PIN, EN_PIN);

double calStepPerUnit(double angleStep, int microStep, double disPerRound) {
  double stepsPerUnit = (1.0 / disPerRound) * 360.0 * microStep / angleStep;  //steps/round or steps/mm ...
  return stepsPerUnit;
}

//unit is mm
double X0 = 0;
double X1 = 0.5;
double X2 = 0.75;
double Y0 = 0;
double Y1 = 30;
double Y2 = 200;
double speedX = 0.25;  //set 2 round/s
double speedY = 100.0;  //set 100 mm/s

//time ms
long start_time_delay = 3000;

//
double microStepX = 16;
double angleStepX = 1.8;
double microStepY = 16;
double angleStepY = 1.8;
double disPerRoundX = 1;
double disPerRoundY = 40;  //20x2=40mm
bool startDirX = HIGH;
bool startDirY = LOW;

double stepsPerUnitX = calStepPerUnit(angleStepX, microStepX, disPerRoundX);
double stepsPerUnitY = calStepPerUnit(angleStepY, microStepY, disPerRoundY);

//
double targetX = 0;
double targetY = 0;
double positionY = 0;
bool Y2_trigger = false;
long start_time_trigger = 0;

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(PUL1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(PUL2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  pinMode(Pump_PIN, OUTPUT);

  pinMode(X0_PIN, INPUT_PULLUP);
  pinMode(Y0_PIN, INPUT_PULLUP);
  pinMode(Y2_PIN, INPUT_PULLUP);
  pinMode(START_PIN, INPUT_PULLUP);


  stepperX.setStepsPerUnit(stepsPerUnitX);
  stepperX.setSpeed(speedX);
  stepperX.setStartDirection(startDirX);

  stepperX.setEnable(LOW);

  myservo.attach(Servo_PIN);
  pump("OFF");
  servo(0);
  delay(3000);
}

void loop() {
  updateState(currentState);
  // Serial.println(digitalRead(Y2_PIN));
}

void updateState(byte aState) {
  // do state change
  switch (aState) {
    case STATE_STARTUP:
      Serial.println("STATE_STARTUP");
      currentState = STATE_HOME_X;
      Serial.println("STATE_HOME_X");
      break;
    case STATE_HOME_X:
      runHomeX();
      stepperX.setZero();
      currentState = STATE_HOME_Y;
      Serial.println("STATE_HOME_Y");
      break;
    case STATE_HOME_Y:
      runHomeY();
      positionY = 0;
      currentState = STATE_WAIT_START;
      Serial.println("STATE_WAIT_START");
      break;
    case STATE_WAIT_START:
      pump("OFF");
      servo(0);
      if (digitalRead(START_PIN) == 0) {
        currentState = STATE_CHECK_START;
        Serial.println("STATE_CHECK_START");
        start_time_trigger = millis();
        Serial.print(start_time_trigger);
      }
      break;
    case STATE_CHECK_START:
      Serial.println(millis());
      if (digitalRead(START_PIN) == 1){
        currentState = STATE_WAIT_START;
        Serial.println("STATE_WAIT_START");
      }
      if (digitalRead(START_PIN) == 0 && (millis() - start_time_delay >= start_time_trigger) ) {
        currentState = STATE_MOVE_Y2;
        Serial.println("STATE_MOVE_Y2");
        Y2_trigger = true;
        pump("ON");
        servo(90);
        delay(500);
      }
      break;
    case STATE_MOVE_Y2:
      runStepperY(Y2);
      if (digitalRead(Y2_PIN) == 0) {
        currentState = STATE_MOVE_Y0;
        Serial.println("STATE_MOVE_Y0");
        Y2_trigger = false;
        delay(3000);
      }
      break;
    case STATE_MOVE_Y0:
      runStepperY(Y0);
      currentState = STATE_MOVING;
      Serial.println("STATE_MOVING");
      delay(1000);

      break;
    case STATE_MOVING:
      runStepperX(X1);
      delay(1000);
      runStepperY(Y1);
      delay(5000);
      runStepperX(X2);
      delay(4000);

      pump("OFF");
      delay(3000);
      servo(0);
      delay(3000);

      runStepperX(X1);
      delay(1000);
      runStepperY(Y0);
      delay(1000);
      runStepperX(X0);
      delay(1000);

      currentState = STATE_WAIT_START;
      Serial.println("STATE_WAIT_START");
      break;
  }
}

void runHomeX() {
  long positionX = stepperX.currentPosition();
  long target = -9999999;
  double step_delay = 1000L * 1000L / stepsPerUnitX / (0.5 * speedX);
  double _delay = 0.5 * step_delay - 5;
  bool pul_status = LOW;
  bool direction = LOW;
  if (positionX == target) {
    return;
  } else {
    if (positionX < target) {
      direction = HIGH;
    } else {
      direction = LOW;
    }
  }

  digitalWrite(DIR1_PIN, startDirX ? direction : !direction);

  while (positionY != target) {
    if (digitalRead(X0_PIN) == 0) {
      Serial.println("HOME X ON");
      break;
    }
    digitalWrite(PUL1_PIN, HIGH);
    delayMicroseconds(_delay);
    digitalWrite(PUL1_PIN, LOW);
    delayMicroseconds(_delay);
    positionX = (positionX < target) ? positionX + 1 : positionX - 1;
  }
}

void runHomeY() {
  long target = -9999999;
  double step_delay = 1000L * 1000L / stepsPerUnitY / (0.5 * speedY);
  double _delay = 0.5 * step_delay - 5;
  bool pul_status = LOW;
  bool direction = LOW;
  if (positionY == target) {
    return;
  } else {
    if (positionY < target) {
      direction = HIGH;
    } else {
      direction = LOW;
    }
  }

  digitalWrite(DIR2_PIN, startDirY ? direction : !direction);

  while (positionY != target) {
    if (digitalRead(Y0_PIN) == 0) {
      Serial.println("HOME Y ON");
      positionY = 0;
      break;
    }
    digitalWrite(PUL2_PIN, HIGH);
    delayMicroseconds(_delay);
    digitalWrite(PUL2_PIN, LOW);
    delayMicroseconds(_delay);
    positionY = (positionY < target) ? positionY + 1 : positionY - 1;
  }
}

void runStepperX(double pos) {
  stepperX.setStartDirection(startDirX);
  stepperX.setStepsPerUnit(stepsPerUnitX);
  stepperX.setSpeed(speedX);
  stepperX.moveTo(pos);
}

void runStepperY(double absolute) {
  long target = absolute * stepsPerUnitY;
  double step_delay = 1000L * 1000L / stepsPerUnitY / speedY;
  double _delay = 0.5 * step_delay - 5;
  bool pul_status = LOW;
  bool direction = LOW;
  if (positionY == target) {
    return;
  } else {
    if (positionY < target) {
      direction = HIGH;
    } else {
      direction = LOW;
    }
  }

  digitalWrite(DIR2_PIN, startDirY ? direction : !direction);

  while (positionY != target) {
    digitalWrite(PUL2_PIN, HIGH);
    delayMicroseconds(_delay);
    digitalWrite(PUL2_PIN, LOW);
    delayMicroseconds(_delay);
    positionY = (positionY < target) ? positionY + 1 : positionY - 1;
    if (digitalRead(Y2_PIN) == 0 && Y2_trigger) {
      currentState = STATE_MOVE_Y0;
      Serial.println("STATE_MOVE_Y0");
      Y2_trigger = false;
      delay(1000);
      break;
    }
  }
}

void pump(String s) {
  bool status = LOW;  //LOW: relay low level, HIGH: relay high level
  if (s == "ON") {
    digitalWrite(Pump_PIN, status);
  } else if (s == "OFF") {
    digitalWrite(Pump_PIN, !status);
  } else {
    digitalWrite(Pump_PIN, !status);
  }
}

void servo(int deg) {
  myservo.write(deg);
  // for (angle = 0; angle <= deg; angle += 1) {
  //   myservo.write(angle);
  //   delay(15);
  // }
}
