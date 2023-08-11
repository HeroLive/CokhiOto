//
#include <EEPROM.h>
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
#include "StepperMotor.h"
#define PUL_PIN 11
#define DIR_PIN 12
#define EN_PIN 13
#define Run 10
#define Up 9
#define Down 8
#define ROT_PIN A0

StepperMotor stepperX(PUL_PIN, DIR_PIN);

float disPerRoundX = 360;
float gearX = 1;
int microStepX = 800;
float stepsPerUnitX = microStepX * gearX / disPerRoundX;
long speedX = 99990;  //chinh toc do X

bool debug = false;
bool MXA = false;
bool MXB = false;
long t = millis();
long t_delay = 100;  //thoi gian dung ms
int address = 0;     //address store
long maxSpeedSet = 99999;

long Xa = 0;
long Xb = 360 * stepsPerUnitX;  //vi tri X den B

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(Run, INPUT_PULLUP);
  pinMode(Up, INPUT_PULLUP);
  pinMode(Down, INPUT_PULLUP);

  pinMode(EN_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, 0);

  //read data from EEPROM
  speedX = EEPROMReadlong(address);
  if (speedX < 0 || speedX > maxSpeedSet) {
    speedX = 0;
  }


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
  // put your main code here, to run repeatedly:
  if (!MXA && !MXB) {

    /*
    ********chinh toc do bang bien tro***********
    speedX = 8 * long(analogRead(ROT_PIN));
    // Serial.println(analogRead(ROT_PIN));
    //***************************************
    */

    //*********chinh toc do bang nut nhan Up Down**********
    if (!digitalRead(Up)) {
      t = millis();
      while (!digitalRead(Up)) {
        if (millis() - t > 1000) {
          if (speedX < maxSpeedSet) {
            speedX++;
          }
          // delay(1);
          if (debug) {
            Serial.println(speedX);
          }
          oled.clearDisplay();
          oledDisplay(4, 0, 25, speedX);  //size,x,y,value
        }
      }
      if (speedX < maxSpeedSet) {
        speedX++;
      }
      if (debug) {
        Serial.println(speedX);
      }
      EEPROMWritelong(address, speedX);
    }
    if (!digitalRead(Down)) {
      t = millis();
      while (!digitalRead(Down)) {
        if (millis() - t > 1000) {
          if (speedX > 0) {
            speedX--;
          }

          // delay(1);
          if (debug) {
            Serial.println(speedX);
          }
          oled.clearDisplay();
          oledDisplay(4, 0, 25, speedX);  //size,x,y,value
        }
      }
      if (speedX > 0) {
        speedX--;
      }
      if (debug) {
        Serial.println(speedX);
      }
      EEPROMWritelong(address, speedX);
    }
    //***************************************
    oled.clearDisplay();
    oledDisplay(4, 0, 25, speedX);  //size,x,y,value
    //***************************************
    if (!digitalRead(Run)) {
      t = millis();
      while (!digitalRead(Run)) {
        if (debug) {
          Serial.println("Press Run button ");
        }
      }
      if (millis() - t > 10) {
        MXA = false;
        MXB = true;
      }
    }
  } else {
    if (MXB) {
      stepperX.DRVA(Xb, speedX);
      if (stepperX.getExeCompleteFlag()) {
        MXB = false;
        MXA = true;
        if (debug) {
          Serial.print("X go to B - Position: ");
          Serial.println(stepperX.getCurrentPosition());
          Serial.println(speedX);
        }
        delay(t_delay);
      }
    } else if (MXA) {
      stepperX.DRVA(Xa, speedX);
      if (stepperX.getExeCompleteFlag()) {
        MXA = false;
        if (debug) {
          Serial.print("X go to A - Position: ");
          Serial.println(stepperX.getCurrentPosition());
          Serial.println(speedX);
        }
      }
    }
  }
}

void oledDisplay(int size, int x, int y, float value) {
  oled.setTextSize(size);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(x, y);
  oled.print(value, 0);
  oled.display();
}

void EEPROMWritelong(int address, long value) {
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
long EEPROMReadlong(long address) {
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
