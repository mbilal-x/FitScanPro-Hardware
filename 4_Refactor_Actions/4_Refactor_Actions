// Receiver Arduino Code for height weight and calculating BMI
// And adding all other stuff in next version 1.2 FOLDER

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


// firebase
// bilal's wifi
// const char ssid[] = "HUAWEI-wzJd";
// const char pass[] = "Anw98JEj";
// firebase
// mushi's hotspot
const char ssid[] = "SHO Gulbano";
const char pass[] = "@minecraft@";
// bilal's hotspot
// const char ssid[] = "redmi 8";
// const char pass[] = "spothot20101";

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


// SETUP SETUP SETUP SETUP
void setup() {
  Serial.begin(9600); // Initialize serial communication

  // wifi
    Serial.begin(9600);
    WiFi.begin(ssid, pass);

  // rfid
    SPI.begin();
    rfid.init();

  // lcd
    lcd.init();                      // initialize the lcd 
    lcd.clear();
    lcd.backlight();

  // time
    timeClient.begin();
    timeClient.setTimeOffset(utcOffsetInSeconds);
    
    // wifi
    // calling wifi connect function
    connect();

  // firebase
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
}


// addEntry function to authenticate registered users and send data to firebase
void addEntry (String temp)    
{

  // ALL PRINT STATEMENTS MUTED FOR SERIAL COMMUNICATION

    // card found printed here [and not in the check if statement] to avoid delay
    Serial.println("Card Detected"); 
    // lcd
    lcd.clear();
    lcd.setCursor(0,0);   
    lcd.print("CARD DETECTED!");
    lcd.setCursor(0,1);   
    lcd.print("MEASURING DATA...");

// time
  time_t epochTime = timeClient.getEpochTime();
  // Serial.print("Epoch Time: ");
  // Serial.println(epochTime);

  struct tm *ptm = gmtime ((time_t *)&epochTime);

  int monthDay = ptm->tm_mday;
  // Serial.print("Month day: ");
  // Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  // Serial.print("Month: ");
  // Serial.println(currentMonth);

  String currentMonthName = months[currentMonth-1];
  // Serial.print("Month name: ");
  // Serial.println(currentMonthName);

  int currentYear = ptm->tm_year+1900;
  // Serial.print("Year: ");
  // Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

    if(Firebase.getInt(firebaseData, uidPath+"/users/"+temp)){
  
          alertMsg="LOGGING IN";
          // lcd
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,0);   
          lcd.print(alertMsg);
          Serial.println(alertMsg);

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

          alertMsg="Saved..";
          // Serial.println(alertMsg);

          // lcd
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,0); 
          lcd.print(alertMsg);
          alertMsg="LOGGING OUT";
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,1);   
          lcd.print(alertMsg);
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      // lcd
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("LOGIN FAILED!");
      delay(5000);
    }
}


void loop() {

  // condition 1 for serial com
  if (Serial.available() > 0) {
    // Read the data until a newline character is received
    String data = Serial.readStringUntil('\n');

    // Parse the received data
    receivedHeight = data.substring(0, data.indexOf(',')).toFloat();
    personWeight = data.substring(data.indexOf(',') + 1).toInt();

    // uuuuuu
    personHeight = standHeight - receivedHeight;
    personHeightInMeter = personHeight / 100;
    // Serial.print("Person's height in feet: ");
    // Serial.print(personHeight * 0.0328);
    // Serial.print("\n");

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
      lcd.print(personHeight);
    // display bmi
      lcd.setCursor(-4,2); 
      lcd.print("BMI: ");
      lcd.print(bmi);
    // tap card message
      lcd.setCursor(-4,3); 
      lcd.print("SCAN CARD :)");
      delay(1000);
  }

  timeClient.update();
  // condition 2 for rfid detection
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
      addEntry (temp);  //Check if the identified tag is an allowed and add data to firebase [if yes]
    } 
    rfid.selectTag(str); //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
    
  }
  rfid.halt();

}
