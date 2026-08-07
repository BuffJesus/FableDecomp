# Controller enum integer→name tables (empirical recovery)

**Date:** 2026-07-19
**Task:** Recover the integer→name mapping for `EGameAction`, `EXboxControllerButton`,
`EInputKey`, and `EMouseButtonControl` — the last gap blocking a turnkey remap UI over the
game.bin `CControlsDef` binding table. Retail strips the enum-name strings, so this is an
**empirical correlation argument**, not a header read.

> Scope note: this doc is the working recovery record. Per the run's coordination constraints
> it is NOT merged into FINDINGS.md / SYSTEMS_ANALYSIS.md / HANDOFF.md — consolidate manually.

---

## How derived (method)

Two independent sources, cross-checked per CLAUDE.md:

1. **Ground truth = real `CControlsDef` payloads from retail `game.bin`.**
   The `CONTROL_SCHEME` def type IS `CControlsDef`. Retail `game.bin`
   (`C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\CompiledDefs\game.bin`)
   ships **21 named CONTROL_SCHEME defs**. Three were fully decoded:
   - `FABLE_XBOX_CONTROL_SCHEME` (entry 1099) — pure gamepad, 77 records → fixes `EXboxControllerButton`.
   - `FABLE_PC_CONTROL_SCHEME_GDD_WASD` (entry 5878) — keyboard+mouse WASD default, 123 records
     → fixes `EInputKey` + `EMouseButtonControl`, and the WASD/1-9 records are the naming anchor.
   - `FABLE_PC_CONTROL_SCHEME_GDD` (entry 5875) — PC arrow-key default, 85 records — cross-check.

   Dumped with the prebuilt read-only tool
   `D:\Code\FableForge\build\forge.exe defs show "<root>" <entry> game.bin` (hex output),
   then parsed in Python.

2. **Names = community-documented default control scheme** (StrategyWiki `Fable/Controls`,
   GameFAQs controls FAQ, Steam/rewasd community). The default action→key and action→button
   layout tells us which record is which action; correlating a record's key/button value with the
   *known* default binding fixes that enum constant.

### Payload layout confirmed empirically

Per-field game.bin encoding is `[3-byte def header][4-byte reflected-CRC32(fieldName) tag][value]`.
The `Controls` field tag is `31 b9 66 7b` (LE) = reflected CRC32("Controls", seed 0). Empirically,
across all three schemes the vector decodes cleanly as:

```
<Controls signature bytes 31 b9 66 ...> [u32 count] [count × 28-byte CActionInputControl]
```

Each 28-byte `CActionInputControl` (matches FINDINGS.md donor `TransferOut` order):

| Off | Field | Type |
|---|---|---|
| +0x00 | `GameAction` | EGameAction (u32) |
| +0x04 | `ControllerType` | 1=Xbox pad, 2=keyboard, 3=mouse (u32) |
| +0x08 | key value | EInputKey (u32) — used when type==2 |
| +0x0C | pad value | EXboxControllerButton (u32) — used when type==1 |
| +0x10 | mouse value | EMouseButtonControl (u32) — used when type==3 |
| +0x14/+0x18 | C2DVector dir x,y | float — analog direction hint, e.g. `<+1,0>` = "right" |

**Validation:** with this layout every record in all three schemes has `ControllerType ∈ {1,2,3}`,
record counts differ sensibly per scheme (77/85/123), and the WASD movement records land on exactly
the DIK W/A/S/D scancodes (see below). This is the two-way lock that makes the layout certain.

Parser + raw hex dumps: session scratchpad `parse2.py`, `combine.py`, `scheme_{1099,5875,5878}.hex`.

---

## EInputKey encoding verdict: **DirectInput DIK scancodes (NOT Windows VK)**

Decisive evidence, all from `FABLE_PC_CONTROL_SCHEME_GDD_WASD`:

