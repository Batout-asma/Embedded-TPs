#include <Servo.h>

#define TRIG_PIN  9
#define ECHO_PIN  10 
#define SERVO_PIN 7
#define DISTANCE_THRESHOLD 25

Servo servo;

long duration;
float distance_cm;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);  
  servo.attach(SERVO_PIN);   
  servo.write(90); 
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 20000);
  
  if (duration > 0) {
    distance_cm = duration * 0.034 / 2;
  } else {
    distance_cm = -1;
  }

  if (distance_cm > 0 && distance_cm < DISTANCE_THRESHOLD) {
    servo.write(180);
    delay(2700);

  } else {
    servo.write(90); 
  }

  Serial.print("Distance: ");
  if (distance_cm > 0) {
    Serial.print(distance_cm);
    Serial.println(" cm");
  } else {
    Serial.println("No object detected");
  }

  delay(500);
}
