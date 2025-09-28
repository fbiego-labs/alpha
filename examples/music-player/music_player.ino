/**
 * Title: Music Player
 * Description: An Arduino-based music player that plays several pre-defined melodies on a piezo buzzer. Features include song selection and tempo control using potentiometers and buttons. 
 */



// Include the "pitches.h" header file.
// This file typically contains definitions for musical note frequencies (e.g., NOTE_C4, NOTE_FS5).
// It's a common way to make melody definitions more readable.
#include "alpha.h"
#include <pitches.h>

// Define REST as 0. This is often used in melody arrays to signify a pause or silence.
#define REST 0

// --- Pin Definitions ---
// Define the analog pins used for potentiometers and buttons.

#define AL_BUZZER 9  // Digital Pin: Output for the piezo buzzer to play tones.
#define AL_LED_GREEN 11  // Digital Pin: Output for the green LED (e.g., to indicate a song is playing).
#define AL_LED_RED 13  // Digital Pin: Output for the red LED (e.g., to indicate an error or specific state).
#define AL_POT_A1 A1  // Potentiometer 1: Used for selecting a song (when AL_BUTTON_A4 is pressed without AL_BUTTON_A5).
#define AL_POT_A2 A2  // Potentiometer 2: Used for controlling the tempo of the songs.
#define AL_BUTTON_A4 A4  // Button: When pressed, plays the song selected by AL_POT_A1 or the currentSongNumber.
#define AL_BUTTON_A5 A5  // Button: When pressed, increments to the next song in the sequence.


       

// --- Global Variables ---
// Global variable to store the tempo (beats per minute).
// This can be adjusted by AL_POT_A2. A higher value means a faster song.
int tempo = 108;
// Global variable to keep track of the currently selected song number.
// This is updated by the AL_BUTTON_A5 or by AL_POT_A1 when AL_BUTTON_A4 is pressed.
int currentSongNumber = 1;

/*
  --- Melody Definitions ---
  Each melody is an array of integers.
  The array consists of pairs of values: [NOTE, DURATION].
  - NOTE: Defined in "pitches.h" (e.g., NOTE_AS4, NOTE_F5). REST (0) means no note.
  - DURATION:
    - Positive numbers (e.g., 4, 8, 16) represent standard note durations:
      1: whole note, 2: half note, 4: quarter note, 8: eighth note, 16: sixteenth note.
    - Negative numbers (e.g., -4, -8) represent dotted notes.
      A dotted note's duration is 1.5 times its regular duration.
      For example, -4 means a dotted quarter note (duration of a quarter note * 1.5).
*/

// Melody array for Star Wars Main Theme
int StarWars_melody[] = {
  NOTE_AS4,8, NOTE_AS4,8, NOTE_AS4,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,8, NOTE_C5,8, NOTE_C5,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,-8, NOTE_C5,16,
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C5,-8, NOTE_C5,16,
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_C6,-8, NOTE_G5,16, NOTE_G5,2, REST,8, NOTE_C5,8,
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C6,-8, NOTE_C6,16,
  NOTE_F6,4, NOTE_DS6,8, NOTE_CS6,4, NOTE_C6,8, NOTE_AS5,4, NOTE_GS5,8, NOTE_G5,4, NOTE_F5,8,
  NOTE_C6,1
};

// Melody array for Happy Birthday
int Happy_Birthday_melody[] = {
  NOTE_C4,4, NOTE_C4,8,
  NOTE_D4,-4, NOTE_C4,-4, NOTE_F4,-4,
  NOTE_E4,-2, NOTE_C4,4, NOTE_C4,8,
  NOTE_D4,-4, NOTE_C4,-4, NOTE_G4,-4,
  NOTE_F4,-2, NOTE_C4,4, NOTE_C4,8,
  NOTE_C5,-4, NOTE_A4,-4, NOTE_F4,-4,
  NOTE_E4,-4, NOTE_D4,-4, NOTE_AS4,4, NOTE_AS4,8,
  NOTE_A4,-4, NOTE_F4,-4, NOTE_G4,-4,
  NOTE_F4,-2,
};

