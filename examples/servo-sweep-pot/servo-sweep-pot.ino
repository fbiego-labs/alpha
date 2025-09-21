/**

Title: Automated Servo Sweep pot

Description: Automatically sweeps a servo motor back and forth continuously.
*/

#include <Servo.h>

#include "alpha.h"

Servo myServo;
int pos = 0;
int sweepDirection = 1;

void setup() {
myServo.attach(AL_SERVO_D5);
}

void loop() {
// Move the servo
myServo.write(pos);

// Update position for next iteration
pos += sweepDirection;

// Reverse direction at limits
if (pos >= 180 || pos <= 0) {
sweepDirection *= -1;
}

// Control sweep speed
delay(15);
}