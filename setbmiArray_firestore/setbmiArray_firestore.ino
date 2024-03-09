
// This example shows how to create a document in a document collection. This operation required Email/password, custom or OAUth2.0 authentication.

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Danyal"
#define WIFI_PASSWORD "12349999"

/* 2. Define the API Key */
#define API_KEY "AIzaSyDtMLCu_XUJUviuXjc0fZ8D_7kkoK9Pj_Q"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "testing2-4f7dd"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "muhammadbx4@gmail.com"
#define USER_PASSWORD "123456"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;

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

void setup() {

  Serial.begin(115200);

  // connecting to wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);


  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

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

  // For sending payload callback
  // config.cfs.upload_callback = fcsUploadCallback;

  // You can use TCP KeepAlive in FirebaseData object and tracking the server connection status, please read this for detail.
  // https://github.com/mobizt/Firebase-ESP-Client#about-firebasedata-object
  // fbdo.keepAlive(5, 5, 1);
}

void loop() {
  // Firebase.ready() should be called repeatedly to handle authentication tasks.

  if (Firebase.ready() && (millis() - dataMillis > 5000 || dataMillis == 0)) {
    dataMillis = millis();
    count++;

    // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create_Edit_Parse/Create_Edit_Parse.ino
    FirebaseJson content;

    // aa is the collection id, bb is the document id.
    // memberID = LKlFqggl1I1Ksdv7vv6d
    String documentPath = "bmiCollection/LKlFqggl1I1Ksdv7vv6d";

    // array
    content.set("fields/bmiArray/arrayValue/values/[0]/integerValue", "20");
    // timestamp
    // content.set("fields/Timestamp/timestampValue", "2014-10-02T15:01:23Z"); // RFC3339 UTC "Zulu" format

    // update
    if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), "bmiArray")) {
      Serial.printf("ok patched\n%s\n\n", fbdo.payload().c_str());
      return;
    } else {
      Serial.println(fbdo.errorReason());
    }
  }
}