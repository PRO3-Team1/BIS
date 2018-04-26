#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int resistorPin = A0;
const int delaytime = 1;
int resistorValue = 0;


// Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int task1(void) {
  resistorValue = analogRead(resistorPin);
  Serial.println(resistorValue);
  if (resistorValue > 344 && resistorValue < 444) { // Should test if analogRead(resistorPin) is between two set values
    Serial.println("Done!");
    return 0;
  }
}

int task2(void) {
  if (1) { // Should test if digitalRead(dipSwitchPin) is 1
    return 0;
  }
}

int task3(void) {
  if (1) { // Should test if digitalRead(sensorPin) is 1
    return 0;
  }
}

int task4(void) {
  /*
     Analog read value should be written to the display
     Some timing functionality is needed here (maybe milis()) to test if value is stable for 2 seconds
  */

  if (1) {// Should test if analogRead(voltagePin) is between two set values
    //start/restart the milis() timer
    if (1) { // if milis() > 2000 && analogRead(voltagePin) is between two set values
      return 0;
    }
  }
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);  // initialize the lcd
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
  lcd.setCursor(0, 1);
  lcd.print("...");
  delay(delaytime);

  lcd.setCursor(0, 0);
  lcd.print("Task 1");
  while (task1() != 0) {} // Wait here till task1 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!");
  delay(delaytime);
  lcd.setCursor(0, 1);
  lcd.print("...!");

  lcd.setCursor(0, 0);
  lcd.print("Task 2");
  while (task2() != 0) {} // Wait here till task2 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!");
  delay(delaytime);
  lcd.setCursor(0, 1);
  lcd.print("...!");

  lcd.setCursor(0, 0);
  lcd.print("Task 3");
  while (task3() != 0) {} // Wait here till task3 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!");
  delay(delaytime);
  lcd.setCursor(0, 1);
  lcd.print("...!");


  lcd.setCursor(0, 0);
  lcd.print("Task 4");
  while (task4() != 0) {} // Wait here till task4 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!");
  delay(delaytime);
  lcd.setCursor(0, 1);
  lcd.print("...!");
}
