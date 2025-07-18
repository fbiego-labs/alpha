/**
 * Title: Serial Stopwatch with LEDs & Audible Buzzer
 * Description: Stopwatch with button control, LED indicators, buzzer tones, and serial display. Button A4 = Start/Pause/Resume  Button A5 = Reset
 */

#include "alpha.h"

// State
bool running = false;
bool started = false;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

// Button state tracking
bool lastButtonA4 = HIGH;
bool lastButtonA5 = HIGH;

void setup() {
  // Pin modes
  pinMode(AL_BUTTON_A4, INPUT_PULLUP);
  pinMode(AL_BUTTON_A5, INPUT_PULLUP);
  pinMode(AL_BUZZER, OUTPUT);
  pinMode(AL_LED_RED, OUTPUT);
  pinMode(AL_LED_YELLOW, OUTPUT);
  pinMode(AL_LED_GREEN, OUTPUT);

  Serial.begin(9600);
  delay(500);

  Serial.println("== Serial Stopwatch ==");
  Serial.println("Press Button A4 to start");

  showStatus("stopped");
}

void loop() {
  // Read buttons
  bool buttonA4 = digitalRead(AL_BUTTON_A4);
  bool buttonA5 = digitalRead(AL_BUTTON_A5);

  // Handle A4 - Start/Pause/Resume
  if (buttonA4 == LOW && lastButtonA4 == HIGH) {
    if (!started) {
      // Start the stopwatch
      started = true;
      running = true;
      startTime = millis();
      beepStart();
      showStatus("running");
      Serial.println("Running... Press A4 to pause, A5 to reset.");
    } else {
      // Toggle running state (pause/resume)
      running = !running;
      if (running) {
        startTime = millis() - elapsedTime;
        Serial.println("Resumed...");
        showStatus("running");
        beepStart();
      } else {
        elapsedTime = millis() - startTime;
        Serial.println("Paused. Press A4 to resume, A5 to reset.");
        showStatus("paused");
        beepPause();
      }
    }
    delay(200);  // debounce
  }
  lastButtonA4 = buttonA4;

  // Handle A5 - Reset
  if (buttonA5 == LOW && lastButtonA5 == HIGH && started) {
    running = false;
    started = false;
    elapsedTime = 0;
    Serial.println("== Stopwatch Reset ==");
    Serial.println("Press Button A4 to start");
    showStatus("stopped");
    beepReset();
    delay(200);  // debounce
  }
  lastButtonA5 = buttonA5;

  // Update display if running
  if (started && running) {
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate >= 200) {
      unsigned long current = millis() - startTime;
      printTime(current);
      lastUpdate = millis();
    }
  }
}

void printTime(unsigned long ms) {
  unsigned int minutes = (ms / 60000) % 60;
  unsigned int seconds = (ms / 1000) % 60;
  unsigned int centis = (ms % 1000) / 10;

  char buffer[16];
  sprintf(buffer, "%02u:%02u.%02u", minutes, seconds, centis);
  Serial.println(buffer);
}

// Audible tone functions
void beepStart() {
  tone(AL_BUZZER, 1000, 150);  // 1kHz tone for 150ms
  delay(200);
  noTone(AL_BUZZER);
}

void beepPause() {
  tone(AL_BUZZER, 500, 150);  // 500Hz tone for 150ms
  delay(200);
  noTone(AL_BUZZER);
}

void beepReset() {
  tone(AL_BUZZER, 800, 100);
  delay(150);
  tone(AL_BUZZER, 600, 100);
  delay(150);
  noTone(AL_BUZZER);
}

void showStatus(String state) {
  digitalWrite(AL_LED_GREEN, LOW);
  digitalWrite(AL_LED_YELLOW, LOW);
  digitalWrite(AL_LED_RED, LOW);

  if (state == "running") {
    digitalWrite(AL_LED_GREEN, HIGH);
  } else if (state == "paused") {
    digitalWrite(AL_LED_YELLOW, HIGH);
  } else {
    digitalWrite(AL_LED_RED, HIGH);
  }
}
