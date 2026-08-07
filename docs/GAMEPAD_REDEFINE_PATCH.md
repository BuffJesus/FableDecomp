# Gamepad Redefine patch — design & plan

Goal: split retail's single **"Redefine Keys"** frontend option into
**"Redefine Keys (Keyboard)"** (the existing screen, unchanged) and a new
**"Redefine Keys (Gamepad)"** screen that rebinds controller inputs, using
Unreal-Engine-style key names ("Face Button Bottom", "D-Pad Up", …).

This is a **base-game patch target**: the reconstruction reproduces the retail
redefine subsystem byte-for-byte (see `REDEFINE_INPUT_SYSTEM.md`), and this doc
is the delta a patch author applies on top of that understood code. Nothing here
requires a new storage layout — the engine already models controller-class input
records.

## 0. Why this is feasible (recap of the RE)

From `REDEFINE_INPUT_SYSTEM.md`:
- Bindings live in `CUserProfileManager` as two 28-byte-record vectors
  (`+0x54/+0x58` primary, `+0x60/+0x64` passive). Record = `{EGameAction +0,
  input +4}`.
- Input records are **device-type-aware**: `CKeyRedefiner::GetSubTypeForAction`
  (0x557CA0) already branches on `record[0]` type `0x3C` (analog/controller
  movement) returning four directional subtypes `0x0A–0x0D`, plus button classes
  `0x37`/`0x38`. **The store is not keyboard-only.**
- Control schemes are **named & data-driven** (`ResetAssignedInputs` 0x4085F0 /
  `ResetAssignedInputsWASD` 0x408820 resolve a scheme by name string and apply
  it to both vectors). A gamepad default is a *data + one apply call*, not a
  rewrite.

So the missing piece at retail is purely a **frontend screen** that (a) presents
gamepad bindings and (b) captures controller input into `0x3C`/button-type
records. Everything downstream (dispatch, persistence) already works.

## 1. Menu split

Retail frontend has one entry routing to detail `screen==4` ("Redefine Keys").
Patch:

| Menu label | Detail screen | Bindings shown | Capture device |
|------------|---------------|----------------|----------------|
| Redefine Keys (Keyboard) | 4 (unchanged) | primary vector `+0x54`, keyboard records | keyboard |
| Redefine Keys (Gamepad)  | **5 (new)**   | passive/controller records | gamepad |

In the reconstruction's checkpoint the detail title is chosen by `screen`:
- `screen==4` → title string becomes **"Redefine Keys (Keyboard)"**.
- `screen==5` → new title **"Redefine Keys (Gamepad)"**.

The options-list row that today opens screen 4 is duplicated into two rows; the
second sets the active screen to 5. (Reconstruction: the options-list builder in
`visual_boot_checkpoint.cpp` / `visual_boot_d3d9.cpp` — add the second row and a
hit-region that sets `screen=5`.)

## 2. UE gamepad key-name table

The gamepad screen renders bound inputs as UE-style names. The value column maps
a **captured controller input code → UE label**. This table is the display layer;
the stored record stays the engine's native `0x3C`/button type + code.

Standard XInput-class mapping (Unreal `FKey` naming, Xbox layout):

| UE label | UE FKey id | Xbox physical | XInput bit / axis |
|----------|-----------|---------------|-------------------|
| Face Button Bottom | `Gamepad_FaceButton_Bottom` | A | `XINPUT_GAMEPAD_A` 0x1000 |
| Face Button Right  | `Gamepad_FaceButton_Right`  | B | `XINPUT_GAMEPAD_B` 0x2000 |
| Face Button Left   | `Gamepad_FaceButton_Left`   | X | `XINPUT_GAMEPAD_X` 0x4000 |
| Face Button Top    | `Gamepad_FaceButton_Top`    | Y | `XINPUT_GAMEPAD_Y` 0x8000 |
| D-Pad Up    | `Gamepad_DPad_Up`    | D-Pad Up    | 0x0001 |
| D-Pad Down  | `Gamepad_DPad_Down`  | D-Pad Down  | 0x0002 |
| D-Pad Left  | `Gamepad_DPad_Left`  | D-Pad Left  | 0x0004 |
| D-Pad Right | `Gamepad_DPad_Right` | D-Pad Right | 0x0008 |
| Left Shoulder  | `Gamepad_LeftShoulder`  | LB | 0x0100 |
| Right Shoulder | `Gamepad_RightShoulder` | RB | 0x0200 |
| Left Trigger   | `Gamepad_LeftTrigger`   | LT | axis (Z+ / bLeftTrigger) |
| Right Trigger  | `Gamepad_RightTrigger`  | RT | axis (Z- / bRightTrigger) |
| Left Thumbstick Button  | `Gamepad_LeftThumbstick`  | L3 | 0x0040 |
| Right Thumbstick Button | `Gamepad_RightThumbstick` | R3 | 0x0080 |
| Special Left  | `Gamepad_Special_Left`  | Back/View  | 0x0020 |
| Special Right | `Gamepad_Special_Right` | Start/Menu | 0x0010 |
| Left Stick Up    | `Gamepad_LeftStick_Up`    | LS ↑ | thumbLY+ |
| Left Stick Down  | `Gamepad_LeftStick_Down`  | LS ↓ | thumbLY− |
| Left Stick Left  | `Gamepad_LeftStick_Left`  | LS ← | thumbLX− |
| Left Stick Right | `Gamepad_LeftStick_Right` | LS → | thumbLX+ |
| Right Stick Up    | `Gamepad_RightStick_Up`    | RS ↑ | thumbRY+ |
| Right Stick Down  | `Gamepad_RightStick_Down`  | RS ↓ | thumbRY− |
| Right Stick Left  | `Gamepad_RightStick_Left`  | RS ← | thumbRX− |
| Right Stick Right | `Gamepad_RightStick_Right` | RS → | thumbRX+ |

