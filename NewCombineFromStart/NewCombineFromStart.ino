
// rfid
#include <SPI.h>
#include <RFID.h>


// firebase
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library

// firebase
#define FIREBASE_HOST "testing2-4f7dd-default-rtdb.firebaseio.com"  //Without http:// or https:// schemes
#define FIREBASE_AUTH "AIzaSyDtMLCu_XUJUviuXjc0fZ8D_7kkoK9Pj_Q"
const char ssid[] = "HUAWEI-wzJd";
const char pass[] = "Anw98JEj";
String uidPath = "/";
//Define FirebaseESP8266 data object
FirebaseData firebaseData;


// rfid
RFID rfid(D8, D4);           //D8:pin of tag reader SDA. D0:pin of tag reader RST
unsigned char str[MAX_LEN];  //MAX_LEN is 16: size of the array

// firebase connect function
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\n connected!");
}

// SETUP
void setup() {
  // firebase
  Serial.begin(9600);
  // Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // rfid
  SPI.begin();
  rfid.init();

  // firebase
  connect();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}
// SETUP ENDS


// firebase pushUser function
void pushUser(String temp)  //Function to check if an identified tag is registered to allow access
{
  Serial.println("PUSHING USER ID: " + temp);
  Firebase.setInt(firebaseData, uidPath + "users/" + temp, 0);
}
void loop() {
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)  //Wait for a tag to be placed near the reader
  {
    Serial.println("Card found");
    String temp = "";                 //Temporary variable to store the read RFID number
    if (rfid.anticoll(str) == MI_OK)  //Anti-collision detection, read tag serial number
    {
      Serial.print("The card's ID number is : ");
      for (int i = 0; i < 4; i++)  //Record and display the tag serial number
      {
        temp = temp + (0x0F & (str[i] >> 4));
        temp = temp + (0x0F & str[i]);
      }
      Serial.println(temp);
      pushUser(temp);  //Check if the identified tag is an allowed to open tag
    }
    rfid.selectTag(str);  //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
  }
  rfid.halt();
}