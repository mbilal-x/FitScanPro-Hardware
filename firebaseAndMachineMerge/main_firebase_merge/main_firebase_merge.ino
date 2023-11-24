// Some of the RFID codes which we have for testing (now with sir)
// bilal's white Card UID: A0 86 76 20
// bilal's blue keychain UID: 83 42 AC 1D

// firebase
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
FirebaseData firebaseData;
String send_data = "OFF";
String read_data = "";

// ultrasonic
#include <NewPing.h>

// load cell
#include "HX711.h"
#include <SPI.h>
#include <MFRC522.h>

// firebase

// hassaan's firebase link
#define FIREBASE_HOST "iotproject-a16f1-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyBs-ygCraphLawYvpQgSbb_p_LmH9CLUJ4"

// my testing2 firebase realtime database link
// #define FIREBASE_HOST "testing2-4f7dd-default-rtdb.firebaseio.com"
// #define FIREBASE_AUTH "AIzaSyDtMLCu_XUJUviuXjc0fZ8D_7kkoK9Pj_Q"



#define WIFI_SSID "HUAWEI-wzJd"                                          
#define WIFI_PASSWORD "Anw98JEj"  


// ultrasonic
#define TRIGGER_PIN  D1  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     D0  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// load cell
byte pinData = D2;
byte pinClk = D3;
HX711 scale;
// load cell => default weight for testing 
float weight_in_kilogram = 60; //Parameter to calibrate weight
float calibration_factor = 14450; //14255 

// rfid
#define SS_PIN D8
#define RST_PIN D4
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

// ultrasonic
float stand_height = 177.8;   //for 5feet 10inch stand just for testing agains my height
float measured_distance = 0;
float person_height = 0;
float person_height__in_meter = 0;
float bmi = 0;
// ultrasonic
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


void setup() { 
  Serial.begin(9600);

// rfid
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init RC522 

// firebase
  pinMode(LED_BUILTIN, OUTPUT);         
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);       
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());            
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   
  Firebase.reconnectWiFi(true);
  Serial.println();       
  delay(1000);


// load cell => (one) stuff turned off for testing code without loadcell at home.

  // // load cell
  // scale.begin(pinData, pinClk);
  // //apply the calibration
  // scale.set_scale();
  // //initializing the tare. 
  // scale.tare();	//Reset the scale to 0gz


}

void loop() {




// load cell => (two) stuff turned off for testing code without loadcell at home.

  // // load cell
  // scale.set_scale(calibration_factor); //Adjust to this calibration factor
  // Serial.print("Weight: ");
  // Serial.print(scale.get_units(), 2);
  // Serial.print(" kgs"); //Change this to grams and re-adjust the calibration factor if you follow SI units like a sane person
  // Serial.println();
  // delay(1000);


// dummy load cell value generated and utilized
  Serial.print("Weight: ");
  Serial.print(weight_in_kilogram);
  Serial.print(" kgs"); //Change this to grams and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.println();

// ultrasonic
  delay(1000);  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
  Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm\n");
  measured_distance =  sonar.ping_cm();
  person_height = stand_height - measured_distance;
  person_height__in_meter = person_height / 100;
//  Serial.print("Person's height: ");
//  Serial.print(person_height);
//  Serial.print("\n");
  Serial.print("Person's height in feet: ");
  Serial.print(person_height * 0.0328);
  Serial.print("\n");

  // bmi logic 
  float BMI = (weight_in_kilogram)/pow(person_height__in_meter, 2);
  Serial.print("BMI: ");
  Serial.print(BMI);
  Serial.print("\n");

  delay(5000);


  // firebase
  if(send_data == "ON")
    send_data = "OFF";
  else
    send_data = "ON";

if (Firebase.setString(firebaseData, "/data", send_data)) {   
  
               Serial.print("data ");
               Serial.print("'");               
               Serial.print(send_data);
               Serial.print("'");                  
               Serial.println(" Uploaded Successfully");
             
     }

else {        
    Serial.println(firebaseData.errorReason());
  }

delay(1000);

if (Firebase.getString(firebaseData, "/data")) {   

      read_data = firebaseData.stringData();
      Serial.print("Read_data = ");     
      Serial.println(read_data);

  if (read_data == "ON") 
  {                                 
    Serial.println("LED turned ON");                                                        
    digitalWrite(LED_BUILTIN, LOW);    // LED active low 
  } 
  else if (read_data == "OFF") 
  {                                            
    Serial.println("LED turned OFF");
    digitalWrite(LED_BUILTIN, HIGH);          
  }
    Serial.println();
  } 
  else {
    Serial.println(firebaseData.errorReason());
  }
      delay(3000);  


// rfid
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.print(F("RFID Tag UID:"));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println("");
  rfid.PICC_HaltA(); // Halt PICC
}
 // void loop ends

// rfid
void printHex(byte *buffer, byte bufferSize) { //Routine to dump a byte array as hex values to Serial. 
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX); 
  }
}

