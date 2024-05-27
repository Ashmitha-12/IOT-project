// Include necessary libraries
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define DHTPIN 2    
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

// Set our wifi name and password
const char* ssid = "IOT";
const char* password = "Apple@123";

// Your thingspeak channel url with api_key query
String serverName = "https://api.thingspeak.com/update?api_key=PWQUV91Z4CX1G6XP";

// Moisture sensor pin
int moisturePin = 5;

// Define thresholds for rice and wheat storage
float wheatTempMin = 10;
float wheatTempMax = 20;
float wheatHumidityMin = 65;
float wheatHumidityMax = 75;
int wheatMoistureMin = 12;
int wheatMoistureMax = 14;

float riceTempMin = 10;
float riceTempMax = 15;
float riceHumidityMin = 60;
float riceHumidityMax = 70;
int riceMoistureMin = 12;
int riceMoistureMax = 13;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(moisturePin, INPUT);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int moistureValue = digitalRead(moisturePin);
  sendData(t, h, moistureValue); 
  Serial.print(F("Humidity "));
  Serial.print(h);
  Serial.print(F("%  Temperature "));
  Serial.print(t);
  Serial.print(" Moisture Value: ");
  Serial.println(moistureValue);

  // Check conditions for rice storage
  if (t >= riceTempMin && t <= riceTempMax && h >= riceHumidityMin && h <= riceHumidityMax && moistureValue >= riceMoistureMin && moistureValue <= riceMoistureMax) {
    // Control one DC motor for rice storage
    // Code to turn on one DC motor
    digitalWrite(4,HIGH);
    Serial.println("Rice storage conditions met. Turning on one DC motor.");
  }
  else
  {
    digitalWrite(4,LOW);
    Serial.println("Rice storage conditions not  met. Turning off one DC motor.");
  }

  // Check conditions for wheat storage
  if (t >= wheatTempMin && t <= wheatTempMax && h >= wheatHumidityMin && h <= wheatHumidityMax && moistureValue >= wheatMoistureMin && moistureValue <= wheatMoistureMax) {
    // Control two DC motors for wheat storage
    // Code to turn on two DC motors
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
    Serial.println("Wheat storage conditions met. Turning on two DC motors.");
  }
  else
  {
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    Serial.println("Wheat storage conditions not met. Turning off two  DC motor.");
  }

  delay(60000); // Wait for a minute before taking the next reading
}

void sendData(float t, float h, int moistureValue) {
  HTTPClient http;

  String url = serverName + "&field1=" + h + "&field2=" + t + "&field3=" + moistureValue;
  http.begin(url.c_str());
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
