#include <AccelStepper.h>
// Define a stepper and the pins it will use
int stepPin = 2;
int dirPin = 5;
int en = 8;
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

// This defines the analog input pin for reading the control voltage
// Tested with a 10k linear pot between 5v and GND

int curStep = 20000;
void setup()
{
  Serial.begin(9600);
  pinMode(en, OUTPUT);
  digitalWrite(en, LOW);
  stepper.setSpeed(100);
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
}

void loop()
{
  stepper.runToNewPosition(curStep);
  delay(1000);
  stepper.runToNewPosition(0);
  delay(1000);
}
