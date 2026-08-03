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

### Why INPUT_PULLUP works — current changes with the circuit open or closed

- **Open:** no current flows through the pull-up → **no voltage drop across it**
  → the pin sits at the full 5 V → **HIGH**
- **Closed:** current flows 5 V → resistor → button → GND → the entire 5 V drops
  across the resistor → the pin sits at 0 V → **LOW**
- So: **OPEN = HIGH, CLOSED = LOW.** That's active-low, and it falls out of the
  circuit rather than being a property of buttons. Pull-down + switch to 5 V
  would give the opposite convention.
- The resistor is ~30 kΩ and lives **on the silicon** — nothing on the
  breadboard, nothing in the button.

### Floating pins

- An input pin connected to nothing has no defined voltage — it picks up ambient noise
- Signature of a float: the reading scrambles between HIGH and LOW
- A rock-solid constant reading means the pin is tied to something, *not* floating

### The 4-leg button

- 4 legs, only 2 nodes — each face's two legs are permanently joined inside.
  Pressing joins face to face.
- **Rule: use diagonal legs.** A diagonal always takes one leg from each face,
  so orientation stops mattering.
- Wrong pair = permanent short = pin stuck LOW forever.

### Input pins vs output pins ← most transferable idea

- Input pins **measure** — draw ~nothing, so huge resistors are fine
- Output pins **drive** — supply real current, so resistors must be small
- Same reason 30 kΩ works on D2 and is useless on D9

### Code details

- `==` is an operator that produces a value — it isn't owned by `if`
- `HIGH`/`LOW` are just 1/0
- `bool pressed = (digitalRead(PIN) == LOW);` is the **translation layer**:
  electrical convention on the left, human meaning on the right
- Watch `=` vs `==` — sometimes compiles and silently misbehaves

## Lessons learned

### Diagnostic method ← the actual skill from this session

- **Split the system before guessing** — the input path and output path are independent
- Physically remove the suspect part to halve the fault space
- Serial is an **instrument, not a log**: `println` and `digitalWrite` read the
  same variable, so the monitor shows what *software believes*, independently of
  what the hardware does
- "Stuck" is a positive measurement, not an absence — a constant `PRESSED` proves
  the pin is tied to GND
- Distinguish *no output* from *output that never changes* — different causes entirely

### Serial Monitor mechanics

- Baud in the monitor must match `Serial.begin(9600)`
- Opening the monitor **resets the board**
- D0/D1 are shared with USB — never wire signals there

### Misc

- Reading a schematic: supply at top, ground at bottom, signal left to right.
  The LED triangle points toward the bar; triangle = anode = long leg. Ground
  symbols are net labels — every one is the same node.
- Series order doesn't matter. Resistor before or after the LED gives identical
  current; only the LED's polarity matters.
- A sketch runs whenever the board has power — there's no stop button. Unplug it
  or overwrite it with an empty sketch.
- `delay(50)` here keeps Serial from flooding and happens to mask bounce too.
  Both are the wrong fix — see [P03b](../03b_Button_Toggle/).
