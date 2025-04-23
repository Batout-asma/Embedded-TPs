#include <Arduino.h>

// Motor driver pins
#define IN1 12       // Direction pin 1
#define IN2 13       // Direction pin 2
#define ENA 14       // PWM pin (connected to ENA of motor driver)

// PWM configuration
#define PWM_CHANNEL 0
#define PWM_FREQ 5000         // 5 kHz
#define PWM_RESOLUTION 8      // 8 bits -> values from 0 to 255

void setup() {
  // Set motor direction pins as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  // Optional: Change PWM frequency (default is 1kHz)
  analogWriteFreq(5000); // Set to 5kHz (adjust if needed)
}

void loop() {
  // Forward @ 50% speed
  Serial.println("Forward @ 50%");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 512);  // 50% of 1023 (10-bit PWM)
  delay(500);

  // Stop
  Serial.println("Stop");
  analogWrite(ENA, 0);
  delay(4000);

  // Reverse @ 30% speed
  Serial.println("Reverse @ 30%");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 307);  // 30% of 1023
  delay(500);

// Stop
  Serial.println("Stop");
  analogWrite(ENA, 0);
  delay(4000);

}