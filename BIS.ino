#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h> 


#define DELAY_TIME 			5000
Servo lock
#define LOCK_PIN 			D2
#define LOCK_ANGLE_LOCKED 	10
#define LOCK_ANGLE_UNLOCKED 170

//Resistor task
#define TASK1_PIN			A1
#define TASK1_LOWER			344
#define TASK1_UPPER			444

//Binary 
#define TASK2_PIN_1			D3
#define TASK2_PIN_2			D4

//Hall sensor
#define TASK3_PIN			A2
#define TASK3_LOWER			350
#define TASK3_UPPER			450

#define TASK4_PIN			A3
#define TASK4_LOWER			500
#define TASK4_UPPER			524
#define TASK4_HOLDTIME		2000
static unsigned long holdtime = 0;

// Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int task1(void) {
  int resistorValue = analogRead(TASK1_PIN);
  //test if analogRead(TASK1_PIN) is between two set values
  if (resistorValue > TASK1_LOWER && resistorValue < TASK1_UPPER) {  
    Serial.print("Task 1 - Done: ");
	Serial.println(resistorValue)
    return 0;
  }
}

//Binary task
int task2(void) {
	if (digital(TASK2_PIN_1) == HIGH && digitalRead(TASK2_PIN_2) == HIGH) { 
		Serial.println("Task 2 - Done");
		return true;
	}
	return false;
}

//HALL sensor task
int task3(void) {
	int hall_sensor_value = analogRead(TASK3_PIN);
	if(hall_sensor_value < TASK3_LOWER || hall_sensor_value > TASK3_UPPER){
		Serial.print("Task 3 - Done: ");
		Serial.println(hall_sensor_value);
		return true;
	}
	return false;
}

int task4(void) {
	int a_value = analogRead(TASK4_PIN);
	if (a_value > TASK4_LOWER && a_value < TASK4_UPPER && holdtime == 0) {
		holdtime = millis();
		Serial.print("Task 4 - Value: ");
		Serial.println(a_value);
	} else {
		holdtime = 0;
	}
	
	if(holdtime && millis() - holdtime > TASK4_HOLDTIME) {
		Serial.println("Task 4 - Done");
		return true;
	}
	return false;	
}

void setup() {
  Serial.begin(115200);
  lock.attach(LOCK_PIN);
  lock.write(LOCK_ANGLE_LOCKED);
  myservo.write(90);  // set servo to mid-point
  lcd.begin(16, 2);  // initialize the lcd
  lcd.clear();
  
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
  lcd.setCursor(0, 1);
  lcd.print("...");
  delay(DELAY_TIME);

  lcd.setCursor(0, 0);
  lcd.print("Task 1");
  while (task1() == 0) {} // Wait here till task1 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!");
  delay(DELAY_TIME);
  lcd.setCursor(0, 1);
  lcd.print("...!");

  lcd.setCursor(0, 0);
  lcd.print("Task 2");
  while (task2() == 0) {} // Wait here till task2 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!");
  delay(DELAY_TIME);
  lcd.setCursor(0, 1);
  lcd.print("...!");

  lcd.setCursor(0, 0);
  lcd.print("Task 3");
  while (task3() == 0) {} // Wait here till task3 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!");
  delay(DELAY_TIME);
  lcd.setCursor(0, 1);
  lcd.print("...!");


  lcd.setCursor(0, 0);
  lcd.print("Task 4");
  while (task4() == 0) {} // Wait here till task4 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!");
  delay(DELAY_TIME);
  lcd.setCursor(0, 1);
  lcd.print("...!");

  lcd.setCursor(0, 0);
  lcd.print("Quest complete!");
  delay(DELAY_TIME);
  lock.write(LOCK_ANGLE_UNLOCKED);
  while(1) {} // Wait forever
}
