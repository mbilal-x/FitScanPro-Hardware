// firebase
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>

// LCD
#include <LiquidCrystal_I2C.h>

// rfid
#include <SPI.h>
#include <RFID.h>

// wifi
/*  Define the WiFi credentials */
// #define WIFI_SSID "Bilal"
// #define WIFI_PASSWORD "spothot20101"
#define WIFI_SSID "HUAWEI-wzJd"
#define WIFI_PASSWORD "Anw98JEj"
// old code wifi references
// bilal's wifi
// const char ssid[] = "HUAWEI-wzJd";
// const char pass[] = "Anw98JEj";
// mushi's hotspot
// const char ssid[] = "SHO Gulbano";
// const char pass[] = "@minecraft@";
// bilal's mobile hotspot
// const char ssid[] = "redmi 8";
// const char pass[] = "spothot20101";
// bilal's PC hotspot
// const char ssid[] = "bilal";
// const char pass[] = "12345678";

// firebase
// Define the API Key */
#define API_KEY "AIzaSyDtMLCu_XUJUviuXjc0fZ8D_7kkoK9Pj_Q"
// Define the project ID */
#define FIREBASE_PROJECT_ID "testing2-4f7dd"
// Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "muhammadbx4@gmail.com"
#define USER_PASSWORD "123456"


// rfid
RFID rfid(D8, D4);           //D8:pin of tag reader SDA. D0:pin of tag reader RST
unsigned char str[MAX_LEN];  //MAX_LEN is 16: size of the array

// lcd
LiquidCrystal_I2C lcd(0x27, 16, 4);  // set the LCD address to 0x27 or 0x20

// uuuuu
float standHeight = 177.8;  //for 5feet 10inch stand just for testing agains my height
float measuredDistance = 0;
float personHeight = 0;
float personHeightInMeter = 0;

// bmi
float receivedHeight = 0;
float personWeight = 0;
float bmi = 0;

// firebase
// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;



// wifi
// wifi connect function
void connect() {
  Serial.print("checking wifi...");
  // lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("checking wifi...");
  // firebase
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\n connected!");
  // lcd message
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("CONNECTED");
}

// function to sync the data transfer
void clearSerialBuffer() {
  while (Serial.available() > 0) {
    char incomingByte = Serial.read();  // Read and discard each byte
  }
}


// The Firestore payload upload callback function
void fcsUploadCallback(CFS_UploadStatusInfo info) {
  if (info.status == firebase_cfs_upload_status_init) {
    Serial.printf("\nUploading data (%d)...\n", info.size);
  } else if (info.status == firebase_cfs_upload_status_upload) {
    Serial.printf("Uploaded %d%s\n", (int)info.progress, "%");
  } else if (info.status == firebase_cfs_upload_status_complete) {
    Serial.println("Upload completed ");
  } else if (info.status == firebase_cfs_upload_status_process_response) {
    Serial.print("Processing the response... ");
  } else if (info.status == firebase_cfs_upload_status_error) {
    Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
  }
}

// SETUP SETUP SETUP SETUP
void setup() {
  Serial.begin(9600);  // Initialize serial communication

  // connecting to wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // rfid
  SPI.begin();
  rfid.init();

  // lcd
  lcd.init();  // initialize the lcd
  lcd.clear();
  lcd.backlight();

  // wifi
  connect();  // calling wifi connect function

  // firebase
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h
  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);
  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);
}

