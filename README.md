# Arduino Learning

Sketchbook for *Arduino, Electronics & the Road to Robotics*. Every sketch gets
its own folder with the code, a README explaining the concept, and a wiring photo
where there's something to see.

## Layout

```
Arduino/                        ← sketchbook root
├── 00_phase0_setup/            ← first light: Blink, external LED
│   ├── 01_Blink/
│   └── 02_External_LED/
├── 01_outputs/                 ← traffic light (P04), fade (P05), RGB (P06)
│   ├── 04_Traffic_Light/
│   └── 04b_Traffic_Light_Walk/
├── 02_digital_inputs/          ← button (P03), tilt, reed, shake, PIR
│   ├── 03_Button_LED/
│   └── 03b_Button_Toggle/
├── 03_analog_inputs/           ← pot, LDR, thermistor, joystick
├── 04_displays/                ← LCD, 7-seg, shift register
├── 05_comms/                   ← Serial, I²C (MPU6050), IR remote, RTC
├── 06_actuators/               ← servo, DC motor, stepper, relay
├── 07_integration/             ← multi-sensor projects
├── 99_capstone/                ← final build
└── _docs/                      ← foundations, cheat sheets
    ├── 00_foundations.md
    ├── 01_sessions_1_to_6.md
    ├── pinout_cheatsheet.md
    ├── common_wiring_patterns.md
    └── library_notes.md
```

The folders are **thematic, not chronological**. They group by concept — outputs,
digital inputs, analog inputs — while sketch numbers (`01_`, `02_`, `03_`) record
the order things were built. A sketch lands in the folder for the idea it
teaches, not the week it was written, so `03_Button_LED` sits under
`02_digital_inputs/` even though it came before anything in `01_outputs/`.

## Conventions

- **Folder name matches the `.ino` name.** Arduino requires this — `01_Blink/`
  must contain `01_Blink.ino`, or the IDE opens it as an untitled sketch.
- **One concept per sketch.** Variants get a letter suffix (`03b_Button_Toggle`)
  and stay next to the sketch they build on.
- **Every sketch folder has a README** following the same template: phase,
  session, status, what it does, parts, wiring table, concepts learned, lessons
  learned. Plus a `wiring.jpg` phone photo where there's something to see.
- **Write the README while the build is fresh.** This is 50+ projects. In six
  months there will be roughly zero memory of project #7 unless it's written
  down — and the capstone gets built by pulling patterns back out of these.
- **Pin numbers are named constants** at the top of the file, never bare numbers
  scattered through the logic.

## Progress

| # | Sketch | Phase | Session | Concept | Status |
| --- | --- | --- | --- | --- | --- |
| P01 | [Blink](00_phase0_setup/01_Blink/) | 0 — Setup | 1 | `pinMode`, `digitalWrite`, `delay` | ✓ Working |
| P02 | [External LED](00_phase0_setup/02_External_LED/) | 0 — Setup | 2 | breadboarding, current-limiting resistor, Serial | ✓ Working |
| P03 | [Button LED](02_digital_inputs/03_Button_LED/) | 2 — Digital Inputs | 3 | `INPUT_PULLUP`, active-LOW logic | ✓ Working |
| P03b | [Button Toggle](02_digital_inputs/03b_Button_Toggle/) | 2 — Digital Inputs | 3 | state variables, edge detection, bounce | ✓ Working |
| P04 | [Traffic Light](01_outputs/04_Traffic_Light/) | 1 — Outputs | 4 | `millis()` state machine, `enum class`, no `delay()` | ✓ Working |
| P04b | [Traffic Light + WALK](01_outputs/04b_Traffic_Light_Walk/) | 1 — Outputs | 4 | adding an FSM state, Serial RX/TX buffers, bounce | ✓ Working |

## Hardware

Arduino UNO (or compatible), breadboard, jumpers, 220 Ω resistors, assorted LEDs,
tactile push buttons.
