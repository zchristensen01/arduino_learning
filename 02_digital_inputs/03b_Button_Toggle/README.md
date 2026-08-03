# P03b — Button Toggle (latching)

**Phase:** 2 — Digital Inputs
**Session:** 3
**Status:** ✓ Working

## What it does

Same wiring as [P03](../03_Button_LED/), different behavior: each press flips the
LED and it *stays* flipped. Press on, press off. Prints `LED ON` / `LED OFF` only
when the state actually changes.

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

- **Level** ([P03](../03_Button_LED/)) = "is it down right now?"
- **Edge** (this sketch) = "did it just change?" — **requires stored history**
- An edge doesn't exist on the pin. It only exists as a comparison between two
  samples in time.

### Where state can live

- `loop()` → **new stack every call.** Local vars are built and destroyed each
  pass, so they can't remember anything.
- **Global vars live in static storage** — they survive between `loop()` calls.
  That's the only place history can go.
- `loop()` is a function called repeatedly, not a loop.
- The output state lives in a variable; the pin is just a copy of it.

### The 4 combinations of `if (pressed && !lastPressed)`

| pressed | lastPressed | What it means |
|---------|-------------|---------------|
| no | no | nothing happening |
| yes | yes | still held down, nothing new |
| **yes** | **no** | **the transition — the press event, fires once** |
| no | yes | the release event (usable if you want release-triggered actions) |

Then to end every iteration → `lastPressed = pressed;`

```cpp
if (pressed && !lastPressed) {   // the press moment
  ledOn = !ledOn;
  digitalWrite(PIN_LED, ledOn);
}
lastPressed = pressed;           // outside the if — runs every pass
```

### Switch bounce

- Contacts physically bounce 1–5 ms → one press looks like several
- Symptom: the toggle lands on the wrong state, button "works half the time"
- `delay(20)` dodges it by sampling slowly; the real fix is `millis()` later

## Lessons learned

- `lastPressed = pressed;` **must be outside the `if`.** Inside, it latches and
  the button works exactly once. This is the first thing to check when the toggle
  misbehaves.
- A ~100 ms human press covers hundreds of loop iterations. Acting on the level
  would flip the LED hundreds of times per press and land on a coin flip; acting
  on the transition fires exactly once.
- Compare-now-against-last-time keeps coming back — debouncing, rotary encoders,
  pulse counting, and the pedestrian button in
  [P04](../../01_outputs/04_Traffic_Light/) are all this same four-line shape.
  Worth internalizing here where it's small.
- Printing only inside the `if` makes Serial readable — one line per press
  instead of a wall of text.
- `delay(20)` works and is still `delay()` — 20 ms the board can do nothing else.
  The `millis()` version records *when* the last edge happened and ignores edges
  closer than ~20 ms, without ever blocking.
