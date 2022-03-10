// Run a A4998 Stepstick from an Arduino UNO.
int stepPinX = 10 ;
int dirPinX = 11 ;
int en = 12 ;
int endPinXA = 8;
int endPinXB = 9;
int pinPot = A7;


bool startDirX = HIGH;
bool dirX = startDirX;
bool lastEndXA = HIGH;
bool lastEndXB = HIGH;

int curPot = 0;
int lastPot = 0;
int stepsPerUnit = 1600;
int rpm = 0;
int rpm_max = 235; //so thu 4 de chinh toc do toi da
int rpm_min = 3;
float curSpeed = 0;
float period = 0;


void setup()
{
  Serial.begin(9600);
  pinMode(en, OUTPUT); // Enable
  pinMode(stepPinX, OUTPUT); // Step
  pinMode(dirPinX, OUTPUT); // Dir
  pinMode(endPinXA, INPUT_PULLUP);
  pinMode(endPinXB, INPUT_PULLUP);
  digitalWrite(en, LOW); // Set Enable low
  digitalWrite(dirPinX, dirX);
}

void loop() {
  //  Serial.println(digitalRead(endPinX));
  curPot = analogRead(pinPot);
  if (abs(curPot - lastPot) > 10) {
    lastPot = curPot;
    rpm = map(curPot, 0, 1023, 0, rpm_max);
    curSpeed = rpm / 60.0 * stepsPerUnit;
    period = 0.5 / curSpeed * 1000000 - 60.0;
//    Serial.print("curPot:");
//    Serial.print(curPot);
//    Serial.print(" rpm:");
//    Serial.print(rpm);
//    Serial.print(" period:");
//    Serial.println(period);
  }

  if (digitalRead(endPinXA) != lastEndXA) {
    if (digitalRead(endPinXA) == 0) {
      lastEndXA = LOW;
      if (dirX == startDirX) {
        delay(1000);
      }
      dirX = !startDirX;
      digitalWrite(dirPinX, dirX);

    } else {
      lastEndXA = HIGH;
    }
  }
  if (digitalRead(endPinXB) != lastEndXB) {
    if (digitalRead(endPinXB) == 0) {
      lastEndXB = LOW;
      if (dirX == (!startDirX)) {
        delay(1000);
      }
      dirX = startDirX;
      digitalWrite(dirPinX, dirX);
    } else {
      lastEndXB = HIGH;
    }
  }
  if (rpm > rpm_min) {
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(period);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(period);
  }

}