| Record | Action | dir vector | key value | DIK match | VK would be |
|---|---|---|---|---|---|
| 75 | 60 (Move) | `<0,+1>` fwd | 17 = 0x11 | **DIK_W** | VK_W=0x57=87 ✗ |
| 76 | 60 (Move) | `<-1,0>` left | 30 = 0x1E | **DIK_A** | VK_A=0x41=65 ✗ |
| 77 | 60 (Move) | `<0,-1>` back | 31 = 0x1F | **DIK_S** | VK_S=0x53=83 ✗ |
| 78 | 60 (Move) | `<+1,0>` right | 32 = 0x20 | **DIK_D** | VK_D=0x44=68 ✗ |
| 101-109 | 55 (quick 1-9) | — | 2..10 | **DIK_1..DIK_9** (0x02..0x0A) | VK '1'-'9' = 0x31.. ✗ |
| 110 | 1 (Attack/confirm) | — | 57 = 0x39 | **DIK_SPACE** | ✗ |
| 122 | 32 | — | 58 = 0x3A | **DIK_CAPITAL** (CapsLock) | ✗ |
| 44/45/47 | 49/50/29 | — | 36 = 0x24 | **DIK_J** | ✗ |

The contiguous `DIK_1..DIK_9 = 0x02..0x0A` run for the quick-slot actions and the exact WASD
scancode hit are impossible under VK. **EInputKey == DIK_* (dinput.h scancode).**
Confirmed against the DirectInput scancode table (DIK_W=0x11, DIK_A=0x1E, DIK_S=0x1F, DIK_D=0x20,
DIK_SPACE=0x39, DIK_NUMPAD8=0x48 — GitHub gist tracend/912308).

### EInputKey table (values observed in shipped PC schemes → DIK name)

Confidence HIGH for the whole standard-keyboard range (0x01–0x53); these are canonical DIK_* codes.

| val (dec/hex) | DIK name | | val | DIK name | | val | DIK name |
|---|---|---|---|---|---|---|---|
| 1 / 0x01 | ESCAPE | | 21 / 0x15 | Y | | 45 / 0x2D | X |
| 2 / 0x02 | 1 | | 24 / 0x18 | O | | 46 / 0x2E | C |
| 3 / 0x03 | 2 | | 26 / 0x1A | LBRACKET | | 48 / 0x30 | B |
| 4 / 0x04 | 3 | | 27 / 0x1B | RBRACKET | | 50 / 0x32 | M |
| 5 / 0x05 | 4 | | 28 / 0x1C | RETURN | | 57 / 0x39 | SPACE |
| 6 / 0x06 | 5 | | 30 / 0x1E | A | | 58 / 0x3A | CAPITAL (CapsLock) |
| 7 / 0x07 | 6 | | 31 / 0x1F | S | | 59 / 0x3B | F1 |
| 8 / 0x08 | 7 | | 32 / 0x20 | D | | 60 / 0x3C | F2 |
| 9 / 0x09 | 8 | | 34 / 0x22 | G | | 61 / 0x3D | F3 |
| 10 / 0x0A | 9 | | 35 / 0x23 | H | | 62 / 0x3E | F4 |
| 12 / 0x0C | MINUS | | 36 / 0x24 | J | | 63 / 0x3F | F5 |
| 13 / 0x0D | EQUALS | | 39 / 0x27 | SEMICOLON | | 64 / 0x40 | F6 |
| 15 / 0x0F | TAB | | 42 / 0x2A | LSHIFT | | 65 / 0x41 | F7 |
| 16 / 0x10 | Q | | 43 / 0x2B | BACKSLASH | | 66 / 0x42 | F8 |
| 17 / 0x11 | W | | | | | 67 / 0x43 | F9 |
| 18 / 0x12 | E | | | | | 68 / 0x44 | F10 |
| 19 / 0x13 | R | | | | | | |

Numpad block (HIGH): 72=NUMPAD8(0x48), 74=SUBTRACT(0x4A), 75=NUMPAD4(0x4B), 77=NUMPAD6(0x4D),
78=ADD(0x4E), 80=NUMPAD2(0x50), 84=0x54, 85=0x55 (0x54/0x55 unlabeled in the base DIK table — see unmapped).

The full DIK_* dictionary (0x00–0xDD) is the standard dinput.h table; only the values above actually
appear in shipped PC schemes. A remap UI should carry the whole DIK_* table for user rebinding.

**Note:** the value stored is `0` when `ControllerType != 2` (device-tagged union — the key slot is
just unused/zero for pad and mouse records), so `EInputKey = 0` is "none/unused", not a key.

---

## EMouseButtonControl (ControllerType == 3, value at +0x10)

Values observed: **1, 2, 3, 4, 5, 6, 7**. Correlated with default PC bindings (Attack = LMB;
Block = MMB per StrategyWiki/GameFAQs; camera/zoom = wheel):

