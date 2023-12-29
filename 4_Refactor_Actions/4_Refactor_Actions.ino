// firebase
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#include <Wire.h> 

// LCD
#include <LiquidCrystal_I2C.h>

// rfid
#include <SPI.h> 
#include <RFID.h>

// time server
#include <WiFiUdp.h>
#include <NTPClient.h>

// firebase
// hassaan's firebase link
// #define FIREBASE_HOST "iotproject-a16f1-default-rtdb.firebaseio.com"
// #define FIREBASE_AUTH "AIzaSyBs-ygCraphLawYvpQgSbb_p_LmH9CLUJ4"
// bilal's firebase link
#define FIREBASE_HOST "testing2-4f7dd-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "AIzaSyDtMLCu_XUJUviuXjc0fZ8D_7kkoK9Pj_Q"

// rfid
RFID rfid(D8, D4);       //D8:pin of tag reader SDA. D0:pin of tag reader RST 
unsigned char str[MAX_LEN]; //MAX_LEN is 16: size of the array 

// time
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 18000; //(UTC+5)
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// lcd
LiquidCrystal_I2C lcd(0x27,16,4);  // set the LCD address to 0x27 or 0x20

// uuuuu
  float standHeight = 177.8;   //for 5feet 10inch stand just for testing agains my height
  float measuredDistance = 0;
  float personHeight = 0;
  float personHeightInMeter = 0;

// bmi
  float receivedHeight = 0;
  float personWeight = 0;
  float bmi = 0;

// firebase
String uidPath= "/";
//Define FirebaseESP8266 data object
FirebaseData firebaseData;
// FirebaseJson json;
FirebaseJson updateData;
// firebase
unsigned long lastMillis = 0;
String alertMsg;
boolean checkIn = true;

// time
  //Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
String currentDate = "";

// firebase
// bilal's wifi
const char ssid[] = "HUAWEI-wzJd";
const char pass[] = "Anw98JEj";
// firebase
// mushi's hotspot
// const char ssid[] = "SHO Gulbano";
// const char pass[] = "@minecraft@";
// bilal's mobile hotspot
// const char ssid[] = "redmi 8";
// const char pass[] = "spothot20101";
// bilal's PC hotspot
// const char ssid[] = "bilal";
// const char pass[] = "12345678";
// Generic hotspot
// const char ssid[] = "wifi";
// const char pass[] = "password";


// wifi
// wifi connect function
void connect() {
  Serial.print("checking wifi...");
  // lcd
  lcd.clear();
  lcd.setCursor(0,0);   
  lcd.print("checking wifi...");
  // firebase
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println("\n connected!");
  // lcd message 
  lcd.clear();
  lcd.setCursor(0,1);   
  lcd.print("CONNECTED");
}

// function for time utilities
String getTime(){
  // time
        timeClient.begin();
        timeClient.setTimeOffset(utcOffsetInSeconds);
        // time
        timeClient.update();  
        // time calculations
        time_t epochTime = timeClient.getEpochTime();
        struct tm *ptm = gmtime ((time_t *)&epochTime);
        int monthDay = ptm->tm_mday;
        int currentMonth = ptm->tm_mon+1;
        String currentMonthName = months[currentMonth-1];
        int currentYear = ptm->tm_year+1900;
        //Print complete date:
        currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
        Serial.print("Current date: ");
        Serial.println(currentDate);

        return currentDate;
}

// function to sync the data transfer
void clearSerialBuffer() {
  while (Serial.available() > 0) {
    char incomingByte = Serial.read(); // Read and discard each byte
  }
}

// SETUP SETUP SETUP SETUP
void setup() {
  Serial.begin(9600); // Initialize serial communication

  // wifi
    WiFi.begin(ssid, pass);

  // rfid
    SPI.begin();
    rfid.init();

  // lcd
    lcd.init();                      // initialize the lcd 
    lcd.clear();
    lcd.backlight();
  
    // wifi 
    connect();  // calling wifi connect function

  // firebase
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
}

