// Run a A4998 Stepstick from an Arduino UNO.
int stepPinX = 10 ;
int dirPinX = 11 ;
int en = 12 ;
int endPinXA = 8;
int endPinXB = 9;
int pinPot = A7;

bool dirX = LOW;
bool startDirX = LOW;
bool lastEndXA = HIGH;
bool lastEndXB = HIGH;

int curPot = 0;
int lastPot = 0;
int stepsPerUnit = 800;
int rpm = 0;
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
    rpm = map(curPot, 0, 1023, 0, 250);
    curSpeed = rpm / 60.0 * stepsPerUnit;
    period = 1.0 / curSpeed * 1000000;
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
      dirX = !startDirX;
      digitalWrite(dirPinX, dirX);
    } else {
      lastEndXA = 1;
    }
  }
  if (digitalRead(endPinXB) != lastEndXB) {
    if (digitalRead(endPinXB) == 0) {
      lastEndXB = LOW;
      dirX = startDirX;
      digitalWrite(dirPinX, dirX);
    } else {
      lastEndXB = 1;
    }
  }
  if (rpm > 1) {
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(0.4 * period);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(0.5 * period);
  }

}
