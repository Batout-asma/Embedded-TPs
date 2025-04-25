#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

int motor1Pin1 = 2;  // IN1
int motor1Pin2 = 4;  // IN2
int enable1Pin = 5;  // ENA

int motor2Pin1 = 19; // IN1
int motor2Pin2 = 21; // IN2
int enable2Pin = 18; // ENA

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 255;
int backwardDutyCycle = 120;

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
  SerialBT.begin("ESP32"); // Name of your ESP32 in the Lightblue app
  Serial.println("Bluetooth Device Started! Connect with Lightblue app.");

  // testing
  Serial.println("Testing DC Motors (initial)...");
  stopMotors(); // Initialize motors to stop
}

void loop() {
  if (SerialBT.available()) {
    char command = SerialBT.read();
    Serial.print("Received command: ");
    Serial.println(command);

    switch (command) {
      case 'F':
        moveForward();
        break;
      case 'B':
        moveBackward();
        break;
      case 'S':
        stopMotors();
        break;
      case 'L':
        TurnLeft();
        delay(400);
        stopMotors();
        break;
      case 'R':
        TurnRight();
        delay(400); // Turn for 1 second
        stopMotors();
        break;
      default:
        Serial.println("Unknown command!");
        break;
    }
  }
  delay(20); // Small delay to prevent busy-waiting
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