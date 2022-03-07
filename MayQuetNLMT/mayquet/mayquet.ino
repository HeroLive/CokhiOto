#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // or 0x3F

// Define a stepper and the pins it will use
int stepPinX = 11;
int dirPinX = 12;
int en = 13;

#define STATE_STARTUP 0
#define STATE_NLIMIT 1
#define STATE_TDELAY 2
#define STATE_SPEED 3
#define STATE_WAITSTART 4
#define STATE_MOVING 5

#define STATUS_SET 1
#define STATUS_RUN 0

byte currentState = STATE_STARTUP;

//encoder
static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
static int enSW = 4; //The select switch for our encoder.

volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
long encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
long oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
//---------
int N_Limit = 10;// Set so vong gioi han
int t_delay = 5;
int rpm = 100;
int modeStatus = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(enSW, INPUT_PULLUP);
  attachInterrupt(0, PinA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, PinB, RISING);
  //
  pinMode(en, OUTPUT);
  digitalWrite(en, LOW);
  //
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print("** Hero Live **");
  lcd.setCursor(0, 1);
  lcd.print("Stepper Control");
  delay(1000);
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
      currentState = STATE_NLIMIT;
      updateLCD();
      break;
    case STATE_NLIMIT:
      Serial.println("STATE_NLIMIT");
      updateLCD();
      settingValue(currentState);
      if (digitalRead(enSW) == 0) {
        while (digitalRead(enSW) == 0);
        currentState = STATE_TDELAY;
        lcd.clear();
      }
      break;
    case STATE_TDELAY:
      Serial.println("STATE_TDELAY");
      updateLCD();
      settingValue(currentState);
      if (digitalRead(enSW) == 0) {
        while (digitalRead(enSW) == 0);
        currentState = STATE_SPEED;
        lcd.clear();
      }
      break;
    case STATE_SPEED:
      Serial.println("STATE_SPEED");
      updateLCD();
      settingValue(currentState);
      if (digitalRead(enSW) == 0) {
        while (digitalRead(enSW) == 0);
        currentState = STATE_WAITSTART;
        lcd.clear();
      }
      break;
    case STATE_WAITSTART:
      Serial.println("STATE_WAITSTART");
      updateLCD();
      settingValue(currentState);
      if (digitalRead(enSW) == 0) {
        while (digitalRead(enSW) == 0);        
        switch (modeStatus) {
          case STATUS_RUN:
            currentState = STATE_MOVING;
            break;
          case STATUS_SET:
            currentState = STATE_NLIMIT;
            break;
        }
        lcd.clear();
      }
      break;
    case STATE_MOVING:
      Serial.println("STATE_MOVING");
      updateLCD();
      if (digitalRead(enSW) == 0) {
        while (digitalRead(enSW) == 0);
        currentState = STATE_STARTUP;
        lcd.clear();
      }
      break;
  }
}

//-------setting value of menu 0X----
void settingValue(byte mode) {
  int count = 0;
  if (oldEncPos != encoderPos) {
    if (oldEncPos < encoderPos) {
      count++;
    } else {
      count--;
    }
    oldEncPos = encoderPos;
    //      updateSettingValue(menuSelect);
  }
  switch (mode)
  {
    case STATE_NLIMIT:
      N_Limit = N_Limit + count;
      N_Limit = N_Limit > 0 ? N_Limit : 1;
      break;
    case STATE_TDELAY:
      t_delay = t_delay + count;
      t_delay = t_delay > 0 ? t_delay : 1;
      break;
    case STATE_SPEED:
      rpm = rpm + count;
      rpm = rpm > 0 ? rpm : 1;
      break;
    case STATE_WAITSTART:
      modeStatus = modeStatus + count;
      if (modeStatus < 0) {
        modeStatus = 1;
      } else if (modeStatus > 1) {
        modeStatus = 0;
      }
      break;
  }
}

//---------------------------
void updateLCD() {
  char f1 = ':';
  char f2 = ':';
  char f3 = ':';
  char f4 = ':';
  if ((millis() * 5 / 1000) % 2 == 0) {
    switch (currentState)
    {
      case STATE_NLIMIT:
        f1 = ' ';
        break;
      case STATE_TDELAY:
        f2 = ' ';
        break;
      case STATE_SPEED:
        f3 = ' ';
        break;
      case STATE_WAITSTART:
        f4 = ' ';
        break;
    }
  }
  //0123456789012345
  //N:xxx T:xx S:xxx
  lcd.setCursor(0, 0);
  lcd.print("N");
  lcd.print(f1);
  lcd.print(N_Limit);
  for (int i = 0; i < 4 - numDigit(N_Limit); i++) {
    lcd.print(" ");
  }

  lcd.setCursor(6, 0);
  lcd.print("T");
  lcd.print(f2);
  lcd.print(t_delay);
  for (int i = 0; i < 3 - numDigit(t_delay); i++) {
    lcd.print(" ");
  }

  lcd.setCursor(11, 0);
  lcd.print("S");
  lcd.print(f3);
  lcd.print(rpm);
  for (int i = 0; i < 4 - numDigit(rpm); i++) {
    lcd.print(" ");
  }

  lcd.setCursor(0, 1);
  lcd.print("M");
  lcd.print(f4);
  switch (modeStatus) {
    case STATUS_RUN:
      lcd.print("RUN");
      break;
    case STATUS_SET:
      lcd.print("SET");
      break;
  }
}

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
int numDigit(int n) {
  int count = 0;
  if (n <= 0) {
    count = 1;
  }
  while (n != 0)
  {
    //sau một vòng lặp thì count sẽ tăng lên 1
    count++;
    //chia lấy nguyên cho 10, cứ một lần chia thì số n sẽ có một chữ số
    n = n / 10;
  }
  return count;
}
