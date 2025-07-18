/**
 * Title: Serial Countdown 
 * Description: * A functional timer using only the onboard components of the AlphaBoard.
 
 */

#include "alpha.h"

enum TimerState { STOPPED,
                  RUNNING,
                  PAUSED,
                  TIME_UP };
TimerState timerState = STOPPED;

unsigned long startTime = 0;
unsigned long pausedTime = 0;
unsigned long countdownMillis = 0;

bool lastStartBtnState = HIGH;
bool lastResetBtnState = HIGH;

int lastMins = -1;
int lastSecs = -1;
bool showedIdleMessage = false;

void setup() {
  Serial.begin(9600);

  pinMode(AL_BUTTON_A4, INPUT_PULLUP);  // Start/Pause/Resume
  pinMode(AL_BUTTON_A5, INPUT_PULLUP);  // Reset

  pinMode(AL_LED_RED, OUTPUT);
  pinMode(AL_LED_YELLOW, OUTPUT);
  pinMode(AL_LED_GREEN, OUTPUT);
  pinMode(AL_BUZZER, OUTPUT);

  showState();

  Serial.println("⏱️ TIMER READY");
  Serial.println("🔧 Use POT A1 for minutes, POT A2 for seconds.");
  Serial.println("▶️ Press A4 to START. ⏸️ Press again to PAUSE. ▶️ Press again to RESUME.");
  Serial.println("🔁 Press A5 to RESET.");
}

void loop() {
  bool startBtn = digitalRead(AL_BUTTON_A4);
  bool resetBtn = digitalRead(AL_BUTTON_A5);

  // Handle STOPPED state — set time from potentiometer
  if (timerState == STOPPED) {
    int mins = map(analogRead(AL_POT_A1), 0, 1023, 0, 59);
    int secs = map(analogRead(AL_POT_A2), 0, 1023, 0, 59);

    if (mins != lastMins || secs != lastSecs) {
      countdownMillis = (mins * 60UL + secs) * 1000UL;
      printTime(countdownMillis);
      lastMins = mins;
      lastSecs = secs;
      showedIdleMessage = false;
    } else if (!showedIdleMessage) {
      Serial.println("🛑 Timer ready. Press A4 to start or adjust pots to change time.");
      showedIdleMessage = true;
    }
  }

  // Start / Pause / Resume Button (A4)
  if (startBtn == LOW && lastStartBtnState == HIGH) {
    delay(50);  // debounce
    if (timerState == STOPPED && countdownMillis > 0) {
      countdownStartup();  // 3-second beep+blink countdown
      startTime = millis();
      timerState = RUNNING;
      beep(1);
      Serial.println("✅ Timer started.");
    } else if (timerState == RUNNING) {
      pausedTime = millis();
      timerState = PAUSED;
      beep(1);
      Serial.println("⏸️ Timer paused.");
    } else if (timerState == PAUSED) {
      startTime += millis() - pausedTime;
      timerState = RUNNING;
      beep(1);
      Serial.println("▶️ Timer resumed.");
    } else if (timerState == TIME_UP) {
      startTime = millis();
      timerState = RUNNING;
      beep(1);
      Serial.println("🔁 Restarting timer.");
    }
    showState();
  }
  lastStartBtnState = startBtn;

  // Reset Button (A5)
  if (resetBtn == LOW && lastResetBtnState == HIGH) {
    delay(50);  // debounce
    timerState = STOPPED;
    countdownMillis = 0;
    beep(2);
    Serial.println("🔁 Timer reset.");
    showState();
  }
  lastResetBtnState = resetBtn;

  // Countdown display
  if (timerState == RUNNING) {
    unsigned long elapsed = millis() - startTime;
    if (elapsed < countdownMillis) {
      printTime(countdownMillis - elapsed);
    } else {
      timerState = TIME_UP;
      Serial.println("⏰ TIME'S UP! Press A4 to restart or A5 to reset.");
      showState();
    }
  }

  // Continuous buzzer tone during TIME_UP
  if (timerState == TIME_UP) {
    tone(AL_BUZZER, 1000);  // 1kHz tone
  } else {
    noTone(AL_BUZZER);
  }

  delay(100);
}

// Display MM:SS on Serial
void printTime(unsigned long msLeft) {
  int totalSec = msLeft / 1000;
  int mm = totalSec / 60;
  int ss = totalSec % 60;
  Serial.print("⏳ Time Left: ");
  if (mm < 10) Serial.print("0");
  Serial.print(mm);
  Serial.print(":");
  if (ss < 10) Serial.print("0");
  Serial.println(ss);
}

// Beep n times
void beep(int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(AL_BUZZER, HIGH);
    delay(100);
    digitalWrite(AL_BUZZER, LOW);
    delay(150);
  }
}

// Update LED state
void showState() {
  digitalWrite(AL_LED_RED, timerState == STOPPED);
  digitalWrite(AL_LED_YELLOW, timerState == PAUSED);
  digitalWrite(AL_LED_GREEN, timerState == RUNNING || timerState == TIME_UP);

  Serial.print("📟 Status: ");
  if (timerState == STOPPED) Serial.println("STOPPED");
  if (timerState == PAUSED) Serial.println("PAUSED");
  if (timerState == RUNNING) Serial.println("RUNNING");
  if (timerState == TIME_UP) Serial.println("TIME'S UP");
}

// 3-second startup countdown with buzzer + LED blink
void countdownStartup() {
  Serial.println("⏳ Starting in:");
  for (int i = 3; i >= 1; i--) {
    Serial.print(i);
    Serial.println("...");
    digitalWrite(AL_LED_YELLOW, HIGH);
    digitalWrite(AL_BUZZER, HIGH);
    delay(300);
    digitalWrite(AL_LED_YELLOW, LOW);
    digitalWrite(AL_BUZZER, LOW);
    delay(700);
  }
  Serial.println("▶️ Go!");
}
