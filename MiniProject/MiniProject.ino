#include "BluetoothSerial.h"
#include <Ultrasonic.h>

BluetoothSerial SerialBT;
Ultrasonic ultrasonicFront(22, 23);
Ultrasonic ultrasonicLeft (12, 13);
Ultrasonic ultrasonicRight(25, 26);

int motor1Pin1 = 2;  // IN1
int motor1Pin2 = 4;  // IN2
int enable1Pin = 5;  // ENA

int motor2Pin1 = 19; // IN1
int motor2Pin2 = 21; // IN2
int enable2Pin = 18; // ENA

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 255;
int backwardDutyCycle = 90;

const long frontStopDistanceCm = 10;
const long sideCheckDelayMs = 200;
const long turnDurationMs = 750;

bool obstacleDetected = false;
bool isMovingForward = false;

void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // configure LEDC PWM
  ledcAttachChannel(enable1Pin, freq, resolution, pwmChannel);
  ledcAttachChannel(enable2Pin, freq, resolution, pwmChannel + 1);

  Serial.begin(115200);
  SerialBT.begin("ESP32");

  stopMotors(); // Initialize motors to stop
}

void loop() {
  long frontDistanceCm = ultrasonicFront.read(); // Measure the distance with the front sensor
  Serial.print("Front Distance: ");
  Serial.print(frontDistanceCm);
  Serial.println(" cm");

  if (frontDistanceCm <= frontStopDistanceCm && isMovingForward && !obstacleDetected) {
    obstacleDetected = true;
    stopMotors();
    delay(500); // Give time for motors to stop

    long leftDistanceCm = ultrasonicLeft.read();
    Serial.print("Left Distance: ");
    Serial.print(leftDistanceCm);
    Serial.println(" cm");
    delay(sideCheckDelayMs);

    long rightDistanceCm = ultrasonicRight.read();
    Serial.print("Right Distance: ");
    Serial.print(rightDistanceCm);
    Serial.println(" cm");

    if (leftDistanceCm >= rightDistanceCm) {
      Serial.println("Turning Left.");
      TurnLeft();
      delay(turnDurationMs);
      stopMotors();
      delay(500);
      moveForward(); // Continue forward after turning
    } else {
      Serial.println("Turning Right.");
      TurnRight();
      delay(turnDurationMs);
      stopMotors();
      delay(500);
      moveForward(); // Continue forward after turning
    }
    obstacleDetected = false; // Reset the obstacle detected flag
  } else if (frontDistanceCm <= frontStopDistanceCm) {
    Serial.println("Obstacle detected! Stopping motors.");
    stopMotors();
    obstacleDetected = false; // Reset the flag in case it was set previously
  } else {
    if (SerialBT.available()) {
      char command = SerialBT.read();
      Serial.print("Received command: ");
      Serial.println(command);

      switch (command) {
        case 'F':
          moveForward();
          isMovingForward = true; // Set the flag when moving forward
          obstacleDetected = false;
          break;
        case 'B':
          moveBackward();
          isMovingForward = false; // Reset the flag
          obstacleDetected = false;
          break;
        case 'S':
          stopMotors();
          isMovingForward = false; // Reset the flag
          obstacleDetected = false;
          break;
        case 'L':
          TurnLeft();
          delay(1000); // Turn for 1 second
          stopMotors(); // Stop after turning
          isMovingForward = false; // Reset the flag
          obstacleDetected = false;
          break;
        case 'R':
          TurnRight();
          delay(1000); // Turn for 1 second
          stopMotors(); // Stop after turning
          isMovingForward = false; // Reset the flag
          obstacleDetected = false;
          break;
        default:
          Serial.println("Unknown command!");
          break;
      }
    }
  }
  delay(100); // Small delay for sensor readings and Bluetooth
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
  ledcWrite(enable1Pin, dutyCycle); // Apply speed to the turning motors
  ledcWrite(enable2Pin, dutyCycle);
}

void TurnRight() {
  Serial.println("Turn Right");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  ledcWrite(enable1Pin, dutyCycle); // Apply speed to the turning motors
}