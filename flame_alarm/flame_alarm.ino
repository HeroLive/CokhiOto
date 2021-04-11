/*
   Alarm fire

*/
#include <Servo.h>
Servo servo;

#define STATE_STARTUP 0
#define STATE_TRACKING 1
#define STATE_FIRE 2
#define STATE_CHECKFIRE 3
#define STATE_PUMP 4
byte currentState = STATE_STARTUP;

int flameD0 = 4;
int btStop = 5;
int tempPin = A0;
int buzzer = 8;
int servoPin = 9;
int pumpPin = 10;

//for temp ntc b3950
int Vo;
float R1 = 10000;
float logR2, R2, T, temp;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float tempFire = 40;
//for servo
int servoPos1 = 0;
int servoPos2 = 90;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  servo.attach(servoPin);

  pinMode(flameD0, INPUT);
  pinMode(btStop, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(servoPin, OUTPUT);
  pinMode(tempPin, INPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(pumpPin, HIGH);
  servo.write(servoPos1);

  updateState(STATE_TRACKING);
}

void loop() {
  // put your main code here, to run repeatedly:

}

// ------state machine---------------------
void updateState(byte aState)
{
  if (aState == currentState)
  {
    return;
  }

  // do state change
  switch (aState)
  {
    case STATE_STARTUP:
      Serial.println("STATE_STARTUP");
      break;
    case STATE_TRACKING:
      Serial.println("STATE_TRACKING");
      tracking();
      break;
    case STATE_FIRE:
      Serial.println("STATE_FIRE");
      fire();
      break;
    case STATE_CHECKFIRE:
      Serial.println("STATE_CHECKFIRE");
      checkfire();
      break;
    case STATE_PUMP:
      Serial.println("STATE_PUMP");
      pump();
      break;
  }

  currentState = aState;
}
//--------------------------------
void tracking() {
  digitalWrite(buzzer, LOW);
  servo.write(servoPos1);
  while (true) {
    if (digitalRead(flameD0) == 1) {
      updateState(STATE_FIRE);
      break;
    }
  }
}
void fire() {
  long t = millis();
  while (millis() - t < 3000) {}
  temp = get_temp();
  Serial.print(digitalRead(flameD0));
  Serial.print("\t");
  Serial.println(temp);
  if ( digitalRead(flameD0) == 1) {
    servo.write(servoPos2);
    digitalWrite(buzzer, HIGH);
    delay(1000);
    updateState(STATE_CHECKFIRE);
  } else {
    updateState(STATE_TRACKING);
  }
  if (temp > tempFire)  {
    updateState(STATE_PUMP);
  }
}

void checkfire() {
  long t = millis();
  while (millis() - t < 10000) {
    temp = get_temp();

    if (temp > tempFire)  {
      Serial.println(temp);
      updateState(STATE_PUMP);
      break;
    }
    if (digitalRead(btStop) == 0) {
      updateState(STATE_TRACKING);
      break;
    }
  }
  if ( digitalRead(flameD0) == 1) {
    //warning to SIM
    Serial.println("SIM WARNING FIRE");
  }
  while (millis() - t < 15000) {
    temp = get_temp();
    //    Serial.println(temp);
    if (temp > tempFire)  {
      Serial.println(temp);
      updateState(STATE_PUMP);
      break;
    }
    if (digitalRead(btStop) == 0) {
      updateState(STATE_TRACKING);
      break;
    }
  }
  Serial.println("SIM not fire");
  servo.write(servoPos1);
  delay(1000);
  updateState(STATE_TRACKING);
}

void pump() {
  Serial.println("SIM Killing fire");
//  senMsg("SIM Killing fire");
  while (temp > tempFire || digitalRead(flameD0) == 1) {
    temp = get_temp();
    digitalWrite(pumpPin, LOW);
  }
  digitalWrite(pumpPin, HIGH);
  Serial.println("SIM Kill fire done");
  Serial.println(temp);
  delay(1000);
  updateState(STATE_TRACKING);
}

float get_temp() {
  Vo = analogRead(tempPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  return T - 273.15; //độ C
}
