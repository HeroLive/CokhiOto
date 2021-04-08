/*
 * Alarm fire
 * 
 */
#define STATE_STARTUP 0
#define STATE_TRACKING 1
#define STATE_FIRE 2
#define STATE_PUMP 3
byte currentState = STATE_STARTUP;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  updateState(STATE_TRACKING);
}

void loop() {
  // put your main code here, to run repeatedly:

}

// ---------------------------
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
    case STATE_PUMP:
      Serial.println("STATE_PUMP");
      break;
  }

  currentState = aState;
}
//--------------------------------
void tracking(){
  delay(1000);
  updateState(STATE_FIRE);
}
void fire(){ 
  delay(10000);  
  updateState(STATE_PUMP);
}

void pump(){  
  delay(5000);
  updateState(STATE_TRACKING);
}
