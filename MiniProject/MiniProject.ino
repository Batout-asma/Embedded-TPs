int motor1Pin1 = 2;  // IN1
int motor1Pin2 = 4; // IN2
int enable1Pin = 5; // ENA

int motor2Pin1 = 19;  // IN1
int motor2Pin2 = 21; // IN2
int enable2Pin = 18; // ENA

void setup() {
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
    
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  Serial.begin(115200);

  // Keep motor enabled
  digitalWrite(enable1Pin, HIGH);
  digitalWrite(enable2Pin, HIGH);

}

void loop() {
  // Forward
  Serial.println("Forward M1");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  delay(2000);

  // Stop
  Serial.println("Stop M1");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);

  // Backward
  Serial.println("Backward M1");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  delay(2000);

  // Stop
  Serial.println("Stop M1");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);





  // Forward
  Serial.println("Forward M2");
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(2000);

  // Stop
  Serial.println("Stop M2");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);

  // Backward
  Serial.println("Backward M2");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(2000);

  // Stop
  Serial.println("Stop M2");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);
}