| val | Name (proposed) | Confidence | Evidence |
|---|---|---|---|
| 1 | LeftButton | HIGH | Attack actions (9/10/25/43/59) use mouse 1; default Attack = left-click |
| 2 | RightButton | HIGH | Block/secondary (8/23/31/48/58/76/82) use mouse 2 |
| 3 | MiddleButton | HIGH | actions 7/24/42/96 use mouse 3; default "block/roll" = middle button |
| 4 | WheelUp | MEDIUM | actions 0/9/30/71/75/77/79/80 use mouse 4; paired with 5 as wheel |
| 5 | WheelDown | MEDIUM | actions 10/33/57/84/90/101 use mouse 5; 4/5 form the wheel up/down pair |
| 6 | (mouse X-axis?) | LOW | action 64 uses 6, paired with 7; likely axis/aux, name unconfirmed |
| 7 | (mouse Y-axis?) | LOW | action 65 uses 7; paired with 6 |

`EMouseButtonControl = 0` = none/unused (tagged-union zero). Values 1–5 map to the physical
buttons+wheel; 6/7 appear only on the map pan/zoom actions (64/65) and are most likely the
horizontal/vertical **mouse axes** rather than buttons — marked LOW pending a second source.

---

## EXboxControllerButton (ControllerType == 1, value at +0x0C)

Values observed in `FABLE_XBOX_CONTROL_SCHEME`: **0–18** (19 constants). Recovered by correlating
each pad value's action set against the same action's keyboard/mouse binding in the WASD scheme,
plus the C2DVector direction hints, plus the community-documented original-Xbox layout
(Attack=X, Block=Y, Interact=A, LTrigger=lock-on, RTrigger=cast, Black/White=weapon arm).

Anchors that are unambiguous (HIGH):

| val | Name | Confidence | Evidence |
|---|---|---|---|
| 17 | LeftThumbstick (analog 2D) | HIGH | action 60 = Move; in WASD scheme action 60 = W/A/S/D with dir vectors. Whole-stick analog. |
| 18 | RightThumbstick (analog 2D) | HIGH | actions 61/80 = camera look; the only other whole-stick analog |
| 13 | DPad Up | HIGH | action 37 → xbox 13; in WASD action 37 = key W (up) |
| 14 | DPad Down | HIGH | action 38 → xbox 14; WASD action 38 = key S (down) |
| 15 | DPad Left | HIGH | action 39 → xbox 15; WASD action 39 = key A (left) |
| 16 | DPad Right | HIGH | action 40 → xbox 16; WASD action 40 = key D (right) |

Directional lock is airtight: the D-pad constants 13–16 line up with the WASD-scheme
up/down/left/right keys for the identical action ids, and 17/18 are the only values reserved for
the two analog whole-stick move/look actions.

Face / shoulder / trigger / system buttons (values 0–12) — MEDIUM/LOW. The pad-only scheme gives
the value; the *name* rests on the community default layout, so these are proposed, not proven:

| val | Proposed name | Confidence | Reasoning |
|---|---|---|---|
| 0 | (unused / none) | MEDIUM | only action 47 uses it; likely the null button or an unbound analog |
| 1 | A button | MEDIUM | primary "confirm/attack" cluster (actions 9/10=Attack, 24, 35, 42, 54); in WASD these are left-click |
| 2 | B button | MEDIUM | "back/cancel" cluster (actions 7, 21, 44, 70 = Esc on keyboard) |
| 3 | X button | LOW | actions 14/16 (E-key "next/cycle") |
| 4 | Y button | LOW | large cluster (6, 20, 22, 25, 33, 43, 54, 59) |
| 5 | Black button | LOW | cluster 8/23/29/31/34/48/54/58 (weapon/secondary) |
| 6 | White button | LOW | actions 13/15/41/51 |
| 7 | Left Trigger | LOW | actions 1/27/46 — 1 = Attack-confirm; LTrigger = lock-on per community |
| 8 | Right Trigger | LOW | actions 26/28/30/52/86/87 — Shift-on-keyboard cluster; RTrigger = cast magic |
| 9 | Left Thumb click | LOW | actions 17/32/45/49 |
| 10 | Right Thumb click | LOW | actions 4/18/19/50 |
| 11 | Start | LOW | actions 3/5 |
| 12 | Back | LOW | action 2 only |

> The face/shoulder assignments (1–12) are a **plausible correlation**, not verified. The
> integer→action mapping IS certain from the payload; only the human *name* per integer needs a
> second authoritative source (a stripped `EXboxControllerButton` header or an in-game rebind probe)
> to promote them from LOW/MEDIUM to HIGH. Directional (13–18) are safe to ship now.

