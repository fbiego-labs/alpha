/**
 * Title: Servo Control with Potentiometer and Serial Monitor
 * Description: Reads the position of a potentiometer and moves a servo motor accordingly,
 *              while also printing the potentiometer value and mapped servo angle to the Serial Monitor.
 *              
 * Hardware Setup:
 *  - Servo motor connected to D6 on AlphaBoard.
 *  - Potentiometer connected to analog input A1:
 *      - One side to GND
 *      - Other side to 5V
 *      - Middle (wiper) pin to A1
 *  - Make sure servo is powered correctly (external power may be needed).
 * 
 * Usage:
 *  - Run with AlphaBoard connected via USB.
 *  - Select board: **Arduino Micro**
 *  - Open Serial Monitor (Ctrl+Shift+M) in Arduino IDE at 9600 baud.
 * 
 * Level: Beginner - Introduces analog input, actuator control, and serial output.
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
