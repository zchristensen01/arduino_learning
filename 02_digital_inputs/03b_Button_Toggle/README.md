# P03b — Button Toggle (latching)

**Phase:** 2 — Digital Inputs
**Session:** 3
**Status:** ✓ Working

## What it does

Same wiring as [P03](../03_Button_LED/), different behavior: each press flips the
LED and it *stays* flipped. Press on, press off. Prints `LED ON` / `LED OFF` to
Serial only when the state actually changes.

## Parts

Identical to [P03](../03_Button_LED/) — nothing is rewired, only the code changes.

- 1 × red LED
- 1 × 220Ω resistor
- 1 × tactile push button (4-leg)
- 4 × jumper wires
- Breadboard

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

### Level vs edge

- **Level** = "is it down right now" → LED mirrors button ([P03](../03_Button_LED/))
- **Edge** = "did it just change" → LED toggles on press (this sketch)
- An edge **doesn't exist on the pin** — it only exists as a comparison between
  two samples in time
- Therefore edge detection *requires stored history*

### State & loop()

- `loop()` is a function called repeatedly, **not a loop**
- Locals are built and destroyed every pass → can't remember anything
- Globals live in static storage → the only thing that survives between calls
- `lastPressed = pressed;` must be **outside** the `if`, or it latches and the
  button works once
- Output state lives in a variable; the pin is just a copy of it

```cpp
if (pressed && !lastPressed) {   // rising edge: was up, now down
  ledOn = !ledOn;
  digitalWrite(PIN_LED, ledOn);
}
lastPressed = pressed;           // outside the if — runs every pass
```

### Four states of (pressed, lastPressed)

| pressed | lastPressed | Meaning |
|---------|-------------|---------|
| F | F | idle |
| T | F | **the press moment — the only case that fires** |
| T | T | held down (why holding doesn't strobe) |
| F | T | release moment (usable for release-triggered actions) |

### Switch bounce

- Contacts physically bounce 1–5 ms → one press looks like several
- Symptom: toggle lands on the wrong state, button "works half the time"
- `delay(20)` dodges it by sampling slowly; real fix is `millis()` later

### Code details

- `digitalWrite(PIN_LED, ledOn)` — a `bool` works directly where HIGH/LOW is
  expected, since `true`/`false` are 1/0
- `ledOn = !ledOn` is the standard "flip a flag" idiom

## Lessons learned

- Why `loop()` runs thousands of times per second matters here: a ~100 ms human
  press reads as "pressed" on hundreds of consecutive iterations. Acting on the
  level would flip the LED hundreds of times per press and land on a coin flip.
  Acting on the transition fires exactly once.
- Compare-now-against-last-time is the whole trick, and it keeps coming back —
  same shape as debouncing, rotary encoder reading, and pulse counting later on.
  Worth internalizing here where it's four lines.
- `delay(20)` works and is still `delay()` — those are 20 ms the board can do
  nothing else. The `millis()` version records *when* the last edge happened and
  ignores edges closer than ~20 ms to it, without ever blocking.
- Printing only inside the `if` instead of every pass makes the Serial Monitor
  readable — one line per press instead of a wall of text. Serial output is also
  slow enough to distort timing if you spam it.
- Toggle behaving backwards, or working exactly once, is almost always
  `lastPressed = pressed` having drifted inside the `if` block.