// Melody array for Hedwig's Theme (Harry Potter)
int HarryPotter_melody[] = {
  REST, 2, NOTE_D4, 4,
  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
  NOTE_G4, 2, NOTE_D5, 4,
  NOTE_C5, -2,
  NOTE_A4, -2,
  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
  NOTE_F4, 2, NOTE_GS4, 4,
  NOTE_D4, -1,
  NOTE_D4, 4,
  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
  NOTE_G4, 2, NOTE_D5, 4,
  NOTE_F5, 2, NOTE_E5, 4,
  NOTE_DS5, 2, NOTE_B4, 4,
  NOTE_DS5, -4, NOTE_D5, 8, NOTE_CS5, 4,
  NOTE_CS4, 2, NOTE_B4, 4,
  NOTE_G4, -1,
  NOTE_AS4, 4,
  NOTE_D5, 2, NOTE_AS4, 4,
  NOTE_D5, 2, NOTE_AS4, 4,
  NOTE_DS5, 2, NOTE_D5, 4,
  NOTE_CS5, 2, NOTE_A4, 4,
  NOTE_AS4, -4, NOTE_D5, 8, NOTE_CS5, 4,
  NOTE_CS4, 2, NOTE_D4, 4,
  NOTE_D5, -1,
  REST,4, NOTE_AS4,4,
  NOTE_D5, 2, NOTE_AS4, 4,
  NOTE_D5, 2, NOTE_AS4, 4,
  NOTE_F5, 2, NOTE_E5, 4,
  NOTE_DS5, 2, NOTE_B4, 4,
  NOTE_DS5, -4, NOTE_D5, 8, NOTE_CS5, 4,
  NOTE_CS4, 2, NOTE_AS4, 4,
  NOTE_G4, -1,
};

// Melody array for Nokia Ringtone
int NokiaRingtone_melody[] = {
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4,
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4,
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2,
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4,
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4,
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2,
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4,
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4,
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2,
};

// Melody array for We Wish You a Merry Christmas
int MerryChristmas_melody[] = {
  NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,
  NOTE_E5,2, NOTE_E5,4,
  NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,
  NOTE_C5,2, NOTE_A5,4,
  NOTE_AS5,4, NOTE_A5,4, NOTE_G5,4,
  NOTE_C6,4, NOTE_C5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,
  NOTE_E5,2, NOTE_E5,4,
  NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,
  NOTE_C5,2, NOTE_A5,4,
  NOTE_AS5,4, NOTE_A5,4, NOTE_G5,4,
  NOTE_C6,4, NOTE_C5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, REST,4
};

