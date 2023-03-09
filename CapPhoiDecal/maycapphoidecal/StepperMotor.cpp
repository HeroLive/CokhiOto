#include "Arduino.h"
#include "StepperMotor.h"
/*
   updateShiftRegister() - This function sets the latchPin to low, then calls the Arduino function 'shiftOut' to shift out contents of variable 'leds' in the shift register before putting the 'latchPin' high again.
*/

StepperMotor::StepperMotor(int p, int d, int en)
  : pin_pul(p), pin_dir(d), pin_en(en) {
  stepsPerUnit = 1;
  direction = 0;
  startDirection = LOW;
  positon = 0;
  last_step_time = 0;
}
void StepperMotor::setZero() {
  positon = 0;
}
void StepperMotor::setSpeed(double whatSpeed) {
  // step_delay = whatSpeed;
  step_delay = 1000L * 1000L / stepsPerUnit / whatSpeed;
}
void StepperMotor::setStepsPerUnit(double ppu) {
  stepsPerUnit = ppu;
}
void StepperMotor::setStartDirection(bool dir) {
  startDirection = dir;
}
void StepperMotor::setEnable(bool en) {
  digitalWrite(pin_en, en);
}
double StepperMotor::currentPosition() {
  return positon / stepsPerUnit;
}
void StepperMotor::moveTo(double absolute) {
  long target = absolute * stepsPerUnit;
  double _delay = 0.5 * step_delay - 5;
  boolean pul_status = LOW;
  if (positon == target) {
    return;
  } else {
    if (positon < target) {
      direction = HIGH;
    } else {
      direction = LOW;
    }
  }

  digitalWrite(pin_dir, startDirection ? direction : !direction);

  while (positon != target) {
    digitalWrite(pin_pul, HIGH);
    delayMicroseconds(_delay);
    digitalWrite(pin_pul, LOW);
    delayMicroseconds(_delay);
    positon = (positon < target) ? positon + 1 : positon - 1;
  }
}
