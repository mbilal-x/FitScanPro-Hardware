//Compatible with the Arduino IDE 1.6.5
//Library version:1.1
//   A4 - SCL, A5 - SDA, VCC, GND
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,4);  // set the LCD address to 0x27 or 0x20

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();                      // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Hello!");
  lcd.setCursor(0,1);
  lcd.print("AxyeTb!");
  lcd.setCursor(0,2);
  lcd.print("I2C 1604 DEMO");
  lcd.setCursor(0,3);
  lcd.print("UGR Edition");
}


void loop()
{
}