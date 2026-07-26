const uint8_t PIN_LED = 9; // LED via 220Ω -> GND
const uint8_t PIN_BTN = 2; // button leg -> D2, other leg -> GND

bool ledOn = false;
bool lastPressed = false;

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP); // internal pull-up ON: open = HIGH, pressed = LOW
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600); // opens the serial channel to PC
}

void loop() {
  bool pressed = (digitalRead(PIN_BTN) == LOW); // active-LOW: pressed pulls pin to GND
  
  if (pressed && !lastPressed) { // rising edge: was up, now down
    ledOn = !ledOn; // flip the latch
    digitalWrite(PIN_LED, ledOn);
    Serial.println(ledOn ? "LED ON" : "LED OFF");
  }
  lastPressed = pressed; // remember for next iteration
  
  delay(20); // slow the Serial spam; we'll do this properly with millis() soon
}