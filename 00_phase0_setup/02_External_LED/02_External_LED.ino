const uint8_t PIN_LED = 9; // pin 9 - PWM-capable

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600); // opens the serial channel to PC
  Serial.println("P02 - external LED ready");
}

void loop() {
  digitalWrite(PIN_LED, HIGH);
  Serial.println("ON");
  delay(500);
  digitalWrite(PIN_LED, LOW);
  Serial.println("OFF");
  delay(500);
}