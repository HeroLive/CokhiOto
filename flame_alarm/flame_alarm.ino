/*
   Alarm fire

*/

#define STATE_STARTUP 0
#define STATE_TRACKING 1
#define STATE_FIRE 2
#define STATE_CHECKFIRE 3
#define STATE_PUMP 4
byte currentState = STATE_STARTUP;

int flameD0 = 2;
int btStop = 3;
int buzzer = 8;
int pumpPin = 9;
int servoPin = 10;

int temp = 30;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(flameD0, INPUT);
  pinMode(btStop, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(servoPin, OUTPUT);

  digitalWrite(buzzer,LOW);
  digitalWrite(pumpPin,LOW);
  
  updateState(STATE_TRACKING);
}

void loop() {
  // put your main code here, to run repeatedly:

}

// ------state machine---------------------
void updateState(byte aState)
{
  if (aState == currentState)
  {
    return;
  }

  // do state change
  switch (aState)
  {
    case STATE_STARTUP:
      Serial.println("STATE_STARTUP");
      break;
    case STATE_TRACKING:
      Serial.println("STATE_TRACKING");
      tracking();
      break;
    case STATE_FIRE:
      Serial.println("STATE_FIRE");
      fire();
      break;
    case STATE_CHECKFIRE:
      Serial.println("STATE_CHECKFIRE");
      checkfire();
      break;
    case STATE_PUMP:
      Serial.println("STATE_PUMP");
      pump();
      break;
  }

  currentState = aState;
}
//--------------------------------
void tracking() {
  while (true) {
    Serial.println(digitalRead(flameD0));
    if (digitalRead(flameD0) == 1) {
      updateState(STATE_FIRE);
      break;
    }
  }
}
void fire() {
  long t = millis();
  while (millis() - t < 3000) {}
  if ( digitalRead(flameD0) == 1) {
    //move servo up
    //buzzer on
    updateState(STATE_CHECKFIRE);
  } else {
    updateState(STATE_TRACKING);
  }
}

void checkfire() {
  long t = millis();
  while (millis() - t < 30000) {
    if (temp > 50)  {
      updateState(STATE_PUMP);
      break;
    } else if (btStop == 0) {
      updateState(STATE_TRACKING);
      break;
    } else {
      
    }
  }
}

void pump() {
  delay(5000);
  updateState(STATE_TRACKING);
}
