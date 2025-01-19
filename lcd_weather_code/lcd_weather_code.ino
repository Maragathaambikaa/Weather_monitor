#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>  // For parsing JSON data

// LCD Pins
LiquidCrystal lcd(D1, D2, D3, D4, D5, D6);

// WiFi Credentials
const char* ssid = "SECE_WIFI";  // Replace with your WiFi SSID
const char* password = "Sece@123";  // Replace with your WiFi password

// Weather API Configuration
const char* weatherAPI = "http://api.openweathermap.org/data/2.5/weather?q=Coimbatore&appid=7f76582998c33ba827e60c88220b44f9&units=metric";  // Fetching for New York

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(115200);
  
  // Initialize LCD
  lcd.begin(20, 4);
  lcd.print("Connecting to WiFi");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.print(".");
  }
  lcd.clear();
  lcd.print("WiFi connected");
  Serial.print("Connected to WiFi, IP address: ");
  Serial.println(WiFi.localIP());

  // Fetch and display weather data
  fetchWeatherData();
}

void loop() {
  // Update the weather data every 10 minutes
  delay(600000);  // 10 minutes in milliseconds
  fetchWeatherData();
}

void fetchWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(weatherAPI);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      String payload = http.getString();
      lcd.clear();
      
      // Parse JSON
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      
      // Extract weather data
      String city = doc["name"].as<String>();
      float temp = doc["main"]["temp"].as<float>();
      float humidity = doc["main"]["humidity"].as<float>();
      float windSpeed = doc["wind"]["speed"].as<float>();

      // Display weather data
      lcd.setCursor(0, 0);
      lcd.print("City : " + city);
      lcd.setCursor(0, 1);
      lcd.print("Temp : " + String(temp) + " Celsius");
      lcd.setCursor(0, 2);
      lcd.print("Hum  : " + String(humidity) + " %");
      lcd.setCursor(0, 3);

      
      lcd.print("Wind : " + String(windSpeed) + " m/s");

    } else {
      lcd.clear();
      lcd.print("Error fetching data");
    }
    http.end();
  } else {
    lcd.clear();
    lcd.print("WiFi not connected");
  }
}
