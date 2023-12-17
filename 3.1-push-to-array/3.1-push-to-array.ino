#include <ESP8266WiFi.h>
#include <Wire.h>
#include "FirebaseESP8266.h"

#define FIREBASE_HOST "bmi-gym-testing-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "38450d957cb1ead9af1348db91d40f2cd30c8c20"
#define WIFI_SSID "HUAWEI-wzJd"
#define WIFI_PASSWORD "Anw98JEj"

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void pushDataToFirebase() {
  // Get current timestamp
  String timestamp = String(millis());

  // Create a random BMI value for demonstration purposes
  float randomBMI = random(18.5, 30.0);

  // Create data object
  FirebaseObject dataObj = FirebaseObject();
  dataObj.set("time", timestamp);
  dataObj.set("bmi", randomBMI);

  // Push data object to Firebase array
  Firebase.push("data", dataObj);
}

void loop() {
  // Push new data to Firebase each time the device is restarted or re-uploaded
  pushDataToFirebase();

  delay(5000); // Adjust delay as needed
}