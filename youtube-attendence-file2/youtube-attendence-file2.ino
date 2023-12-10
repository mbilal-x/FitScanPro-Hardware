#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <SPI.h> 
#include <RFID.h>
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <NTPClient.h>
#include <WiFiUdp.h>

#define FIREBASE_HOST "testing2-4f7dd-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "AIzaSyDtMLCu_XUJUviuXjc0fZ8D_7kkoK9Pj_Q"
RFID rfid(D8, D4);       //D8:pin of tag reader SDA. D0:pin of tag reader RST 
unsigned char str[MAX_LEN]; //MAX_LEN is 16: size of the array 

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 18000; //(UTC+5)
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const char ssid[] = "HUAWEI-wzJd";
const char pass[] = "Anw98JEj";
// const char ssid[] = "SHO Gulbanop";
// const char pass[] = "@minecraft@";

String uidPath= "/";
//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseJson json;
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
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\n connected!");
}

void setup()
{

  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  SPI.begin();
  rfid.init();
  
  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);
  connect();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}


//Function to check if an identified tag is registered to allow access
void addEntry (String temp)    
{
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
      
          alertMsg="CHECKING IN";
          Serial.println(alertMsg);
          delay(1000);

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
          delay(1000);
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
    }
}
void loop() {
  timeClient.update();
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)   //Wait for a tag to be placed near the reader
  {
    Serial.println("Card found"); 
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

  delay(500);
}