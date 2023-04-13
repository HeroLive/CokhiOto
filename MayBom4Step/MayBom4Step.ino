#include <math.h>
#include "StepperMotor.h"
#include <ArduinoJson.h>  //data Json
#include <StreamUtils.h>  //eeprom

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // or 0x3F

StaticJsonDocument<256> DataDoc;

#define BT1_PIN 9
#define MODE 10
#define DOWN 11
#define UP 12
#define PUL1_PIN 4
#define PUL2_PIN 5
#define PUL3_PIN 6
#define PUL4_PIN 7
#define DIR1_PIN 13
#define DIR2_PIN 2
StepperMotor motor_01(PUL1_PIN, DIR1_PIN);
StepperMotor motor_02(PUL2_PIN, DIR2_PIN);
StepperMotor motor_03(PUL3_PIN, DIR2_PIN);
StepperMotor motor_04(PUL4_PIN, DIR2_PIN);

int state = 0;
bool debug = true;

bool M1 = false;
bool M2 = false;
bool M3 = false;
bool M4 = false;
//motor pulse value
long D200 = 100;
long D220 = 200;
long D240 = 300;
long D260 = 400;
//motor frequency
long D202 = 3000;
long D222 = 3000;
long D242 = 3000;
long D262 = 3000;
//motor interval time
long D204 = 5;
long D224 = 5;
long D244 = 5;
long D264 = 5;
//target position
long D0 = 0;
long D20 = 0;
long D40 = 0;
long D60 = 0;
//
long _t1 = millis();
long _t2 = millis();
long _t3 = millis();
long _t4 = millis();
long _t_Press = millis();
;

String menu[] = { "So xung M1", "Toc do M1", "Thoi gian M1", "So xung M2", "Toc do M2", "Thoi gian M2", "So xung M3", "Toc do M3", "Thoi gian M3", "So xung M4", "Toc do M4", "Thoi gian M4" };
long menuValue[] = { D200, D202, D204, D220, D222, D224, D240, D242, D244, D260, D262, D264 };
int menuLen = 12;

void setup() {
  Serial.begin(9600);


  pinMode(BT1_PIN, INPUT_PULLUP);
  pinMode(MODE, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);


  //sweep data in EEPROM
  // sweepData();

  //read data from EEPROM
  readData();

  //For OLED I2C
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);  //frint from column 3, row 0
  lcd.print("Hero Live");
  lcd.setCursor(0, 1);
  lcd.print("Xin chao cac ban");
  delay(1000);

  M1 = true;
  M2 = true;
  M3 = true;
  M4 = true;
  lcd.clear();
}

void loop() {
  if (digitalRead(BT1_PIN) == 0) {
    
    if (digitalRead(MODE) == 0) {
      lcd.clear();
      Serial.println(state);
      while (digitalRead(MODE) == 0)
        ;
      state = state + 1;
    }

    Serial.print(menu[state]);
    Serial.println(menuValue[state]);
    if (state == menuLen) {
      sweepData();
      delay(1000);
      writeData();
      state = 0;
      delay(1000);
    } else {
      updateState(state);
    }

  } else {
    if (millis() - _t1 >= menuValue[2] * 1000) {
      _t1 = millis();
      M1 = true;
    }
    if (millis() - _t2 >= menuValue[5] * 1000) {
      _t2 = millis();
      M2 = true;
    }
    if (millis() - _t3 >= menuValue[8] * 1000) {
      _t3 = millis();
      M3 = true;
    }
    if (millis() - _t4 >= menuValue[11] * 1000) {
      _t4 = millis();
      M4 = true;
    }
    if (M1) {
      motor_01.DRVI(menuValue[0], menuValue[1]);
      if (motor_01.getExeCompleteFlag()) {
        M1 = false;
      }
    }
    if (M2) {
      motor_02.DRVI(menuValue[3], menuValue[4]);
      if (motor_02.getExeCompleteFlag()) {
        M2 = false;
      }
    }
    if (M3) {
      motor_03.DRVI(menuValue[6], menuValue[7]);
      if (motor_03.getExeCompleteFlag()) {
        M3 = false;
      }
    }
    if (M4) {
      motor_04.DRVI(menuValue[9], menuValue[10]);
      if (motor_04.getExeCompleteFlag()) {
        M4 = false;
      }
    }
  }
}

void displayLCD(){
  lcd.setCursor(0, 0);
  lcd.print("Dang cho");
  lcd.setCursor(0, 1);
  lcd.print(".....");
}

void updateState(int i) {
  // do state change
  lcd.setCursor(0, 0);
  lcd.print(menu[i]);
  lcd.setCursor(0, 1);
  lcd.print(menuValue[i]);
  if (digitalRead(DOWN) == 0) {
    while (digitalRead(DOWN) == 0) {
    }

    menuValue[i] = menuValue[i] <= 0 ? 0 : menuValue[i] - 1;
    lcd.clear();
  }
  if (digitalRead(UP) == 0) {

    while (digitalRead(UP) == 0) {
    }
    menuValue[i] = menuValue[i] + 1;
    lcd.clear();
  }
}

void readData() {
  if (debug) Serial.println("Read data from EEPROM");
  EepromStream eepromStream(0, 128);
  deserializeJson(DataDoc, eepromStream);
  if (debug) serializeJsonPretty(DataDoc, Serial);
  if (DataDoc["isStore"] == 1) {
    menuValue[0] = DataDoc["p1"];
    menuValue[1] = DataDoc["f1"];
    menuValue[2] = DataDoc["t1"];
    menuValue[3] = DataDoc["p2"];
    menuValue[4] = DataDoc["f2"];
    menuValue[5] = DataDoc["t2"];
    menuValue[6] = DataDoc["p3"];
    menuValue[7] = DataDoc["f3"];
    menuValue[8] = DataDoc["t3"];
    menuValue[9] = DataDoc["p4"];
    menuValue[10] = DataDoc["f4"];
    menuValue[11] = DataDoc["t4"];
    for (int i = 0; i < menuLen; i++) {
      Serial.println(menuValue[i]);
    }
  }

  else {
    writeData();
  }
}
void writeData() {
  DataDoc["isStore"] = 1;
  DataDoc["p1"] = menuValue[0];
  DataDoc["f1"] = menuValue[1];
  DataDoc["t1"] = menuValue[2];
  DataDoc["p2"] = menuValue[3];
  DataDoc["f2"] = menuValue[4];
  DataDoc["t2"] = menuValue[5];
  DataDoc["p3"] = menuValue[6];
  DataDoc["f3"] = menuValue[7];
  DataDoc["t3"] = menuValue[8];
  DataDoc["p4"] = menuValue[9];
  DataDoc["f4"] = menuValue[10];
  DataDoc["t4"] = menuValue[11];
  if (debug) Serial.println("Save data to EEPROM");
  EepromStream eepromStream(0, 128);
  serializeJson(DataDoc, eepromStream);
  if (debug) serializeJsonPretty(DataDoc, Serial);
  delay(1000);
}

void sweepData() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
}
