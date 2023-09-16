//Sender Arduino Board Code (Master)
char mystring[15] = "Your bmi is 20";     //String data which is to be sent
void setup() {                
 Serial.begin(9600);                 // Begin the Serial at 9600 Baud rate
}
void loop() {
 Serial.write(mystring,15);             //Write the serial data
 delay(1000);
}

// _________________________________________________________

//Receiver Arduino Board Code
// char mystring[20];               //Initialized variable to store receive
// void setup() {
//  Serial.begin(9600);             // Begin the Serial at 9600 Baud
// }
// void loop() {
//  Serial.read(mystring,15);  //Read the serial data
//  Serial.println(mystring);       //Print data on Serial Monitor
//  Serial.println(" RECEIVER");
//  delay(1000);    
// }