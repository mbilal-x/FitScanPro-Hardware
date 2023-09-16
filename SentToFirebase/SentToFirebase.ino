#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FirebaseArduino.h>
// Constants for Wi-Fi AP mode
const char* apSSID = "WemosAP";
const char* apPassword = "";
// Constants for Firebase
const char* firebaseHost = "iotproject101-5209a-default-rtdb.firebaseio.com";
const char* firebaseAuth = "uiJtqLnjsRd5KvhO3FibtSQfDmzUt99EeXvRLPea";
// Create an instance of the web server
ESP8266WebServer server(80);
// Variables to store Wi-Fi credentials
String wifiSSID;
String wifiPassword;
int fireStatus = 3;
bool wificonnect = 0;
// Handle root URL
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Wi-Fi Configuration</h1>";
  html += "<form method='POST' action='/save'>";
  html += "SSID: <input type='text' name='ssid'><br>";
  html += "Password: <input type='password' name='password'><br>";
  html += "<input type='submit' value='Save'>";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}
// Handle form submission
void handleSave() {
  // Connect to Firebase
server.send(200, "text/plain", "Wi-Fi credentials saved and connected to
network.");
Serial.println("Connected To WiFi");
wifiSSID = server.arg("ssid");
wifiPassword = server.arg("password");
// Connect to user's Wi-Fi network
WiFi.mode(WIFI_STA);
WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
// Wait for Wi-Fi connection
while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
}
// Connect to Firebase
Firebase.begin(firebaseHost, firebaseAuth);
Serial.println("Firebase Connected");

wificonnect=1;
}
void setup() {
  pinMode(D6, OUTPUT);
  Serial.begin(115200);
  delay(1000);
  // Set up Wi-Fi AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword);
  // Initialize the web server
  server.on("/", handleRoot);
  server.on("/save", handleSave);
  // Start the server
  server.begin();

  Serial.println();
  Serial.println("Wi-Fi Configuration");
  Serial.println("--------------------");
  Serial.print("Access Point SSID: ");
  Serial.println(apSSID);
  Serial.print("Access Point Password: ");
  Serial.println(apPassword);
  Serial.print("Access Point IP Address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println();
  Serial.println("Ready.");
}
bool ledstatus;
void loop() {
  server.handleClient();
  if (wificonnect) {
    fireStatus = Firebase.getBool("light1/light1");
    if (fireStatus == 1 && ledstatus == 0) {  // compare the input
      of led status received from firebase
        Serial.println("Led Turned ON");
      ledstatus = 1;
      digitalWrite(D6, HIGH);
      // make external led ON
    } else if (fireStatus == 0 & ledstatus == 1) {
      ledstatus = 0;
      // compare the input of led status received from firebase
      Serial.println("Led Turned OFF");
      digitalWrite(D6, LOW);
      // make external led OFF
    } else if (fireStatus != 0 && fireStatus != 1) {
      Serial.print("Command Error! Please send ON/OFF Received:");
    }
  }
}