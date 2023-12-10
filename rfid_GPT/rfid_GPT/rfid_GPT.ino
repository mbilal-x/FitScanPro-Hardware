#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  SPI.begin();          // Initiate SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Scan your RFID card...");
}

void loop() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Show UID on serial monitor
    Serial.print("Card UID: ");
    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      cardUID += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println(cardUID);

    // Store the card UID in a variable for later use
    processCard(cardUID);

    // Halt until the card is removed
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

void processCard(String uid) {
  // Your code to handle the card UID goes here
  // For example, you can compare it with known UIDs or perform other actions

  // Continue with the rest of your code
  Serial.println("Card detected! Continue with the rest of the code...");
}
