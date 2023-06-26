//HX711 load cell calibration program code https://www.circuitschools.com
#include "HX711.h"

//data pin and clock pin
byte pinData = D2;
byte pinClk = D3;

HX711 scale;
//Parameter to calibrate weight and sensor, different for different sensors
float calibration_factor = 14450; //14255 

void setup() {
  Serial.begin(9600);
 
  scale.begin(pinData, pinClk);
  //apply the calibration
  scale.set_scale();
  //initializing the tare. 
  scale.tare();	//Reset the scale to 0gz
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Weight: ");
  Serial.print(scale.get_units(), 2);
  Serial.print(" kgs"); //Change this to grams and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.println();
  delay(1000);
}