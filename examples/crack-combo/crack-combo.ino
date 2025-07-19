/**
 * Project: Combo Crackdown
 * Description: A combo-cracking game using two potentiometers (A1 & A2), LEDs, and buzzer. Twist potentiometers to match a secret combo and press a button to unlock.
 */

#include "alpha.h"

// === Constants ===
const int comboTolerance = 30;  // Acceptable range for matching combo
const int nearRange = 15;       // Threshold for “almost” message

// === Variables ===
int combo1 = 0;  // Target for LEFT potentiometer (A1)
int combo2 = 0;  // Target for RIGHT potentiometer (A2)
bool lastCheckButton = HIGH;
bool lastResetButton = HIGH;

// === Setup ===
void setup() {
  Serial.begin(9600);
  pinMode(AL_BUTTON_A5, INPUT_PULLUP);  // OK button
  pinMode(AL_BUTTON_A4, INPUT_PULLUP);  // RESET button

  pinMode(AL_LED_GREEN, OUTPUT);
  pinMode(AL_LED_RED, OUTPUT);
  pinMode(AL_LED_YELLOW, OUTPUT);
  pinMode(AL_BUZZER, OUTPUT);

  randomSeed(analogRead(0));
  generateCombo();

  Serial.println("\n===============================");
  Serial.println("🔐  AlphaCombo Crackdown Game");
  Serial.println("===============================\n");
  Serial.println("🎯 Twist both potentiometers to match the secret combo.");
  Serial.println(" - LEFT  knob = POT A1");
  Serial.println(" - RIGHT knob = POT A2");
  Serial.println("🔘 Press OK (A5) to unlock. RESET (A4) for new combo.\n");
}

// === Main Loop ===
void loop() {
  bool checkButton = digitalRead(AL_BUTTON_A5);  // OK
  bool resetButton = digitalRead(AL_BUTTON_A4);  // RESET

  // OK button pressed
  if (lastCheckButton == HIGH && checkButton == LOW) {
    int pot1 = analogRead(AL_POT_A1);  // LEFT
    int pot2 = analogRead(AL_POT_A2);  // RIGHT

    // Show feedback
    Serial.println("🔎 Checking Combo...");
    Serial.print("LEFT  (A1): ");
    Serial.print(pot1);
    Serial.print(" → ");
    Serial.println(getHint(pot1, combo1));
    Serial.print("RIGHT (A2): ");
    Serial.print(pot2);
    Serial.print(" → ");
    Serial.println(getHint(pot2, combo2));

    if (isCloseEnough(pot1, combo1) && isCloseEnough(pot2, combo2)) {
      successSequence();
      generateCombo();
    } else {
      failSequence();
    }
    Serial.println();
  }

  // RESET button pressed
  if (lastResetButton == HIGH && resetButton == LOW) {
    Serial.println("🔁 Combo manually reset.\n");
    generateCombo();
  }

  lastCheckButton = checkButton;
  lastResetButton = resetButton;
  delay(50);  // Debounce
}

// === Generate New Combo ===
void generateCombo() {
  combo1 = random(100, 900);
  combo2 = random(100, 900);
  Serial.println("🎯 New secret combo ready!");
}

// === Hint Message Based on Proximity ===
String getHint(int value, int target) {
  int diff = value - target;
  if (abs(diff) <= 5) return "✅ Perfect!";
  if (abs(diff) <= nearRange) return "🟡 Almost!";
  if (diff > 0 && diff <= 100) return "🔼 Slightly High";
  if (diff < 0 && abs(diff) <= 100) return "🔽 Slightly Low";
  if (diff > 0) return "⬆️ Too High";
  return "⬇️ Too Low";
}

// === Match Validation ===
bool isCloseEnough(int value, int target) {
  return abs(value - target) <= comboTolerance;
}

// === Success Feedback ===
void successSequence() {
  Serial.println("🎉 COMBO CRACKED! Well done!");
  for (int i = 0; i < 2; i++) {
    digitalWrite(AL_LED_GREEN, HIGH);
    tone(AL_BUZZER, 1200);
    delay(200);
    tone(AL_BUZZER, 1200);
    delay(200);
    digitalWrite(AL_LED_GREEN, LOW);
    noTone(AL_BUZZER);
    delay(200);
  }
}

// === Failure Feedback ===
void failSequence() {
  Serial.println("❌ Incorrect combo. Try again.");
  digitalWrite(AL_LED_RED, HIGH);
  tone(AL_BUZZER, 400);
  delay(800);
  digitalWrite(AL_LED_RED, LOW);
  noTone(AL_BUZZER);
}
