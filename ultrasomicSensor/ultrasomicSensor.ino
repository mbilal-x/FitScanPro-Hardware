#include <NewPing.h>

// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------


#define TRIGGER_PIN  D7  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     D6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

//float stand_height = 228.6; // 7.5feet or 7feet 6inch  -stand
float stand_height = 177.8;   //for 5feet 10inch stand just for testing agains my height
float measured_distance = 0;
float person_height = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  

  
}

void loop() {
  delay(1000);  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
  Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm\n");
  
  measured_distance =  sonar.ping_cm();
  person_height = stand_height - measured_distance;
//  Serial.print("Person's height: ");
//  Serial.print(person_height);
//  Serial.print("\n");
  Serial.print("Person's height in feet: ");
  Serial.print(person_height * 0.0328);
  Serial.print("\n");
  
  
}
