# P01 — Blink

**Phase:** 0 — Setup
**Session:** 1
**Status:** ✓ Working

## What it does

Blinks the on-board LED (D13) at 100 ms on / 75 ms off. No external parts — this
is the "does anything work at all" test: board, USB cable, driver, and upload
toolchain, all proven before a breadboard enters the picture.

## Parts

- 1 × Arduino UNO
- 1 × USB-B cable

## Wiring

None. `LED_BUILTIN` is already wired to D13 on the UNO board itself.

| From | To |
|---------------|-----------------------|
| UNO USB port | PC |

## Concepts learned

- `setup()` runs once at power-on or reset; `loop()` runs forever after it
- `pinMode(pin, OUTPUT)` sets pin **direction** — drive voltage, don't read it
- `digitalWrite(pin, HIGH/LOW)` sets pin **voltage** — 5 V or 0 V
- `delay(ms)` pauses the program for that many milliseconds
- `LED_BUILTIN` is a named constant, not a number — it resolves to whichever pin
  the on-board LED is on for the board you selected (D13 on UNO, D6 on MKR1000)
- Upload workflow: Tools → Board, Tools → Port, then Upload

## Lessons learned

- The stock example uses `delay(1000)`. Dropping it to 100/75 ms makes the LED
  read as a *flicker* rather than a blink — useful for feeling where "blinking"
  turns into "on, but looks dim."
- `delay()` blocks. The board can do nothing else while it waits — no reading a
  button, no Serial. Fine here; a real problem by the time there's a button.
  `millis()` is the fix, and it shows up a few sketches from now.
- If upload fails, it's almost always the wrong **Port** under Tools, not the code.

## Reference

<https://docs.arduino.cc/built-in-examples/basics/Blink/>
