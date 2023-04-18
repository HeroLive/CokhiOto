#include <Wire.h>
#define I2C_SLAVE_ADDRESS 99

int count = 0;

void setup() {
  Wire.begin(); //SDA GPIO21, SCL GPIO22
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  String msg = "Send data to Nano " + String(count);
  sendI2C(I2C_SLAVE_ADDRESS, msg);
  delay(1000);
  count++;
}

void sendI2C(int addI2C, String para) {
  //Send value 12 to slave
  Wire.beginTransmission(addI2C);
  Wire.write(para.c_str());
  Serial.print("sending ");
  Serial.print(para);
  Serial.print(" to I2C address ");
  Serial.println(addI2C);
  Wire.endTransmission();
}

void requestI2C(int addI2C) {
  String para = "";
  Wire.requestFrom(addI2C, 4);
  while (Wire.available()) {  // slave may send less than requested
    char c = Wire.read();     // receive a byte as character
    para += c;
    Serial.print(c);  // print the character para = M-nuMotor-T-motorType
  }
}
