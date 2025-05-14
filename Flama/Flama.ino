#include <BluetoothSerial.h>
#include <Ultrasonic.h>

#define ENA 5
#define ENB 18
#define IN1 2
#define IN2 4
#define IN3 19
#define IN4 21
#define MQ2 27
#define buzzerPin 16

BluetoothSerial BTSerial;
Ultrasonic ultrasonicFront(22, 23);
Ultrasonic ultrasonicBack(12, 14);

bool isMovingForward = false;
bool isMovingBackward = false;
int mq2Value = 0;

bool autoMode = false;

enum AutoState {
  IDLE,
  MOVE_FORWARD,
  TURN_RIGHT_AND_MOVE,
};

AutoState autoState = IDLE;
unsigned long lastActionTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(MQ2, INPUT);

  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  BTSerial.begin("ESP32_Flama");
}

void loop() {
  if (BTSerial.available()) {
    char command = BTSerial.read();
    Serial.println(command);
    autoMode = false;

    switch (command) {
      case 'F':
        moveForward();
        break;
      case 'B':
        moveBackward();
        break;
      case 'L':
        turnLeft();
        delay(200);
        moveForward();
        delay(150);
        turnLeft();
        delay(200);
        moveForward();
        delay(150);
        turnLeft();
        delay(200);
        stopMotors();
        break;
      case 'R':
        turnRight();
        delay(200);
        moveBackward();
        delay(150);
        turnRight();
        delay(200);
        stopMotors();
        break;
      case 'S':
        stopMotors();
        break;
      case 'A':
        autoMode = true;
        autoState = MOVE_FORWARD;
        break;
    }
  }

  long distanceFront = ultrasonicFront.read();
  long distanceBack = ultrasonicBack.read();

  mq2Value = analogRead(MQ2);
  Serial.print("MQ-2 Value: ");
  Serial.println(mq2Value);

  if (mq2Value > 0) {
    // BTSerial.println("G");
    Serial.println("Flame detected!");
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    delay(1000);
  }

  if (!autoMode) {
    if (isMovingForward && distanceFront > 0 && distanceFront < 30) stopMotors();
    if (isMovingBackward && distanceBack > 0 && distanceBack < 30) stopMotors();
  }

  if (autoMode) {
    handleAutoMode(distanceFront);
  }

  delay(100);
}

void moveForward() {
  isMovingForward = true;
  isMovingBackward = false;
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  isMovingBackward = true;
  isMovingForward = false;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  isMovingForward = false;
  isMovingBackward = false;
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  isMovingForward = false;
  isMovingBackward = false;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  isMovingForward = false;
  isMovingBackward = false;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void handleAutoMode(long distanceFront) {
  static unsigned long turnStartTime = 0;
  static bool isTurning = false;
  unsigned long now = millis();

  switch (autoState) {
    case MOVE_FORWARD:
      if (distanceFront > 35 || distanceFront == 0) {
        moveForward();
      } else {
        stopMotors();
        isTurning = true;
        turnStartTime = now;
        autoState = TURN_RIGHT_AND_MOVE;
      }
      break;

    case TURN_RIGHT_AND_MOVE:
      if (isTurning) {
        if (now - turnStartTime < 800) {
          turnRight();
          delay(200);
          moveBackward();
          delay(150);
          turnRight();
          delay(200);
          moveBackward();
          delay(150);
          turnRight();
          delay(200);
          moveBackward();
          delay(150);
          turnRight();
          delay(200);
          moveBackward();
          delay(150);
          turnRight();
          delay(200);
          moveBackward();
          delay(150);
          turnRight();
          delay(200);
          moveBackward();
          delay(150);
          turnRight();
          delay(200);
          stopMotors();
        } else {
          stopMotors();
          isTurning = false;
          turnStartTime = now;
        }
      } else {
        if (now - turnStartTime >= 800) {
          autoState = MOVE_FORWARD;
        }
      }
      break;

    case IDLE:
    default:
      stopMotors();
      break;
  }
}
