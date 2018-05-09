#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define NOTE_SUSTAIN 160

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
Servo lock;

#define DELAY_TIME 			2000
#define LOCK_PIN 			13
#define LOCK_ANGLE_LOCKED 	10
#define LOCK_ANGLE_UNLOCKED 170
//Resistor task
#define TASK1_PIN			A1
#define TASK1_LOWER			500
#define TASK1_UPPER			524
//Binary
#define TASK2_PIN_1			2
#define TASK2_PIN_2			3
//Hall sensor
#define TASK3_PIN			A3
#define TASK3_LOWER			480
#define TASK3_UPPER			560
// Measure
#define TASK4_PIN			A6
#define TASK4_LOWER			367
#define TASK4_UPPER			377
static unsigned long holdtime = 0;
// Buzzer
#define BUZZER_PIN  12

// Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int task1(void) {
  int resistorValue = analogRead(TASK1_PIN);
  delay (1000);
  //test if analogRead(TASK1_PIN) is between two set values
  if (resistorValue > TASK1_LOWER && resistorValue < TASK1_UPPER) {
    return true;
  }
  return false;
}

//Binary task
int task2(void) {
  delay(1000);
  if (digitalRead(TASK2_PIN_1) == LOW && digitalRead(TASK2_PIN_2) == LOW) {
    return true;
  }
  return false;
}

//HALL sensor task
int task3(void) {
  delay(1000);
  int hall_sensor_value = analogRead(TASK3_PIN);
  if (hall_sensor_value < TASK3_LOWER || hall_sensor_value > TASK3_UPPER) {
    return true;
  }
  return false;
}

int task4(void) {
  int a_value = analogRead(TASK4_PIN);
  delay(100);
  int displayvalue = map(a_value, 0, 1023, 0, 500);
  lcd.setCursor(11, 0);
  lcd.print((float)displayvalue / 100, 2);
  lcd.setCursor(15, 0);
  lcd.print("V");
  if (TASK4_LOWER <= displayvalue && displayvalue <= TASK4_UPPER)
  {
    holdtime++;
    if (holdtime >= 20) {
      return true;
    }
  } else
  {
    holdtime = 0;
  }
  return false;
}

char notes[] = "gabygabyxzCDxzCDabywabywzCDEzCDEbywFCDEqywFGDEqi        azbC"; // a space represents a rest
int length = sizeof(notes); // the number of notes
int beats[] = { 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 2,3,3,16,};
int tempo = 75/4;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(tone);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'x', 'a', 'z', 'b', 'C', 'y', 'D', 'w', 'E', 'F', 'q', 'G', 'i' };
  // c=C4, C = C5. These values have been tuned.
  int tones[] = { 1898, 1690, 1500, 1420, 1265, 1194, 1126, 1063, 1001, 947, 893, 843, 795, 749, 710, 668, 630, 594 };
   
  // play the tone corresponding to the note name
  for (int i = 0; i < 18; i++) {
    if (names[i] == note) {
      playTone((tones[i])/17, duration);
    }
  }
}


void Succes2(void){
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    
    // pause between notes
    delay(tempo / 2); 
}
  }

void Succes(void) {
  tone(BUZZER_PIN, NOTE_A5);
  delay(NOTE_SUSTAIN);
  tone(BUZZER_PIN, NOTE_B5);
  delay(NOTE_SUSTAIN);
  tone(BUZZER_PIN, NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(BUZZER_PIN, NOTE_B5);
  delay(NOTE_SUSTAIN);
  tone(BUZZER_PIN, NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(BUZZER_PIN, NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(BUZZER_PIN, NOTE_C5);
  delay(NOTE_SUSTAIN);
  tone(BUZZER_PIN, NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(BUZZER_PIN, NOTE_E5);
  delay(NOTE_SUSTAIN);
  tone(BUZZER_PIN, NOTE_D5);
  delay(NOTE_SUSTAIN);
  tone(BUZZER_PIN, NOTE_E5);
  delay(NOTE_SUSTAIN);
  tone(BUZZER_PIN, NOTE_E5);
  delay(NOTE_SUSTAIN);
  noTone (BUZZER_PIN);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Team 1");
  lock.attach(LOCK_PIN);
  lock.write(LOCK_ANGLE_LOCKED);
  Serial.println("setup in progress");
  lcd.begin(16, 2);  // initialize the lcd
  lcd.clear();
  Serial.println("setup done");
}

void loop() {
  Serial.println("loop start");
  lcd.setCursor(0, 0);
  lcd.print("Can you open");
  lcd.setCursor(0, 1);
  lcd.print("The Quest Box");
  delay(DELAY_TIME);

  lcd.clear() ;
  lcd.setCursor(0, 0);
  lcd.print("T1 Formodstand");
  lcd.setCursor(0, 1);
  lcd.print("OHMS lov");
  while (task1() == 0) {} // Wait here till task1 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!      ");
  Succes();
  delay(DELAY_TIME);
  lcd.setCursor(0, 1);
  lcd.print("...!");

  lcd.clear() ;
  lcd.setCursor(0, 0);
  lcd.print("T2 Convert 0x28");
  lcd.setCursor(0, 1);
  lcd.print("Hex to Binary");
  while (task2() == 0) {} // Wait here till task2 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!      ");
  Succes2();
  delay(DELAY_TIME);
  lcd.setCursor(0, 1);
  lcd.print("...!");

  lcd.clear() ;
  lcd.setCursor(0, 0);
  lcd.print("T3 Datasheet");
  lcd.setCursor(0, 1);
  lcd.print("Activate SS49E");
  while (task3() == 0) {} // Wait here till task3 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!      ");
  Succes2();
  delay(DELAY_TIME);
  lcd.setCursor(0, 1);
  lcd.print("...!");

  lcd.clear() ;
  lcd.setCursor(0, 0);
  lcd.print("T4 Reading");
  lcd.setCursor(0, 1);
  lcd.print("Measure & Adjust");
  while (task4() == 0) {} // Wait here till task4 is completed
  lcd.setCursor(0, 1);
  lcd.print("Completed!      ");
  Succes();
  delay(DELAY_TIME);

  lcd.clear() ;
  lcd.setCursor(0, 0);
  lcd.print("Quest complete!");
  delay(DELAY_TIME);
  lcd.setCursor(0, 1);
  lcd.print("Reward Time");
  delay(DELAY_TIME);
  lock.write(LOCK_ANGLE_UNLOCKED);
  while (1) {} // Wait forever
}
