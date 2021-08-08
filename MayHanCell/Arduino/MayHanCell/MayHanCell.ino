#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F

#define STATE_STARTUP 0
#define STATE_SPEED 1
#define STATE_SETX 2
#define STATE_MOVEX 3
#define STATE_LENGTH 4
#define STATE_DELAY 5
#define STATE_WAITSTART 6
#define STATE_MOVING 7
byte currentState = STATE_STARTUP;

// Define a stepper and the pins it will use
int PUL1_PIN = 11;
int DIR1_PIN = 12;
int EN_PIN = 13;
int RUN = 10;

//encoder
static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
static int MODE = 5; //The select switch for our encoder.

volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
long encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
long oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
//---------

float microStep = 4;
float angleStep = 1.8;
float disPerRound = 0.125; // round/mm
float stepsPerUnit = disPerRound * 360.0 * microStep / angleStep;

boolean DIR1 = LOW;
float maxSpeeds = 400;
float positions = 0;
float target = 0;
boolean runDone = false;
//---------
float _speed = 5;
float _posX = 0;
float _length = 40;
float _delay = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(MODE, INPUT_PULLUP);
  attachInterrupt(0, PinA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, PinB, RISING);
  pinMode(RUN , INPUT_PULLUP);
  //
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
  //
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print("** Hero Live **");
  lcd.setCursor(0, 1);
  lcd.print("Stepper Control");
  delay(3000);
  lcd.clear();
}

void loop() {
  updateState(currentState);
}

void updateState(byte aState) {
  // do state change
  switch (aState)
  {
    case STATE_STARTUP:
      Serial.println("STATE_STARTUP");
      setLCD();
      currentState = STATE_SPEED;
      lcd.clear();
      break;
    case STATE_SPEED:
      Serial.println("STATE_SPEED");
      settingValue(currentState);
      maxSpeeds = round(1000000 / (_speed * stepsPerUnit));
      setLCD();
      if (digitalRead(MODE) == 0) {
        currentState = STATE_SETX;
        while (digitalRead(MODE) == 0);
        lcd.clear();
      }
      break;
    case STATE_SETX:
      Serial.println("STATE_SETX");
      settingValue(currentState);
      setLCD();
      if (digitalRead(RUN) == 0) {
        currentState = STATE_MOVEX;
        target = _posX * stepsPerUnit;
        Serial.println("STATE_MOVEX");
      }
      if (digitalRead(MODE) == 0) {
        currentState = STATE_LENGTH;
        while (digitalRead(MODE) == 0);
        lcd.clear();
      }
      break;
    case STATE_MOVEX:
      motorRun(PUL1_PIN, DIR1_PIN);
      if (positions == target) {
        currentState = STATE_SETX;
        lcd.clear();
      }
      break;
    case STATE_LENGTH:
      Serial.println("STATE_LENGTH");
      settingValue(currentState);
      setLCD();
      if (digitalRead(MODE) == 0) {
        currentState = STATE_DELAY;
        while (digitalRead(MODE) == 0);
        lcd.clear();
      }
      break;
    case STATE_DELAY:
      Serial.println("STATE_DELAY");
      settingValue(currentState);
      setLCD();
      if (digitalRead(MODE) == 0) {
        currentState = STATE_WAITSTART;
        while (digitalRead(MODE) == 0);
        lcd.clear();
        Serial.println("STATE_WAITSTART");
      }
      break;
    case STATE_WAITSTART:
      settingValue(currentState);
      _posX = round(positions / stepsPerUnit);
      maxSpeeds = round(1000000 / (_speed * stepsPerUnit));
      setLCD();
      if (digitalRead(RUN) == 0) {
        currentState = STATE_MOVING;
        while (digitalRead(RUN) == 0);
        target = 0;
        runDone = false;
        Serial.println("STATE_MOVING");
      } else if (digitalRead(MODE) == 0) {
        currentState = STATE_STARTUP;
        while (digitalRead(MODE) == 0);
        lcd.clear();
      }
      break;
    case STATE_MOVING:
      motorRun(PUL1_PIN, DIR1_PIN);
      if (runDone == false) {
        if (positions == 0) {
          target = _length * stepsPerUnit;
          runDone = true;
          delay(200);
        }
      } else {
        if (positions == 0) {
          currentState = STATE_WAITSTART;
          runDone = false;
          Serial.println("STATE_WAITSTART");
        } else if (positions == _length * stepsPerUnit) {
          delay(_delay * 1000);
          target = 0;
        }
      }
      break;
  }
}

void setLCD() {
  char f1 = 'T';
  char f2 = 'V';
  char f3 = 'H';
  char f4 = 'T';
  if ((millis() * 5 / 1000) % 2 == 0) {
    switch (currentState)
    {
      case STATE_SPEED:
        f1 = ' ';
        break;
      case STATE_SETX:
        f2 = ' ';
        break;
      case STATE_LENGTH:
        f3 = ' ';
        break;
      case STATE_DELAY:
        f4 = ' ';
        break;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print(f1);
  lcd.print("Do");
  lcd.setCursor(4, 0);
  lcd.print(f2);
  lcd.print("Tr");
  lcd.setCursor(8, 0);
  lcd.print(f3);
  lcd.print("Tr");
  lcd.setCursor(12, 0);
  lcd.print(f4);
  lcd.print("re");

  lcd.setCursor(0, 1);
  lcd.print(_speed, 0);
  lcdSpace(4 - numDigit(_speed));
  lcd.setCursor(4, 1);
  lcd.print(_posX, 0);
  lcdSpace(4 - numDigit(_posX));
  lcd.setCursor(8, 1);
  lcd.print(_length, 0);
  lcdSpace(4 - numDigit(_length));
  lcd.setCursor(12, 1);
  lcd.print(_delay, 1);
  lcd.print(" ");
}
//Menu


//-------Encoder reading-------------
void PinA() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge

    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detentom  rotation
  sei(); //restart interrupts
}

//---------------------------
float settingValue(byte mode) {
  int count = 0;
  if (oldEncPos != encoderPos) {
    if (oldEncPos < encoderPos) {
      count++;
    } else {
      count--;
    }
    oldEncPos = encoderPos;
    switch (mode)
    {
      case STATE_SPEED:
      case STATE_WAITSTART:
      case STATE_MOVING:
        _speed = _speed + count;
        _speed = _speed > 0 ? _speed : 1;
        break;
      case STATE_SETX:
        _posX = _posX + count;
        break;
      case STATE_LENGTH:
        _length = _length + count;
        _length = _length > 0 ? _length : 0;
        break;
      case STATE_DELAY:
        _delay = _delay + 0.1 * count;
        _delay = _delay > 0 ? _delay : 0;
        break;
    }
  }
}
//------------------------
int numDigit(float n) {
  int count = 0;
  long num = (long)n;
  String str = "";     // empty string
  str.concat(num);
  count = str.length();
  return count;
}
//------------------------
void lcdSpace(int n) {
  for (int i = 0; i < n; i++) {
    lcd.print(" ");
  }
}
//------------------------
void motorRun(int pul, int dir) {
  if (positions != target) {
    if (positions < target) {
      digitalWrite(dir, DIR1);
      positions++;
    } else {
      digitalWrite(dir, !DIR1);
      positions--;
    }
    digitalWrite(pul, HIGH);
    delayMicroseconds(0.4 * maxSpeeds);
    digitalWrite(pul, LOW);
    delayMicroseconds(0.6 * maxSpeeds);
  }
}