//  LOOP LOOP LOOP LOOP
void loop() {

  // calling clearSerialBuffer function
  clearSerialBuffer();

  // tap card message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("FITSCANPRO");
  lcd.setCursor(-4, 3);
  lcd.print("[SCAN YOUR CARD]");

  // condition 1 for rfid detection
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)  //Wait for a tag to be placed near the reader
  {

    String temp = "";                 //Temporary variable to store the read RFID number
    if (rfid.anticoll(str) == MI_OK)  //Anti-collision detection, read tag serial number
    {
      // Serial.print("The card's ID number is : ");
      for (int i = 0; i < 4; i++)  //Record and display the tag serial number
      {
        temp = temp + (0x0F & (str[i] >> 4));
        temp = temp + (0x0F & str[i]);
      }
      Serial.println(temp);
      Serial.println("Card Detected");
      // lcd
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CARD DETECTED!");
      delay(1000);
      lcd.setCursor(-4, 2);
      lcd.print("CHECKING");
      lcd.setCursor(-4, 3);
      lcd.print("MEMBERSHIP");

      // condition 2 rfid Authentication
      // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create_Edit_Parse/Create_Edit_Parse.ino
      FirebaseJson content;
      // info is the collection id, countries is the document id in collection info.
      String documentPath = "bmiCollection/"+ temp;
      String mask = "bmiArray";
      // If the document path contains space e.g. "a b c/d e f"
      // It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"
      Serial.print("Get a document... ");
      if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), mask.c_str())){
        Serial.printf("ok member exists...\n%s\n\n", fbdo.payload().c_str());


        // condition 3 for getting and calculating data from sensors on Wemos D1R1
        if (Serial.available() > 0) {
          // Read the data until a newline character is received
          String data = Serial.readStringUntil('\n');

          // Parse the received data
          receivedHeight = data.substring(0, data.indexOf(',')).toFloat();
          personWeight = data.substring(data.indexOf(',') + 1).toInt();

          // uuuuuu
          personHeight = standHeight - receivedHeight;
          personHeightInMeter = personHeight / 100;
          float personHeightInFeet = personHeight * 0.0328;
          int feet = int(personHeightInFeet);
          int inches = int((personHeightInFeet - feet) * 12);
          // Serial.print("Person's height in feet: ");
          // Serial.print(feet, "feet", inches, "inches");
          // height
          Serial.print("Received Height: ");
          Serial.println(personHeight);
          // weight
          Serial.print("Received Weight: ");
          Serial.println(personWeight);
          // bmi
          bmi = personWeight / (personHeightInMeter * personHeightInMeter);
          Serial.print("bmi: ");
          Serial.println(bmi);

          // lcd
          lcd.clear();
          // display weight
          lcd.setCursor(0, 0);
          lcd.print("WEIGHT: ");
          lcd.print(personWeight);
          // display height
          lcd.setCursor(0, 1);
          lcd.print("HEIGHT: ");
          lcd.print(feet);
          lcd.print("ft");
          lcd.print(inches);
          lcd.print("in");
          // display bmi
          lcd.setCursor(-4, 2);
          lcd.print("BMI: ");
          lcd.print(bmi);
          delay(5000);
        } else {
          Serial.print("Serial Error!!!");
        }



        // append data to firestore
        Serial.print("Commit a document (append array)... ");
        // The dyamic array of write object firebase_firestore_document_write_t.
        std::vector<struct firebase_firestore_document_write_t> writes;
        // A write object that will be written to the document.
        struct firebase_firestore_document_write_t transform_write;
        // Set the write object write operation type.
        // firebase_firestore_document_write_type_update,
        // firebase_firestore_document_write_type_delete,
        // firebase_firestore_document_write_type_transform
        transform_write.type = firebase_firestore_document_write_type_transform;
        // Set the document path of document to write (transform)
        transform_write.document_transform.transform_document_path = "bmiCollection/"+temp;
        // Set a transformation of a field of the document.
        struct firebase_firestore_document_write_field_transforms_t field_transforms;
        // Set field path to write.
        field_transforms.fieldPath = "bmi";
        // Set the transformation type.
        // firebase_firestore_transform_type_set_to_server_value,
        // firebase_firestore_transform_type_increment,
        // firebase_firestore_transform_type_maaximum,
        // firebase_firestore_transform_type_minimum,
        // firebase_firestore_transform_type_append_missing_elements,
        // firebase_firestore_transform_type_remove_all_from_array
        field_transforms.transform_type = firebase_firestore_transform_type_append_missing_elements;
        // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create_Edit_Parse/Create_Edit_Parse.ino
        FirebaseJson content;
        content.set("values/[0]/integerValue", static_cast<int>(bmi));
        // Set the transformation content.
        field_transforms.transform_content = content.raw();

        // Add a field transformation object to a write object.
        transform_write.document_transform.field_transforms.push_back(field_transforms);

        // Add a write object to a write array.
        writes.push_back(transform_write);

        if (Firebase.Firestore.commitDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, writes /* dynamic array of firebase_firestore_document_write_t */, "" /* transaction */))
          Serial.printf("ok transformed\n%s\n\n", fbdo.payload().c_str());
        else
          Serial.println(fbdo.errorReason());


        // lcd
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("[SAVED]");
        delay(1000);
        lcd.clear();
      } else {
        Serial.println("REASON: " + fbdo.errorReason());
        Serial.println("user is not a valid member");
        // lcd
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("REQUEST FAILED!");
        delay(3000);
      }
    }
    rfid.selectTag(str);  //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
  }
  delay(1500);
  rfid.halt();
}
