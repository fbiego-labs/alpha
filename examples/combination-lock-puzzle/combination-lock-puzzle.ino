/**
 * Title: Advanced Combination Lock Logic Puzzle
 * Description: An advanced, multi-stage lock requiring a sequence of button presses,
 * light level changes, and timed actions to unlock. Features an arming
 * sequence and a lockdown mode after multiple failures.
 *
 *
 * @hardware
 * - LDR (A2): For light-based inputs.
 * - Button 1 (A4), Button 2 (A5): For sequence inputs.
 * - Red LED (A13): Indicates failure, lockdown, and arming status.
 * - Yellow LED (A12): Indicates the current stage of the combination.
 * - Green LED (A11): Indicates success/unlocked state.
 * - Buzzer (9): Provides rich auditory feedback.
 */


#include "alpha.h"

// Pin Definitions

#define AL_BUZZER 9
#define AL_LED_GREEN 11
#define AL_LED_YELLOW 12
#define AL_LED_RED 13
#define AL_LDR A0
#define AL_POT_A1 A1
#define AL_POT_A2 A2
#define AL_BUTTON_A4 A4
#define AL_BUTTON_A5 A5

// Lock Configuration 
const int DARK_THRESHOLD = 400;  // LDR value considered "dark"
const int LIGHT_THRESHOLD = 700; // LDR value considered "light"
const int TIMED_STAGE_WINDOW = 2000; // 2 seconds for the timed challenge
const int MAX_FAILURES = 3; // Max attempts before lockdown

//  State Machine 
enum LockState {
  IDLE,
  ARMING,
  ARMED,
  STAGE_1,
  STAGE_2,
  STAGE_3_TIMED,
  STAGE_4,
  UNLOCKED,
  FAILED,
  LOCKDOWN
};
LockState currentState = IDLE;

// State Variables 
int failureCount = 0;
unsigned long stageStartTime = 0;

// Setup Function 
void setup() {
  pinMode(AL_BUTTON_A4, INPUT_PULLUP);
  pinMode(AL_BUTTON_A5, INPUT_PULLUP);
  pinMode(AL_LED_RED, OUTPUT);
  pinMode(AL_LED_YELLOW, OUTPUT);
  pinMode(AL_LED_GREEN, OUTPUT);
  pinMode(AL_BUZZER, OUTPUT);
  Serial.begin(9600);
  Serial.println("Advanced Combination Lock Initialized. Hold both buttons to arm.");
}

// Main Loop: State Machine 
void loop() {
  switch (currentState) {
    case IDLE:
      handleIdleState();
      break;
    case ARMING:
      handleArmingState();
      break;
    case ARMED:
      // Transition to the first stage immediately after arming
      Serial.println("System Armed. Begin sequence.");
      currentState = STAGE_1;
      stageStartTime = millis();
      break;
    case STAGE_1:
      // Stage 1: Cover LDR to make it dark
      updateStageIndicator(1);
      if (analogRead(AL_LDR) < DARK_THRESHOLD) {
        advanceStage(STAGE_2);
      }
      break;
    case STAGE_2:
      // Stage 2: While dark, press Button 1
      updateStageIndicator(2);
      if (analogRead(AL_LDR) < DARK_THRESHOLD && digitalRead(AL_BUTTON_A4) == LOW) {
        advanceStage(STAGE_3_TIMED);
      }
      break;
    case STAGE_3_TIMED:
      // Stage 3: Uncover LDR to make it light within 2 seconds
      updateStageIndicator(3);
      if (analogRead(AL_LDR) > LIGHT_THRESHOLD) {
        if (millis() - stageStartTime <= TIMED_STAGE_WINDOW) {
          advanceStage(STAGE_4);
        } else {
          Serial.println("Timed stage failed: Too slow!");
          failSequence();
        }
      }
      // Fail if time runs out
      if (millis() - stageStartTime > TIMED_STAGE_WINDOW) {
        Serial.println("Timed stage failed: Ran out of time!");
        failSequence();
      }
      break;
    case STAGE_4:
      // Stage 4: While light, press Button 2
      updateStageIndicator(4);
      if (analogRead(AL_LDR) > LIGHT_THRESHOLD && digitalRead(AL_BUTTON_A5) == LOW) {
        unlockSequence();
      }
      break;
    case UNLOCKED:
      // The unlock sequence handles this state. It will transition back to IDLE.
      break;
    case FAILED:
      // The fail sequence handles this state. It will transition back to IDLE or LOCKDOWN.
      break;
    case LOCKDOWN:
      handleLockdownState();
      break;
  }
}

