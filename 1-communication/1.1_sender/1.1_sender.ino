// Sender Arduino Code

void setup() {
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  float height = 1.7;  // Example value for height in meters
  int weight = 60;    // Example value for weight in kg

  // Sending data as a string in the format "height,weight"
  Serial.print(height);
  Serial.print(",");
  Serial.println(weight);

  delay(1000); // Delay for readability and to avoid data flooding
}
