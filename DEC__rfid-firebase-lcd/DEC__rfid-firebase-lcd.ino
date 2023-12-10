// firebase
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#include <Wire.h> 

// LCD
#include <LiquidCrystal_I2C.h>

// ultrasonic
#include <NewPing.h>

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

// ultrasonic
// #define TRIGGER_PIN  D1  // Arduino pin tied to trigger pin on the ultrasonic sensor.
// #define ECHO_PIN     D0  // Arduino pin tied to echo pin on the ultrasonic sensor.
// #define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// time
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 18000; //(UTC+5)
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// lcd
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// ultrasonic
float stand_height = 177.8;   //for 5feet 10inch stand just for testing agains my height
float measured_distance = 0;
float person_height = 0;
float person_height__in_meter = 0;
float bmi = 0;
// NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// firebase
// bilal's wifi
const char ssid[] = "HUAWEI-wzJd";
const char pass[] = "Anw98JEj";
// firebase
// mushi's hotspot
// const char ssid[] = "SHO Gulbano";
// const char pass[] = "@minecraft@";
// bilal's hotspot
// const char ssid[] = "redmi 8";
// const char pass[] = "spothot20101";

// firebase
String uidPath= "/";
//Define FirebaseESP8266 data object
FirebaseData firebaseData;
// FirebaseJson json;
FirebaseJson updateData;

unsigned long lastMillis = 0;
String alertMsg;
boolean checkIn = true;

  //Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


void connect() {
  Serial.print("checking wifi...");
  // lcd message 
  lcd.setCursor(1,0);   
  lcd.print("checking wifi..");
  // firebase
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\n connected!");
}

void setup()
{

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

  // calling connect function
  connect();

// firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}


//Function to check if an identified tag is registered to allow access
void addEntry (String temp)    
{
    // card found printed here [and not in the check if statement] to avoid delay
    Serial.println("Card found"); 
    // lcd
    lcd.clear();
    lcd.setCursor(1,0);   
    lcd.print("CARD DETECTED!");
    delay(2000);

// lcd
  lcd.setCursor(1,0);   
  lcd.print("SCAN YOUR RFID");

// time
  time_t epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);

  struct tm *ptm = gmtime ((time_t *)&epochTime);

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  String currentMonthName = months[currentMonth-1];
  Serial.print("Month name: ");
  Serial.println(currentMonthName);

  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

    if(Firebase.getInt(firebaseData, uidPath+"/users/"+temp)){
      
          alertMsg="LOGGING IN";
          // lcd
          lcd.clear();
          lcd.setCursor(2,1);   
          lcd.print(alertMsg);
          Serial.println(alertMsg);
          delay(3000);

          updateData.set("date", String(currentDate));
          updateData.set("time", String(timeClient.getFormattedTime()));
          updateData.set("uid", temp);

          
          if (Firebase.updateNode(firebaseData, uidPath+ "/users/"+temp, updateData)){
            Serial.println(firebaseData.dataPath() +"/"+ firebaseData.pushName()); 
          } else {
            Serial.println(firebaseData.errorReason());
          }

          alertMsg="Saved..";
          Serial.println(alertMsg);
          // lcd
          lcd.clear();
          lcd.setCursor(1,5); 
          lcd.print(alertMsg);
          delay(3000);
          alertMsg="LOGGING OUT";
          // lcd
          lcd.clear();
          lcd.setCursor(2,1);   
          lcd.print(alertMsg);
          delay(3000);
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      // lcd
      lcd.clear();
      lcd.setCursor(1,1); 
      lcd.print("LOGIN FAILED!");
      delay(5000);
    }
}
void loop() {
  timeClient.update();
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)   //Wait for a tag to be placed near the reader
  {
    
    String temp = "";                             //Temporary variable to store the read RFID number
    if (rfid.anticoll(str) == MI_OK)              //Anti-collision detection, read tag serial number 
    { 
      Serial.print("The card's ID number is : "); 
      for (int i = 0; i < 4; i++)                 //Record and display the tag serial number 
      { 
        temp = temp + (0x0F & (str[i] >> 4)); 
        temp = temp + (0x0F & str[i]); 
      } 
      Serial.println (temp);
      addEntry (temp);     //Check if the identified tag is an allowed to open tag
    } 
    rfid.selectTag(str); //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
    
  }
  rfid.halt();

// lcd
  lcd.clear();
  lcd.setCursor(1,0);   
  lcd.print("SCAN YOUR RFID");
  lcd.setCursor(7,1);   
  lcd.print(":)");
  delay(500);
  lcd.clear();


}