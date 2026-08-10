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

**Encoding — mostly RESOLVED (2026-08-09):** the def/scheme-side record encoding
is already recovered in `docs/CONTROLLER_ENUMS.md`: per-record layout `+0x00
GameAction`, `+0x04 ControllerType` (1=Xbox pad), `+0x0C EXboxControllerButton`
(A=1, B=2, …, LeftThumbstick=17, RightThumbstick=18), `+0x14/+0x18` C2DVector dir
hint. `FABLE_XBOX_CONTROL_SCHEME` (def entry 1099, 77 records) IS the native
gamepad default — so the §5/§7 gamepad scheme is buildable directly, no live
probe needed, and the EGameAction ordinals come from `debug_build/FableWin.pdb`
(see `tools/render_fable_frontend_subscreens.py`). The ONLY still-open piece is
the *runtime* 28-byte `CUserProfileManager` binding-record encoding under
`GetSubTypeForAction` types `0x37`/`0x38`/`0x3C` (the scheme→runtime apply in
`ResetAssignedInputs` 0x4085F0); confirm that against `CONTROLLER_ENUMS.md` before
assuming the runtime form matches the scheme form.

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

## 4b. RETAIL patch architecture (2026-08-09 — the actual deliverable)

The `visual_boot_*` screen-5 work is a **reconstruction mockup / visual spec only**
— retail does NOT bake menu screens; it builds the frontend dynamically. A patch
that runs on retail `Fable.exe` operates on that dynamic system. RE-confirmed
data-vs-code split:

**Controller value scheme — USE MODERN XInput / UE names, NOT retail's enum.**
The original Xbox controller (Duke/S) had Black/White buttons and **no bumpers**,
so retail's native `EXboxControllerButton` (Black=5/White=6, no LB/RB;
`docs/CONTROLLER_ENUMS.md`) is the WRONG scheme for modern pads. We bind/display
via the **UE-style modern XInput values** already in `visual_boot_d3d9.cpp`
(`kGamepadKeyValues`/`kGamepadKeyValueLabels`: Face Button Bottom/Right/Left/Top,
Left/Right Shoulder, triggers, thumbstick clicks, stick directions → XInput bits +
the engine's `0x3C` analog subtypes `0x0A–0x0D`). This is a design choice for
modern controllers, not a reproduction of retail's original-Xbox defaults, so the
low-confidence face/shoulder rows in CONTROLLER_ENUMS.md do NOT gate us.

**DATA (frontend defs — forge-moddable, no binary edit):**
- **Options list def #219** (rooted `(200,150)`, 30px pitch; rows = actions
  9/13/12/283, per `docs/FRONTEND_FORMAT.md`): add a **5th row** referencing a new
  action id + label text-tag. Rows come from the compiled UI def tree = data.
- **New "Redefine Keys (Gamepad)" screen def** — clone `UI_FRONTEND_SCREEN_REDEFINE_
  KEYS_PC`. `CRedefinerList::Initialise` (0x5566A0) builds its rows by iterating the
  screen def's UI-component children (reads the def ptr at `this+0x1a0`, then virtual
  child-walk) — **so the row/action set is data**. The clone's rows carry
  controller-type expected records so capture accepts pad events.
- **Gamepad default scheme** — a new named input scheme (RE doc §2: schemes are
  data, applied by `ResetAssignedInputs` 0x4085F0 via one `0x411B90` apply call),
  values from the UE/XInput table above.

**CODE (unavoidable detours, retail VAs):**
- **`CFrontEndManager::Action` @ 0x0059A238** — add a switch case for the new row's
  action id → set its used-key → `GotoNextScreen(...)`.
- **`CFrontEndManager::Init2` @ 0x00598A1C** — bind that new key → the new gamepad
  screen def id (Init2 does per-screen named binds; add one block or detour).
- **Capture needs NO code — CONFIRMED (disasm 2026-08-09).** `CKeyRedefiner::
  Redefine` (0x557D20) is device-agnostic: it compares `this+0x1A8` (expected
  state/type) vs `event+0x4` (incoming event type) only to pick a state branch,
  then on the accept path copies the incoming event's **28-byte record wholesale**
  into the candidate `this+0x1A4` via `rep movsd (7 dwords)`. No keyboard
  hardcoding — it captures whatever event arrives, keyboard OR controller.
  `GetSubTypeForAction` (0x557CA0) already handles controller types `0x37/0x38/
  0x3C`. So a gamepad screen driven by controller events captures pad records
  unmodified.

**Three opens — ALL VERIFIED (2026-08-09):** (1) frontend defs are editable data
in `data/CompiledDefs/frontend.bin` (parser+writer: `tools/parse_frontend.py`);
(2) `Action()` default path `0x59a7ff` is a clean `pop/leave/ret 4`, so an unused
action id is inert-safe (dispatch tail = `0x59a7d2`); (3) `Redefine` 0x557D20 is
device-agnostic (confirmed above).