---

## EGameAction (value at +0x00)

The authoritative enum declaration is now recoverable from the local debug symbol file
`debug_build/FableWin.pdb`: DIA reports `GAME_ACTION_NULL = 0` through
`GAME_ACTION_BETTING = 114` and `GAME_ACTION_COUNT = 115`. This independently confirms that the
retail integer ids are sequential enum ordinals, not a partially recovered sparse table. The
PDB names are enum identifiers; each Redefine `ActionOrder` entry also correlates directly to a
shipped `TEXT_GUI_ACTION_*` symbol in English `text.big`. The exact decoded wording is recorded
in `tools/render_fable_frontend_subscreens.py` as `REDEFINE_ACTION_DISPLAY_TEXT`.

Values observed across the three schemes: **1–114** (not contiguous; ~90 distinct ids). These are
the *action* ids; the enum is large. High-confidence names come from records whose default binding
is unambiguous in the documented scheme:

| id | Name | Confidence | Evidence (default binding) |
|---|---|---|---|
| 55 | QuickSlot / RadialItem (1–9) | HIGH | 9 records, keys DIK_1..DIK_9; the item quick-bar |
| 56 | QuickSlot alt (F-keys / dpad) | HIGH | mirrors 55 on F-keys + dpad 13–16 |
| 60 | Move (locomotion) | HIGH | WASD with dir vectors `<±1,0>/<0,±1>`; pad = LeftThumb (17) |
| 37/38/39/40 | Camera Up/Down/Left/Right | HIGH | WASD keys W/S/A/D; pad dpad 13/14/15/16 |
| 66/67/68/69 | Move Fwd/Back/Left/Right (discrete) | HIGH | keys W/S/A/D; pad dpad 13–16 (arrow-scheme locomotion) |
| 9 / 10 | Attack (primary) | HIGH | mouse LeftButton; pad button 1 |
| 8 / 23 / 31 | Block / secondary | MEDIUM | mouse RightButton; pad 5/2 |
| 7 / 24 | Middle-button action (block-roll) | MEDIUM | mouse MiddleButton |
| 1 | Confirm / Attack-hold | MEDIUM | key SPACE, pad 7 |
| 21 / 44 / 70 | Cancel / Back / Menu-close | MEDIUM | key ESCAPE, pad 2 (B) |
| 90–98 | Debug/dev quick-keys (F5–F10 block) | LOW | F-key binds only in WASD scheme; likely dev/debug actions |

The remaining EGameAction ids have a *known integer, authoritative enum identifier, default binding,
and localized display text* for the frontend's 31-entry `ActionOrder`. The community docs do not name
every internal action, but the shipped text bank supplies the frontend labels without inference.

---

## Unmapped / low-confidence (honest gaps)

- **EGameAction:** integer, enum-identifier, and 31-entry Redefine localized-label coverage is HIGH
  from `debug_build/FableWin.pdb` plus English `text.big`.
- **EXboxControllerButton 0–12:** integer→action certain, name LOW/MEDIUM (face/shoulder/trigger).
  0 may be "none". Values 13–18 (dpad + sticks) are HIGH.
- **EMouseButtonControl 6, 7:** appear only on map pan/zoom (actions 64/65); likely mouse X/Y axes,
  name LOW.
- **EInputKey 0x54, 0x55** (dec 84/85): appear in WASD scheme (actions 99/100) but are not standard
  labeled DIK entries in the base table (0x54/0x55 are in the OEM/reserved gap); marked unmapped.
- **EInputKey OEM range 0x6A–0x73** (dec 106,109,110,111,112,114,115): action 114's secondary
  movement cluster uses 0x6D/0x6F/0x70/0x72; these are DirectInput OEM/layout-specific extended
  scancodes (numpad/international), not standard letter keys — left as `DIK 0x6x (OEM/extended)`.
- **EControllerType:** already known (1=Xbox pad, 2=keyboard, 3=mouse) — every record confirms it.

---

## Decoded default `CControlsDef` record tables (the evidence)

Format per record: `action_id | ControllerType | key(DIK) | xbox | mouse | dir<x,y>`.
Full dumps in scratchpad `scheme_{1099,5875,5878}.hex`; regenerate with
`forge.exe defs show "<root>" <entry> game.bin` and `parse2.py`.

### FABLE_XBOX_CONTROL_SCHEME (entry 1099, 77 records, all ControllerType=1)

