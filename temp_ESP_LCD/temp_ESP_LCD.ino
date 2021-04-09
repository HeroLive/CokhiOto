#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>

// Data wire is plugged into pin D3 on the ESP8266
#define ONE_WIRE_BUS D3

// LCD pins
#define SDA_PIN D4
#define SCL_PIN D5
//LED, buzzer
#define LED_ALARM D8
#define LED_NORMAL D7
#define buzzer D6


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature DS18B20(&oneWire);

LiquidCrystal_I2C lcd(0x27, 16, 2); // The I2C address may be different in your project, see the notes below.

float tempC;
float previousTemp;
float tempAlarm = 32;
float tempMin = 30;

void setup() {

  Serial.begin(9600);
  pinMode(LED_ALARM, OUTPUT);
  pinMode(LED_NORMAL, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Wire.begin(SDA_PIN, SCL_PIN);

  digitalWrite(LED_ALARM, LOW);
  digitalWrite(LED_NORMAL, LOW);
  digitalWrite(buzzer, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Canh bao nhiet ");
  lcd.setCursor(0, 1);
  lcd.print("Cam bien DS18B20");

  DS18B20.begin();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nhiet do: ");  

  lcd.setCursor(0, 1);
  lcd.print("Nhiet bao: ");
  lcd.setCursor(14, 1);
  lcd.print(tempAlarm);
  delay(5000);
}

void getTemperature() {
  do {
    DS18B20.requestTemperatures();
    tempC = DS18B20.getTempCByIndex(0);
    if (tempC == (-127)) {
      delay(100);
    }
  } while (tempC == (-127.0));
}

void checkTemp() {
  if (tempC > tempAlarm) {
    Serial.println("Temp overload");
    digitalWrite(LED_ALARM, HIGH);
    digitalWrite(buzzer, HIGH);
    digitalWrite(LED_NORMAL, LOW);

  } else if (tempC <= tempAlarm && tempC >= tempMin) {
    Serial.println("Temp in range");
    digitalWrite(LED_ALARM, LOW);
    digitalWrite(buzzer, LOW);
    digitalWrite(LED_NORMAL, HIGH);
  } else {
    Serial.println("Temp lower range");
    digitalWrite(LED_ALARM, LOW);
    digitalWrite(buzzer, LOW);
    digitalWrite(LED_NORMAL, LOW);
  }
}

void loop() {

  getTemperature();
  checkTemp();
  Serial.println(tempC);

  if (tempC != previousTemp) {

    lcd.setCursor(11, 0);
    lcd.print(tempC);
    previousTemp = tempC;

  }
  delay(1000);
}
