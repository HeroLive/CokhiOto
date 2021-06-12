#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>
#include <AccelStepper.h>

//driver for the axis 1 - X
#define PUL1_PIN 2
#define DIR1_PIN 5
#define EN1_PIN 8
//driver for the axis 2 - Y
#define PUL2_PIN 4
#define DIR2_PIN 7

AccelStepper stepper1(AccelStepper::DRIVER, PUL1_PIN, DIR1_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, PUL2_PIN, DIR2_PIN);

#define RUN A3 //start in home
#define DOWN A0
#define MODE A1
#define UP A2
#define RELAY 12
#define CONT 13 // on/off one circle


#define STATE_STARTUP 0
#define STATE_Z1 1
#define STATE_Z2 2
#define STATE_X 3
#define STATE_SPEED 4
#define STATE_HOMEX 5
#define STATE_HOMEZ1 6
#define STATE_HOMEZ2 7
#define STATE_WAITSTART 8
#define STATE_WAITMOVE 9
#define STATE_MOVING 10

LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F

byte currentState = STATE_STARTUP; //STATE_WAITSTART;//


int microStep[2] = {4, 1};
float angleStep[2] = {1.8, 1.8};
float disPerRound[2] = {40, 8};
float positions[3] = {503, 140, 200}; //11 - 1 - 2
long steps[3] = {0, 0, 0};
long maxSpeeds[2] = {50000, 2000};
long acceleration[2] = {40000, 1500};

float setPositions[3] = {500,  130, 190};
long setSteps[3] = {0, 0, 0};

float PercentSpeed = 10;

long N = 0;
long t = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //
  pinMode(PUL1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(PUL2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(EN1_PIN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(CONT, INPUT);

  digitalWrite(EN1_PIN, LOW);
  digitalWrite(RELAY, LOW);
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
  //  Serial.print(positions[0]);
  //  Serial.print(" ");
  //  Serial.print(positions[1]);
  //  Serial.print(" ");
  //  Serial.print(positions[2]);
  //  Serial.print(" ");
  Serial.println(digitalRead(CONT));
  updateLCD();
  updateState(currentState);
}
void updateState(byte aState) {
  switch (aState)
  {
    case STATE_STARTUP:
      Serial.println("STATE_STARTUP");
      currentState = STATE_HOMEZ1;
      //      if (analogRead(MODE) > 500) {
      //        while (analogRead(MODE) > 500);
      //        currentState = STATE_Z1;
      //        //lcd.clear();
      //      }
      break;
    case STATE_HOMEZ1:
      Serial.println("STATE_HOMEZ1");
      setPositions[1] = ajustValue(STATE_HOMEZ1);
      if (analogRead(RUN) > 500) {
        while (analogRead(RUN) > 500);
        moveOne('1');
      }
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_HOMEX;
        lcd.clear();
      }
      break;
    case STATE_HOMEX:
      Serial.println("STATE_HOMEX");
      setPositions[0] = ajustValue(STATE_HOMEX);
      if (analogRead(RUN) > 500) {
        while (analogRead(RUN) > 500);
        moveOne('0');
      }
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_HOMEZ2;
        N = 0;
        lcd.clear();
      }
      break;
    case STATE_HOMEZ2:
      Serial.println("STATE_HOMEZ2");
      setPositions[2] = ajustValue(STATE_HOMEZ2);
      if (analogRead(RUN) > 500) {
        while (analogRead(RUN) > 500);
        moveOne('2');
      }
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_Z1;
        lcd.clear();
      }
      break;

    case STATE_Z1:
      Serial.println("STATE_Z1");
      positions[1] = ajustValue(STATE_Z1);
      Serial.println(positions[1]);
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_X;
        lcd.clear();
      }
      break;
    case STATE_X:
      Serial.println("STATE_X");
      positions[0] = ajustValue(STATE_X);
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_Z2;
        lcd.clear();
      }
      break;
    case STATE_Z2:
      Serial.println("STATE_Z2");
      positions[2] = ajustValue(STATE_Z2);
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_WAITSTART;
        lcd.clear();
      }
      break;

    case STATE_SPEED:
      Serial.println("STATE_SPEED");
      PercentSpeed = ajustValue(STATE_SPEED);
      if (analogRead(MODE) > 500) {
        while (analogRead(MODE) > 500);
        currentState = STATE_WAITSTART;
        lcd.clear();
      }
      break;

    case STATE_WAITSTART:
      Serial.println("STATE_WAITSTART");
      if (analogRead(RUN) > 500) {
        while (analogRead(RUN) > 500);
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
      if (digitalRead(CONT) == 1) {
        currentState = STATE_MOVING;
        lcd.clear();
      } else {
        currentState = STATE_WAITSTART;
      }
      break;
    case STATE_MOVING:
      Serial.println("STATE_MOVING");
      moving();
      N++;
      currentState = STATE_WAITMOVE;
      break;
  }
}

