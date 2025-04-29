#include "BluetoothSerial.h"
#include <Ultrasonic.h>
#include <ESP32Servo.h>

BluetoothSerial SerialBT;
Ultrasonic ultrasonicFront(22, 23);
Ultrasonic ultrasonicLeft(12, 13);
Ultrasonic ultrasonicRight(25, 26);

Servo servoSensor;
const int servoPin = 27;
const int servoForwardAngle = 0;
const int servoBackwardAngle = 180;
const long rearStopDistanceCm = 7;

int motor1Pin1 = 2;
int motor1Pin2 = 4;
int enable1Pin = 5;

int motor2Pin1 = 19;
int motor2Pin2 = 21;
int enable2Pin = 18;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;
int backwardDutyCycle = 100;

const long frontStopDistanceCm = 10;
const long sideCheckDelayMs = 200;
const long turnDurationMs = 750;

bool obstacleDetected = false;
bool isMovingForward = false;
bool isMovingBackward = false;

const int mq2Pin = 36;
const int mq2Threshold = 5;
int mq2Value = 0;

const int buzzerPin = 34;

void setup() {
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  ledcAttachChannel(enable1Pin, freq, resolution, pwmChannel);
  ledcAttachChannel(enable2Pin, freq, resolution, pwmChannel + 1);

  Serial.begin(115200);
  SerialBT.begin("ESP32");

  servoSensor.attach(servoPin);
  servoSensor.write(servoForwardAngle);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  
  stopMotors();
}

void loop() {
  long frontDistanceCm = ultrasonicFront.read();

  mq2Value = analogRead(mq2Pin);
  Serial.print("MQ-2 Value: ");
  Serial.println(mq2Value);

  if (mq2Value > 0) {
    Serial.println("Flame detected! Activating buzzer.");
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    delay(1000);
  }

  if (frontDistanceCm <= frontStopDistanceCm && isMovingForward && !obstacleDetected) {
    obstacleDetected = true;
    stopMotors();
    delay(500);

    long leftDistanceCm = ultrasonicLeft.read();
    Serial.println(leftDistanceCm);
    delay(sideCheckDelayMs);
    long rightDistanceCm = ultrasonicRight.read();
    Serial.println(rightDistanceCm);

    if (leftDistanceCm >= rightDistanceCm) {
      TurnLeft();
      delay(turnDurationMs);
      stopMotors();
      delay(500);
      moveForward();
    } else {
      TurnRight();
      delay(turnDurationMs);
      stopMotors();
      delay(500);
      moveForward();
    }
    obstacleDetected = false;
    checkObstacleAndStop();
  }

  if (frontDistanceCm <= frontStopDistanceCm && isMovingBackward && !obstacleDetected) {
    checkObstacleAndStop();
  }

  if (SerialBT.available()) {
    char command = SerialBT.read();
    Serial.println(command);
    switch (command) {
      case 'F':
        servoSensor.write(servoForwardAngle);  // Set servo to forward on 'F'
        delay(100);
        moveForward();
        isMovingForward = true;
        isMovingBackward = false;
        obstacleDetected = false;
        break;
      case 'B':
        servoSensor.write(servoBackwardAngle);  // Set servo to backward on 'B'
        delay(100);
        moveBackward();
        isMovingForward = false;
        isMovingBackward = true;
        obstacleDetected = false;
        break;
      case 'S':
        stopMotors();
        isMovingForward = false;
        isMovingBackward = false;
        obstacleDetected = false;
        break;
      case 'L':
        TurnLeft();
        delay(1000);
        stopMotors();
        isMovingForward = false;
        isMovingBackward = false;
        obstacleDetected = false;
        break;
      case 'R':
        TurnRight();
        delay(1000);
        stopMotors();
        isMovingForward = false;
        isMovingBackward = false;
        obstacleDetected = false;
        break;
      default:
        Serial.println("Unknown command!");
        break;
    }
  }
  delay(100);
}

void checkObstacleAndStop() {
  long frontDistanceCm = ultrasonicFront.read();
  if (frontDistanceCm <= frontStopDistanceCm && isMovingForward) {
    stopMotors();
    obstacleDetected = true;
  }else if (frontDistanceCm <= rearStopDistanceCm && isMovingBackward) {
    stopMotors();
    obstacleDetected = true;
  }
}

void moveForward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  ledcWrite(enable1Pin, dutyCycle);
  ledcWrite(enable2Pin, dutyCycle);
}

void moveBackward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  ledcWrite(enable1Pin, backwardDutyCycle);
  ledcWrite(enable2Pin, backwardDutyCycle);
}

void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  ledcWrite(enable1Pin, 0);
  ledcWrite(enable2Pin, 0);
}

void TurnLeft() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  ledcWrite(enable1Pin, dutyCycle);
  ledcWrite(enable2Pin, dutyCycle);
}

void TurnRight() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  ledcWrite(enable1Pin, dutyCycle);
  ledcWrite(enable2Pin, dutyCycle);
}