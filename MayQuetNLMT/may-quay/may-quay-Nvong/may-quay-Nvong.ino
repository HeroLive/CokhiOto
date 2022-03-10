#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
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
#define STATE_WAITMOVE 5
#define STATE_MOVING 6
#define STATE_DELAY 7

#define STATUS_SET 1
#define STATUS_RUN 0

unsigned int T_addr = 0;  //0 -> 65,535
unsigned int N_addr = 4;
unsigned int S_addr = 8;

bool debug = true;
bool isStored = true; //true: luu vao eeprom

byte currentState = STATE_STARTUP;

//Set cung thog so neu ko dung luu vao eeprom
long N_limit = 50;// Set so vong gioi han
long t_delay = 60;
long rpm = 100;

long t_time = 0;
int modeStatus = 0;
bool startDirX = HIGH;
int stepsPerUnit = 200;
int rpm_max = 500; //so thu 4 de chinh toc do toi da
int rpm_min = 1;
float period = 0;
long N_count = 1;

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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(en, OUTPUT); // Enable
  pinMode(stepPinX, OUTPUT); // Step
  pinMode(dirPinX, OUTPUT); // Dir

  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(enSW, INPUT_PULLUP);
  attachInterrupt(0, PinA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, PinB, RISING);
  //
  digitalWrite(en, LOW); // Set Enable low
  digitalWrite(dirPinX, startDirX);
  //
  lcd.init();
  lcd.backlight();
  
  //0123456789012345
  //NANG LUONG MTROI
  //May quay N vong
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print("NANG LUONG MTROI");
  lcd.setCursor(0, 1);
  lcd.print("May quay N vong");
  delay(1000);
  lcd.clear();
  
  //0123456789012345
  //T: delay(giay)
  //S: toc do(v/ph)
  lcd.setCursor(0, 0); //frint from column 3, row 0
  lcd.print("T: delay(giay)");
  lcd.setCursor(0, 1);
  lcd.print("S: toc do(v/ph)");
  delay(2000);
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
      if (isStored) {
        N_limit = EEPROMReadlong(N_addr);
        t_delay = EEPROMReadlong(T_addr);
        rpm = EEPROMReadlong(S_addr);
      }

      if (debug) {
        Serial.println("STATE_STARTUP");
        Serial.print("Stored data - N: ");
        Serial.print(N_limit);
        Serial.print(" Delay: ");
        Serial.print(t_delay);
        Serial.print(" Speed: ");
        Serial.println(rpm);
      }

      period = rpmToPeriod(rpm);
      currentState = STATE_WAITSTART;
      updateLCD();
      break;

    case STATE_NLIMIT:
      if (debug) {
        Serial.println("STATE_NLIMIT");
      }
      updateLCD();
      settingValue(currentState);
      if (digitalRead(enSW) == 0) {
        while (digitalRead(enSW) == 0);
        currentState = STATE_TDELAY;
        lcd.clear();
      }
      break;
    case STATE_TDELAY:
      if (debug) {
        Serial.println("STATE_TDELAY");
      }
      updateLCD();
      settingValue(currentState);
      if (digitalRead(enSW) == 0) {
        while (digitalRead(enSW) == 0);
        currentState = STATE_SPEED;
        lcd.clear();
      }
      break;
    case STATE_SPEED:
      if (debug) {
        Serial.println("STATE_SPEED");
      }
      updateLCD();
      settingValue(currentState);
      period = rpmToPeriod(rpm);
      if (digitalRead(enSW) == 0) {
        while (digitalRead(enSW) == 0);
        currentState = STATE_WAITSTART;

        if (isStored) {
          EEPROMWritelong(N_addr, N_limit);
          EEPROMWritelong(T_addr, t_delay);
          EEPROMWritelong(S_addr, rpm);
          if (debug) {
            Serial.println("Stored to EEPROM");
          }
        }

        lcd.clear();
      }
      break;
    case STATE_WAITSTART:
      if (debug) {
        Serial.print("STATE_WAITSTART ");
        Serial.print("rpm: ");
        Serial.print(rpm);
        Serial.print(" period: ");
        Serial.println(period);
      }

      updateLCD();
      settingValue(currentState);
      if (digitalRead(enSW) == 0) {
        while (digitalRead(enSW) == 0);
        switch (modeStatus) {
          case STATUS_RUN:
            currentState = STATE_WAITMOVE;
            break;
          case STATUS_SET:
            currentState = STATE_NLIMIT;
            break;
        }
        lcd.clear();
      }
      break;
    case STATE_WAITMOVE:
      if (debug) {
        Serial.println("STATE_WAITMOVE");
      }

      currentState = STATE_MOVING;
      updateLCD();
      break;
    case STATE_MOVING:
      if (digitalRead(enSW) == 0) {
        while (digitalRead(enSW) == 0);
        currentState = STATE_WAITSTART;
        lcd.clear();
      }

      digitalWrite(stepPinX, HIGH);
      delayMicroseconds(period);
      digitalWrite(stepPinX, LOW);
      delayMicroseconds(period);

      if (N_count >= N_limit * stepsPerUnit) {
        N_count = 1;
        currentState = STATE_DELAY;
      } else {
        N_count++;
      }
      break;
    case STATE_DELAY:
      long t_time = millis();
      updateLCD();
      while (true) {
        if (digitalRead(enSW) == 0) {
          while (digitalRead(enSW) == 0);
          currentState = STATE_WAITSTART;
          lcd.clear();
          break;
        }
        if (millis() - t_time >= t_delay * 1000) {
          currentState = STATE_WAITMOVE;
          break;
        }
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
      N_limit = N_limit + count;
      N_limit = N_limit > 0 ? N_limit : 1;
      break;
    case STATE_TDELAY:
      t_delay = t_delay + 10 * count;
      t_delay = t_delay > 0 ? t_delay : 0;
      break;
    case STATE_SPEED:
      rpm = rpm + count;
      if (rpm <= rpm_min) {
        rpm = rpm_min;
      } else if (rpm >= rpm_max) {
        rpm = rpm_max;
      }
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
      case STATE_WAITMOVE:
      case STATE_MOVING:
        f4 = ':';
        break;
    }
  }
  //0123456789012345
  //Mod:xxxxx  T:
  lcd.setCursor(0, 0);
  lcd.print("Mod");
  lcd.print(f4);
  if (currentState == STATE_DELAY || currentState == STATE_MOVING) {
    switch (currentState) {
      case STATE_DELAY:
        lcd.print("Delay");
        break;
      case STATE_MOVING:
        lcd.print("Move ");
        break;
    }

  } else {
    switch (modeStatus) {
      case STATUS_RUN:
        lcd.print("Run ");
        break;
      case STATUS_SET:
        lcd.print("Set ");
        break;
    }
  }

  lcd.setCursor(10, 0);
  lcd.print("T");
  lcd.print(f2);
  lcd.print(t_delay);
  for (int i = 0; i < 5 - numDigit(t_delay); i++) {
    lcd.print(" ");
  }

  //0123456789012345
  //N:xxxx    S:xxx
  lcd.setCursor(0, 1);
  lcd.print("N");
  lcd.print(f1);
  lcd.print(N_limit);
  for (int i = 0; i < 8 - numDigit(N_limit); i++) {
    lcd.print(" ");
  }


  lcd.setCursor(10, 1);
  lcd.print("S");
  lcd.print(f3);
  lcd.print(rpm);
  for (int i = 0; i < 5 - numDigit(rpm); i++) {
    lcd.print(" ");
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
//----------------------
float rpmToPeriod(long _rpm) {
  float _curSpeed = _rpm / 60.0 * stepsPerUnit;
  float _period = 0.5 / _curSpeed * 1000000 - 20;
  return _period;
}

/*EEPROM WRITE/READ*/
//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to adress + 3.
void EEPROMWritelong(int address, long value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}
//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to adress + 3.
long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
