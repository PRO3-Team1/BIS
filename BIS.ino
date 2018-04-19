#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
void setup()
{
lcd.begin(16, 2);  // initialize the lcd
lcd.clear();

}
void loop()
{
lcd.setCursor(0, 0);
lcd.print("Line 1");
lcd.setCursor(0, 1);
lcd.print("Line 2");
}
