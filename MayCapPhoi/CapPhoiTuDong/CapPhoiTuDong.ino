#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

//driver for the axis 1 - X
#define PUL1_PIN 2
#define DIR1_PIN 5
#define EN1_PIN 8
//driver for the axis 2 - Y
#define PUL2_PIN 4
#define DIR2_PIN 7

#define CONT A3
#define DOWN A0
#define MODE A1
#define UP A2
boolean DIR2 = LOW;

#define STATE_STARTUP 0
#define STATE_Z1 1
#define STATE_Z2 2
#define STATE_X 3
#define STATE_SPEED 4
#define STATE_HOMEX 5
#define STATE_HOMEZ 6
#define STATE_WAITSTART 7
#define STATE_WAITMOVE 8
#define STATE_MOVING 9

LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F

byte currentState = STATE_STARTUP;
unsigned long LastStateChangeTime;

long steps[2] = {0, 0};
int microStep[2] = {16, 16};
float angleStep[2] = {1.8, 1.8};
float gear[2] = {1, 1};
float positions[2] = {0, 0};

float curSpeed = 20;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //
  pinMode(PUL1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(PUL2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(EN1_PIN, OUTPUT);
  digitalWrite(EN1_PIN, LOW);
  //
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print("** Hero Live **");
  lcd.setCursor(0, 1);
  lcd.print("Cap phoi tu dong");
  delay(3000);
  lcd.clear();

}

void loop() {
  //  delay(200);
  //  Serial.print(analogRead(STOP));
  //  Serial.print(" ");
  //  Serial.print(analogRead(DOWN));
  //  Serial.print(" ");
  //  Serial.print(analogRead(MODE));
  //  Serial.print(" ");
  //  Serial.println(analogRead(UP));
  updateState(currentState);
  updateLCD();
}
void updateState(byte aState) {
  switch (aState)
  {
    case STATE_STARTUP:
      Serial.println("STATE_STARTUP");
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_Z1;
        lcd.clear();
      }
      break;
    case STATE_Z1:
      Serial.println("STATE_Z1");
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_Z2;
        lcd.clear();
      }
      break;
    case STATE_Z2:
      Serial.println("STATE_Z2");
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_X;
        lcd.clear();
      }
      break;
    case STATE_X:
      Serial.println("STATE_X");
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_SPEED;
        lcd.clear();
      }
      break;
    case STATE_SPEED:
      Serial.println("STATE_SPEED");
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_HOMEX;
        lcd.clear();
      }
      break;
    case STATE_HOMEX:
      Serial.println("STATE_HOMEX");
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_HOMEZ;
        lcd.clear();
      }
      if (analogRead(UP) > 500) {
        while (analogRead(UP) > 500) {
          moveOne('X', 'R');
        }
      } else if (analogRead(DOWN) > 500) {
        while (analogRead(DOWN) > 500) {
          moveOne('X', 'L');
        }
      }
      break;
    case STATE_HOMEZ:
      Serial.println("STATE_HOMEZ");
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_WAITSTART;
        lcd.clear();
      }
      if (analogRead(UP) > 500) {
        while (analogRead(UP) > 500) {
          moveOne('Z', 'R');
        }
      } else if (analogRead(DOWN) > 500) {
        while (analogRead(DOWN) > 500) {
          moveOne('Z', 'L');
        }
      }
      break;
    case STATE_WAITSTART:
      Serial.println("STATE_WAITSTART");
      if (analogRead(UP) > 500) {
        while (analogRead(UP) > 500);
        currentState = STATE_WAITMOVE;
        lcd.clear();
      }
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_STARTUP;
        lcd.clear();
      }
      break;
    case STATE_WAITMOVE:
      Serial.println("STATE_WAITMOVE");
      if (analogRead(CONT) > 500) {
        currentState = STATE_MOVING;
      } else {
        currentState = STATE_WAITSTART;
      }
      break;
    case STATE_MOVING:
      Serial.println("STATE_MOVING");
      delay(2000);
      currentState = STATE_WAITMOVE;
      break;
  }
}

void updateLCD() {}

void moveOne(char m, char LR) {
  if (LR == 'L') {
    digitalWrite(DIR1_PIN, HIGH);
    digitalWrite(DIR2_PIN, HIGH);
  }
  if (LR == 'R') {
    digitalWrite(DIR1_PIN, LOW);
    digitalWrite(DIR2_PIN, LOW);
  }
  if (m == 'X') {
    digitalWrite(PUL1_PIN, HIGH);
    delayMicroseconds(curSpeed);
    digitalWrite(PUL1_PIN, LOW);
    delayMicroseconds(curSpeed);
  }
  if (m == 'Z') {
    digitalWrite(PUL2_PIN, HIGH);
    delayMicroseconds(curSpeed);
    digitalWrite(PUL2_PIN, LOW);
    delayMicroseconds(curSpeed);
  }
}
