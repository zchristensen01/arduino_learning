# P04 — Traffic Light

**Phase:** 1 — Outputs
**Session:** 4
**Status:** ✓ Working

## What it does

Three LEDs cycle RED (4 s) → GREEN (6 s) → YELLOW (1.5 s) → RED, driven by a
`millis()` state machine with no `delay()` anywhere. A pedestrian button cuts
GREEN short — but only after cars have had a fair 1.5 s minimum.

## Parts

- 1 × red LED, 1 × yellow LED, 1 × green LED
- 3 × 220Ω resistors
- 1 × tactile push button (4-leg)
- Jumper wires
- Breadboard

## Wiring

| From | To |
|----------------|------------------------|
| UNO D9 | Red LED anode → 220Ω → GND |
| UNO D10 | Yellow LED anode → 220Ω → GND |
| UNO D11 | Green LED anode → 220Ω → GND |
| UNO D2 | Button leg A |
| Button leg B | UNO GND |

> _TODO: add `wiring.jpg` — phone photo of the breadboard. Swap this line for `![wiring](wiring.jpg)` once it exists._

## Concepts learned

### enum

- A C++ `enum` is just an integer wearing a nametag. `RED`, `YELLOW`, etc. are
  really 0, 1, 2 stored as ints.
- `enum class` makes it a **scoped** enum — the names live inside `Light`, and
  the type won't silently convert to an int.
- `::` — scope resolution operator. Java: `Light state = Light.RED;`
  C++: `Light state = Light::RED;`
- `: uint8_t` specifies the underlying storage type — tells the compiler to
  store this in one byte.

### Timing without delay()

- `millis()` returns the number of milliseconds since the chip powered on, as an
  `unsigned long`.
- **Stamp the start time whenever you enter a state.** `elapsed` has to be
  measured from when *this state* began, not from power-on — that's the whole
  point of re-stamping `stateStart` on every transition.
- `elapsed = millis() - stateStart` — computed fresh every pass, measures time
  since the current state began.
- **Always `now - start >= duration`. Never `now >= start + duration`.**

  The subtraction is overflow-safe: `millis()` wraps to 0 after ~49.7 days, and
  unsigned arithmetic wraps correctly, so the *difference* stays right even
  across the rollover. `start + duration` can overflow past the top of an
  `unsigned long` and wrap to a tiny number — then the comparison either fires
  instantly or never fires again. Same math, one version silently breaks.

### Structure

- With just `if (pressed)`, that branch would fire ~50,000 times per second.
- **Edge detection converts a continuous condition into a discrete event.**
- Every `case` has the same two parts: (1) do the state's job — drive the LEDs
  to match; (2) check whether it's time to leave, and if so change state and
  re-stamp `stateStart`.
- `bool` → `digitalWrite` works because `HIGH`/`true` = 1 and `LOW`/`false` = 0 —
  which is what lets `setLight(bool, bool, bool)` take plain `true`/`false`.

### Keeping the two halves apart

- **Sensor code and control code should never be entangled.** The button-reading
  code and the traffic-light code never talk to each other directly — only
  through the shared `walkRequested` boolean. One side sets it, the other
  consumes and clears it.
- That flag is the **seam** between the two halves. It survives thousands of loop
  iterations until the state machine consumes it, which is exactly why the button
  never has to know a traffic light exists.
- Clearing it belongs at the moment the request is **satisfied** — here, the
  GREEN → YELLOW transition. Leave it set and the next green gets cut short by a
  press nobody made.
- Polling the button on *every* iteration is the whole point. With `delay()` in
  the loop, a press during the wait is simply gone — nothing is listening.

> Serial gets a proper treatment in
> [P04b](../04b_Traffic_Light_Walk/#how-serial-actually-works) — `Serial.begin`,
> `UDR0`, the ring buffers, and the blocking asymmetry all live there.

## Lessons learned

- The math is the same as [P03b](../../02_digital_inputs/03b_Button_Toggle/) —
  edge detection, a flag that survives across iterations — just with three
  branches instead of one. Once the pattern clicked, the extra states were free.
- I first justified re-stamping `stateStart` as skipping Arduino core bootup
  time. Wrong reason — bootup is a rounding error. The real reason is that
  `elapsed` must be relative to the current state's start, or every state after
  the first would compare against time-since-power-on and fire immediately.

## Where this goes next

[P04b](../04b_Traffic_Light_Walk/) adds a fourth `WALK` state and finds the catch
in the flag rule above: *where* the clear belongs isn't fixed. Insert a state
between the request and its satisfaction, and the clear has to move with it —
which is the same idea one level up, that a flag is a **contract** between
whoever sets it and whoever clears it.
