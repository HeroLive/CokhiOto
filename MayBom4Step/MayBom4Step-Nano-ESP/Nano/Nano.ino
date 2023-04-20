#include <Wire.h>
#include "StepperMotor.h"
#define PUL0_PIN 4
#define PUL1_PIN 5
#define PUL2_PIN 6
#define PUL3_PIN 7
#define DIR0_PIN 13

#define I2C_SLAVE_ADDRESS 99


StepperMotor motor_00(PUL0_PIN, DIR0_PIN);
StepperMotor motor_01(PUL1_PIN, DIR0_PIN);
StepperMotor motor_02(PUL2_PIN, DIR0_PIN);
StepperMotor motor_03(PUL3_PIN, DIR0_PIN);

int state = 0;
bool debug = false;
int numChars = 50;

bool M1 = false;
bool M2 = false;
bool M3 = false;
bool M4 = false;
//motor run
int M[] = { 0, 0, 0, 0 };
//motor pulse value
long P[] = { 6400, 200, 300, 400 };
//motor frequency
long F[] = { 1000, 2000, 3000, 4000 };

void setup() {
  Wire.begin(I2C_SLAVE_ADDRESS);
  Serial.begin(9600);
  Serial.print("I am Slave with I2C ");
  Serial.println(I2C_SLAVE_ADDRESS);

  Wire.onRequest(requestEvents);
  Wire.onReceive(receiveEvents);
}

void loop() {
  if (M[0]) {
    if (debug) Serial.println(P[0]);
    motor_00.DRVI(P[0], F[0]);
    if (motor_00.getExeCompleteFlag()) {
      M[0] = false;
    }
  }
  if (M[1]) {
    if (debug) Serial.println(P[1]);
    motor_01.DRVI(P[1], F[1]);
    if (motor_01.getExeCompleteFlag()) {
      M[1] = false;
    }
  }
  if (M[2]) {
    if (debug) Serial.println(P[2]);
    motor_02.DRVI(P[2], F[2]);
    if (motor_02.getExeCompleteFlag()) {
      M[2] = false;
    }
  }
  if (M[3]) {
    if (debug) Serial.println(P[3]);
    motor_03.DRVI(P[3], F[3]);
    if (motor_03.getExeCompleteFlag()) {
      M[3] = false;
    }
  }
}

void requestEvents() {
  Serial.println(F("---> recieved request"));
  Serial.print(F("sending value : "));
  String msg = String(I2C_SLAVE_ADDRESS) + 'T';
  Serial.println(msg);
  Wire.write(msg.c_str());
}

void receiveEvents(int numBytes) {
  //  Serial.println(F("---> recieved events"));
  String data = "";
  while (Wire.available()) {
    data += (char)Wire.read();
  }
  const char* msg = data.c_str();
  Serial.print("Received data ");
  Serial.println(data);
  parseData(msg);
  for (int i = 0; i < 4; i++) {
    Serial.print(M[i]);
    Serial.print(" ");
    Serial.print(P[i]);
    Serial.print(" ");
    Serial.println(F[i]);
  }
}

void parseData(char* receivedChars_) {
  char g[5] = "";
  String g_ = "";
  char chars[10] = "";
  String stringValue = "";
  const char delim[2] = " ";
  Serial.println(receivedChars_);
  // split the data into its parts

  char* token;  // this is used by strtok() as an index

  token = strtok(receivedChars_, delim);  // get the first part - the string
  strcpy(g, token);                       //
  g_ = String(g);
  int noM = 0;
  while (token != NULL && g_.startsWith("G10")) {
    token = strtok(NULL, delim);  // this continues where the previous call left off
    strcpy(chars, token);
    stringValue = String(chars);
    Serial.println(stringValue);
    if (stringValue.startsWith("M")) {
      noM = toPosition(stringValue);
      M[noM] = true;
    } else if (stringValue.startsWith("P")) {
      P[noM] = toPosition(stringValue);
    } else if (stringValue.startsWith("F")) {
      F[noM] = toPosition(stringValue);
    }
  }
}


float toPosition(String s) {
  String dataInS = s.substring(1, s.length());
  char buff[dataInS.length() + 1];
  dataInS.toCharArray(buff, dataInS.length() + 1);
  return atof(buff);
}
