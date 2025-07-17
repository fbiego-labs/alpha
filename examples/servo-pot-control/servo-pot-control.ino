/**
 * Title: Servo Control
 * Description: Rotate a servo motor based on potentiometer position using AlphaBoard.
 * Level: Beginner - suitable as an intro to analog input and actuator control.

 - Ensure your servo is powered correctly (AlphaBoard provides only limited current).
- You may need external power for larger servos.
- Run this sketch with the AlphaBoard connected via USB and board type set as **Arduino Micro**.
 */

#include "alpha.h"
#include <Servo.h>

// Create Servo object
Servo myServo;

void setup() {
  myServo.attach(AL_SERVO_D6); // Attach servo to pin D6
}

void loop() {
  int potValue = analogRead(AL_POT_A1);          // Read potentiometer value (0–1023)
  int angle = map(potValue, 0, 1023, 0, 180);     // Convert to servo angle
  myServo.write(angle);                          // Set servo position
  delay(15);                                     // Small delay for smooth movement
}
