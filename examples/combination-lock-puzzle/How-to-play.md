# Advanced Combination Lock Logic Puzzle

Advanced Combination Lock, a multi-stage logic puzzle for your ATmega32U4-based board! This isn't just a simple lock; it's a test of sequence, timing, and observation. You'll need to use buttons and light sensors in the correct order to crack the code and unlock the system.

uses a state machine to create a challenging and interactive experience. Fail too many times, and you'll trigger a system lockdown!

## Features

- **Multi-Stage Puzzle:** A 4-stage sequence that requires more than just button presses.
- **Timed Challenge:** One stage must be completed within a 2-second window.
- **Arming Sequence:** The lock must be armed before an attempt can be made.
- **Rich Feedback:** LEDs and a buzzer provide clear feedback for your actions, current stage, success, and failure.
- **Lockdown Mode:** Three consecutive failures will temporarily lock the system to prevent brute-force attempts.

## Components Used

- **LDR (Light Dependent Resistor):** To detect light and dark conditions.
- **Button 1 & Button 2:** For sequence inputs.
- **Red LED:** Indicates failure, lockdown, and arming status.
- **Yellow LED:** Pulses to indicate the current stage of the puzzle.
- **Green LED:** Indicates a successful unlock.
- **Buzzer:** Provides auditory feedback for all actions.

## How to Play: The Winning Sequence

Follow these instructions exactly to solve the puzzle and win. Pay close attention to the LED and sound cues!

### Step 1: Arm the System

The lock starts in an `IDLE` state. To begin, you must arm it.

1.  **Press and hold both Button 1 and Button 2 simultaneously.**
2.  The **Red LED will start pulsing**, and you'll hear a low tone. This indicates the system is `ARMING`.
3.  **Release both buttons.**
4.  The Red LED will turn off. The system is now `ARMED` and the puzzle has begun!

### Step 2: Solve the 4-Stage Combination

After arming, the system immediately moves to Stage 1. The **Yellow LED will pulse once** to show you are on the first stage.

- **Stage 1: The Darkness**
  - **Action:** **Cover the LDR sensor completely** with your finger to make it dark.
  - **Feedback:** You will hear a short, high-pitched beep, and the **Yellow LED will pulse twice**, indicating you have advanced to Stage 2.
- **Stage 2: The First Key**
  - **Action:** While **keeping the LDR covered**, **press Button 1**.
  - **Feedback:** You'll hear another confirmation beep, and the **Yellow LED will pulse three times**. This signals the start of the timed stage!
- **Stage 3: The Timed Reveal**
  - **Action:** You have **only 2 seconds** for this step! **Quickly remove your finger from the LDR** to expose it to light.
  - **Feedback:** If you are fast enough, you'll hear a beep, and the **Yellow LED will pulse four times**. If you are too slow, the failure sequence will trigger.
- **Stage 4: The Final Key**
  - **Action:** While the **LDR is exposed to light**, **press Button 2**.
  - **Feedback:** If you've done everything correctly, you will have solved the puzzle!

### Step 3: Victory!

Upon completing Stage 4, the system enters the `UNLOCKED` state:

- The **Green LED will light up solid**.
- A happy, ascending chime will play on the buzzer.
- The lock will remain in this "unlocked" state for 3 seconds before automatically resetting to `IDLE`, ready for another attempt.

## Failure and Lockdown

- **Making a Mistake:** If you perform the wrong action at any stage (e.g., press the wrong button, or the light level is incorrect), the `FAILED` sequence will trigger. The **Red LED will light up**, and a sad, descending tone will play. The system will then reset to `IDLE`.
- **Lockdown Mode:** If you fail **three times in a row**, the system will enter `LOCKDOWN` mode for 10 seconds as a penalty. The **Red LED will flash rapidly** with an annoying beep. You cannot interact with the puzzle during this time. After 10 seconds, it will reset to `IDLE`.

