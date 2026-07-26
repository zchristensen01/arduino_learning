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
- **Every sketch folder has a README** covering the wiring, what the code does,
  and the idea it's there to teach.
- **Pin numbers are named constants** at the top of the file, never bare numbers
  scattered through the logic.

## Progress

| # | Sketch | Concept |
| --- | --- | --- |
| 01 | [Blink](00_phase0_setup/01_Blink/) | `pinMode`, `digitalWrite`, `delay` |
| 02 | [External LED](00_phase0_setup/02_External_LED/) | breadboarding, current-limiting resistor, Serial |
| 03 | [Button LED](02_digital_inputs/03_Button_LED/) | `INPUT_PULLUP`, active-LOW logic |
| 03b | [Button Toggle](02_digital_inputs/03b_Button_Toggle/) | state variables, edge detection, bounce |

## Hardware

Arduino UNO (or compatible), breadboard, jumpers, 220 Ω resistors, assorted LEDs,
tactile push buttons.
