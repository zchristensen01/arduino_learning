const uint8_t PIN_RED = 9;
const uint8_t PIN_YEL = 10;
const uint8_t PIN_GRN = 11;
const uint8_t PIN_BTN = 2;

enum class Light : uint8_t { RED, GREEN, YELLOW }; // create the Light type

Light state = Light::RED;
unsigned long stateStart = 0;
bool walkRequested = false;
bool lastPressed = false;

void setLight(bool r , bool y, bool g) { // bool -> digitalWrite conversion works becasue HIGH and LOW is just #defines for 1 and 0
  digitalWrite(PIN_RED, r); // sets the voltage on a pin that is already configured as OUTPUT
  digitalWrite(PIN_YEL, y);
  digitalWrite(PIN_GRN, g); 
}

void setup() {
  pinMode(PIN_RED, OUTPUT); // configures pins electrical direction
  pinMode(PIN_YEL, OUTPUT);
  pinMode(PIN_GRN, OUTPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);
  Serial.begin(9600); // sets up UART hardware at 9600 bits per second
  stateStart = millis();
  Serial.println("Starting state: RED");
}

void loop() {
  unsigned long elapsed = millis() - stateStart; //computed fresh on every pass
  
  // poll the button on EVERY iteration. This is the whole point
  // With delay() in the loop, a press during the wait would be missed
  bool pressed = (digitalRead(PIN_BTN) == LOW); // is the button pressed right now?
  
  if (pressed && !lastPressed) { // the initial press case (falling edge)
    walkRequested = true; // sets a flag that survuves across thousands of loop iterations until the state machine consumes it
    Serial.println("[walk requested]");
  }
  
  lastPressed = pressed;

  /* the traffic-light state machine
    every case has the same 2 parts: 
      1. do the states job (drive LED to match current state)
      2. check whether it is time to leave - and if so, change state and re-stamp stateStart
  */
  switch (state) {
    case Light::RED:
      setLight(true, false, false);
      if (elapsed >= 4000) {
        state = Light::GREEN; 
        stateStart = millis();
        Serial.println("-> GREEN");
      }
      break;
    
    // green normally lasts 6s. But if a pedestrian is waiting (walk requested == true), cut it short
    // once cars have had a fair 1.5s minimum.
    case Light::GREEN:
      setLight(false, false, true);
      if (elapsed >= 6000 || (walkRequested && elapsed >= 1500)) {
        state = Light::YELLOW; 
        stateStart = millis();
        walkRequested = false; // request satisfied
        Serial.println("-> YELLOW");
      }
      break;
    case Light::YELLOW:
      setLight(false, true, false);
      if (elapsed >= 1500) {
        state = Light::RED; 
        stateStart = millis();
        Serial.println("-> RED");
      }
      break;
  }
}

