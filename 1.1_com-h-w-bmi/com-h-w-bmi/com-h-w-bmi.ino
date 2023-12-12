// Sender Arduino Code to send height, weight and bmi

// uuuuu
#include <NewPing.h>

// uuuuu
#define TRIGGER_PIN  D7  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     D6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// load cell
byte pinData = D2;
byte pinClk = D3;
HX711 scale;
// load cell => default weight for testing 
// float weight_in_kilogram = 60;
 //Parameter to calibrate weight
float calibration_factor = 14450; //14255 


// uuuuu
float stand_height = 177.8;   //for 5feet 10inch stand just for testing agains my height
float measured_distance = 0;
float person_height = 0;
float person_height_in_meter = 0;
float bmi = 0;
// uuuuu
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// weight
float weight = 60; 

void setup() {
  Serial.begin(9600); // Initialize serial communication

  // load cell
  scale.begin(pinData, pinClk);
  //apply the calibration
  scale.set_scale();
  //initializing the tare. 
  scale.tare();	//Reset the scale to 0gz

}

void loop() {

  //  uuuuu
  delay(1000);  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  measured_distance =  sonar.ping_cm();

  // load cell
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  // Serial.print(scale.get_units(), 2);


  Serial.print(measured_distance);
  Serial.print(",");
  Serial.println(scale.get_units(), 2);

  delay(1000); // Delay for readability and to avoid data flooding
}
