#include <Arduino.h>

// Analog input pin for variable resistor
#define POT_PIN A0

// Motor driver pins
#define IN1 12       // Direction pin 1
#define IN2 13       // Direction pin 2
#define ENA 14       // PWM pin (connected to ENA of motor driver)

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  analogWriteFreq(5000); // Optional: Set PWM frequency to 5kHz
}

void loop() {
  int potValue = analogRead(POT_PIN); // 0 to 1023
  float voltage = potValue * (3.3 / 1023.0); // Convert to voltage

  Serial.print("Analog: ");
  Serial.print(potValue);
  Serial.print(" -> Voltage: ");
  Serial.println(voltage);

  if (voltage > 3.0) {
    Serial.println("→ Forward");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 512);  // 50% duty cycle
  }
  else if (voltage < 0.5) {
    Serial.println("← Reverse");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 307);  // 30% duty cycle
  }
  else {
    Serial.println("⏹ Stop");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);    // Stop motor
  }

  delay(100);
}
