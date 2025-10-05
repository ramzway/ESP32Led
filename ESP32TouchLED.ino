// --- Pin Definitions ---
const int TOUCH_PIN = 27; // D27 (T7) for capacitive touch input
const int LED_PIN = 26;   // D26 for PWM output to the LED

// --- Timing and Thresholds ---
const unsigned long LONG_PRESS_DURATION = 1000; // 1 second for a long press
const int TOUCH_THRESHOLD = 40; // Lower value = more sensitive. Calibrate if needed.
const unsigned long COOLDOWN_PERIOD = 250;      // <-- NEW: 250ms "no reaction time" after a touch

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
unsigned long lastActionTime = 0; // <-- NEW: Tracks when the last action happened

// --- Setup Function ---
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  
  // Start with the LED off
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("ESP32 Touch Dimmer Ready (with Cooldown)");
}

// --- Main Loop ---
void loop() {
  // <-- NEW: Cooldown Guard Clause ---
  // If we are inside the cooldown period, skip the rest of the loop entirely.
  if (millis() - lastActionTime < COOLDOWN_PERIOD) {
    return;
  }
  
  // Check the current state of the touch pin
  bool currentlyTouched = (touchRead(TOUCH_PIN) < TOUCH_THRESHOLD);

  // --- Edge Detection for Touch Start ---
  if (currentlyTouched && !isTouched) {
    isTouched = true;
    longPressTriggered = false;
    touchStartTime = millis();
    Serial.println("-> Touch Started");
  }

  // --- Edge Detection for Touch Release ---
  if (!currentlyTouched && isTouched) {
    isTouched = false;
    unsigned long touchDuration = millis() - touchStartTime;
    Serial.print("-> Touch Released. Duration: ");
    Serial.println(touchDuration);

    if (touchDuration < LONG_PRESS_DURATION && !longPressTriggered) {
      handleShortPress();
    }
  }

  // --- Long Press Detection ---
  if (isTouched && !longPressTriggered) {
    if (millis() - touchStartTime > LONG_PRESS_DURATION) {
      handleLongPress();
      longPressTriggered = true; 
    }
  }
}

// --- Action for a Short Press ---
void handleShortPress() {
  Serial.println("Action: Short Press");
  if (!isLightOn) {
    isLightOn = true;
    brightnessIndex = 1;
  } else {
    brightnessIndex++;
    if (brightnessIndex >= numBrightnessLevels) {
      brightnessIndex = 1; 
    }
  }
  
  int newBrightness = brightnessLevels[brightnessIndex];
  analogWrite(LED_PIN, newBrightness);
  Serial.print("New Brightness: ");
  Serial.println(newBrightness);
  
  lastActionTime = millis(); // <-- MODIFIED: Update the action timer
}

// --- Action for a Long Press ---
void handleLongPress() {
  Serial.println("Action: Long Press");
  isLightOn = false;
  brightnessIndex = 0;
  analogWrite(LED_PIN, 0);
  Serial.println("Light Off");
  
  lastActionTime = millis(); // <-- MODIFIED: Update the action timer
}