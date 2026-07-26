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

- **State variables** carry information across `loop()` iterations. Two here:
  `ledOn` (what the LED should be) and `lastPressed` (what the button was doing
  last time through). Globals, because anything declared inside `loop()` is
  destroyed and recreated every pass.
- **Edge detection** — act on the *transition*, not the *level*:

  ```cpp
  if (pressed && !lastPressed) {   // rising edge: was up, now down
    ledOn = !ledOn;
    digitalWrite(PIN_LED, ledOn);
  }
  lastPressed = pressed;           // remember for next iteration
  ```

  `loop()` runs thousands of times per second, so a ~100 ms human press reads as
  "pressed" on hundreds of consecutive iterations. Toggling on the level would
  flip the LED hundreds of times per press and land on a coin flip. Toggling on
  the transition fires exactly once.
- **Switch bounce:** a mechanical contact doesn't close cleanly — it chatters for
  a few milliseconds, so one physical press can look like several rising edges
  and the LED ends up wrong.
- `digitalWrite(PIN_LED, ledOn)` — a `bool` works directly where HIGH/LOW is
  expected, since `true`/`false` are 1/0
- `!ledOn` assigned back to itself is the standard "flip a flag" idiom

## Lessons learned

- Compare-now-against-last-time is the whole trick, and it keeps coming back —
  it's the same shape as debouncing, rotary encoder reading, and pulse counting
  later on. Worth internalizing here where it's four lines.
- `delay(20)` is a crude debounce: after any read, ignore the world for 20 ms and
  the chatter passes unseen. It works, and it's still `delay()` — those are 20 ms
  the board can do nothing else. The `millis()` version records *when* the last
  edge happened and ignores edges closer than ~20 ms to it, without ever blocking.
- Printing only inside the `if` instead of every pass makes the Serial Monitor
  actually readable — one line per press instead of a wall of text. Serial output
  is also slow enough to distort timing if you spam it.
- Getting the toggle backwards (LED off when it should be on) usually means the
  `lastPressed = pressed` line drifted inside the `if` block. It has to run every
  iteration, not just on an edge.
