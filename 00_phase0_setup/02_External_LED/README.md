# P02 — External LED

**Phase:** 0 — Setup
**Session:** 2
**Status:** ✓ Working

## What it does

Blinks an external red LED on pin D9 every 500 ms, prints ON/OFF to Serial.

## Parts

- 1 × red LED
- 1 × 220Ω resistor
- 2 × jumper wires
- Breadboard

## Wiring

| From | To |
|---------------|-----------------------|
| UNO D9 | LED anode (long leg) |
| LED cathode | 220Ω resistor |
| 220Ω other end| UNO GND |

> _TODO: add `wiring.jpg` — phone photo of the breadboard. Swap this line for `![wiring](wiring.jpg)` once it exists._

## Concepts learned

- LEDs are **polarized** — long leg = anode = +, short leg = cathode = –.
  Backwards means no light, not damage.
- Every LED needs a **current-limiting resistor**. An LED is a diode, not a
  resistor; nothing inside it limits current, so 5 V straight across it is
  destructive to the LED, the pin, or both.
- Ohm's Law resistor sizing: R = 3V / 0.010A = 300Ω → 220Ω close enough.
  (5 V supply – ~2 V LED forward drop = 3 V across the resistor. 220Ω lands
  around 14 mA, under the UNO's 20 mA per-pin limit.)
- `pinMode(pin, OUTPUT)` sets pin direction; `digitalWrite(pin, HIGH/LOW)` sets voltage
- `Serial.begin()` and `Serial.println()` for debug output — the Serial Monitor
  is the debug channel. The board can't show you a stack trace, so printing state
  as it changes is how you see what it's thinking.
- Sketchbook folder = sketch name rule (`02_External_LED/` must hold
  `02_External_LED.ino`)
- `const uint8_t PIN_LED = 9;` — the pin number lives in one named place, so
  rewiring means changing one line

## Lessons learned

- I plugged both LED legs into the same breadboard row the first time — no light.
  Rows a–e in the same numbered strip are all one electrical node, so both legs
  in one row shorts the LED out entirely. The legs have to straddle the center
  channel or sit in different numbered rows.
- Forgot to open Serial Monitor and thought upload had failed.
- Serial Monitor baud must be set to 9600 to match `Serial.begin(9600)`, or the
  output is garbage characters.
- D9 is PWM-capable. Not needed for on/off, but it means this exact wiring gets
  reused unchanged for the fade sketch in `01_outputs/`.