// Melody array for Game of Thrones Theme
int GameofThrones_melody[] = {
  NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16,
  NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_DS4,16, NOTE_F4,16,
  NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16,
  NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16, NOTE_G4,8, NOTE_C4,8, NOTE_E4,16, NOTE_F4,16,
  NOTE_G4,-4, NOTE_C4,-4,
  NOTE_DS4,16, NOTE_F4,16, NOTE_G4,4, NOTE_C4,4, NOTE_DS4,16, NOTE_F4,16,
  NOTE_D4,-1,
  NOTE_F4,-4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_F4,4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_C4,-1,
  NOTE_G4,-4, NOTE_C4,-4,
  NOTE_DS4,16, NOTE_F4,16, NOTE_G4,4, NOTE_C4,4, NOTE_DS4,16, NOTE_F4,16,
  NOTE_D4,-1,
  NOTE_F4,-4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_F4,4, NOTE_AS3,-4,
  NOTE_DS4,16, NOTE_D4,16, NOTE_C4,-1,
  NOTE_G4,-4, NOTE_C4,-4,
  NOTE_DS4,16, NOTE_F4,16, NOTE_G4,4,  NOTE_C4,4, NOTE_DS4,16, NOTE_F4,16,
  NOTE_D4,-2,
  NOTE_F4,-4, NOTE_AS3,-4,
  NOTE_D4,-8, NOTE_DS4,-8, NOTE_D4,-8, NOTE_AS3,-8,
  NOTE_C4,-1,
  NOTE_C5,-2,
  NOTE_AS4,-2,
  NOTE_C4,-2,
  NOTE_G4,-2,
  NOTE_DS4,-2,
  NOTE_DS4,-4, NOTE_F4,-4,
  NOTE_G4,-1,
  NOTE_C5,-2,
  NOTE_AS4,-2,
  NOTE_C4,-2,
  NOTE_G4,-2,
  NOTE_DS4,-2,
  NOTE_DS4,-4, NOTE_D4,-4,
  NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16, NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16,
  NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16, NOTE_C5,8, NOTE_G4,8, NOTE_GS4,16, NOTE_AS4,16,
  REST,4, NOTE_GS5,16, NOTE_AS5,16, NOTE_C6,8, NOTE_G5,8, NOTE_GS5,16, NOTE_AS5,16,
  NOTE_C6,8, NOTE_G5,16, NOTE_GS5,16, NOTE_AS5,16, NOTE_C6,8, NOTE_G5,8, NOTE_GS5,16, NOTE_AS5,16,
};

/**
 * @brief Plays a song defined by a melody array.
 *
 * This function iterates through the provided melody array, calculates the duration
 * of each note based on the current tempo, and plays the note on the AL_BUZZER.
 *
 * @param melodysong An integer array containing pairs of (note, duration) values.
 * @param arraySize The total number of elements in the `melodysong` array.
 */
void startSongPlay(int melodysong[], int arraySize) {
  // Calculate the number of notes. Each note entry consists of a pitch and a duration,
  // so the actual number of notes is half the size of the array.
  int notes = arraySize / 2;

  // Calculate the duration of a whole note in milliseconds.
  // This is based on the tempo (beats per minute).
  // Formula: (milliseconds per minute * beats per whole note) / beats per minute (tempo)
  // Assuming 4 beats per whole note.
  int wholenote = (60000 * 4) / tempo; // 'tempo' is a global variable updated by AL_POT_A2

  int divider = 0;      // Stores the duration divider for the current note.
  int noteDuration = 0; // Stores the calculated duration of the current note in ms.

  // Iterate through the melody array.
  // 'thisNote' increments by 2 in each step to get the pitch and then the duration.
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // Get the duration divider from the melody array (the element after the note's pitch).
    divider = melodysong[thisNote + 1];

    // Calculate the actual duration of the note based on the divider.
    if (divider > 0) {
      // For regular notes, duration = (whole note duration) / divider.
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // For dotted notes, first calculate regular duration, then increase by 50%.
      // Use abs(divider) for the calculation.
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // Multiply by 1.5 for dotted notes.
    }

    // Play the current note on the AL_BUZZER.
    // The note's pitch is melodysong[thisNote].
    // The note is played for 90% of its calculated duration to create a slight separation
    // between notes (staccato effect), making the melody clearer.
    tone(AL_BUZZER, melodysong[thisNote], noteDuration * 0.9);

    // Delay for the full calculated duration of the note.
    // This ensures the timing of the melody is correct.
    delay(noteDuration);

    // Stop the tone generation on the AL_BUZZER before playing the next note.
    // This is important to prevent notes from overlapping or sounding continuous.
    noTone(AL_BUZZER);
  }
}

/**
 * @brief Selects and plays a song based on the provided song number.
 *
 * This function acts as a dispatcher. Based on `songNumber`, it determines
 * which melody array to play and its size, then calls `startSongPlay()`.
 * It also controls LEDs to give visual feedback.
 *
 * @param songNumber An integer representing the song to be played (1-6).
 */
