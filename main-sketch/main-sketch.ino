// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------
//HX711 load cell calibration program code https://www.circuitschools.com
#include "HX711.h" 
#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define powerPin 8


//data pin and clock pin
byte pinData = 3;
byte pinClk = 2;

HX711 scale;
//Parameter to calibrate weight and sensor, different for different sensors
float calibration_factor = 14450; //14255 



//float stand_height = 228.6; // 7.5feet or 7feet 6inch  -stand
float stand_height = 177.8;   //for 5feet 10inch stand just for testing agains my height
float measured_distance = 0;
float person_height = 0;

float weight_reading = 0;
float person_height_meters = 0;
float bmi = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  
  scale.begin(pinData, pinClk);
  //apply the calibration
  scale.set_scale();
  //initializing the tare. 
  scale.tare();	//Reset the scale to 0gz

  pinMode(powerPin, OUTPUT);
}

void loop() {

  digitalWrite(powerPin, HIGH);
  delay(3000);  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  // Serial.print("Ping: ");
  // Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
  // Serial.println("cm\n");
  
  measured_distance =  sonar.ping_cm();
  person_height = stand_height - measured_distance;
//  Serial.print("Person's height: ");
//  Serial.print(person_height);
//  Serial.print("\n");
  Serial.print("Person's height in feet: ");
  Serial.print(person_height * 0.0328);
  Serial.print("\n");

// load cell code
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  weight_reading = (scale.get_units());
  Serial.print("weight reading is : ");
  Serial.print(weight_reading);
  Serial.print("Weight: ");
  Serial.print(scale.get_units(), 2);
  Serial.print(" kgs"); //Change this to grams and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.println();
  delay(1000);

  person_height_meters = person_height*100;
  Serial.print("Person height: ");
  Serial.print(person_height);
  Serial.print("\n");
  bmi = weight_reading / person_height_meters * person_height_meters;  //weight in kg divided by height in meters squared.
  Serial.print("Your BMI is: ");
  Serial.print(bmi, 2);
  
}