float ajustValue(byte mode) {
  int count = 0;
  if (analogRead(UP) > 500) {
    count++;
    while (analogRead(UP) > 500);
  } else if (analogRead(DOWN) > 500) {
    count--;
    while (analogRead(DOWN) > 500);
  }
  switch (mode)
  {
    case STATE_Z1:
      return positions[1] + count > 0 ? positions[1] + count : 1;
      break;
    case STATE_Z2:
      return positions[2] + count > 0 ? positions[2] + count : 1;
      break;
    case STATE_X:
      return positions[0] + count > 100 ? positions[0] + count : 100;
      break;
    case STATE_HOMEX:
      return setPositions[0] + count;
      break;
    case STATE_HOMEZ1:
      return setPositions[1] + count;
      break;
    case STATE_HOMEZ2:
      return setPositions[2] + count;
      break;
    case STATE_SPEED:
      PercentSpeed = PercentSpeed + 1 * count;
      if (PercentSpeed < 10) {
        PercentSpeed = 10;
      } else if (PercentSpeed > 100) {
        PercentSpeed = 100;
      }
      return PercentSpeed;
      break;
  }
}

void moving() {
  steps[0] = positions[0] * 360 / angleStep[0] * microStep[0] / disPerRound[0];
  steps[1] = positions[1] * 360 / angleStep[1] * microStep[1] / disPerRound[1];
  steps[2] = positions[2] * 360 / angleStep[1] * microStep[1] / disPerRound[1];
  stepper1.setMaxSpeed(maxSpeeds[0]);
  stepper1.setAcceleration(acceleration[0]);
  stepper2.setMaxSpeed(maxSpeeds[1]);
  stepper2.setAcceleration(acceleration[1]);

  digitalWrite(RELAY, LOW);
  stepper1.runToNewPosition(0);
  stepper2.runToNewPosition(0);

  stepper1.runToNewPosition(steps[0]);
  delay(500);

  stepper2.runToNewPosition(steps[1]);
  delay(200);
  digitalWrite(RELAY, HIGH);
  delay(1000);

  stepper2.runToNewPosition(0);
  delay(200);

  stepper1.runToNewPosition(0);
  delay(200);

  stepper2.runToNewPosition(steps[2]);
  delay(200);
  digitalWrite(RELAY, LOW);
  delay(1000);

  stepper2.runToNewPosition(0);
}
void moveOne(char m) {
  setSteps[0] = setPositions[0] * 360 / angleStep[0] * microStep[0] / disPerRound[0];
  setSteps[1] = setPositions[1] * 360 / angleStep[1] * microStep[1] / disPerRound[1];
  setSteps[2] = setPositions[2] * 360 / angleStep[1] * microStep[1] / disPerRound[1];
  stepper1.setMaxSpeed(maxSpeeds[0]);
  stepper1.setAcceleration(acceleration[0]);
  stepper2.setMaxSpeed(maxSpeeds[1]);
  stepper2.setAcceleration(acceleration[1]);
  if (m == '0') {
    stepper1.runToNewPosition(setSteps[0]);
  } else if (m == '1') {
    stepper2.runToNewPosition(setSteps[1]);
  } else if (m == '2') {
    stepper2.runToNewPosition(setSteps[2]);
  }
}
void updateLCD() {
  switch (currentState)
  {
    case STATE_STARTUP:
    case STATE_HOMEX:
    case STATE_HOMEZ1:
    case STATE_HOMEZ2:
      setLCD();
      break;
    default:
      modeLCD();
      break;
  }
}

void modeLCD() {
  char f1 = ':';
  char f2 = ':';
  char f3 = ':';
  char f4 = ':';
  String s_ = "   ";

  switch (currentState)
  {
    case STATE_WAITSTART:
      s_ = "WAI";
      break;
    case STATE_WAITMOVE:
    case STATE_MOVING:
      s_ = "RUN";
      break;
    default:
      s_ = "SET";
      break;
  }

  if ((millis() * 5 / 1000) % 2 == 0) {
    switch (currentState)
    {
      case STATE_Z1:
        f1 = ' ';
        break;
      case STATE_Z2:
        f2 = ' ';
        break;
      case STATE_X:
        f3 = ' ';
        break;
      case STATE_SPEED:
        f4 = ' ';
        break;

    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Z1");
  lcd.print(f1);
  lcd.print(positions[1], 0);
  lcd.print("  ");

  lcd.setCursor(7, 0);
  lcd.print("X");
  lcd.print(f3);
  lcd.print(positions[0], 0);
  lcd.print("  ");

  lcd.setCursor(0, 1);
  lcd.print("Z2");
  lcd.print(f2);

  lcd.print(positions[2], 0);
  lcd.setCursor(7, 1);
  lcd.print("N:");
  lcd.print(N);

  lcd.setCursor(13, 1);
  lcd.print(s_);
}

void setLCD() {
  char f1 = ':';
  char f2 = ':';
  char f3 = ':';

  if ((millis() * 5 / 1000) % 2 == 0) {
    switch (currentState)
    {
      case STATE_HOMEZ1:
        f1 = ' ';
        break;
      case STATE_HOMEZ2:
        f2 = ' ';
        break;
      case STATE_HOMEX:
        f3 = ' ';
        break;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Z1");
  lcd.print(f1);
  lcd.print(setPositions[1], 0);
  lcd.print("  ");

  lcd.setCursor(7, 0);
  lcd.print("X");
  lcd.print(f3);
  lcd.print(setPositions[0], 0);
  lcd.print("  ");

  lcd.setCursor(0, 1);
  lcd.print("Z2");
  lcd.print(f2);
  lcd.print(setPositions[2], 0);

  lcd.setCursor(13, 1);
  lcd.print("HOM");
}
