#include <Wire.h>
#include <Arduino.h>
#include <WebSocketsServer.h>  //import for websocket
#include <ArduinoJson.h>       //data Json
#include <StreamUtils.h>

#define I2C_SLAVE_ADDRESS 99

StaticJsonDocument<512> SettingDoc;
StaticJsonDocument<512> RunDoc;

const char *ssid = "esp8266";    //Wifi SSID (Name)
const char *pass = "123456789";  //wifi password

WebSocketsServer webSocket = WebSocketsServer(81);  //websocket init with port 81

bool debug = true;
int count = 0;
long _t = millis();
bool isConnect = false;

void setup() {
  Wire.begin();  //SDA GPIO21, SCL GPIO22
  Serial.begin(9600);
  initData();
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
  if (millis() - _t > 10000 && isConnect) {
    _t = millis();
    sendRunTime();
  }
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
  RunDoc["type"] = "run";
  RunDoc["runTime"][0] = millis();
  RunDoc["runTime"][1] = millis();
  RunDoc["runTime"][2] = millis();
  RunDoc["runTime"][3] = millis();
  sendDataToApp(RunDoc);
}
void initData() {
  SettingDoc["type"] = "set";
  SettingDoc["motors"][0]["noM"] = 1;
  SettingDoc["motors"][1]["noM"] = 2;
  SettingDoc["motors"][2]["noM"] = 3;
  SettingDoc["motors"][3]["noM"] = 4;
  SettingDoc["motors"][0]["pulse"] = 100;
  SettingDoc["motors"][1]["pulse"] = 200;
  SettingDoc["motors"][2]["pulse"] = 300;
  SettingDoc["motors"][3]["pulse"] = 400;
  SettingDoc["motors"][0]["speed"] = 1000;
  SettingDoc["motors"][1]["speed"] = 2000;
  SettingDoc["motors"][2]["speed"] = 3000;
  SettingDoc["motors"][3]["speed"] = 4000;
  SettingDoc["motors"][0]["setTime"] = 1;
  SettingDoc["motors"][1]["setTime"] = 2;
  SettingDoc["motors"][2]["setTime"] = 3;
  SettingDoc["motors"][3]["setTime"] = 4;
}
