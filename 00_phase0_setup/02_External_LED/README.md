# 02 — External LED

Same blink, but on an LED you wired yourself, plus Serial output.

## Hardware

| Part | Connection |
| --- | --- |
| LED anode (long leg) | D9 through a 220 Ω resistor |
| LED cathode (short leg) | GND |

D9 is PWM-capable — not needed for on/off, but it means this exact wiring is
reused unchanged for the fade sketch in `01_outputs/`.

## What's going on

- `const uint8_t PIN_LED = 9;` — the pin number lives in one named place. Rewire
  to a different pin and there's one line to change.
- `Serial.begin(9600)` — opens the serial channel back to the PC. Open the Serial
  Monitor (Tools → Serial Monitor) and set it to 9600 baud or the output is garbage.
- `Serial.println(...)` — the debugger. The board can't print a stack trace, so
  printing state as it changes is how you see what it's thinking.

## Concept

The resistor is not optional. An LED is a diode, not a resistor — with 5 V
straight across it, current is limited only by whatever the LED and the pin can
stand, and one of them loses. 220 Ω puts it around 15 mA, comfortably under the
UNO's 20 mA per-pin rating.

## Wiring photo

`wiring.jpg` — phone photo of the breadboard. Worth taking; future you will want
to know how this was actually laid out.
