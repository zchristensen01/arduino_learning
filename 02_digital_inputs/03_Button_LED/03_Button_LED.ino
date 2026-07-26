const uint8_t PIN_LED = 9; // LED via 220Ω -> GND
const uint8_t PIN_BTN = 2; // button leg -> D2, other leg -> GND

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP); // internal pull-up ON: open = HIGH, pressed = LOW
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600); // opens the serial channel to PC
}

void loop() {
  bool pressed = (digitalRead(PIN_BTN) == LOW); // active-LOW: pressed pulls pin to GND
  digitalWrite(PIN_LED, pressed ? HIGH : LOW);
  Serial.println(pressed ? "PRESSED" : "released");
  delay(50); // slow the Serial spam; we'll do this properly with millis() soon
}