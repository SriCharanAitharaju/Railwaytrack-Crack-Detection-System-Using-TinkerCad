#include <LiquidCrystal.h>

// ---------- PIN DEFINITIONS ----------
#define IR_SIM 8        // Push button (IR simulation)
#define RED_LED 6       // Red LED = CRACK
#define GREEN_LED 13    // Green LED = SAFE
#define BUZZER 7
#define TRIG 9
#define ECHO 10
#define MOTOR 4

#define SAFE_DISTANCE 15   // cm

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  pinMode(IR_SIM, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(MOTOR, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Railway Track");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring...");
  delay(1500);
  lcd.clear();
}

void loop() {
  // -------- Ultrasonic Sensor --------
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);
  int distance = (duration == 0) ? -1 : duration * 0.034 / 2;

  // -------- IR Button --------
  int irStatus = digitalRead(IR_SIM); // LOW = crack

  // -------- LCD Line 1 --------
  lcd.setCursor(0, 0);
  lcd.print("D:");
  lcd.print(distance);
  lcd.print("cm IR:");
  lcd.print(irStatus);
  lcd.print(" ");

  // -------- CRACK CONDITION --------
  if (irStatus == LOW || distance == -1 || distance > SAFE_DISTANCE) {
    // ---- CRACK ----
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, HIGH);
    digitalWrite(MOTOR, LOW);   // Stop train

    lcd.setCursor(0, 1);
    lcd.print("CRACK DETECTED ");
  } else {
    // ---- SAFE ----
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER, LOW);
    digitalWrite(MOTOR, HIGH);  // Run train

    lcd.setCursor(0, 1);
    lcd.print("TRACK SAFE     ");
  }

  delay(200);
}
