/**
 * Title : Multi-Mode Quantized Theremin
 * Description:An  musical instrument with three modes (Theremin, Arpeggiator, Drone).
 * It uses note quantization to map sensor inputs to a musical scale (C Major Pentatonic).
 * A "Performance Mode" button allows for a 15-second continuous play with LED animations.
 *
 * @hardware
 * - LDR (A0): Controls the primary note selection.
 * - Potentiometer 1 (A1): Controls tempo/arpeggio speed.
 * - Potentiometer 2 (A2): Controls note duration/arpeggio pattern.
 * - Button A5 (Pin A5): Switches between modes.
 * - Button A4 (Pin A4): Triggers 15-second Performance Mode.
 * - Red, Yellow, Green LEDs: Indicate mode and provide animation.
 * - Buzzer (Pin 9): Plays the sounds.
 */
#include "alpha.h"
// --- Pin Definitions ---
#define AL_BUZZER 9
#define AL_LED_RED 13
#define AL_LED_YELLOW 12
#define AL_LED_GREEN 11
#define AL_LDR A0
#define AL_POT_A1 A1
#define AL_POT_A2 A2
#define AL_BUTTON_A5 A5
#define AL_BUTTON_A4 A4 

// --- Musical Definitions ---
const int C_MAJOR_PENTATONIC[] = {
  262, 294, 330, 392, 440, // C4, D4, E4, G4, A4
  523, 587, 659, 784, 880  // C5, D5, E5, G5, A5
};
const int SCALE_LENGTH = sizeof(C_MAJOR_PENTATONIC) / sizeof(int);

// --- Mode Definitions ---
enum InstrumentMode {
  THEREMIN,
  ARPEGGIATOR,
  DRONE
};
InstrumentMode currentMode = THEREMIN;

// --- State Variables ---
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
bool lastButtonState = HIGH;

// Performance Mode State
bool isPerformanceModeActive = false;
unsigned long performanceStartTime = 0;
const unsigned long PERFORMANCE_DURATION = 15000; // 15 seconds

// --- Setup Function ---
void setup() {
  pinMode(AL_LED_RED, OUTPUT);
  pinMode(AL_LED_YELLOW, OUTPUT);
  pinMode(AL_LED_GREEN, OUTPUT);
  pinMode(AL_BUZZER, OUTPUT);
  pinMode(AL_BUTTON_A5, INPUT);
  pinMode(AL_BUTTON_A4, INPUT); 
  Serial.begin(9600);
  Serial.println("Advanced Multi-Mode Theremin Initialized.");
  updateModeLEDs();
}

// --- Main Loop ---
void loop() {
  // Handle user inputs first
  handleModeSwitch();
  handlePerformanceButton();

  // If in performance mode, run animations and check timer
  if (isPerformanceModeActive) {
    runPerformanceAnimation();
    if (millis() - performanceStartTime > PERFORMANCE_DURATION) {
      isPerformanceModeActive = false;
      noTone(AL_BUZZER);
      updateModeLEDs(); // Restore normal mode LED
      Serial.println("Performance Mode Ended.");
    }
  }

  // Always run the logic for the current mode.
  // The functions are now non-blocking, so they can run continuously.
  // Sound will only be produced if not muted or if performance mode is active.
  switch (currentMode) {
    case THEREMIN:
      runThereminMode();
      break;
    case ARPEGGIATOR:
      runArpeggiatorMode();
      break;
    case DRONE:
      runDroneMode();
      break;
  }
}

// --- Mode-Specific Functions (Now Non-Blocking) ---

