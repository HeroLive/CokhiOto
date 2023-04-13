#include <math.h>
#include "StepperMotor.h"
#include <ArduinoJson.h>  //data Json
#include <StreamUtils.h>  //eeprom

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // or 0x3F

StaticJsonDocument<128> StepperDoc;
StaticJsonDocument<256> DataDoc;

#define BT1_PIN 9
#define MODE 10
#define DOWN 11
#define UP 12
#define PUL1_PIN 5
#define PUL2_PIN 6
#define PUL3_PIN 7
#define PUL4_PIN 8
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
long D200 = 3000;  //motor 2 frequency
long D202 = 8000;  //motor 2 pulse value
long D204 = 5000;  //motor 2 interval time
long D0 = 0;       //target position
long _t1 = millis();
long _t2 = millis();
long _t3 = millis();
long _t4 = millis();

String menu[] = { "So xung M1", "Toc do M1", "Thoi gian M1", "So xung M2", "Toc do M2", "Thoi gian M2", "So xung M3", "Toc do M3", "Thoi gian M3", "So xung M4", "Toc do M4", "Thoi gian M4" };
long menuValue[] = { 8000, 3000, 5, 1020, 220, 2, 1030, 230, 3, 1040, 240, 4 };
int menuLen = 12;

void setup() {
  Serial.begin(9600);


  pinMode(BT1_PIN, INPUT_PULLUP);
  pinMode(MODE, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);

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
  D0 = D202;

  lcd.clear();
}

void loop() {
  if (digitalRead(BT1_PIN) == 0) {
    if (digitalRead(MODE) == 0) {
      Serial.println(state);
      while (digitalRead(MODE) == 0)
        ;
      state = state + 1;
      lcd.clear();
    }

    Serial.print(menu[state]);
    Serial.println(menuValue[state]);
    if (state == menuLen) {
      writeData();
      state = 0;
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

void updateState(int i) {
  // do state change
  lcd.setCursor(0, 0);
  lcd.print(menu[i]);
  lcd.setCursor(0, 1);
  lcd.print(menuValue[i]);
  if (digitalRead(DOWN) == 0) {
    while (digitalRead(DOWN) == 0)
      ;
    menuValue[i] = menuValue[i] <= 0 ? 0 : menuValue[i] - 1;
    lcd.clear();
  }
  if (digitalRead(UP) == 0) {
    while (digitalRead(UP) == 0)
      ;
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
    // deserializeJson(StepperDoc, DataDoc["motor"]);
    JsonArray arr = DataDoc["motor"].as<JsonArray>();
    for (int i = 0; i < menuLen; i++) {
      menuValue[i] = arr[i];
      if (debug) Serial.println(menuValue[i]);
    }
  }

  else {
    writeData();
  }
}
void writeData() {
  // create an empty array
  JsonArray array = StepperDoc.to<JsonArray>();
  for (int i = 0; i < menuLen; i++) {
    array.add(menuValue[i]);
  }

  DataDoc["isStore"] = 1;
  DataDoc["motor"] = StepperDoc;
  if (debug) Serial.println("Save data to EEPROM");
  EepromStream eepromStream(0, 128);
  serializeJson(DataDoc, eepromStream);
  if (debug) serializeJsonPretty(DataDoc, Serial);
  delay(1000);
}
