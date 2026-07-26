# P03 — Button LED (momentary)

**Phase:** 2 — Digital Inputs
**Session:** 3
**Status:** ✓ Working

## What it does

LED on D9 follows a push button on D2 — held down = on, released = off. Prints
`PRESSED` / `released` to Serial every 50 ms.

## Parts

- 1 × red LED
- 1 × 220Ω resistor
- 1 × tactile push button (4-leg)
- 4 × jumper wires
- Breadboard

No external pull-up resistor — the ATmega has one built into every input pin.

## Wiring

| From | To |
|---------------|-----------------------|
| UNO D9 | LED anode (long leg) |
| LED cathode | 220Ω resistor |
| 220Ω other end| UNO GND |
| UNO D2 | Button leg A |
| Button leg B | UNO GND |

> _TODO: add `wiring.jpg` — phone photo of the breadboard. Swap this line for `![wiring](wiring.jpg)` once it exists._

## Concepts learned

- `pinMode(pin, INPUT_PULLUP)` switches on the chip's internal ~20 kΩ resistor,
  tying the pin to 5 V when nothing else is driving it
- `digitalRead(pin)` returns HIGH or LOW
- **Active-LOW logic:** open button = HIGH, pressed = LOW. Backwards from
  intuition, and standard across nearly all Arduino code.
- **Floating inputs:** a bare input pin with nothing attached picks up ambient
  electrical noise and reads randomly. The pull-up gives it a defined resting
  state; the button's job is to overpower that resistor by connecting straight
  to ground. Ground wins, so pressed = LOW.
- Read the inverted logic **once**, into a `bool` named for what it means
  (`pressed`), then stop thinking about it. `digitalRead(...) == LOW` scattered
  through the logic is how polarity bugs happen.
- `pressed ? HIGH : LOW` — the ternary operator, a compact if/else expression

## Lessons learned

- A 4-leg tactile button has two **pairs** of legs already shorted together
  inside. Wire across the wrong pair and the circuit is permanently closed —
  the LED sits on and the button does nothing. Legs directly across the diagonal
  are always the switched pair.
- The alternative wiring (external pull-*down* resistor, button to 5 V) gives
  normal-feeling logic — pressed = HIGH — but costs an extra resistor and wire.
  `INPUT_PULLUP` is free, which is why everyone uses it and everyone lives with
  the inverted logic.
- `delay(50)` in the loop is there to keep the Serial Monitor from flooding, and
  it happens to mask contact bounce too. Both are the wrong fix — see
  [`03b_Button_Toggle`](../03b_Button_Toggle/) for where that starts to bite,
  and `millis()` for the real answer.
