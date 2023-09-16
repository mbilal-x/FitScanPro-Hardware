//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
// ccccc
char mystring[20];
void setup()
{
// ccccc
Serial.begin(9600);


  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("________________");
  lcd.setCursor(5,1);
  lcd.print("Fitness");
  lcd.setCursor(3,2);
  lcd.print("Gym");
  lcd.setCursor(-4,3);
  lcd.print("________________");
}


void loop()
{
// cccc
delay(1000);
Serial.read(mystring,15);  //Read the serial data
Serial.println(mystring);
Serial.println(" RECEIVER"); 
// lcd

delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Data from D1R2:");
  lcd.setCursor(0,1);
  lcd.print(mystring);

}
