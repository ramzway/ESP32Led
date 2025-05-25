// ESP32 Touch Sensor and LED Toggle Project
// When the touch sensor (aluminum foil) is touched, the LED will toggle

// Define pins
const int LED_PIN = 2;     // Built-in LED on most ESP32 Dev boards
const int TOUCH_PIN = T0;  // GPIO 4 (Touch0) - Connect aluminum foil here

// Variables to handle touch detection and LED state
bool ledState = false;
bool lastTouchState = false;
bool currentTouchState = false;

// Debounce variables
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;    // Adjust this value to change debounce sensitivity

// Threshold for touch detection - may need adjustment based on your setup
const int TOUCH_THRESHOLD = 40;

// Variables for touch reading stability
const int NUM_READINGS = 5;
int touchReadings[5] = {0, 0, 0, 0, 0};
int readIndex = 0;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  
  // Configure LED pin as output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);
}

// Function to get stable touch reading
int getStableTouchReading() {
  // Add new reading
  touchReadings[readIndex] = touchRead(TOUCH_PIN);
  readIndex = (readIndex + 1) % NUM_READINGS;
  
  // Calculate average
  int sum = 0;
  for (int i = 0; i < NUM_READINGS; i++) {
    sum += touchReadings[i];
  }
  return sum / NUM_READINGS;
}

void loop() {
  // Get stable touch reading
  int touchValue = getStableTouchReading();
  
  // Debug output
  Serial.print("Touch value: ");
  Serial.println(touchValue);
  
  // Determine touch state based on threshold
  bool newTouchState = (touchValue < TOUCH_THRESHOLD);
  
  // Check if touch state has changed
  if (newTouchState != lastTouchState) {
    // Reset the debouncing timer
    lastDebounceTime = millis();
  }
  
  // Check if enough time has passed since last state change
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If touch state has changed and debounce period passed
    if (newTouchState != currentTouchState) {
      currentTouchState = newTouchState;
      
      // Only toggle LED if touch is detected (pressed, not released)
      if (currentTouchState) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
      }
    }
  }
  
  // Save the last touch state
  lastTouchState = newTouchState;
  
  // Small delay to stabilize readings
  delay(20);  // Reduced delay since we're using proper debouncing now
}