**DATA HALF — BUILT + round-trip verified (`tools/build_gamepad_redefine_data.py`).**
Concrete frontend.bin structure: options list **#219 `UI_FRONTEND_LIST_OPTIONS_SUB_MENU`**
Children `[347,350,273,344]` = the 4 rows (row **344 `UI_OPTIONS_BUTTON_REDEFINE_KEYS`**
Action=283 is the clone source); redefine screen **#238 `UI_FRONTEND_SCREEN_REDEFINE_
KEYS_PC`**. The builder appends (to a scratch out-dir, never the install):
`UI_OPTIONS_BUTTON_REDEFINE_KEYS_GAMEPAD` (row clone, Action **284**) at gi 810,
`UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD` (screen clone) at gi 811, and rewrites
#219 Children → `[347,350,273,344,810]`. names.bin +2 with **crc0** (verified vs the
contract vectors + 2000/2000 stored). Round-trip re-parse confirms all three.
Remaining data polish: clone row 344's label child (345) to a "(Gamepad)" text tag
(else the row shows the keyboard label); the cloned screen's sub-defs are SHARED per
DEF_LOAD_CONTRACT (title/list stay keyboard until the code detour relabels). Still
TODO: the gamepad control **scheme** lives in game.bin (per CONTROLLER_ENUMS.md), a
separate append. In-game validation needs the CODE detour (below) so action 284 routes.

**Net:** a mostly-DATA patch (2 frontend-def clones/edits + 1 named scheme) + a
**small 2-site code detour** (Action case + Init2 bind). Capture/persistence reuse
the existing 28-byte record + passive vector `+0x60` — no new storage, no
redefiner code change.

Open verification before building: (1) ~~`Redefine` type-driven accept~~ ✓
CONFIRMED; (2) options list #219 def is runtime-editable via the forge frontend-def
path; (3) the new action id is free and survives `Action()`'s default-return
filter; (4) the frontend pumps controller input events to the active redefine
screen (likely — the runtime already resolves controller events via
GetSubTypeForAction; confirm the event source feeding `Redefine` when the gamepad
screen is active).

## 5. Build order (reconstruction mockup — superseded by §4b for the retail patch)

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

## Status (2026-08-09)

- **[data] DONE** (`b1621a8`): `kGamepadKeyValueLabels[24]` / `kGamepadKeyValues[24]`
  (UE-name ↔ XInput-bit table) in `visual_boot_d3d9.cpp`.
- **[title] DONE**: detail title splits screen 4 → "Redefine Keys (Keyboard)",
  screen 5 → "Redefine Keys (Gamepad)"; render clamp raised to `screen<=5`.
  The screen-5 entry point (`FableSetVisualFrontendDetailScreen`) previously
  rejected `screen==5`, making the "(Gamepad)" title dead code — **fixed**
  (guard `screen > 5`), so screen 5 is now reachable and shows its title over
  the redefine backdrop. Compile-verified; visual-QA of the title pending.
- **[screen] DONE (scaffold, compile-verified)**: screen 5 renders over screen
  4's Redefine Keys backdrop (`overlayFrame = 3 + (screen==5?4:screen)` since 5
  has no baked art) with the shared action-label column (`AppendRedefineActionText`
  now gated `screen==4||5`) and a new `AppendRedefineGamepadValueText` value
  column. Value column shows neutral "Unbound" — the per-row action→
  `EXboxControllerButton` default is NOT wired because only movement/DPad are
  HIGH-confidence in CONTROLLER_ENUMS.md (face/shoulder/trigger = LOW). Not yet
  visually verified (screen 5 is unreachable in-game until the menu row lands).
- **[menu] TODO** (step 3): the interactive host's options submenu is the retail
  4-row list — `ActivateVisualOptionsSelection` (guard `g_VisualOptionsSelection
  >= 4`, `detailScreens[4] = {1,3,2,4}`) + `FindVisualOptionsMenuRow` geometry +
  `g_OptionsRowChildren[4]` (all in visual_boot_checkpoint.cpp). A 5th
  "(Gamepad)" row needs: guard→5, `detailScreens[5]={1,3,2,4,5}`, extended
  hit-test rows, AND a 5th *visible* row (authored label/art over the baked
  4-row menu). Layout-sensitive → needs the headless visual-QA cycle
  (`build_bootstrap.ps1 -RetailFrontendBank …/data/graphics/pc/frontend.big` →
  synth-click → screenshot); do NOT do blind (risks regressing the 4-row menu).
- **[bindings] TODO** (step 4/5): per-row gamepad value column needs a
  higher-confidence action→button map than CONTROLLER_ENUMS.md's LOW cluster
  (movement/DPad are usable now; rest need a runtime capture or a 2nd source).
- **[hover] TODO**: `FableSetVisualFrontendRedefineHover` etc. are gated `!= 4`;
  generalize to screen 5 for row highlighting once the screen is reachable.
- **[probe/scheme/patch] TODO** (steps 5–8): unchanged; need the x32dbg capture
  of the per-input record encoding, then the base-game detour.
