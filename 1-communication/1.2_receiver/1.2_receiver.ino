// Receiver Arduino Code

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

    // Do something with the received data
    // For now, just print it to the Serial Monitor
    Serial.print("Received Height: ");
    Serial.println(receivedHeight);
    Serial.print("Received Weight: ");
    Serial.println(receivedWeight);
  }
}
