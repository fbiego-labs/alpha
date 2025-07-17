/**
 * Title: Servo Control with Potentiometer and Serial Monitor
 * Description: Reads the position of a potentiometer and moves a servo motor accordingly,
 *              while also printing the potentiometer value and mapped servo angle to the Serial Monitor.
 */

#include "alpha.h"       // AlphaBoard pin aliases
#include <Servo.h>       // Servo motor control library

Servo myServo;           // Create Servo object

void setup() {
  myServo.attach(AL_SERVO_D6);   // Attach servo to digital pin D6
  Serial.begin(9600);            // Start serial communication at 9600 baud
}

void loop() {
  // Read analog value from potentiometer (0 to 1023)
  int potValue = analogRead(AL_POT_A1);

  // Map analog value to angle (0° to 180°)
  int angle = map(potValue, 0, 1023, 0, 180);

  // Set servo position
  myServo.write(angle);

  // Print values to Serial Monitor
  Serial.print("Potentiometer: ");
  Serial.print(potValue);
  Serial.print(" | Servo Angle: ");
  Serial.println(angle);

  // Delay for smooth motion and readable serial output
  delay(100);
}
