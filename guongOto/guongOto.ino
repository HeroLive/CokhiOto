//input
int lockButton = 2;   // chân nhận tín hiệu khóa cửa từ xa
int unLockButton = 3;   // chân nhận tín hiệu mở cửa từ xa
int upButton = 4;    // nút nhấn lên
int downButton = 5;  // nút nhấn xuống
int leftButton = 6;  // nút nhấn trái
int rightButton = 7; // nút nhấn phải
//output
/*
   Motor 1:
   IN1:HIGH, IN2:LOW  --> out1-ON, out2-OFF
*/
int IN1 = 8; //motor up-down
int IN2 = 9; //motor up-down
int IN3 = 10; //motor left-right
int IN4 = 11; //motor left-right
int IN5 = 12; //motor lock-unlock
int IN6 = 13; //motor lock-unlock


void setup()
{
  Serial.begin(9600);

  pinMode(lockButton, INPUT);
  pinMode(unLockButton, INPUT);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  //stop all 3 motor
  stopUpDown();
  stopLeftRight();
  stopLockUnLock();
};
/*
  void loop() {
  moveUp();
  Serial.println("move up");
  delay(5000);
  moveDown();
  Serial.println("move down");
  delay(5000);
  moveLeft();
  Serial.println("move left");
  delay(5000);
  moveRight();
  Serial.println("move right");
  delay(5000);
  doLock();
  Serial.println("lock");
  delay(5000);
  doUnlock();
  Serial.println("unLock");
  delay(5000);
  }
*/

//------functions--------------
//up-down
void moveUp() {
  Serial.println("move up");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
};
void stopUpDown() {
//  Serial.println("stop up down");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
};
void moveDown() {
  Serial.println("move down");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
};
//left-right
void moveLeft() {
  Serial.println("move left");
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
};
void stopLeftRight() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
};
void moveRight() {
  Serial.println("move right");
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
};
//lock-unlock
void doLock() {
  Serial.println("Gập gương ...");
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);;
};
void stopLockUnLock() {
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
}
void doUnlock() {
  Serial.println("Mở gương ...");
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
};

void loop() {
  int lock =  digitalRead(lockButton);
  int unLock = digitalRead(unLockButton);
  int up =  digitalRead(upButton);
  int down = digitalRead(downButton);
  int left = digitalRead(leftButton);
  int right = digitalRead(rightButton);
  Serial.print(lock);
  Serial.print("--");
  Serial.println(unLock);
  //lock-unlock
  if (lock == 1) {
    while (digitalRead(lockButton) == 1);

    doLock();
    delay(4000);
    stopLockUnLock();
  };
  if (unLock == 1) {
    while (digitalRead(unLockButton) == 1);

    doUnlock();
    delay(4000);
    stopLockUnLock();
  };
  //left-right
  if ((left == 1 && right == 1) || (left == 0 && right == 0)) {
    stopLeftRight();
  } else if (left == 0) {
    moveLeft();
  } else {
    moveRight();
  }
  //up-down
  if ((up == 1 && down == 1) || (up == 0 && down == 0)) {
    stopUpDown();
  } else if (up == 0) {
    moveUp();
  } else {
    moveDown();
  }
}