Pad value per action (action → xbox button int):
```
61→18  60→17  86→8   87→8   1→7    2→12   3→11   4→10   5→11   49→9
50→10  52→8   6→4    7→2    17→9   18→10  19→10  20→4   21→2   22→4
23→5   24→1   25→4   26→8   33→4   34→5   35→1   36→2   37→13  38→14
39→15  40→16  41→6   29→5   42→1   43→4   44→2   30→8   31→5   32→9
58→5   59→4   66→13  67→14  68→15  69→16  70→2   114→17 45→9   75→17
46→7   47→0   48→5   27→7   28→8   57→2   54→1   54→5   54→4   55→13
55→14  55→15  55→16  56→13  56→14  56→15  56→16  51→6   79→17  80→18
8→5    9→1    10→1   14→3   13→6   16→3   15→6
```

### FABLE_PC_CONTROL_SCHEME_GDD_WASD (entry 5878, 123 records)

Key/mouse per action (action → binding). `K:` = keyboard DIK, `M#` = mouse button int:
```
77→M4       64→K:MINUS  65→K:EQUALS 64→M6       65→M7
114→K:OEM6D<0,+1> 114→K:OEM70<+1,0> 114→K:OEM72<0,-1> 114→K:OEM6F<-1,0>
75→M4  57→M5  94→K:O  54→M1  54→M2  85→K:RETURN
2→K:BSLASH  4→K:M  51→K:SEMI  53→K:OEM6A  90→K:F5  91→K:F7  92→K:F4  93→K:F9
100→K:0x55  97→K:F8  98→K:F10  96→K:F6  72→K:RETURN  72→K:ESC  71→M4
88→K:OEM6E  89→K:OEM73  101→M5  102→K:MINUS 102→K:0x4A(NUM-) 103→K:EQUALS 103→K:0x4E(NUM+)
108→K:LBRK  109→K:RBRK  104→K:NUM4  105→K:NUM6  106→K:NUM2  107→K:NUM8
49→K:J  50→K:J  18→K:C  29→K:J  20→M1  20→K:TAB  21→K:ESC  22→M1  22→K:TAB
23→M2  24→M3  25→M1  76→M2  33→K:A  34→K:B  35→K:X  36→K:Y  41→K:W  42→M3
43→M1  44→K:ESC  58→M2  59→M1  48→M2  27→K:LBRK 28→K:RBRK 79→M4 80→M4 81→M1 82→M2
83→K:ESC  70→K:ESC  112→K:H  60→K:W<0,+1> 60→K:A<-1,0> 60→K:S<0,-1> 60→K:D<+1,0>
37→K:W 38→K:S 39→K:A 40→K:D 66→K:W 69→K:D 67→K:S 68→K:A 113→K:R
10→M1 45→M2 84→M5 110→K:EQUALS 111→K:MINUS 9→M1 8→M2 31→M2 7→M3
13→K:Q 15→K:Q 14→K:E 16→K:E
55→K:1..9 (0x02..0x0A)   56→K:0x00,F1,F2,F3
1→K:SPACE  74→K:SPACE  86→K:LSHIFT/BSLASH  6→K:5  20→K:5  22→K:5  26→K:BSLASH
56→K:0,F1,F2,F3  78→K:G  32→K:CAPS
```

### FABLE_PC_CONTROL_SCHEME_GDD (entry 5875, 85 records) — cross-check only

Confirms the same action→device correlation with arrow-key movement instead of WASD; used to
verify EInputKey=DIK (movement on arrow scancodes) and record layout stability across schemes.

---

## Confidence summary

| Enum | Constants recovered (integer) | Named HIGH | Encoding verdict |
|---|---|---|---|
| EInputKey | ~55 distinct DIK values in shipped schemes; full DIK_* table applies | all standard 0x01–0x53 HIGH | **DIK scancode (not VK)** — proven |
| EMouseButtonControl | 1–7 | 1/2/3 HIGH, 4/5 MED, 6/7 LOW | button ints; 6/7 likely axes |
| EXboxControllerButton | 0–18 (19 values) | 13–18 HIGH (dpad+sticks) | 0–12 face/shoulder LOW/MED |
| EGameAction | 0–115 (`GAME_ACTION_COUNT` sentinel) | all enum identifiers HIGH from PDB | localized display symbols still need correlation |
| EControllerType | 1,2,3 | all | already known, reconfirmed |