// run Theremin Mode
void runThereminMode() {
  static unsigned long lastPlayTime = 0;
  int noteDelay = map(analogRead(AL_POT_A1), 0, 1023, 50, 500);

  if (isPerformanceModeActive && (millis() - lastPlayTime > noteDelay)) {
    int ldrValue = analogRead(AL_LDR);
    int noteIndex = map(ldrValue, 0, 1023, 0, SCALE_LENGTH - 1);
    int frequency = C_MAJOR_PENTATONIC[noteIndex];
    int noteDuration = map(analogRead(AL_POT_A2), 0, 1023, 50, 1000);
    tone(AL_BUZZER, frequency, noteDuration);
    lastPlayTime = millis();
  }
}
// run Arpeggiator Mode
void runArpeggiatorMode() {
  static unsigned long lastStepTime = 0;
  static int arpeggioStep = 0;
  int speed = map(analogRead(AL_POT_A1), 0, 1023, 50, 200);
  if (isPerformanceModeActive && (millis() - lastStepTime > speed)) {
    int ldrValue = analogRead(AL_LDR);
    int rootNoteIndex = map(ldrValue, 0, 1023, 0, SCALE_LENGTH - 5);
    int pattern = map(analogRead(AL_POT_A2), 0, 1023, 0, 2);
    int noteToPlay;
    switch (pattern) {
      case 0: noteToPlay = C_MAJOR_PENTATONIC[rootNoteIndex + (arpeggioStep % 4)]; break;
      case 1: noteToPlay = C_MAJOR_PENTATONIC[rootNoteIndex + (arpeggioStep == 1 || arpeggioStep == 3 ? 2 : (arpeggioStep == 2 ? 4 : 0))]; break;
      case 2: noteToPlay = C_MAJOR_PENTATONIC[rootNoteIndex + (arpeggioStep % 4 < 2 ? arpeggioStep % 4 : arpeggioStep % 4 + 1)]; break;
    }
    tone(AL_BUZZER, noteToPlay, speed * 0.9);
    arpeggioStep = (arpeggioStep + 1) % 4;
    lastStepTime = millis();
  }
}
// run  Drone Mode
void runDroneMode() {
  static unsigned long lastSwitchTime = 0;
  static bool onDroneNote = true;
  int switchSpeed = map(analogRead(AL_POT_A2), 0, 1023, 2, 20);
  if (isPerformanceModeActive && (millis() - lastSwitchTime > switchSpeed)) {
    int droneNoteIndex = map(analogRead(AL_POT_A1), 0, 1023, 0, 4);
    int droneFrequency = C_MAJOR_PENTATONIC[droneNoteIndex];
    int melodyNoteIndex = map(analogRead(AL_LDR), 0, 1023, 5, SCALE_LENGTH - 1);
    int melodyFrequency = C_MAJOR_PENTATONIC[melodyNoteIndex];
    if (onDroneNote) {
      tone(AL_BUZZER, droneFrequency);
    } else {
      tone(AL_BUZZER, melodyFrequency);
    }
    onDroneNote = !onDroneNote;
    lastSwitchTime = millis();
  }
}

// --- Helper Functions ---

void handlePerformanceButton() {
  if (digitalRead(AL_BUTTON_A4) == LOW) { // Button is pressed
    delay(50); // Simple debounce
    if (digitalRead(AL_BUTTON_A4) == LOW) {
      if (!isPerformanceModeActive) {
        isPerformanceModeActive = true;
        performanceStartTime = millis();
        Serial.println("Performance Mode Started!");
        while(digitalRead(AL_BUTTON_A4) == LOW); // Wait for release
      }
    }
  }
}

void runPerformanceAnimation() {
  unsigned long time = millis() % 900; // Cycle duration of 900ms
  digitalWrite(AL_LED_GREEN, LOW);
  digitalWrite(AL_LED_YELLOW, LOW);
  digitalWrite(AL_LED_RED, LOW);

  if (time < 300) {
    digitalWrite(AL_LED_GREEN, HIGH);
  } else if (time < 600) {
    digitalWrite(AL_LED_YELLOW, HIGH);
  } else {
    digitalWrite(AL_LED_RED, HIGH);
  }
}

void handleModeSwitch() {
  bool reading = digitalRead(AL_BUTTON_A5);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) {
      currentMode = static_cast<InstrumentMode>((currentMode + 1) % 3);
      Serial.print("Mode switched to: ");
      Serial.println(currentMode);
      if (!isPerformanceModeActive) {
        updateModeLEDs();
      }
      noTone(AL_BUZZER);
     while(digitalRead(AL_BUTTON_A5) == LOW);
    }
  }
  lastButtonState = reading;
}

void updateModeLEDs() {
  digitalWrite(AL_LED_RED, LOW);
  digitalWrite(AL_LED_YELLOW, LOW);
  digitalWrite(AL_LED_GREEN, LOW);
  switch (currentMode) {
    case THEREMIN:
      digitalWrite(AL_LED_GREEN, HIGH);
      break;
    case ARPEGGIATOR:
      digitalWrite(AL_LED_YELLOW, HIGH);
      break;
    case DRONE:
      digitalWrite(AL_LED_RED, HIGH);
      break;
  }
}
