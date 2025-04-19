#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define WIFI_SSID "WIFI NAME"
#define WIFI_PASSWORD "PASSWORD"

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (!isnan(temp) && !isnan(hum)) {
      HTTPClient http;
      http.begin(client, "http://XXX.XXX.XXX.XX:XXXX//data");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String postData = "temperature=" + String(temp) + "&humidity=" + String(hum);
      int httpResponseCode = http.POST(postData);

      Serial.print("POST Status: ");
      Serial.println(httpResponseCode);

      http.end();
    } else {
      Serial.println("Failed to read from DHT");
    }
  } else {
    Serial.println("WiFi not connected");
  }

  delay(10000);
}
