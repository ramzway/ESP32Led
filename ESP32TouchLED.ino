const int buttonPin = D0;        // the number of the pushbutton pin
const int ledPin = LED_BUILTIN;  // the number of the LED pin

// Variables will change:
int ledState = HIGH;        // the current state of the output pin
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long long_press = 1000;

unsigned int intensity = 0;  // Brightness.
bool isLightOn = false;     // Track if light is on
int brightnessLevel = 0;    // Track current brightness level (0-4)

int press_detect() {
  // Debounce detect, returns 2 for long press and 1 for short press, 0 if none.
  // Captive when pressed.
  // read the state of the switch into a local variable:
  unsigned long start_time;
  if (digitalRead(buttonPin) == 0) {
    delay(debounceDelay);
    return 0;
  } else {
    start_time = millis();
  }

  // Maximum press duration (5 seconds)
  const unsigned long MAX_PRESS_DURATION = 5000;

  while (1) {
    delay(debounceDelay);
    // Check for timeout
    if (millis() - start_time > MAX_PRESS_DURATION) {
      Serial.println("Button press timeout");
      return 0;
    }
    
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    if (digitalRead(buttonPin) == 0) {
      if (millis() - start_time > long_press) {
        Serial.println("Long press \n");
        return 2;
      } else {
        Serial.println("Short press \n");
        return 1;
      }
    }
  }
}

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  Serial.println("Setup complete");
}

void loop() {
  int button_press = press_detect();
  if (button_press == 2) {      // long press
    Serial.println("Turning light off");
    digitalWrite(ledPin, LOW);  // Turn it off
    intensity = 0;
    isLightOn = false;
    brightnessLevel = 0;
  } else if (button_press == 1) {
    if (!isLightOn) {
      // First press - start at 75%
      intensity = 192;  // 75% of 255
      brightnessLevel = 3;
      isLightOn = true;
      Serial.println("First press - 75% brightness");
    } else {
      // Cycle through brightness levels
      brightnessLevel = (brightnessLevel + 1) % 5;
      switch (brightnessLevel) {
        case 0: intensity = 64;  break;  // 25%
        case 1: intensity = 128; break;  // 50%
        case 2: intensity = 192; break;  // 75%
        case 3: intensity = 255; break;  // 100%
        case 4: intensity = 64;  break;  // Back to 25%
      }
      Serial.print("Brightness level: ");
      Serial.println(brightnessLevel);
    }
    analogWrite(ledPin, intensity);
  }
}
