# Gamepad binding table — the redefine-screen value-column authority

All three columns proven (2026-08-09): `EXboxControllerButton` values from
`debug_build/FableWin.pdb` (see `ghidra_out/controller_enums_proven.md`), names
confirmed by the FableControllerSupport mod's `controls.def`. UE labels are the
DISPLAY layer (`kGamepadKeyValueLabels` in visual_boot_d3d9.cpp); the value the
engine STORES in a controller record (`+0x04 ControllerType=1`, `+0x0C` slot) is
the `EXboxControllerButton` value.

**Storage note:** the engine's pad enum is the ORIGINAL Xbox controller (Duke/S):
Black/White buttons, **no LB/RB bumpers**. A modern XInput pad has no Black/White
but has LB/RB — so the physical→stored mapping is not 1:1 and needs an XInput
bridge (like the FableControllerSupport DLL) to translate. Rows below marked
DESIGN are mapping choices for modern pads, not native equivalences.

| UE display label | XBOX_PAD_* name (stored) | value | XInput (modern physical) | note |
|---|---|---|---|---|
| Face Button Bottom | XBOX_PAD_A_BUTTON | 4 | A (0x1000) | proven 1:1 |
| Face Button Right  | XBOX_PAD_B_BUTTON | 5 | B (0x2000) | proven 1:1 |
| Face Button Left   | XBOX_PAD_X_BUTTON | 1 | X (0x4000) | proven 1:1 |
| Face Button Top    | XBOX_PAD_Y_BUTTON | 2 | Y (0x8000) | proven 1:1 |
| D-Pad Up    | XBOX_PAD_DPAD_UP_BUTTON    | 13 | 0x0001 | proven 1:1 |
| D-Pad Down  | XBOX_PAD_DPAD_DOWN_BUTTON  | 14 | 0x0002 | proven 1:1 |
| D-Pad Left  | XBOX_PAD_DPAD_LEFT_BUTTON  | 15 | 0x0004 | proven 1:1 |
| D-Pad Right | XBOX_PAD_DPAD_RIGHT_BUTTON | 16 | 0x0008 | proven 1:1 |
| Left Trigger  | XBOX_PAD_LEFT_TRIGGER  | 7 | LT axis | proven 1:1 |
| Right Trigger | XBOX_PAD_RIGHT_TRIGGER | 8 | RT axis | proven 1:1 |
| Left Thumbstick (click)  | XBOX_PAD_LEFT_STICK_BUTTON  | 9  | L3 (0x0040) | proven 1:1 |
| Right Thumbstick (click) | XBOX_PAD_RIGHT_STICK_BUTTON | 10 | R3 (0x0080) | proven 1:1 |
| Special Right (Start) | XBOX_PAD_START_BUTTON | 11 | Start (0x0010) | proven 1:1 |
| Special Left (Back)   | XBOX_PAD_BACK_BUTTON  | 12 | Back (0x0020) | proven 1:1 |
| Left Stick (analog move)  | XBOX_PAD_LEFT_ANALOGUE_STICK  | 17 | LX/LY | whole-stick; engine `0x3C` analog subtypes 0x0A-0x0D per direction |
| Right Stick (analog look) | XBOX_PAD_RIGHT_ANALOGUE_STICK | 18 | RX/RY | whole-stick |
| Left Shoulder  | XBOX_PAD_WHITE_BUTTON | 6 | LB (0x0100) | **DESIGN** — no native bumper; map LB→White (mod convention) |
| Right Shoulder | XBOX_PAD_BLACK_BUTTON | 3 | RB (0x0200) | **DESIGN** — map RB→Black |
| (unbound)      | XBOX_PAD_UNDEFINED_BUTTON | 0 | — | none/unset |

So `kGamepadKeyValues` gains a third column = the stored `EXboxControllerButton`
value above. The redefine screen displays the UE label, stores the value, and an
XInput bridge maps the physical modern button to that same slot. The two bumper
rows are the only non-native mappings and are open to a different choice
(e.g. LB/RB → triggers) if preferred.
