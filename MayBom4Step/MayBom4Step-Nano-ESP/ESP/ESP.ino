#include <Wire.h>
#include <Arduino.h>
#include <WebSocketsServer.h>  //import for websocket
#include <ArduinoJson.h>       //data Json
#include <StreamUtils.h>

#define I2C_SLAVE_ADDRESS 99

StaticJsonDocument<512> SettingDoc;
StaticJsonDocument<512> RunDoc;
StaticJsonDocument<128> MotorDoc;

const char *ssid = "esp8266";    //Wifi SSID (Name)
const char *pass = "123456789";  //wifi password

WebSocketsServer webSocket = WebSocketsServer(81);  //websocket init with port 81

bool debug = true;
int count = 0;
long _t = millis();
long _t0 = millis();
long _t1 = millis();
long _t2 = millis();
long _t3 = millis();
bool isConnect = false;

void setup() {
  Wire.begin();  //SDA GPIO21, SCL GPIO22
  Serial.begin(9600);
  initData();

  EEPROM.begin(512);
  EepromStream eepromStream(0, 512);
  deserializeJson(SettingDoc, eepromStream);
  if (debug) serializeJsonPretty(SettingDoc, Serial);

  Serial.println("Connecting to wifi");
  IPAddress apIP(192, 168, 98, 100);                           //Static IP for wifi gateway
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));  //set Static IP gateway on NodeMCU
  WiFi.softAP(ssid, pass);                                     //turn on WIFI

  webSocket.begin();                  //websocket Begin
  webSocket.onEvent(webSocketEvent);  //set Event for websocket
  Serial.println("Websocket is started");
}

void loop() {
  webSocket.loop();  //keep this line on loop method
  // String msg = "Send data to Nano " + String(count);
  // sendI2C(I2C_SLAVE_ADDRESS, msg);
  // delay(1000);
  // count++;
  if (millis() - _t > 1000 && isConnect) {
    _t = millis();
    sendRunTime();
  }
  if (millis() - _t0 > SettingDoc["motors"][0]["setTime"]) {
    _t0 = millis();
    sendI2C(I2C_SLAVE_ADDRESS, SettingDoc, 0);
  }
  if (millis() - _t1 > SettingDoc["motors"][1]["setTime"]) {
    _t1 = millis();
    sendI2C(I2C_SLAVE_ADDRESS, SettingDoc, 1);
  }
  if (millis() - _t2 > SettingDoc["motors"][2]["setTime"]) {
    _t2 = millis();
    sendI2C(I2C_SLAVE_ADDRESS, SettingDoc, 2);
  }
  if (millis() - _t3 > SettingDoc["motors"][3]["setTime"]) {
    _t3 = millis();
    sendI2C(I2C_SLAVE_ADDRESS, SettingDoc, 3);
  }
}

void sendI2C(int addI2C, StaticJsonDocument<512> Doc, int noM) {
  String msg = "G10 M" + String(noM) + " P" + String(SettingDoc["motors"][noM]["pulse"]) + " F" + String(SettingDoc["motors"][noM]["speed"]);
  //M0 Pxxx Fxxx
  Wire.beginTransmission(addI2C);
  Wire.write(msg.c_str());
  Serial.print("sending ");
  Serial.print(msg);
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
    Serial.print(c);  // print the character para
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  //webscket event method
  String cmd = "";
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Websocket is disconnected");
      isConnect = false;
      break;
    case WStype_CONNECTED:
      {
        Serial.println("Websocket is connected");
        Serial.println(webSocket.remoteIP(num).toString());
        webSocket.sendTXT(num, "connected");
        isConnect = true;
        SettingDoc["type"] = "set";
        sendDataToApp(SettingDoc);
      }
      break;
    case WStype_TEXT:
      cmd = "";
      for (int i = 0; i < length; i++) {
        cmd = cmd + (char)payload[i];
      }  //merging payload to single string
      if (debug) {
        Serial.print("Data from flutter:");
        Serial.println(cmd);
      }
      RunEvent(cmd);
      break;
    case WStype_FRAGMENT_TEXT_START:
      break;
    case WStype_FRAGMENT_BIN_START:
      break;
    case WStype_BIN:
      hexdump(payload, length);
      break;
    default:
      break;
  }
}

void RunEvent(String json) {
  StaticJsonDocument<512> data;
  DeserializationError error = deserializeJson(data, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  String type = data["type"];

  if (type == "set") {
    SettingDoc = data;
    if(debug) Serial.println("Save to EEPROM");
    EepromStream eepromStream(0, 512);
    serializeJson(SettingDoc, eepromStream);
    eepromStream.flush();  // (for ESP)
  }
  if (type == "get") {
    sendDataToApp(SettingDoc);
  }
}

void sendDataToApp(StaticJsonDocument<512> jsonDoc) {
  char msg[512];
  serializeJson(jsonDoc, msg);
  webSocket.broadcastTXT(msg);
  if (debug) serializeJson(jsonDoc, Serial);
}

void sendRunTime() {
  long __t0 = SettingDoc["motors"][0]["setTime"] ;
  long __t1 = SettingDoc["motors"][1]["setTime"] ;
  long __t2 = SettingDoc["motors"][2]["setTime"] ;
  long __t3 = SettingDoc["motors"][3]["setTime"] ;
  RunDoc["type"] = "run";
  RunDoc["runTime"][0] = __t0 - (millis() - _t0);
  RunDoc["runTime"][1] = __t1 - (millis() - _t1);
  RunDoc["runTime"][2] = __t2 - (millis() - _t2);
  RunDoc["runTime"][3] = __t3 - (millis() - _t3);
  sendDataToApp(RunDoc);
}
void initData() {
  SettingDoc["type"] = "set";
  SettingDoc["motors"][0]["noM"] = 0;
  SettingDoc["motors"][1]["noM"] = 1;
  SettingDoc["motors"][2]["noM"] = 2;
  SettingDoc["motors"][3]["noM"] = 3;
  SettingDoc["motors"][0]["pulse"] = 100;
  SettingDoc["motors"][1]["pulse"] = 200;
  SettingDoc["motors"][2]["pulse"] = 300;
  SettingDoc["motors"][3]["pulse"] = 400;
  SettingDoc["motors"][0]["microStep"] = 1;
  SettingDoc["motors"][1]["microStep"] = 1;
  SettingDoc["motors"][2]["microStep"] = 1;
  SettingDoc["motors"][3]["microStep"] = 1;
  SettingDoc["motors"][0]["speed"] = 1000;
  SettingDoc["motors"][1]["speed"] = 2000;
  SettingDoc["motors"][2]["speed"] = 3000;
  SettingDoc["motors"][3]["speed"] = 4000;
  SettingDoc["motors"][0]["setTime"] = 1;
  SettingDoc["motors"][1]["setTime"] = 2;
  SettingDoc["motors"][2]["setTime"] = 3;
  SettingDoc["motors"][3]["setTime"] = 4;
}
