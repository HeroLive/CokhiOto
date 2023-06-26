//https://www.instructables.com/How-to-Use-the-CNC-V4-Board-despite-Its-quirks/
#include <StepperMotor.h>
#define pulX 5
#define dirX 2
#define pulY 6
#define dirY 3
#define Button 9

StepperMotor stepperX(pulX, dirX);
StepperMotor stepperY(pulY, dirY);

float disPerRoundX = 360;                                                      // only round: 1.0 round, vitme T8-4mm: 4.0 mm, GT2 Pulley 16 Teeth: 16x2 = 32.0 mm
int microStepX = 1;                                                          //1: full step, 2: haft step, ...
float angleStepX = 1.8;                                                      //a step angle of 1.8 degrees with 200 steps per revolution
float stepsPerUnitX = (1 / disPerRoundX) * 360.0 * microStepX / angleStepX;  //steps/round or steps/mm ...
float speedX = 1000;    //chinh toc do X                                                        //set deg/s

float disPerRoundY = 360;                                                      // only round: 1.0 round, vitme T8-4mm: 4.0 mm, GT2 Pulley 16 Teeth: 16x2 = 32.0 mm
int microStepY = 1;                                                          //1: full step, 2: haft step, ...
float angleStepY = 1.8;                                                      //a step angle of 1.8 degrees with 200 steps per revolution
float stepsPerUnitY = (1 / disPerRoundY) * 360.0 * microStepY / angleStepY;  //steps/round or steps/mm ...
float speedY = 10;      //chinh toc do Y                                                     //set deg/s

float targetX = 0;
float targetY = 0;
float Xa = 0;
float Xb = 90; //Chinh goc X
float Ya = 0;
float Yb = 90; //chinh goc Y

bool M1 = false;
bool M2 = false;

void setup() {
  Serial.begin(115200);
  pinMode(Button, INPUT_PULLUP);

  stepperX.setStepsPerUnit(stepsPerUnitX);
  stepperX.setSpeed(speedX);
  stepperX.setStartDirection(LOW);

  stepperY.setStepsPerUnit(stepsPerUnitY);
  stepperY.setSpeed(speedY);
  stepperY.setStartDirection(LOW);

  delay(1000);
}

void loop() {
  if (digitalRead(Button) == 0 && !M1 && !M2) {
    while (digitalRead(Button) == 0)
      ;
    M1 = (targetX == Xa && targetY == Ya) ? true : false;
    M2 = (targetX == Xb && targetY == Yb) ? true : false;
  }
  if (M1) {
    Serial.println("Move to B");
    targetX = Xb;
    targetY = Yb;
    stepperX.moveTo(targetX);
    Serial.println(targetX);
    stepperY.moveTo(targetY);
    Serial.println(targetY);
    M1 = false;
  } else if (M2) {
    Serial.println("Move to A");
    targetX = Xa;
    targetY = Ya;
    stepperY.moveTo(targetY);
    Serial.println(targetY);
    stepperX.moveTo(targetX);
    Serial.println(targetX);
    M2 = false;
  }
}