//  LOOP LOOP LOOP LOOP
void loop() {

  // calling clearSerialBuffer function
  clearSerialBuffer();

  // tap card message
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("WELCOME TO");
  lcd.setCursor(0,1);
  lcd.print("FITSCANPRO");
  lcd.setCursor(-4,3);
  lcd.print("[SCAN YOUR CARD]");

  // condition 1 for rfid detection
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)   //Wait for a tag to be placed near the reader
  {
    
    String temp = "";                             //Temporary variable to store the read RFID number
    if (rfid.anticoll(str) == MI_OK)              //Anti-collision detection, read tag serial number 
    {
      // Serial.print("The card's ID number is : "); 
      for (int i = 0; i < 4; i++)                 //Record and display the tag serial number 
      { 
        temp = temp + (0x0F & (str[i] >> 4)); 
        temp = temp + (0x0F & str[i]); 
      }
      // Serial.println (temp);
      Serial.println("Card Detected"); 
      // lcd
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("CARD DETECTED!");
      delay(1000);
      lcd.setCursor(-4,2);   
      lcd.print("CHECKING");
      lcd.setCursor(-4,3);   
      lcd.print("MEMBERSHIP");

      // condition 2 rfid Authentication
      if(Firebase.getInt(firebaseData, uidPath+"/users/"+temp)){


        // condition 3 for getting and calculating data from sensors on Wemos D1R1
        if (Serial.available() > 0) {
          // Read the data until a newline character is received
          String data = Serial.readStringUntil('\n');

          // Parse the received data
          receivedHeight = data.substring(0, data.indexOf(',')).toFloat();
          personWeight = data.substring(data.indexOf(',') + 1).toInt();

          // uuuuuu
          personHeight = standHeight - receivedHeight;
          personHeightInMeter = personHeight / 100;
          float personHeightInFeet = personHeight * 0.0328;
          int feet = int(personHeightInFeet);
          int inches = int((personHeightInFeet - feet) * 12);
          // Serial.print("Person's height in feet: ");
          // Serial.print(feet, "feet", inches, "inches");
          // height
          Serial.print("Received Height: ");
          Serial.println(personHeight);
          // weight
          Serial.print("Received Weight: ");
          Serial.println(personWeight);
          // bmi
          bmi = personWeight / (personHeightInMeter * personHeightInMeter);
          Serial.print("bmi: ");
          Serial.println(bmi);

          // lcd
          lcd.clear();
          // display weight
            lcd.setCursor(0,0); 
            lcd.print("WEIGHT: ");
            lcd.print(personWeight);
          // display height
            lcd.setCursor(0,1); 
            lcd.print("HEIGHT: ");
            lcd.print(feet);
            lcd.print("ft");
            lcd.print(inches);
            lcd.print("in");
          // display bmi
            lcd.setCursor(-4,2); 
            lcd.print("BMI: ");
            lcd.print(bmi);
          delay(5000);
        }
        else{
          Serial.print("Serial Error!!!");
        }

        currentDate = getTime();
        updateData.set("date", String(currentDate));
        updateData.set("time", String(timeClient.getFormattedTime()));
        updateData.set("uid", temp);
        updateData.set("height", personHeight);
        updateData.set("weight", personWeight);
        updateData.set("bmi", bmi);

      
          if (Firebase.updateNode(firebaseData, uidPath+ "/users/"+temp, updateData)){
            Serial.println(firebaseData.dataPath() +"/"+ firebaseData.pushName()); 
          } else {
            Serial.println(firebaseData.errorReason());
          }

          // lcd
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,0); 
          lcd.print("[SAVED]");
          delay(1000);
          lcd.clear();
    }
    else
    {
      Serial.println("REASON: " + firebaseData.errorReason());
      // lcd
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("REQUEST FAILED!");
      delay(3000);
    }
    }
    rfid.selectTag(str); //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
    
  }
  delay(1500);
  rfid.halt();
}
