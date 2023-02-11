#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "SHT85.h"
#include <LowPower.h>
#include "RTClib.h"

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
RTC_DS1307 rtc;

// SHT30
#define SHT85_ADDRESS 0x44
SHT85 sht;

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

// the logging file
File logfile;

int LOG_INTERVAL = 75;    // x SLEEP_8S
#define ECHO_TO_SERIAL 1  // echo data to serial port

void error(char *str) {
  Serial.print("error: ");
  Serial.println(str);
  while (1)
    ;
}

void setup(void) {
  Serial.begin(115200);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  Wire.begin();
  sht.begin(SHT85_ADDRESS);
  Wire.setClock(100000);

  // initialize the SD card
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");

  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i / 10 + '0';
    filename[7] = i % 10 + '0';
    if (!SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
      break;  // leave the loop!
    }
  }

  if (!logfile) {
    error("couldnt create file");
  }

  Serial.print("Logging to: ");
  Serial.println(filename);
}

void loop(void) {

  // fetch the time
  DateTime now = rtc.now();
  sht.read();

  //serial log time and temp, humi
#if ECHO_TO_SERIAL
  //Serial.print(now.unixtime());  // seconds since 1/1/1970
  //Serial.print(", ");
  //Serial.print('"');
  Serial.print(now.day(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.year(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  //Serial.print('"');
  Serial.print(", ");
  Serial.print(sht.getTemperature());
  Serial.print(", ");
  Serial.println(sht.getHumidity());
#endif  //ECHO_TO_SERIAL

  // log time
  logfile.print(now.day(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.year(), DEC);
  logfile.print(", ");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
  logfile.print(", ");

  //log temperature and humidity  
  logfile.print(sht.getTemperature());
  logfile.print(", ");
  logfile.print(sht.getHumidity());
  logfile.println();
  logfile.flush();

  Serial.println("save to SD");
  for (int i = 0; i < LOG_INTERVAL; i++) {
    // LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}