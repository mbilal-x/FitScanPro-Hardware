
/* the following are the UIDs of the card which are authorised
    to know the UID of your card/tag use the example code 'DumpInfo'
    from the library mfrc522 it give the UID of the card as well as 
    other information in the card on the serial monitor of the arduino*/
//bilal's white Card UID: A0 86 76 20
//bilal's blue keychain UID: 83 42 AC 1D

// uuuuu
#include <NewPing.h>
// lllll
#include "HX711.h"

#include <SPI.h>
#include <MFRC522.h>

// uuuuu
// #define TRIGGER_PIN  D1  // Arduino pin tied to trigger pin on the ultrasonic sensor.
// #define ECHO_PIN     D2  // Arduino pin tied to echo pin on the ultrasonic sensor.
// #define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
// lllll
byte pinData = D2;
byte pinClk = D3;
HX711 scale;
//Parameter to calibrate weight and sensor, different for different sensors
float calibration_factor = 14450; //14255 


#define SS_PIN D8
#define RST_PIN D3
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
 
MFRC522::MIFARE_Key key;

// uuuuu
// float stand_height = 177.8;   //for 5feet 10inch stand just for testing agains my height
// float measured_distance = 0;
// float person_height = 0;
// uuuuu
// NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init RC522 
  // lllll
  scale.begin(pinData, pinClk);
  //apply the calibration
  scale.set_scale();
  //initializing the tare. 
  scale.tare();	//Reset the scale to 0gz
}
 
void loop() {

  // lllll
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Weight: ");
  Serial.print(scale.get_units(), 2);
  Serial.print(" kgs"); //Change this to grams and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.println();
  delay(1000);

//  uuuuu
  // delay(1000);  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  // Serial.print("Ping: ");
  // Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
  // Serial.println("cm\n");
  // measured_distance =  sonar.ping_cm();
  // person_height = stand_height - measured_distance;
//  Serial.print("Person's height: ");
//  Serial.print(person_height);
//  Serial.print("\n");
  // Serial.print("Person's height in feet: ");
  // Serial.print(person_height * 0.0328);
  // Serial.print("\n");


  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
 
  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
 
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
 
  Serial.print(F("RFID Tag UID:"));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println("");
 
  rfid.PICC_HaltA(); // Halt PICC
}
 
//Routine to dump a byte array as hex values to Serial. 
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX); 
  }
}
