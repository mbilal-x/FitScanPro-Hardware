#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define FIREBASE_HOST "testing2-4f7dd-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyDtMLCu_XUJUviuXjc0fZ8D_7kkoK9Pj_Q"
#define WIFI_SSID "SHO Gulbano"                                          
#define WIFI_PASSWORD "@minecraft@"    

FirebaseData firebaseData;

String send_data = "OFF";
String read_data = "";

void setup() {

  Serial.begin(9600);                  
  pinMode(LED_BUILTIN, OUTPUT);       
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);       
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());            
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   

  Firebase.reconnectWiFi(true);

  Serial.println();       
  delay(1000);
}

void loop() { 


if(send_data == "ON")
send_data = "OFF";
else
send_data = "ON";

if (Firebase.setString(firebaseData, "/data", send_data)) {   
  
               Serial.print("data ");
               Serial.print("'");               
               Serial.print(send_data);
               Serial.print("'");                  
               Serial.println(" Uploaded Successfully");
             
     }

else {        
    Serial.println(firebaseData.errorReason());
  }

delay(1000);

if (Firebase.getString(firebaseData, "/data")) {   

      read_data = firebaseData.stringData();
      Serial.print("Read_data = ");     
      Serial.println(read_data);

  if (read_data == "ON") 
  {                                 
    Serial.println("LED turned ON");                                                        
    digitalWrite(LED_BUILTIN, LOW);    // LED active low 
  } 
  else if (read_data == "OFF") 
  {                                            
    Serial.println("LED turned OFF");
    digitalWrite(LED_BUILTIN, HIGH);          
  }
    Serial.println();
  } 
  else {
    Serial.println(firebaseData.errorReason());
  }
      delay(3000);  
}