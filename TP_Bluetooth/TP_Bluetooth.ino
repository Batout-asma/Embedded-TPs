#include <BluetoothSerial.h>

BluetoothSerial SerialBT;    

void setup() {
  Serial.begin(115200);       
  SerialBT.begin("ESP32");
  Serial.println("Bluetooth device is ready to pair");
}

void loop() {
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    Serial.print("Received: ");
    Serial.println(incomingChar);
  }
  delay(100);
}
