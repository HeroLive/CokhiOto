// **** INCLUDES *****
#include "LowPower.h"

void setup() {
  Serial.begin(9600);
  // No setup is required for this library
  pinMode(13, OUTPUT);
}

void loop() {
  Serial.println("Going to sleep for 2 seconds");
  digitalWrite(13, HIGH);
  //LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,SPI_OFF, USART0_OFF, TWI_OFF);
  Serial.println("Arduino: Hey I just Woke up");
  Serial.println("");
}
