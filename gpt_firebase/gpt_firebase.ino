#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Firebase credentials
#define FIREBASE_HOST "https://testing2-4f7dd-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyDtMLCu_XUJUviuXjc0fZ8D_7kkoK9Pj_Q"  // Leave this empty if your database has no security rules (not recommended for production)

// Wi-Fi credentials
#define WIFI_SSID "Bilal"
#define WIFI_PASSWORD "spothot20101"

// BMI entry value
float bmi = 18.5;

void setup() {
  Serial.begin(9600);
  
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Store BMI entry in the database
  storeBmiEntry(bmi);
}

void loop() {
  // Your code here (if any)
}

void storeBmiEntry(float bmiValue) {
  // Prepare the path for the BMI entry
  String path = "/bmi/";

  // Convert BMI value to a String for storage in Firebase
  String bmiString = String(bmiValue, 2); // 2 decimal places

  // Add the current timestamp as a child node to make the entry unique
  String rfid = "100867620";

  // Store the BMI value under the generated path
  if (Firebase.setString(path, bmiString)) {
    Serial.println("BMI entry stored successfully.");
  } else {
    Serial.println("Failed to store BMI entry.");
  }
}
