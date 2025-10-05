# ESP32 Touch Sensor LED Toggle

## Worklog

Oct 5th 2025: Downloaded the code onto ESP32 DEVKITV1 using the mac studio, Arduino IDE.
pull from github before you do anything.

This project uses an ESP32's capacitive touch sensing capability to toggle an LED when a touch sensor (aluminum foil) is touched.

## Hardware Requirements

- ESP32 DevKit1 board
- LED (or you can use the built-in LED on GPIO 2)
- Aluminum foil (for the touch sensor)
- Jumper wires

## Connections

1. **LED**: 
   - Connect to GPIO 2 (or use the built-in LED)
   - If using external LED, connect with appropriate resistor (220-330Ω)

2. **Touch Sensor**:
   - Connect aluminum foil to GPIO 4 (Touch0)
   - Make sure the foil is insulated from other components

## Setup Instructions

1. Open the `ESP32TouchLED.ino` file in the Arduino IDE
2. Select your ESP32 board from Tools > Board menu
3. Select the correct port from Tools > Port menu
4. Upload the sketch to your ESP32

## Usage

1. Touch the aluminum foil sensor
2. The LED will toggle (ON/OFF) with each touch
3. Monitor the Serial output (115200 baud) to see touch sensor values

## Adjusting Sensitivity

If the touch detection is too sensitive or not sensitive enough:
1. Monitor the Serial output to see the touch values
2. Adjust the `TOUCH_THRESHOLD` value in the code (default: 40)
   - Lower value = more sensitive
   - Higher value = less sensitive 
