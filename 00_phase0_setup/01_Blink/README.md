# 01 — Blink

Turn an LED on and off.

First light. The stock Arduino example, running on the on-board LED so nothing
needs to be wired up yet — it proves the board, the USB cable, the driver, and
the upload toolchain all work before any breadboard enters the picture.

## Hardware

Nothing external. `LED_BUILTIN` is the on-board LED (D13 on the UNO).

## What's going on

- `pinMode(LED_BUILTIN, OUTPUT)` — tell the pin to drive voltage rather than read it.
- `digitalWrite(pin, HIGH/LOW)` — 5 V or 0 V on that pin.
- `delay(ms)` — block for that many milliseconds.

The stock example uses `delay(1000)` for a one-second heartbeat. This copy is at
100 ms on / 75 ms off, which is fast enough to read as a flicker instead of a
blink — a quick way to feel where "blinking" turns into "on, but dim looking."

## Concept

`delay()` blocks. The whole program stops for the duration, so the board can't
read a button or talk over Serial while it's waiting. That's fine here and a
problem later — `millis()` is the fix, and it shows up around the button sketches.

## Reference

<https://docs.arduino.cc/built-in-examples/basics/Blink/>
