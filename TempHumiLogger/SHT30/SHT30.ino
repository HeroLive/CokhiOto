//
//    FILE: SHT85_demo_plotter.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//     URL: https://github.com/RobTillaart/SHT85
//
// TOPVIEW SHT85  (check datasheet)
//            +-------+
// +-----\    | SDA 4 -----
// | +-+  ----+ GND 3 -----
// | +-+  ----+ +5V 2 -----
// +-----/    | SCL 1 -----
//            +-------+


#include "SHT85.h"

#define SHT85_ADDRESS       0x44

SHT85 sht;


void setup()
{
  Serial.begin(115200);

  Wire.begin();
  sht.begin(SHT85_ADDRESS);
  Wire.setClock(100000);
}


void loop()
{
  sht.read();         // default = true/fast       slow = false

  Serial.print(sht.getTemperature(), 2);
  Serial.print("\t");
  Serial.println(sht.getHumidity(), 2);

  delay(1000);

}

