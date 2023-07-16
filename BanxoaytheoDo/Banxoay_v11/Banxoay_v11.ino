//OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  //https://github.com/adafruit/Adafruit_SSD1306

//OLED define
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//
#include <EEPROM.h>

#include <math.h>
#include "StepperMotor.h"
#define pulX 11
#define dirX 12
#define enX 13

#define Run 4
#define Home 5
#define Home_Limit 6
#define Down 7
#define Up 8

StepperMotor stepperX(pulX, dirX);

bool debug = true;
bool MXA = false;
bool MH = false;
long t = millis();
int address = 0 ; //address store

float disPerRoundX = 360;
float gearX = 1;
int microStepX = 10000;
float stepsPerUnitX = microStepX * gearX / disPerRoundX;
float speedX = 3000;  //chinh toc do X

long Xd = -30;
long Xa = 0;
long X = 0;


void setup() {
  Serial.begin(115200);

  pinMode(enX, OUTPUT);
  pinMode(Run, INPUT_PULLUP);
  pinMode(Home, INPUT_PULLUP);
  pinMode(Home_Limit, INPUT_PULLUP);
  pinMode(Down, INPUT_PULLUP);
  pinMode(Up, INPUT_PULLUP);

  digitalWrite(enX, LOW);
  delay(10);

  //read data from EEPROM
  Xd = EEPROMReadlong(address);

  //For OLED I2C
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  oled.display();
  delay(1000);
  // Clear the buffer
  oled.clearDisplay();
}

void loop() {

  // Serial.println(analogRead(A0));

  if (!MXA && !MH) {
    oled.clearDisplay();
    oledDisplay(5, 0, 15, Xd);  //size,x,y,value

    if (!digitalRead(Run)) {
      t = millis();
      while (!digitalRead(Run)) {
        Serial.println("Press Run button ");
      }
      if (millis() - t > 10) {
        MXA = true;
        Xa = Xa + Xd;
        X = Xa * stepsPerUnitX;
      }
      Serial.print("XA: ");
      Serial.println(Xa);
    } else if (!digitalRead(Home)) {
      t = millis();
      while (!digitalRead(Home)) {
        Serial.println("Press Home button ");
      }
      if (millis() - t > 10) {
        MH = true;
        Xa = 0;
      }
    } else if (!digitalRead(Down)) {
      t = millis();
      while (!digitalRead(Down)) {
        Serial.println("Press Down button ");
      }
      if (millis() - t > 10) {
        Xd = Xd - 1;
        EEPROMWritelong(address, Xd);
      }
    } else if (!digitalRead(Up)) {
      t = millis();
      while (!digitalRead(Up)) {
        Serial.println("Press Down button ");
      }
      if (millis() - t > 10) {
        Xd = Xd + 1;
        EEPROMWritelong(address, Xd);
      }
    }
  }

  if (MXA) {
    stepperX.DRVA(X, speedX);
    if (stepperX.getExeCompleteFlag()) {
      MXA = false;
      if (debug) {
        Serial.print("X go to A: ");
        Serial.println(X);
        Serial.print("Position: ");
        Serial.println(stepperX.getCurrentPosition());
      }
      delay(10);
    }
  }
  if (MH) {
    bool home_dir = Xd < 0 ? false : true;
    stepperX.ZRN(speedX, speedX / 15, Home_Limit, home_dir);
    if (stepperX.getExeCompleteFlag()) {
      MH = false;
      if (debug) {
        Serial.print("Come to home ");
        Serial.println(stepperX.getCurrentPosition());
      }
    }
  }
}

void oledDisplay(int size, int x, int y, float value) {
  oled.setTextSize(size);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(x, y);
  if (value > 0) {
    oled.print("+");
    oled.print(value, 0);
  } else {
    oled.print(value, 0);
  }
  oled.display();
}

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