void selectSongPlay(int songNumber) {
  int melodySize; // Variable to store the size of the selected melody array.

  // Use if-else if statements to select the song.
  if (songNumber == 1) {
    // Calculate the size of the StarWars_melody array.
    melodySize = sizeof(StarWars_melody) / sizeof(StarWars_melody[0]);
    digitalWrite(AL_LED_GREEN, HIGH); // Turn on green LED to indicate song playing.
    digitalWrite(AL_LED_RED, LOW);    // Ensure red LED is off.
    startSongPlay(StarWars_melody, melodySize); // Play the Star Wars song.
    noTone(AL_BUZZER);               // Ensure AL_BUZZER is off after the song.
    digitalWrite(AL_LED_GREEN, LOW);  // Turn off green LED.
  } else if (songNumber == 2) {
    melodySize = sizeof(GameofThrones_melody) / sizeof(GameofThrones_melody[0]);
    digitalWrite(AL_LED_GREEN, HIGH);
    digitalWrite(AL_LED_RED, LOW);
    startSongPlay(GameofThrones_melody, melodySize);
    noTone(AL_BUZZER);
    digitalWrite(AL_LED_GREEN, LOW);
  } else if (songNumber == 3) {
    melodySize = sizeof(MerryChristmas_melody) / sizeof(MerryChristmas_melody[0]);
    digitalWrite(AL_LED_GREEN, HIGH);
    digitalWrite(AL_LED_RED, LOW);
    startSongPlay(MerryChristmas_melody, melodySize);
    noTone(AL_BUZZER);
    digitalWrite(AL_LED_GREEN, LOW);
  } else if (songNumber == 4) {
    melodySize = sizeof(NokiaRingtone_melody) / sizeof(NokiaRingtone_melody[0]);
    digitalWrite(AL_LED_GREEN, HIGH);
    digitalWrite(AL_LED_RED, LOW);
    startSongPlay(NokiaRingtone_melody, melodySize);
    noTone(AL_BUZZER);
    digitalWrite(AL_LED_GREEN, LOW);
  } else if (songNumber == 5) {
    melodySize = sizeof(HarryPotter_melody) / sizeof(HarryPotter_melody[0]);
    digitalWrite(AL_LED_GREEN, HIGH);
    digitalWrite(AL_LED_RED, LOW);
    startSongPlay(HarryPotter_melody, melodySize);
    noTone(AL_BUZZER);
    digitalWrite(AL_LED_GREEN, LOW);
  } else if (songNumber == 6) {
    melodySize = sizeof(Happy_Birthday_melody) / sizeof(Happy_Birthday_melody[0]);
    digitalWrite(AL_LED_GREEN, HIGH);
    digitalWrite(AL_LED_RED, LOW);
    startSongPlay(Happy_Birthday_melody, melodySize);
    noTone(AL_BUZZER);
    digitalWrite(AL_LED_GREEN, LOW);
  } else {
    // If an invalid song number is provided, turn on the red LED to indicate an error.
    digitalWrite(AL_LED_RED, HIGH);
    digitalWrite(AL_LED_GREEN, LOW); // Ensure green LED is off.
  }
}

/**
 * @brief Standard Arduino setup function. Runs once when the Arduino starts.
 *
 * Initializes serial communication for debugging purposes.
 * Sets the pin modes for all connected components (potentiometers, buttons, AL_BUZZER, LEDs).
 * Sets initial states for LEDs.
 */
