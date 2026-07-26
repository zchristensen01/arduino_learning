# 03 — Button LED (momentary)

LED follows the button: held down = on, released = off.

## Hardware

| Part | Connection |
| --- | --- |
| LED anode | D9 through 220 Ω |
| LED cathode | GND |
| Button leg A | D2 |
| Button leg B | GND |

No external pull-up resistor — the ATmega has one built into every input pin
and `INPUT_PULLUP` switches it on.

## What's going on

- `pinMode(PIN_BTN, INPUT_PULLUP)` — the internal ~20 kΩ resistor ties the pin
  to 5 V. Open button = pin reads HIGH. Pressed = the button shorts it to GND =
  pin reads LOW.
- `digitalRead(pin)` returns HIGH or LOW.
- `pressed ? HIGH : LOW` — read the inverted logic once, into a `bool` named for
  what it means, then stop thinking about it.

## Concept — active-LOW

Pressed reads LOW, which is backwards from intuition. The reason is floating
inputs: a bare input pin with nothing attached picks up ambient electrical noise
and reads randomly. A pull-up gives it a defined resting state, and the button's
job becomes overpowering that resistor by connecting straight to ground. Ground
wins, so pressed = LOW.

The alternative (external pull-*down*, button to 5 V) gives normal-feeling logic
but costs a resistor and a wire. Nearly all Arduino code uses `INPUT_PULLUP`.

## Known rough edge

`delay(50)` in the loop keeps the Serial Monitor from flooding, and it happens to
mask contact bounce too. Both of those are the wrong fix — see
[`03b_Button_Toggle`](../03b_Button_Toggle/) for why it starts to matter, and
`millis()` for the real answer.
