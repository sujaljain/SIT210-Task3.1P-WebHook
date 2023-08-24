#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>  // ThingSpeak library

#define DHTPIN 2       // Pin connected to DHT11 data pin
#define DHTTYPE DHT11  // Type of DHT sensor

char ssid[] = "Sujal Jain";     // WiFi SSID
char pass[] = "12345678"; // WiFi Password

unsigned long channelID = 2250487;         // Your ThingSpeak Channel ID
const char *apiKey = "0BF37U5HSA2OT55X";   // Your ThingSpeak API Key

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  connectWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  delay(2000);  // Wait for a few seconds between readings

  float temperature = dht.readTemperature();     // Read temperature in Celsius
  float humidity = dht.readHumidity();           // Read humidity

  if (!isnan(temperature) && !isnan(humidity)) {
    sendDataToThingSpeak(temperature, humidity);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }
  
  delay(30000); // Wait for 30 seconds
}

void connectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println();
    Serial.print("Connecting to WiFi...");\
    Serial.println();
    WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

void sendDataToThingSpeak(float temp, float humidity) {
  ThingSpeak.setField(1, temp);       // Set field 1 with temperature data
  ThingSpeak.setField(2, humidity);   // Set field 2 with humidity data
  
  int httpCode = ThingSpeak.writeFields(channelID, apiKey);

  if (httpCode == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Error sending data to ThingSpeak");
  }
}