void setup() {
  // Initialize serial communication at 9600 bits per second.
  // Useful for printing debug messages to the Serial Monitor.
  Serial.begin(9600);

  // Set pin modes for potentiometers and buttons as INPUT.
  // For buttons, if no external pull-up/pull-down resistors are used,
  // INPUT_PULLUP might be preferred (button connects to GND when pressed).
  // The current code assumes buttons pull the pin HIGH when pressed.
  pinMode(AL_POT_A1, INPUT);
  pinMode(AL_POT_A2, INPUT);
  pinMode(AL_BUTTON_A4, INPUT);
  pinMode(AL_BUTTON_A5, INPUT);

  // Set pin modes for AL_BUZZER and LEDs as OUTPUT.
  pinMode(AL_BUZZER, OUTPUT);
  pinMode(AL_LED_RED, OUTPUT);
  pinMode(AL_LED_GREEN, OUTPUT);

  // Set initial state for LEDs: red LED on, green LED off.
  // This could be an initial status indicator.
  digitalWrite(AL_LED_RED, HIGH);
  digitalWrite(AL_LED_GREEN, LOW);
}

/**
 * @brief Standard Arduino loop function. Runs repeatedly after setup().
 *
 * Reads inputs from potentiometers and buttons to control song selection,
 * tempo, and playback.
 */
void loop() {
  // Read the analog value from potentiometer 1 (AL_POT_A1).
  // Map this value (0-1023) to a song number range (e.g., 1 to 8).
  // This mapped value is used to select a song if the AL_BUTTON_A4 is pressed
  // and the AL_BUTTON_A5 is NOT pressed.
  int AL_POT_A1_mapped_value = map(analogRead(AL_POT_A1), 0, 1023, 1, 8); // Max 8 songs, adjust if more/less

  // Read the analog value from potentiometer 2 (AL_POT_A2).
  // Map this value (0-1023) to a tempo range (e.g., 60 BPM to 200 BPM).
  // The global 'tempo' variable is updated with this value.
  tempo = map(analogRead(AL_POT_A2), 0, 1023, 60, 200); // Tempo from 60 to 200 BPM

  // Read the digital state of the AL_BUTTON_A4 and AL_BUTTON_A5.
  // Assumes buttons connect the pin to HIGH when pressed.
  int play_btn_value = digitalRead(AL_BUTTON_A4);
  int next_btn_value = digitalRead(AL_BUTTON_A5);

  // Check if the AL_BUTTON_A5 is pressed.
  if (next_btn_value) { // If true (HIGH)
    // Increment the global currentSongNumber.
    currentSongNumber++;
    // If currentSongNumber exceeds the maximum number of defined songs (e.g., 8, matching AL_POT_A1 mapping),
    // loop back to the first song (song number 1).
    if (currentSongNumber > 6) { // Currently 6 songs are defined in selectSongPlay
      currentSongNumber = 1;
    }
    // Play the newly selected song.
    selectSongPlay(currentSongNumber);
    // Add a small delay for debouncing. This helps prevent a single button press
    // from being registered multiple times due to mechanical bouncing of the button contacts.
    delay(200); // Debounce delay, adjust as needed
  }
  // If the AL_BUTTON_A5 is NOT pressed, check if the AL_BUTTON_A4 is pressed.
  else if (play_btn_value) { // If true (HIGH)
    // Set the currentSongNumber based on the value from potentiometer 1.
    currentSongNumber = AL_POT_A1_mapped_value;
    // Play the song selected by the potentiometer.
    selectSongPlay(currentSongNumber);
    // No explicit debounce here, but song playback duration provides a natural delay.
  }
  // If neither button is pressed.
  else {
    // Ensure the AL_BUZZER is off.
    noTone(AL_BUZZER);
    // Optionally, set LEDs to an idle state (e.g., both off or a specific indicator).
    // The current setup() leaves AL_LED_RED HIGH initially.
    // You might want to turn AL_LED_RED LOW here if it's not indicating a specific idle state.
    // digitalWrite(AL_LED_RED, LOW);
    // digitalWrite(AL_LED_GREEN, LOW);
  }
  // A small delay in the main loop can help with stability and responsiveness,
  // especially if reading multiple analog inputs frequently.
  // However, if relying on button presses, long delays here can make the system unresponsive.
  // delay(10); // Optional small delay
}