// --- State Handling Functions ---

void handleIdleState() {
  // In IDLE, wait for the user to hold both buttons to start the ARMING sequence.
  if (digitalRead(AL_BUTTON_A4) == LOW && digitalRead(AL_BUTTON_A5) == LOW) {
    currentState = ARMING;
    Serial.println("Arming sequence initiated...");
    delay(200); // Debounce
  }
}

void handleArmingState() {
  // Pulse the red LED to show the system is arming.
  long armingPulse = millis() % 1000;
  if (armingPulse < 500) {
    digitalWrite(AL_LED_RED, HIGH);
  } else {
    digitalWrite(AL_LED_RED, LOW);
  }

  // If buttons are released, the system becomes ARMED.
  if (digitalRead(AL_BUTTON_A4) == HIGH && digitalRead(AL_BUTTON_A5) == HIGH) {
    digitalWrite(AL_LED_RED, LOW);
    currentState = ARMED;
  }
}

void handleLockdownState() {
  // In lockdown, flash the red LED rapidly and do nothing else.
  digitalWrite(AL_LED_RED, HIGH);
  tone(AL_BUZZER, 100, 50); // Low, annoying beep
  delay(200);
  digitalWrite(AL_LED_RED, LOW);
  delay(200);

  // After 10 seconds, exit lockdown.
  if (millis() - stageStartTime > 10000) {
    Serial.println("Lockdown over. System reset.");
    failureCount = 0;
    currentState = IDLE;
  }
}

// --- Helper Functions ---

void advanceStage(LockState nextStage) {
  Serial.print("Stage ");
  Serial.print(currentState - ARMED);
  Serial.println("complete.");
  tone(AL_BUZZER, 1200, 50); // Positive feedback beep
  currentState = nextStage;
  stageStartTime = millis(); // Reset timer for the next stage
  delay(200); // Debounce
}

void failSequence() {
  Serial.println("Sequence Failed! Resetting.");
  failureCount++;
  digitalWrite(AL_LED_YELLOW, LOW); // Turn off stage indicator
  
  // Play failure sound and flash red LED
  digitalWrite(AL_LED_RED, HIGH);
  tone(AL_BUZZER, 300, 250); delay(270);
  tone(AL_BUZZER, 200, 500);
  delay(1000);
  digitalWrite(AL_LED_RED, LOW);

  if (failureCount >= MAX_FAILURES) {
    Serial.println("Maximum failures reached. System lockdown!");
    currentState = LOCKDOWN;
    stageStartTime = millis(); // Use timer for lockdown duration
  } else {
    currentState = IDLE; // Reset to idle
  }
}

void unlockSequence() {
  Serial.println("SYSTEM UNLOCKED");
  currentState = UNLOCKED;
  digitalWrite(AL_LED_YELLOW, LOW); // Turn off stage indicator
  failureCount = 0; // Reset failure count on success

  // Play success chime and flash green LED
  digitalWrite(AL_LED_GREEN, HIGH);
  tone(AL_BUZZER, 1000, 100); delay(120);
  tone(AL_BUZZER, 1200, 100); delay(120);
  tone(AL_BUZZER, 1500, 100); delay(120);
  tone(AL_BUZZER, 2000, 300);
  delay(3000); // Stay in unlocked state for 3 seconds
  
  digitalWrite(AL_LED_GREEN, LOW);
  currentState = IDLE; // Return to idle
}

void updateStageIndicator(int stage) {
  // Use PWM to create a "breathing" effect on the yellow LED
  // The number of pulses indicates the current stage number.
  for (int i = 0; i < stage; i++) {
    // Fade in
    for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
      analogWrite(AL_LED_YELLOW, fadeValue);
      delay(1);
    }
    // Fade out
    for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
      analogWrite(AL_LED_YELLOW, fadeValue);
      delay(1);
    }
    delay(200);
  }
  delay(500); // Pause between stage indications
}