The four Left-Stick directions map to the engine's existing `0x3C` analog
subtypes `0x0A–0x0D` (§3 of the RE doc) — the movement axes are already modeled;
this display table just gives them UE names.

**Open item (needs live probe):** the exact byte encoding Fable stores in the
`input` record for each physical controller input (the `0x3C` case stores an
analog axis; buttons store a code under type `0x37`/`0x38`). Resolve by
capturing one binding at runtime (x32dbg on `CKeyRedefiner::Redefine` 0x557D20
with a controller event) and reading `this+0x1A4` candidate bytes. Until then the
table is keyed by UE id and the record encoding is filled per-input during the
capture-probe pass.

## 3. Gamepad detail screen (screen 5)

Mirrors the keyboard redefine list but:
1. Row source = the action set that is meaningfully controller-bindable (all
   `EGameAction`s that already carry a passive/controller record, plus the four
   movement axes).
2. Value column = UE label from §2 (fallback: "Unbound").
3. Capture: entering redefine on a row arms `CKeyRedefiner` for controller
   input; the next controller button/axis event is written as the record
   (type `0x37`/`0x38` button or `0x3C` axis), exactly as the keyboard path
   writes keyboard records.
4. Reset button applies the **gamepad default scheme** (new named scheme, §2 of
   RE doc — data + one `0x411B90` apply call). Conflict / coexistence gating
   reuses `AreAllowedToCoexist` (0x5578A0).

Reconstruction status: the checkpoint currently fakes the keyboard list with a
baked page atlas; screen 5 gets the same treatment first (authored list over the
existing redefine backdrop), then converges on the byte-pure `CRedefinerList`
live-scroll path once `Refresh` (0x557000) is reconstructed (task #11).

## 4. Bonus goal — controller hotbar cycling

Separable follow-up (RE doc §6): add one `EGameAction` (the enum is a persisted
`CVectorMap`), bind it to a shoulder/trigger in the passive vector `+0x60`, and
dispatch it in the runtime action consumer (`CGamePlayerInterface`
IsEventGameAction 0x445BD0 / GetControlMovementFromGameActionEvent 0x445C30) to
advance the hotbar index. No new UI required beyond a bindable row in screen 5.

## 5. Build order

1. **[data]** UE gamepad key-name table in the frontend renderer (this commit).
2. **[title]** screen 4 title → "Redefine Keys (Keyboard)"; add screen 5 title.
3. **[menu]** duplicate the options row → second row opens screen 5.
4. **[screen]** authored gamepad list over the redefine backdrop (screen 5),
   value column from §2.
5. **[probe]** live-capture the record encoding per physical controller input;
   fill §2 "record encoding" column.
6. **[capture]** arm `CKeyRedefiner` for controller events on screen 5 (reuses
   `Redefine` 0x557D20 once reconstructed).
7. **[scheme]** gamepad default scheme + reset apply-call.
8. **[patch]** port the delta onto retail `Fable.exe` as a loader/detour patch
   (the reconstruction is the reference implementation; the patch reuses the same
   engine entry points at their retail VAs).

Steps 1–4 are pure reconstruction-side (no live RE needed) and can land now;
5–7 need one x32dbg capture session; 8 is the eventual base-game deliverable.
