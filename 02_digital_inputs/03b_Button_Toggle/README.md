# 03b — Button Toggle (latching)

Same wiring as [`03_Button_LED`](../03_Button_LED/), different behavior: each
press flips the LED and it *stays* flipped. Press on, press off.

## What's going on

Two state variables carry information across loop iterations:

- `ledOn` — what the LED should currently be.
- `lastPressed` — what the button was doing the previous time through `loop()`.

```cpp
if (pressed && !lastPressed) {   // rising edge: was up, now down
  ledOn = !ledOn;
  digitalWrite(PIN_LED, ledOn);
}
lastPressed = pressed;
```

## Concept — edge detection

`loop()` runs thousands of times per second. A human press lasts ~100 ms, so the
board sees "pressed" on hundreds of consecutive iterations. Acting on the *level*
("is it pressed?") would toggle the LED hundreds of times per press and land on a
coin-flip. Acting on the *transition* ("did it just become pressed?") fires
exactly once.

That's the whole trick: compare now against last time, act only on the change.
It's the same shape as debouncing, encoder reading, and pulse counting later on.

## Concept — switch bounce

A mechanical contact doesn't close cleanly. It chatters for a few milliseconds,
so one physical press can look like several rising edges and the LED ends up in
the wrong state. `delay(20)` here is a crude debounce: after any read, ignore the
world for 20 ms and the chatter passes unseen.

It works, and it's still `delay()` — those 20 ms are 20 ms the board can do
nothing else. The `millis()` version records *when* the last edge happened and
ignores edges closer than ~20 ms to it, without ever blocking.
