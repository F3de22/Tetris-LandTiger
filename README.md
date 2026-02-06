# Tetris Clone - LandTiger (NXP LPC1768)

A complete **Tetris** clone developed in C for the **LandTiger** development board based on the **NXP LPC1768 (ARM Cortex-M3)** microcontroller.

This project leverages multiple hardware peripherals (GLCD, ADC, DAC, Timers, Joystick) to provide a smooth gaming experience, featuring background music, sound effects, and advanced mechanics like Power-Ups and Malus events.

## 🎮 Key Features

* **Classic Gameplay**: Full Tetris logic including piece rotation, collision detection, and line clearing.
* **GLCD Graphics**: Smooth rendering on the TFT LCD display (320x240).
* **Audio & Music**:
    * Original "Tetris Theme" (Korobeiniki) generated via **Sinusoidal Synthesis** on the **DAC**.
    * Specific sound effects for movement, rotation, dropping, and Game Over.
    * Audio priority handling to prevent game freezing during playback.
* **Dynamic Speed Control**: Use of the **Potentiometer (ADC)** to adjust the game speed (falling piece speed) in real-time.
* **Scoring System**:
    * Score calculation based on cleared lines (bonus for "Tetris"/4 lines).
    * **High Score** tracking during the session.
* **Power-Ups & Malus**:
    * **Power-Up**: Special blocks that, when cleared, trigger bonuses (e.g., clearing half the screen, slowing down time).
    * **Malus**: Spawns indestructible red "garbage lines" from the bottom as the game progresses or upon specific triggers.

## 🕹️ Controls

| Hardware Input | Game Action |
| :--- | :--- |
| **Joystick LEFT** | Move piece Left |
| **Joystick RIGHT** | Move piece Right |
| **Joystick UP** | Rotate piece |
| **Joystick DOWN** | Soft Drop (Fast descent) |
| **Button KEY 2** | **Hard Drop** (Instant drop) |
| **Button KEY 1** | Start / Pause / Resume |
| **Potentiometer** | Adjust Game Speed |
| **Reset** | Reset Board |

## 🚀 Installation and Debugging

### Requirements
* **Keil uVision 5** (Legacy Support for ARM7/9 recommended).
* LandTiger NXP LPC1768 Board.

### Emulator Instructions (Keil)
Since the Keil simulator does not simulate physical analog inputs by default:
1.  Set the project target to **"Use Simulator"**.
2.  A `debug.ini` script is included to map the virtual potentiometer to the ADC registers.
3.  Open the **Toolbox** window during debugging to use the **"Gira + / Gira -"** virtual buttons to simulate turning the potentiometer knob.

## 👥 Credits
Project developed for the *Computer Architectures* course.
