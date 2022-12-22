#include <SPI.h>
#include <SD.h>
#include <Wire.h>

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

// the logging file
File logfile;

#define LOG_INTERVAL 3000  // mills between calls to flush() - to write data to the card

void error(char *str) {
  Serial.print("error: ");
  Serial.println(str);
  while (1)
    ;
}

void setup(void) {
  Serial.begin(115200);

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

  // log milliseconds since starting
  uint32_t m = millis();
  logfile.print(m);  // milliseconds since start
  logfile.print(", ");
  logfile.println();
  logfile.flush();

  delay(LOG_INTERVAL);
}