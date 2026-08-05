# P04b — Traffic Light + WALK State

**Phase:** 1 — Outputs
**Session:** 4
**Status:** ✓ Working

Extension of [P04](../04_Traffic_Light/), working through all four "Try this
next" exercises from the course:

1. **A real `WALK` state** — a fourth enum value instead of just cutting green short
2. **A blinking pedestrian LED** — its own 500 ms rhythm *while the traffic FSM keeps running*
3. **Serial control** — type a letter to force a transition
4. **Watch bounce again** — a press counter to make the invisible countable

## What it does

RED (4 s) → GREEN (6 s, or 1.5 s if someone's waiting) → YELLOW (1.5 s) → **WALK
(4 s)** → GREEN. During WALK the car lights hold red while a dedicated pedestrian
LED on D5 blinks 4 times. Typing `r` / `g` / `y` / `w` in the Serial Monitor
forces that state immediately.

## Parts

Same as [P04](../04_Traffic_Light/), plus:

- 1 × white or blue LED (the pedestrian "walk" signal)
- 1 × 220Ω resistor

## Wiring

| From | To |
|----------------|------------------------|
| UNO D9 | Red LED anode → 220Ω → GND |
| UNO D10 | Yellow LED anode → 220Ω → GND |
| UNO D11 | Green LED anode → 220Ω → GND |
| UNO D5 | Walk LED anode → 220Ω → GND |
| UNO D2 | Button leg A |
| Button leg B | UNO GND |

> _TODO: add `wiring.jpg` — phone photo of the breadboard. Swap this line for `![wiring](wiring.jpg)` once it exists._

## Concepts learned

### 1. Adding a state — the most common FSM edit there is

- **Two states are the same only if their *transitions* match, not their
  outputs.** RED and WALK both light the red LED, but different durations and
  entry conditions make them distinct states.
- **If the difference is just a number, use data. If it's behaviour, use a state.**
- **A flag is a contract between whoever sets it and whoever clears it — and
  inserting a new state silently rewrites that contract.** [P04](../04_Traffic_Light/)
  established that the clear belongs where the request is *satisfied*; what it
  couldn't show is that "where" isn't fixed. `walkRequested` had to move its clear
  from GREEN → YELLOW to WALK → GREEN, because WALK is now the thing that actually
  satisfies the request. **Nothing in the compiler checks this** — the old code
  still compiles and still runs, just wrong.

### 2. Timed behaviour inside a state

- **Derive it from `elapsed` when it lives and dies with one state:**
  `blinkOn = ((elapsed / HALF) % 2 == 0)` needs no extra globals and can't fall
  out of sync.
- **Only run a separate timer when the behaviour outlives the state.**
- This is the real payoff of non-blocking code: two independent timed behaviours
  at once, no threads.

### 3. Serial control — the germ of a command interface

- **An override only jumps.** Assign the state, re-stamp `stateStart`, print.
  The main `switch` already knows how to *be* each state — don't duplicate its
  body. (Forget the re-stamp and the new state inherits an old clock and exits
  instantly.)
- `Serial.available()` **inspects**, `Serial.read()` **consumes** — hence
  check-then-consume, since `read()` on an empty buffer returns -1.

### How Serial actually works

- `Serial.begin(9600)` **sets up the UART hardware** at 9600 bits per second.
- `Serial.begin()` and the Serial Monitor are **two ends of one pipe**. The
  Monitor is just an app at the far end, fully interchangeable with PuTTY or a
  Python script. Baud must match on both sides because **UART has no clock line**.
- **The sketch runs from flash, not from the wire.** Upload writes it once; after
  that PC→Arduino carries nothing until I type. Unplug USB and the board runs
  identically.
- The inbound path has four stages: **`UDR0`** (1 byte, in hardware) →
  **interrupt** → **RX ring buffer** (64 bytes, in RAM) → **`Serial.read()`**.
  The interrupt is automatic, installed by `Serial.begin`; `read()` is the only
  piece I have to write. The buffer fills itself and never empties itself.

#### `UDR0` — the one-byte doorstep

- `UDR0` is a **one-byte parking spot inside the UART hardware** — a doorstep
  with room for one parcel. It's where a byte first lands coming in, and where it
  sits just before being shifted out going out.
- **`UDR0` moves nothing. Hardware raises flags; code moves bytes.** The hardware
  signals "a byte is here" or "I'm empty" — the *interrupt handler* is what
  actually copies between `UDR0` and the ring buffer. `UDR0` is a storage
  location, not an agent.
- Same division of labour everywhere on this chip, **including the timer behind
  `millis()`**.

#### What's actually mine to write

- My code's entire job in the serial system is: `loop()` reads out of RX,
  `loop()` prints into TX. **Nothing I write ever touches `UDR0`, the interrupts,
  or the wire.**
- Both calls are fast, local array operations in RAM. **`print` isn't sending
  anything** — it drops bytes in a queue and walks away. Actual transmission
  happens later, on the interrupt's schedule, at wire speed.

#### Why the buffers exist

- **TX and RX are separate buffers**, and buffers only ever hold bytes crossing
  the wire — internal variables never touch them. A buffer adds no speed; it buys
  **decoupling**, so neither side waits on the other.
- **TX exists because the wire is slow** — ~1.04 ms per character at 9600 baud,
  about 16,000 CPU instructions' worth. **RX exists because `UDR0` holds only one
  byte** — the buffer converts a hard 1 ms hardware deadline into a soft 64 ms one.

### ⭐ The blocking asymmetry

On **TX** my code is the producer, so it *can* be made to wait — `Serial.print`
blocks: **data safe, timing damaged.**

On **RX** the outside world is the producer and *cannot* be made to wait — bytes
are silently dropped: **data lost, timing fine.**

This is the shape of every producer–consumer boundary. First ask whether I'm the
fast side or the slow side.

### 4. Bounce, again

- **Bounce is mechanical** — springy contacts rebound and re-touch over ~1–5 ms,
  and no code prevents it. At ~50 µs sampling I get ~40 looks across the bounce,
  so one press fires several edges. **Release bounces too.**
- **P04 hides bounce because setting an already-true flag is idempotent.** The
  press counter doesn't fix anything — it makes an invisible event countable, so
  my debounce dead-time comes from measuring my own hardware. The moment an edge
  does something non-idempotent, three edges means three actions.

## Lessons learned

- **`digitalWrite` on a pin that's still `INPUT` toggles the internal pull-up
  instead of driving it** — ~0.1 mA, an invisible glow that looks exactly like
  dead hardware. Check `pinMode` before suspecting the breadboard.
- **A log line that reports the wrong state is worse than no log line.** Serial
  is the only window into the system, so a wrong print turns a logic bug into an
  apparent hardware bug.
- Every one of these uses the same skeleton: `enum` for states, `switch` in
  `loop()`, `millis()` for timing, transition logic. That skeleton is a robotics
  controller — prosthetic and drone state machines grow exactly this way.

## Bug found and fixed — the wrong-log-line lesson, live

YELLOW has two exits but originally had **one** print, sitting after the
`if/else` where both paths converge:

```cpp
if (walkRequested) { state = Light::WALK; }
else               { state = Light::RED;  }
stateStart = millis();
Serial.println("-> RED");   // fires on BOTH exits
```

So a transition into WALK announced itself as `-> RED`. The lights were doing the
right thing; the log was lying about it. Exactly the failure mode described above
— Serial is the only window into the system, so a wrong print turns a logic bug
into an apparent hardware bug. Worse here, because the LEDs *also* go red on the
walk phase, so the display corroborated the wrong story.

The fix is to put each print **inside its own branch**, next to the assignment it
describes:

```cpp
if (walkRequested) {
  state = Light::WALK;
  Serial.println("-> WALK");
} else {
  state = Light::RED;
  Serial.println("-> RED");
}
stateStart = millis();
```

**The general rule:** a log line belongs with the branch that caused it. Hoisting
it to where the paths converge is how it drifts out of sync with the code — and a
print that survives a refactor unchanged is the one most likely to be wrong.
`stateStart = millis()` *does* belong after the merge, because it's genuinely
common to both paths. Tell those two apart.
