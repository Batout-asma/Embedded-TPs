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

  stopMotors();
}

void loop() {
  long frontDistanceCm = ultrasonicFront.read();

  if (frontDistanceCm <= frontStopDistanceCm && isMovingForward && !obstacleDetected) {
    Serial.println("Obstacle detected while moving forward! Initiating obstacle avoidance.");
    obstacleDetected = true;
    stopMotors();
    delay(500);

    long leftDistanceCm = ultrasonicLeft.read();
    Serial.print("Left Distance: ");
    Serial.println(leftDistanceCm);
    delay(sideCheckDelayMs);
    long rightDistanceCm = ultrasonicRight.read();
    Serial.print("Right Distance: ");
    Serial.println(rightDistanceCm);

    if (leftDistanceCm >= rightDistanceCm) {
      Serial.println("Turning Left.");
      TurnLeft();
      delay(turnDurationMs);
      stopMotors();
      delay(500);
      moveForward();
    } else {
      Serial.println("Turning Right.");
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
    Serial.print("Received command: ");
    Serial.println(command);
    switch (command) {
      case 'F':
        Serial.println("Command: F");
        servoSensor.write(servoForwardAngle);  // Set servo to forward on 'F'
        delay(100);
        moveForward();
        isMovingForward = true;
        isMovingBackward = false;
        obstacleDetected = false;
        break;
      case 'B':
        Serial.println("Command: B");
        servoSensor.write(servoBackwardAngle);  // Set servo to backward on 'B'
        delay(100);
        moveBackward();
        isMovingForward = false;
        isMovingBackward = true;
        obstacleDetected = false;
        break;
      case 'S':
        Serial.println("Command: S");
        stopMotors();
        isMovingForward = false;
        isMovingBackward = false;
        obstacleDetected = false;
        break;
      case 'L':
        Serial.println("Command: L");
        TurnLeft();
        delay(1000);
        stopMotors();
        isMovingForward = false;
        isMovingBackward = false;
        obstacleDetected = false;
        break;
      case 'R':
        Serial.println("Command: R");
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
  Serial.print("Distance (in check function): ");
  Serial.println(frontDistanceCm);
  if (frontDistanceCm <= frontStopDistanceCm && isMovingForward) {
    Serial.println("Obstacle detected in front! Stopping motors.");
    stopMotors();
    obstacleDetected = true;
  }else if (frontDistanceCm <= rearStopDistanceCm && isMovingBackward) {
    Serial.println("Obstacle detected in back! Stopping motors.");
    stopMotors();
    obstacleDetected = true;
  }
}

void moveForward() {
  Serial.println("Moving Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  ledcWrite(enable1Pin, dutyCycle);
  ledcWrite(enable2Pin, dutyCycle);
}

void moveBackward() {
  Serial.println("Moving Backwards at 30% speed");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  ledcWrite(enable1Pin, backwardDutyCycle);
  ledcWrite(enable2Pin, backwardDutyCycle);
}

void stopMotors() {
  Serial.println("Motors stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  ledcWrite(enable1Pin, 0);
  ledcWrite(enable2Pin, 0);
}

void TurnLeft() {
  Serial.println("Turn Left");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  ledcWrite(enable1Pin, dutyCycle);
  ledcWrite(enable2Pin, dutyCycle);
}

void TurnRight() {
  Serial.println("Turn Right");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  ledcWrite(enable1Pin, dutyCycle);
  ledcWrite(enable2Pin, dutyCycle);
}