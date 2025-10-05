// Using the ESP32 DEVKIT1 board.
// --- Pin Definitions ---
const int TOUCH_PIN = 27; // D27 (T7) for capacitive touch input
const int LED_PIN = 26;   // D26 for PWM output to the LED

// --- Timing and Thresholds ---
const unsigned long LONG_PRESS_DURATION = 1000; // 1 second for a long press
const int TOUCH_THRESHOLD = 40; // Lower value = more sensitive. Calibrate if needed.

// --- LED Brightness Levels ---
// Levels: 0=OFF, 25%, 50%, 75%, 100%
const int brightnessLevels[] = {0, 64, 128, 192, 255};
const int numBrightnessLevels = sizeof(brightnessLevels) / sizeof(brightnessLevels[0]);

// --- State Tracking Variables ---
bool isLightOn = false;
int brightnessIndex = 0; // Current index in the brightnessLevels array

// Variables for touch detection
bool isTouched = false;
bool longPressTriggered = false;
unsigned long touchStartTime = 0;

// --- Setup Function ---
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  
  // Start with the LED off
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("ESP32 Touch Dimmer Ready");
  Serial.print("Using Touch Pin: ");
  Serial.println(TOUCH_PIN);
  Serial.print("Using LED Pin: ");
  Serial.println(LED_PIN);
}

// --- Main Loop ---
void loop() {
  // Check the current state of the touch pin
  bool currentlyTouched = (touchRead(TOUCH_PIN) < TOUCH_THRESHOLD);

  // --- Edge Detection for Touch Start ---
  // This block runs only once when the pin is first touched.
  if (currentlyTouched && !isTouched) {
    isTouched = true;
    longPressTriggered = false;
    touchStartTime = millis();
    Serial.println("-> Touch Started");
  }

  // --- Edge Detection for Touch Release ---
  // This block runs only once when the pin is released.
  if (!currentlyTouched && isTouched) {
    isTouched = false;
    unsigned long touchDuration = millis() - touchStartTime;
    Serial.print("-> Touch Released. Duration: ");
    Serial.println(touchDuration);

    // If the touch was short AND a long press hasn't already been triggered...
    if (touchDuration < LONG_PRESS_DURATION && !longPressTriggered) {
      handleShortPress();
    }
  }

  // --- Long Press Detection ---
  // This block checks if the pin is being held down long enough.
  if (isTouched && !longPressTriggered) {
    if (millis() - touchStartTime > LONG_PRESS_DURATION) {
      handleLongPress();
      longPressTriggered = true; // Prevents this from running repeatedly
    }
  }
}

// --- Action for a Short Press ---
void handleShortPress() {
  Serial.println("Action: Short Press");
  if (!isLightOn) {
    // If light is off, turn it on to the first brightness level (25%)
    isLightOn = true;
    brightnessIndex = 1; // Index 1 is 64 (25%)
  } else {
    // If light is on, cycle to the next brightness level
    brightnessIndex++;
    // If we've passed the last level, loop back to the first (25%)
    if (brightnessIndex >= numBrightnessLevels) {
      brightnessIndex = 1; 
    }
  }
  
  int newBrightness = brightnessLevels[brightnessIndex];
  analogWrite(LED_PIN, newBrightness);
  Serial.print("New Brightness: ");
  Serial.println(newBrightness);
}

// --- Action for a Long Press ---
void handleLongPress() {
  Serial.println("Action: Long Press");
  // Turn the light off
  isLightOn = false;
  brightnessIndex = 0; // Index 0 is 0 (OFF)
  analogWrite(LED_PIN, 0);
  Serial.println("Light Off");
}
