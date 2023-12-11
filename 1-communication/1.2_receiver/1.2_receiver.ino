// Receiver Arduino Code for height weight and calculating BMI
// And adding all other stuff in next version 1.2 FOLDER

// uuuuu
  float standHeight = 177.8;   //for 5feet 10inch stand just for testing agains my height
  float measuredDistance = 0;
  float personHeight = 0;
  float personHeightInMeter = 0;
  float bmi = 0;


void setup() {
  Serial.begin(9600); // Initialize serial communication

}

void loop() {
  if (Serial.available() > 0) {
    // Read the data until a newline character is received
    String data = Serial.readStringUntil('\n');

    // Parse the received data
    float receivedHeight = data.substring(0, data.indexOf(',')).toFloat();
    int receivedWeight = data.substring(data.indexOf(',') + 1).toInt();

// uuuuuu
    personHeight = standHeight - receivedHeight;
    personHeightInMeter = personHeight / 100;
    Serial.print("Person's height in feet: ");
    Serial.print(personHeight * 0.0328);
    Serial.print("\n");

    Serial.print("Received Weight: ");
    Serial.println(receivedWeight);

    bmi = receivedWeight / (personHeightInMeter * personHeightInMeter);
    Serial.print("bmi: ");
    Serial.println(bmi);

  }
}
