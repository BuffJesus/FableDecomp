# Input — controller enums, binding store, Redefine Keys, gamepad patch (shelved)

Merged reference (2026-09-07) for the retail `Fable.exe` input system: the `CControlsDef` binding
records and their enum encodings (`EInputKey`/`EMouseButtonControl`/`EXboxControllerButton`/
`EGameAction`/`EControllerType`), the `CUserProfileManager` binding store and `CKeyRedefiner`/
`CRedefinerList` redefine subsystem, and the gamepad-redefine patch line (**SHELVED 2026-08-10**).
Consolidates the former `CONTROLLER_ENUMS.md` (2026-07-19, PDB-superseded 2026-08-09),
`REDEFINE_INPUT_SYSTEM.md` and `GAMEPAD_REDEFINE_PATCH.md` (2026-08-09/10). All addresses are retail
VAs (ImageBase 0x400000); function list is `rebuild/manifest/functions.tsv`.

Contents

- [Dated findings](#dated-findings)
- [Controller enum tables](#controller-enum-tables)
- [Binding store and redefine subsystem](#binding-store-and-redefine-subsystem)
- [Gamepad Redefine patch (SHELVED 2026-08-10)](#gamepad-redefine-patch-shelved-2026-08-10)
- [Open gaps](#open-gaps)
- [Superseded](#superseded)

---

## Dated findings

- **2026-07-19** — `EInputKey` == DirectInput DIK scancodes (NOT Windows VK), proven by the WASD
  scheme (W=0x11, A=0x1E, S=0x1F, D=0x20; quick-slots DIK_1..DIK_9 = 0x02..0x0A). Per-record
  `CActionInputControl` is 28 bytes; retail ships 21 named `CONTROL_SCHEME` defs.
- **2026-08-09** — `debug_build/FableWin.pdb` yields the button/action/device enums directly via DIA
  (`llvm-pdbutil pretty --enums`; `ghidra_out/controller_enums_proven.md`): `EXboxControllerButton`
  is `X=1, Y=2, BLACK=3, A=4, B=5, WHITE=6`; `EGameAction` runs `GAME_ACTION_NULL = 0` …
  `GAME_ACTION_BETTING = 114`, `GAME_ACTION_COUNT = 115`.
- **2026-08-09** — `CKeyRedefiner::GetSubTypeForAction` 0x557CA0 reconstructed byte-exact
  (RELOCATION_MATCH 120/120) by raw COFF extraction — the harness can't auto-verify embedded jump
  tables; `AreAllowedToCoexist` 0x5578A0 likewise (147/147).
- **2026-08-09** — Gamepad patch data half built (`tools/build_gamepad_redefine_data.py`: row
  clone gi 810 Action 284, screen clone gi 811) and code half (Action@0x59A238 detour) LIVE-TESTED:
  5th options row renders and routes. Later the same day: the "gamepad screen opened" belief was
  wrong (`ActionOnLeftUnclicked=283` data bug); #811 renders BLACK when correctly routed;
  `GetPrimaryInputVector` 0x4088E0 is NOT the row-value source.
- **2026-08-10** — Entire redefine/gamepad line **SHELVED**: reverted from the rebuild and the live
  install (stock frontend.bin/names.bin, FSE-only Mods.ini); 8 landed `CKeyRedefiner`/`CRedefinerList`
  functions un-landed; findings docs and PDB naming identities kept. Patch package at commit `07c1037`.

---

## Controller enum tables

*Originally `CONTROLLER_ENUMS.md` (2026-07-19). Task: recover the integer→name mapping for
`EGameAction`, `EXboxControllerButton`, `EInputKey`, and `EMouseButtonControl` — the last gap
blocking a turnkey remap UI over the game.bin `CControlsDef` binding table. Retail strips the
enum-name strings, so the original work was an **empirical correlation argument**, not a header read.*

> **PDB supersession (2026-08-09):** the debug `debug_build/FableWin.pdb` yields these enums
> *directly* via DIA (`llvm-pdbutil pretty --enums`) — see `ghidra_out/controller_enums_proven.md`.
> The empirical inferences below were RIGHT about `EControllerType` (NONE=0/XBOX_PAD=1/KEYBOARD=2/
> MOUSE=3) and the analog/DPad rows, but **WRONG about the face buttons**: proven
> `EXboxControllerButton` is `X=1, Y=2, BLACK=3, A=4, B=5, WHITE=6` (the empirical pass guessed
> A=1/B=2/X=3/Y=4). Trust the PDB values. The FableControllerSupport mod's `controls.def` confirms the
> *names* (`XBOX_PAD_A_BUTTON` … `XBOX_PAD_WHITE_BUTTON`, no bumpers — original-Xbox pad).

### How derived (method)

Two independent sources, cross-checked per CLAUDE.md:

1. **Ground truth = real `CControlsDef` payloads from retail `game.bin`.** The `CONTROL_SCHEME` def
   type IS `CControlsDef`. Retail `game.bin` (`C:\Programs\Steam\steamapps\common\Fable The Lost
   Chapters\data\CompiledDefs\game.bin`) ships **21 named CONTROL_SCHEME defs**. Three were fully
   decoded:
   - `FABLE_XBOX_CONTROL_SCHEME` (entry 1099) — pure gamepad, 77 records → fixes `EXboxControllerButton`.
   - `FABLE_PC_CONTROL_SCHEME_GDD_WASD` (entry 5878) — keyboard+mouse WASD default, 123 records →
     fixes `EInputKey` + `EMouseButtonControl`, and the WASD/1-9 records are the naming anchor.
   - `FABLE_PC_CONTROL_SCHEME_GDD` (entry 5875) — PC arrow-key default, 85 records — cross-check.

   Dumped with the prebuilt read-only tool `D:\Code\FableForge\build\forge.exe defs show "<root>"
   <entry> game.bin` (hex output), then parsed in Python.

2. **Names = community-documented default control scheme** (StrategyWiki `Fable/Controls`, GameFAQs
   controls FAQ, Steam/rewasd community). The default action→key and action→button layout tells us
   which record is which action; correlating a record's key/button value with the *known* default
   binding fixes that enum constant.

#### Payload layout confirmed empirically

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

### EInputKey encoding verdict: DirectInput DIK scancodes (NOT Windows VK)

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
scancode hit are impossible under VK. **EInputKey == DIK_* (dinput.h scancode).** Confirmed against
the DirectInput scancode table (DIK_W=0x11, DIK_A=0x1E, DIK_S=0x1F, DIK_D=0x20, DIK_SPACE=0x39,
DIK_NUMPAD8=0x48 — GitHub gist tracend/912308).

#### EInputKey table (values observed in shipped PC schemes → DIK name)

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
78=ADD(0x4E), 80=NUMPAD2(0x50), 84=0x54, 85=0x55 (0x54/0x55 unlabeled in the base DIK table — see
unmapped).

The full DIK_* dictionary (0x00–0xDD) is the standard dinput.h table; only the values above actually
appear in shipped PC schemes. A remap UI should carry the whole DIK_* table for user rebinding.

**Note:** the value stored is `0` when `ControllerType != 2` (device-tagged union — the key slot is
just unused/zero for pad and mouse records), so `EInputKey = 0` is "none/unused", not a key.

### EMouseButtonControl (ControllerType == 3, value at +0x10)

Values observed: **1, 2, 3, 4, 5, 6, 7**. Correlated with default PC bindings (Attack = LMB; Block =
MMB per StrategyWiki/GameFAQs; camera/zoom = wheel):

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

### EXboxControllerButton (ControllerType == 1, value at +0x0C)

**Proven values (PDB, 2026-08-09): `X=1, Y=2, BLACK=3, A=4, B=5, WHITE=6`.** The empirical recovery
below is kept for the action→integer evidence; its face-button *names* for 1–6 are superseded.

Values observed in `FABLE_XBOX_CONTROL_SCHEME`: **0–18** (19 constants). Recovered by correlating each
pad value's action set against the same action's keyboard/mouse binding in the WASD scheme, plus the
C2DVector direction hints, plus the community-documented original-Xbox layout (Attack=X, Block=Y,
Interact=A, LTrigger=lock-on, RTrigger=cast, Black/White=weapon arm).

Anchors that are unambiguous (HIGH, and consistent with the PDB):

| val | Name | Confidence | Evidence |
|---|---|---|---|
| 17 | LeftThumbstick (analog 2D) | HIGH | action 60 = Move; in WASD scheme action 60 = W/A/S/D with dir vectors. Whole-stick analog. |
| 18 | RightThumbstick (analog 2D) | HIGH | actions 61/80 = camera look; the only other whole-stick analog |
| 13 | DPad Up | HIGH | action 37 → xbox 13; in WASD action 37 = key W (up) |
| 14 | DPad Down | HIGH | action 38 → xbox 14; WASD action 38 = key S (down) |
| 15 | DPad Left | HIGH | action 39 → xbox 15; WASD action 39 = key A (left) |
| 16 | DPad Right | HIGH | action 40 → xbox 16; WASD action 40 = key D (right) |

Directional lock is airtight: the D-pad constants 13–16 line up with the WASD-scheme
up/down/left/right keys for the identical action ids, and 17/18 are the only values reserved for the
two analog whole-stick move/look actions.

Face / shoulder / trigger / system buttons (values 0–12) — the original empirical proposals (the
integer→action mapping is certain from the payload; the *names* for 1–6 are now overridden by the PDB):

| val | Proposed name (empirical, 2026-07-19) | Confidence | Reasoning |
|---|---|---|---|
| 0 | (unused / none) | MEDIUM | only action 47 uses it; likely the null button or an unbound analog |
| 1 | A button — **PDB: X** | superseded | primary "confirm/attack" cluster (actions 9/10=Attack, 24, 35, 42, 54); in WASD these are left-click |
| 2 | B button — **PDB: Y** | superseded | "back/cancel" cluster (actions 7, 21, 44, 70 = Esc on keyboard) |
| 3 | X button — **PDB: BLACK** | superseded | actions 14/16 (E-key "next/cycle") |
| 4 | Y button — **PDB: A** | superseded | large cluster (6, 20, 22, 25, 33, 43, 54, 59) |
| 5 | Black button — **PDB: B** | superseded | cluster 8/23/29/31/34/48/54/58 (weapon/secondary) |
| 6 | White button — **PDB: WHITE** | confirmed | actions 13/15/41/51 |
| 7 | Left Trigger | LOW | actions 1/27/46 — 1 = Attack-confirm; LTrigger = lock-on per community |
| 8 | Right Trigger | LOW | actions 26/28/30/52/86/87 — Shift-on-keyboard cluster; RTrigger = cast magic |
| 9 | Left Thumb click | LOW | actions 17/32/45/49 |
| 10 | Right Thumb click | LOW | actions 4/18/19/50 |
| 11 | Start | LOW | actions 3/5 |
| 12 | Back | LOW | action 2 only |

### EGameAction (value at +0x00)

The authoritative enum declaration is recoverable from the local debug symbol file
`debug_build/FableWin.pdb`: DIA reports `GAME_ACTION_NULL = 0` through `GAME_ACTION_BETTING = 114`
and `GAME_ACTION_COUNT = 115`. This independently confirms that the retail integer ids are sequential
enum ordinals, not a partially recovered sparse table. The PDB names are enum identifiers; each
Redefine `ActionOrder` entry also correlates directly to a shipped `TEXT_GUI_ACTION_*` symbol in
English `text.big`. The exact decoded wording is recorded in
`tools/render_fable_frontend_subscreens.py` as `REDEFINE_ACTION_DISPLAY_TEXT`.

Values observed across the three schemes: **1–114** (not contiguous; ~90 distinct ids). These are the
*action* ids; the enum is large. High-confidence names from records whose default binding is
unambiguous in the documented scheme:

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

### Unmapped / low-confidence (honest gaps)

- **EGameAction:** integer, enum-identifier, and 31-entry Redefine localized-label coverage is HIGH
  from `debug_build/FableWin.pdb` plus English `text.big`.
- **EXboxControllerButton 0–12:** integer→action certain; names for 1–6 now PROVEN by PDB; 7–12
  (trigger/thumb-click/Start/Back) and 0 remain empirical LOW/MEDIUM. Values 13–18 (dpad + sticks) HIGH.
- **EMouseButtonControl 6, 7:** appear only on map pan/zoom (actions 64/65); likely mouse X/Y axes,
  name LOW.
- **EInputKey 0x54, 0x55** (dec 84/85): appear in WASD scheme (actions 99/100) but are not standard
  labeled DIK entries in the base table (0x54/0x55 are in the OEM/reserved gap); marked unmapped.
- **EInputKey OEM range 0x6A–0x73** (dec 106,109,110,111,112,114,115): action 114's secondary
  movement cluster uses 0x6D/0x6F/0x70/0x72; these are DirectInput OEM/layout-specific extended
  scancodes (numpad/international), not standard letter keys — left as `DIK 0x6x (OEM/extended)`.
- **EControllerType:** 1=Xbox pad, 2=keyboard, 3=mouse (PDB: CONTROLLER_NONE=0) — every record confirms it.

### Decoded default `CControlsDef` record tables (the evidence)

Format per record: `action_id | ControllerType | key(DIK) | xbox | mouse | dir<x,y>`. Full dumps in
scratchpad `scheme_{1099,5875,5878}.hex`; regenerate with `forge.exe defs show "<root>" <entry>
game.bin` and `parse2.py`.

#### FABLE_XBOX_CONTROL_SCHEME (entry 1099, 77 records, all ControllerType=1)

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

#### FABLE_PC_CONTROL_SCHEME_GDD_WASD (entry 5878, 123 records)

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

#### FABLE_PC_CONTROL_SCHEME_GDD (entry 5875, 85 records) — cross-check only

Confirms the same action→device correlation with arrow-key movement instead of WASD; used to verify
EInputKey=DIK (movement on arrow scancodes) and record layout stability across schemes.

### Confidence summary

| Enum | Constants recovered (integer) | Named HIGH | Encoding verdict |
|---|---|---|---|
| EInputKey | ~55 distinct DIK values in shipped schemes; full DIK_* table applies | all standard 0x01–0x53 HIGH | **DIK scancode (not VK)** — proven |
| EMouseButtonControl | 1–7 | 1/2/3 HIGH, 4/5 MED, 6/7 LOW | button ints; 6/7 likely axes |
| EXboxControllerButton | 0–18 (19 values) | 1–6 PROVEN (PDB), 13–18 HIGH (dpad+sticks) | 0, 7–12 LOW/MED |
| EGameAction | 0–115 (`GAME_ACTION_COUNT` sentinel) | all enum identifiers HIGH from PDB | localized display symbols still need correlation |
| EControllerType | 1,2,3 | all | already known, reconfirmed |

---

## Binding store and redefine subsystem

*Originally `REDEFINE_INPUT_SYSTEM.md`. Byte-level RE of the retail input-binding and key-redefine
subsystem, so it can be understood and **patched** (gamepad redefinition, controller hotbar cycling,
custom menu entries).*

### Where bindings live: `CUserProfileManager`

The active profile owns **two parallel binding vectors** (STL `vector<CInputBinding>`, begin/end
pointer pairs):

| field | meaning |
|-------|---------|
| `this+0x54` / `this+0x58` | **primary** binding vector (begin/end) |
| `this+0x60` / `this+0x64` | **passive / alternate** binding vector (begin/end) |

Each record is **0x1C (28) bytes** — proven by the `imul 0x92492493; sar 4` divide-by-28 count idiom
in `GetAssignedInputForAction` (0x408C90):

```
CInputBinding {                 // 0x1C bytes
    +0x00  EGameAction action;  // dword key; linear-searched by action
    +0x04  <input record>       // 0x18 bytes: the assigned physical input(s)
}
```

`GetAssignedInputForAction(EGameAction, bool usePassive)` @ **0x408C90**:
- `usePassive == 0` → search the `+0x54` vector; `!= 0` → search `+0x60`.
- If the chosen vector is **empty** (begin == end) it lazily calls `ResetAssignedInputs` (0x4085F0)
  to populate defaults, then linear-scans 28-byte records comparing `record->action == target`.
- `GetAssignedInputForMovementAction` (0x408D90) and `GetAssignedInputForContextSensitiveItemAction`
  (0x408F50) are the same shape over the passive vector — movement/context actions live in the
  alternate set.

(Note: the 2026-08-09 live-test notes read the `usePassive` polarity the other way — `!=0` →
keyboard `+0x54`, `==0` → controller `+0x60`. The two readings were never reconciled before the
shelving; re-derive from the disassembly at 0x408C90 before relying on either.)

Live-instrumented fact (2026-08-09): on the retail profile-manager singleton (`*(void**)0x13B7D4C`)
the primary `+0x54` and passive `+0x60` vectors BOTH hold **123** 28-byte records (`COUNTS a=0x7B
b=0x7B`) — controller bindings already exist in `+0x60`.

### Named binding schemes: reset applies a scheme by name

`ResetAssignedInputsWASD` @ **0x408820** and `ResetAssignedInputs` @ **0x4085F0** do NOT hardcode key
codes — they look up a **named input scheme** and apply it:

- WASD (0x408820): loads scheme-name string const `0x122E1FC` (or `0x122E1D8` under a global flag),
  constructs a `CWideString` (0x99EBF0, len −1), then via globals `0x13B86A0`/`0x13B8760` calls
  `0x44C6B0` or `0x43368D` to resolve the scheme, and applies it to BOTH binding maps (`this+0x54`
  and `this+0x60`) via `0x411B90`.
- This is the GDD/config-driven control-scheme system: schemes are data, keyed by name. **Adding a
  new scheme (e.g. a gamepad default) is a data + one apply-call change, not a rewrite.**
  `FABLE_XBOX_CONTROL_SCHEME` (def entry 1099, 77 records) IS the native gamepad default.

The reconstruction's authored visual checkpoint mirrors this with `kVisualRedefineDefaults` (WASD
`[31,9,27,12,...]`) vs `kVisualRedefineArrowDefaults` (`[41,42,43,44,...]`); only the first four
(movement) rows differ between schemes.

### Input records are DEVICE-TYPE-AWARE (the gamepad foundation)

`CKeyRedefiner::GetSubTypeForAction` @ **0x557CA0** branches on the input record's **type field**
(`record[0]`):

| type (`record[0]`) | handling |
|--------------------|----------|
| `0x37` | increments global counter `0x13B8AD0` (device/event class A) |
| `0x38` | increments global counter `0x13B8ACC` (device/event class B) |
| `0x3C` | **analog/controller movement** — calls `0x40ED10`, then a 4-way jump table returning subtypes **0x0A–0x0D** (four directions) |
| other | returns 0 |

The `0x3C` case with four directional subtypes is analog-stick / control-axis movement. **The engine
already models controller-class input records** — the binding store and redefiner are not
keyboard-only. What's missing at retail is a *frontend screen* that captures gamepad input into these
records.

#### Byte-exact reconstruction (verified RELOCATION_MATCH, 120/120 bytes)

`GetSubTypeForAction` is `__stdcall` (single stack arg, `ret 0x4`). Reconstructed source — proven
byte-identical to retail via direct COFF-section extraction + relocation masking (10 reloc slots; the
harness's objdump path can't auto-verify this one, see the jump-table gotcha in CLAUDE.md; use
`tools/decomp_pipeline/verify_land_jumptable.py`). `AreAllowedToCoexist` 0x5578A0 was verified the
same way (147/147). (Both were un-landed in the 2026-08-10 shelving; sources recoverable from git.)

```c
extern int g_subtypeCounterA;   // 0x13B8AD0  (type 0x37 event class)
extern int g_subtypeCounterB;   // 0x13B8ACC  (type 0x38 event class)
extern int __fastcall GetSubTypeAnalogDirection(void* record);   // 0x40ED10
int __stdcall GetSubTypeForAction(int* record){
    switch (*record) {
    case 0x37: return ++g_subtypeCounterA;
    case 0x38: return ++g_subtypeCounterB;
    case 0x3c:
        switch (GetSubTypeAnalogDirection(record)) {   // 0x40ED10, record kept in ecx
        case 1: return 0xa;   // subtype 0x0A
        case 2: return 0xb;   // subtype 0x0B
        case 3: return 0xc;   // subtype 0x0C
        case 4: return 0xd;   // subtype 0x0D
        }
    }
    return 0;
}
```

The outer `switch` compiles to the retail subtract-chain (`sub 0x37; je / dec; je / sub 4; jne`); the
inner `switch` on the analog direction becomes the embedded `dec; cmp 3; ja; jmp [eax*4+table]` jump
table (subtypes 0x0A–0x0D). This is the exact device-type dispatch a gamepad-redefine screen drives.

### The redefine UI

- `CRedefinerList` @ **0x5566A0** (`Initialise`), **0x557000** (`Refresh`), **0x5570F0**
  (`OnActivate`), **0x556810** (`UpdateUndefinedWarning`), **0x5567B0** (`ClearList`):
  builds/refreshes the scrollable list of redefinable action rows. This is the class whose **live
  scroll rendering** the visual checkpoint fakes with a baked page atlas — reconstructing `Refresh` +
  the row draw is the byte-pure native-scroll path. `Initialise` builds its rows by iterating the
  screen def's UI-component children (reads the def ptr at `this+0x1a0`, then virtual child-walk) —
  the row/action set is data.
- `CKeyRedefiner` @ **0x558770** (`Initialise`), **0x557D20** (`Redefine`), **0x557EB0**
  (`ProcessEvent`), **0x557A10** (`UpdateKeyText`), **0x557BD0** (`CancelSelection`), **0x5580B0**
  (`ClearDuplicateDefinitions`), **0x557C10** (`ChangeState`): the per-row capture control. State
  lives in `this+0x1A8`; the captured candidate binding at `this+0x1A4` (fields at +0x08/+0x10
  compared against the incoming event in `Redefine`). It is a type-aware state machine — it matches
  the event's type (`event+0x4`) against the expected type before accepting.
- `Redefine` (0x557D20) is **device-agnostic** (disasm 2026-08-09): it compares `this+0x1A8` vs
  `event+0x4` only to pick a state branch, then on the accept path copies the incoming event's
  **28-byte record wholesale** into the candidate `this+0x1A4` via `rep movsd (7 dwords)`. No keyboard
  hardcoding — it captures whatever event arrives, keyboard OR controller.
- `AreAllowedToCoexist` @ **0x5578A0** and `IsRedefinableKey` (`CKeyRedefiner::IsRedefinableKey`,
  referenced from the visual bridge) gate which keys can share/replace a binding.
- `CRedefinerList::Refresh` @0x557000 at **0x557008** calls `GetPrimaryInputVector` @ **0x4088E0** on
  the `CUserProfileManager` singleton (from `0x40D2A0`, global `[0x13B7D4C]`), then feeds the vector
  to the row builder (`0x556A40`). `GetPrimaryInputVector` is hardcoded to the primary vector:
  ```
  mov eax,[ecx+0x54]      ; primary begin
  mov edx,[ecx+0x58]      ; primary end
  cmp eax,edx
  lea esi,[ecx+0x54]      ; return &primary
  jne +                   ; if empty -> EnsureDefaults (0x4085F0)
  call 0x4085F0
  mov eax,esi ; ret       ; returns &this[0x54]
  ```
  There is NO device/usePassive parameter. (Live test 2026-08-09 showed a screen-gated redirect of
  0x4088E0 has ZERO effect on displayed row values — it is NOT the row-value source; candidates are
  `GetAssignedInputForAction` 0x408C90 called from big builders at 0x48CAF0 / 0x642A60.)
- The redefine screen is data-driven: list **#217** (`ActionOrder` = the 31 EGameAction ordinals,
  `ActionMap` = action→text tags, `Redefiner`=**#412** `UI_KEY_REDEFINER_BASE`); screen **#238**
  `UI_FRONTEND_SCREEN_REDEFINE_KEYS_PC` (`Type=10`, `Children=[217,342,632,120,585]`,
  `NonScrollingChildren=[0,1]`). The redefiner has NO keyboard-vs-controller def field — the vector
  choice is CODE.

### Action → input dispatch at runtime

- `CGamePlayerInterface::IsEventGameAction` @ **0x445BD0**, `GetControlMovementFromGameActionEvent` @
  **0x445C30**, `IsGameActionEventInQueue` @ **0x445CB0/0x446B10**,
  `CInputProcessGameBase::IsGameActionEventInQueue` @ **0x687D80**: the runtime consumes bound inputs
  → `EGameAction` events. `EGameAction` is the enum keyed through `CVectorMap<EGameAction,…>`
  (0x447CE0) and persisted via `CPersistContext::TransferMap<EGameAction,CCharString>`
  (0x463334/0x475026).
- Device infrastructure: `EControllerType` (CONTROLLER_NONE=0/XBOX_PAD=1/KEYBOARD=2/MOUSE=3) tags
  every input record, and a polymorphic `CInputType*` hierarchy distinguishes the source event:
  `CInputTypeKeyboardKeyEvent`, `CInputTypeMouseButtonEvent`, `CInputTypeXboxPadButtonEvent`,
  `...XboxPadLeftStickEvent`, `...XboxPadRightStickEvent`, `...MouseMovementEvent`,
  `...MouseWheelMovement*` (each `GetType()->NControlSystem::EInputType`).

### Patch surface

- **Gamepad key-redefine screen.** The store and records already support controller-class inputs. A
  patch adds: (a) a gamepad control scheme (data + one apply call), and (b) a frontend redefine screen
  that drives `CKeyRedefiner` capturing `0x3C`-type (and button-type) records into the binding
  vectors. No new storage layout needed — reuse the 28-byte record.
- **Controller hotbar cycling.** Add an `EGameAction` (the enum is a persisted `CVectorMap`), bind it
  in the passive/context vector (`+0x60`), and dispatch it in the runtime action consumer
  (`IsEventGameAction` 0x445BD0 / `GetControlMovementFromGameActionEvent` 0x445C30) to advance the
  hotbar index. No new UI required beyond a bindable row on a gamepad screen.
- **Custom menu items.** Separate subsystem (`CFrontEndManager` / `Init2` component list @
  0x00598A1C); the redefine work does not block it.

### Byte-pure reconstruction status

These functions are mapped for byte-exact reconstruction (RELOCATION_MATCH + behavior) via the crawl
methodology; small accessors/leaves land first, the larger `Refresh`/`ProcessEvent`/`Redefine` bodies
get dedicated passes. Eight were landed then **un-landed 2026-08-10** (00556580, 00557850, 00557860,
00557880, 005578a0, 00557bd0, 00557c10, 00557ca0); they sit in `tools/decomp_pipeline/crawl/
gen_tried.txt` so the crawl won't re-suggest them. The PDB-backed naming identities stay in
`manifest/functions.tsv` and `rebuild/corrections/function_overrides.tsv`.

---

## Gamepad Redefine patch (SHELVED 2026-08-10)

*Originally `GAMEPAD_REDEFINE_PATCH.md`. **This entire line was shelved on 2026-08-10 per user
direction** — reverted from the rebuild and the live install to refocus on the binary-wide
byte-parity crawl. The design, live-test findings and the two open blockers are kept here as the
record; nothing is lost, only un-landed. Do not treat any of it as shipped.*

Goal: split retail's single **"Redefine Keys"** frontend option into **"Redefine Keys (Keyboard)"**
(the existing screen, unchanged) and a new **"Redefine Keys (Gamepad)"** screen that rebinds
controller inputs, using Unreal-Engine-style key names ("Face Button Bottom", "D-Pad Up", …). This is
a base-game patch target: the delta a patch author applies on top of the understood redefine
subsystem above. Nothing requires a new storage layout — the engine already models controller-class
input records (`+0x54/+0x58` primary, `+0x60/+0x64` passive, 28-byte `{EGameAction +0, input +4}`
records; `GetSubTypeForAction` 0x557CA0 types `0x37`/`0x38`/`0x3C`; named schemes via
`ResetAssignedInputs` 0x4085F0 / `ResetAssignedInputsWASD` 0x408820). The missing piece at retail is
purely a **frontend screen** that (a) presents gamepad bindings and (b) captures controller input into
`0x3C`/button-type records.

### Menu split

Retail frontend has one entry routing to detail `screen==4` ("Redefine Keys"). Patch:

| Menu label | Detail screen | Bindings shown | Capture device |
|------------|---------------|----------------|----------------|
| Redefine Keys (Keyboard) | 4 (unchanged) | primary vector `+0x54`, keyboard records | keyboard |
| Redefine Keys (Gamepad)  | **5 (new)**   | passive/controller records | gamepad |

In the reconstruction's checkpoint the detail title is chosen by `screen`: `screen==4` → "Redefine
Keys (Keyboard)"; `screen==5` → "Redefine Keys (Gamepad)". The options-list row that opens screen 4
is duplicated; the second sets the active screen to 5 (options-list builder in
`visual_boot_checkpoint.cpp` / `visual_boot_d3d9.cpp`).

### UE gamepad key-name table

The gamepad screen renders bound inputs as UE-style names; the stored record stays the engine's native
`0x3C`/button type + code. Standard XInput-class mapping (Unreal `FKey` naming, Xbox layout):

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

The four Left-Stick directions map to the engine's existing `0x3C` analog subtypes `0x0A–0x0D`.

**Encoding — mostly RESOLVED (2026-08-09):** the def/scheme-side record encoding is the
`CActionInputControl` layout above (`+0x00 GameAction`, `+0x04 ControllerType` (1=Xbox pad), `+0x0C
EXboxControllerButton`, `+0x14/+0x18` C2DVector dir hint). `FABLE_XBOX_CONTROL_SCHEME` (def entry
1099, 77 records) IS the native gamepad default, so the gamepad scheme is buildable directly with no
live probe, and the EGameAction ordinals come from `debug_build/FableWin.pdb`
(`tools/render_fable_frontend_subscreens.py`). The ONLY still-open encoding piece is the *runtime*
28-byte `CUserProfileManager` binding-record form under `GetSubTypeForAction` types
`0x37`/`0x38`/`0x3C` (the scheme→runtime apply in `ResetAssignedInputs` 0x4085F0); confirm it
matches the scheme form before assuming so.

**Controller value scheme — USE MODERN XInput / UE names, NOT retail's enum.** The original Xbox
controller (Duke/S) had Black/White buttons and **no bumpers**, so retail's native
`EXboxControllerButton` (Black/White, no LB/RB) is the WRONG scheme for modern pads. Bind/display via
the UE-style modern XInput values (`kGamepadKeyValues`/`kGamepadKeyValueLabels`) — a design choice for
modern controllers, not a reproduction of retail's original-Xbox defaults, so the low-confidence
face/shoulder rows in the enum tables do NOT gate the patch.

### Gamepad detail screen (screen 5)

Mirrors the keyboard redefine list but: (1) row source = the action set that is meaningfully
controller-bindable (all `EGameAction`s that already carry a passive/controller record, plus the four
movement axes); (2) value column = UE label (fallback "Unbound"); (3) capture: entering redefine on a
row arms `CKeyRedefiner` for controller input; the next controller button/axis event is written as
the record (type `0x37`/`0x38` button or `0x3C` axis), exactly as the keyboard path writes keyboard
records; (4) Reset applies the **gamepad default scheme** (data + one `0x411B90` apply call);
conflict/coexistence gating reuses `AreAllowedToCoexist` (0x5578A0). The checkpoint fakes the
keyboard list with a baked page atlas; screen 5 would get the same treatment first, then converge on
the byte-pure `CRedefinerList` live-scroll path once `Refresh` (0x557000) is reconstructed.

### RETAIL patch architecture (2026-08-09 — the actual deliverable)

The `visual_boot_*` screen-5 work is a **reconstruction mockup / visual spec only** — retail does NOT
bake menu screens; it builds the frontend dynamically. RE-confirmed data-vs-code split:

**DATA (frontend defs — forge-moddable, no binary edit):**
- **Options list def #219** (rooted `(200,150)`, 30px pitch; rows = actions 9/13/12/283, per
  `docs/formats/FRONTEND_FORMAT.md`): add a **5th row** referencing a new action id + label text-tag.
- **New "Redefine Keys (Gamepad)" screen def** — clone `UI_FRONTEND_SCREEN_REDEFINE_KEYS_PC`.
  `CRedefinerList::Initialise` (0x5566A0) builds rows from the screen def's children — the row/action
  set is data. The clone's rows carry controller-type expected records so capture accepts pad events.
- **Gamepad default scheme** — a new named input scheme applied by `ResetAssignedInputs` 0x4085F0
  via one `0x411B90` apply call, values from the UE/XInput table.

**CODE (unavoidable detours, retail VAs) — concrete values pinned (2026-08-09):**
- New row **action = 284** (`0x11C`), **used-key = 0x17**. Verified free: `Action()` assigns keys
  `{0x1,0x3-0xa,0xc,0xf,0x10,0x14,0x16,0x18,0x19,0x1a,0x1c}` (0x16 = keyboard Redefine); **0x17 is
  unused** — the natural neighbour. Action 284 has no `Action()` case → falls to the clean
  `0x59a7ff` default (inert `pop/leave/ret 4`) until detoured.
- **`CFrontEndManager::Action` @ 0x0059A238** — detour: `if (action==284) { usedKey = 0x17; goto
  dispatch; }` where dispatch = the existing `0x59a7d2` tail (`lea eax,[ebp+8]; push eax; lea
  ecx,[esi+0x54]; jmp GotoNextScreen 0x596763`). Action id is read as `**(int**)eventArg` (body reads
  `[[ebp+8]]` ~0x59A281).
- **`CFrontEndManager::Init2` @ 0x00598A1C** — bind key `0x17` → the new
  `UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD` def. **Bind mechanism decoded** (@0x598F4D, the redefine
  block): Init2 is a sequence of ~30-byte bind blocks, each: build `CWideString`(screenName,-1) via
  **0x99EBF0**; `mov [slot], KEY` (numeric key inline — `0x16`=keyboard redefine); `call 0x59B5D7`
  (ecx=this, arg=&slot → get-or-create bind slot for KEY); get FE singleton **0x41E5F2**; `call
  0x41DB1D` (resolve screen def by name); store `[slot]=screen`; dtor **0x99EAE0**. So the "detour"
  is replaying ONE bind block post-Init2 for (key=0x17,
  name="UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD") with ecx=the CFrontEndManager `this` — callable
  straight from an FSE plugin. No mid-function patch needed; the screen resolves by name.
- **Capture needs NO code — CONFIRMED** (`Redefine` 0x557D20 device-agnostic, see above).

**Three opens — ALL VERIFIED (2026-08-09):** (1) frontend defs are editable data in
`data/CompiledDefs/frontend.bin` (parser+writer: `tools/parse_frontend.py`); (2) `Action()` default
path `0x59a7ff` is a clean `pop/leave/ret 4`, so an unused action id is inert-safe (dispatch tail =
`0x59a7d2`); (3) `Redefine` 0x557D20 is device-agnostic.

**DATA HALF — BUILT + round-trip verified (`tools/build_gamepad_redefine_data.py`, since deleted in
the shelving; recoverable from git).** Concrete frontend.bin structure: options list **#219
`UI_FRONTEND_LIST_OPTIONS_SUB_MENU`** Children `[347,350,273,344]` = the 4 rows (row **344
`UI_OPTIONS_BUTTON_REDEFINE_KEYS`** Action=283 is the clone source); redefine screen **#238
`UI_FRONTEND_SCREEN_REDEFINE_KEYS_PC`**. The builder appends (to a scratch out-dir, never the
install): `UI_OPTIONS_BUTTON_REDEFINE_KEYS_GAMEPAD` (row clone, Action **284**) at gi 810,
`UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD` (screen clone) at gi 811, and rewrites #219 Children →
`[347,350,273,344,810]`. names.bin +2 with **crc0** (verified vs the contract vectors + 2000/2000
stored). Round-trip re-parse confirms all three. Remaining data polish: clone row 344's label child
(345) to a "(Gamepad)" text tag (else the row shows the keyboard label); the cloned screen's sub-defs
are SHARED per DEF_LOAD_CONTRACT (title/list stay keyboard until the code detour relabels). The
builder MUST also set #810.`ActionOnLeftUnclicked = 284` (0x1C010000) — see live test below. The
gamepad control **scheme** lives in game.bin, a separate append.

**Net:** a mostly-DATA patch (2 frontend-def clones/edits + 1 named scheme) + a **small 2-site code
detour** (Action case + Init2 bind). Capture/persistence reuse the existing 28-byte record + passive
vector `+0x60`.

Remaining pre-build verification at the time: (2) options list #219 def runtime-editable via the
forge frontend-def path; (3) the new action id is free and survives `Action()`'s default-return
filter; (4) the frontend pumps controller input events to the active redefine screen (likely; confirm
the event source feeding `Redefine` when the gamepad screen is active).

### Reconstruction-mockup status (2026-08-09, all reverted 2026-08-10)

- **[data] DONE** (`b1621a8`): `kGamepadKeyValueLabels[24]` / `kGamepadKeyValues[24]` (UE-name ↔
  XInput-bit table) in `visual_boot_d3d9.cpp`.
- **[title] DONE**: detail title splits screen 4 → "Redefine Keys (Keyboard)", screen 5 → "Redefine
  Keys (Gamepad)"; render clamp raised to `screen<=5`. `FableSetVisualFrontendDetailScreen` previously
  rejected `screen==5` (dead-code title) — fixed (guard `screen > 5`). Compile-verified; visual-QA
  of the title pending.
- **[screen] DONE (scaffold, compile-verified)**: screen 5 renders over screen 4's Redefine Keys
  backdrop (`overlayFrame = 3 + (screen==5?4:screen)` since 5 has no baked art) with the shared
  action-label column (`AppendRedefineActionText` gated `screen==4||5`) and a new
  `AppendRedefineGamepadValueText` value column showing neutral "Unbound" — the per-row
  action→`EXboxControllerButton` default was NOT wired because only movement/DPad were
  HIGH-confidence at the time.
- **[menu] TODO**: the interactive host's options submenu is the retail 4-row list —
  `ActivateVisualOptionsSelection` (guard `g_VisualOptionsSelection >= 4`, `detailScreens[4] =
  {1,3,2,4}`) + `FindVisualOptionsMenuRow` geometry + `g_OptionsRowChildren[4]` (all in
  visual_boot_checkpoint.cpp). A 5th "(Gamepad)" row needs: guard→5, `detailScreens[5]={1,3,2,4,5}`,
  extended hit-test rows, AND a 5th *visible* row. Layout-sensitive → needs the headless visual-QA
  cycle (`build_bootstrap.ps1 -RetailFrontendBank …/data/graphics/pc/frontend.big` → synth-click →
  screenshot); do NOT do blind.
- **[bindings] TODO**: per-row gamepad value column needs a higher-confidence action→button map
  (now available via the PDB enum values).
- **[hover] TODO**: `FableSetVisualFrontendRedefineHover` etc. are gated `!= 4`; generalize to screen 5.
- **[probe/scheme/patch] TODO**: x32dbg capture of the per-input record encoding, then the base-game detour.

### Device-aware UI swapping (Xbox-UI reference) — feasibility (2026-08-09)

Idea: show keyboard prompts when the player uses kbd/mouse and controller glyphs when they use a pad,
swapping live. Findings:

- **Engine has the device infrastructure** (`EControllerType` + the `CInputType*` event hierarchy,
  see runtime dispatch above) — an "active device = EControllerType of the last input event" signal
  is trivially derivable.
- **Retail PC almost certainly does NOT auto-swap** (Xbox build is always-pad, PC build always
  kbd/mouse; no active-device getter or button-prompt widget surfaces in the manifest). Device-aware
  prompts are a NEW feature, not a latent one.
- **The Xbox build is the controller-UI answer key.** `refs/xbox/headers/front_end_bank_xbox.h` names
  the controller glyphs (`HUD_ABXY_BIG_A/B/Y_FE`, `UI_THUMB_STICK_U/D/L/R_OFF_FE`,
  `FRONTEND_BUTTON_L/R/M`); `Media/Fable.uix` is the Xbox UI skin; glyph pixels live in
  `data/graphics/xbox/frontend.biz`. The FableControllerSupport mod's `player_gui.def` is a *static*
  controller-HUD (always pad glyphs while its DLL is active) — the reference for the layout.
- **Implementation shape (a HUD/prompt feature, bigger than the redefine screen):** (1) track
  last-input device (hook the input event pump → store EControllerType); (2) HUD/frontend
  button-prompt widgets pick glyph-vs-key text from it; (3) supply the controller glyph atlas (port
  Xbox `frontend.biz` ABXY/stick sprites into the PC frontend bank). Data = glyph atlas + prompt defs
  (forge); code = the last-device tracker + prompt selection (FSE plugin). Pairs with an XInput
  bridge like the FableControllerSupport DLL.

Scope note: a separate, larger workstream than the gamepad-redefine screen — it touches HUD prompts
game-wide. Recommended order: redefine screen first, then device-aware prompts.

### LIVE-TESTED in retail (2026-08-09) — first pass

Deployed to the real install (backups: `*.gamepadbak`) and driven in-game:
- **DATA half VERIFIED:** the Options submenu shows a 5th row **"Redefine Keys (Gamepad)"** (distinct
  label — the engine renders the literal CWideString, no text.big edit needed). The
  DEF_LOAD_CONTRACT append loads with no crash.
- **CODE half VERIFIED:** clicking the gamepad row opens a redefine screen (action 284, inert without
  the hook, now routes) — the Action @0x59A238 detour fires and `GotoNextScreen`s to the resolved
  gamepad screen #811. Boot-safe (Init2 is NOT hooked; the earlier 2-hook build crashed at frontend
  init, redesigned to a single on-demand resolve in the Action hook). DLL:
  `rebuild/integration/gamepad_patch/gamepad_redefine_hook.c`; inject via FSE_Launcher (Mods\ +
  Mods.ini). *(The evening session below showed this "opened #811" belief was wrong.)*

**Remaining — show CONTROLLER values on the gamepad screen.** #811 is a byte-clone of keyboard
screen #238, so it displays keyboard bindings; the vector choice is CODE. Displaying controller
bindings needs a code change: make CRedefinerList/CKeyRedefiner read the passive vector and label
values via the gamepad table (`rebuild/integration/gamepad_binding_table.md`, proven XBOX_PAD_*
values), plus a gamepad default scheme applied to the passive vector.

### Controller-values step — decision point and hook (2026-08-09)

RE'd the redefine display path: `CRedefinerList::Refresh` @0x557000 → `GetPrimaryInputVector`
@0x4088E0 (hardcoded primary `+0x54`; disassembly above). Ready-to-build hook (Phase 3):
1. In the Action-284 detour, set a module global `g_gamepadRedefine = 1` before `GotoNextScreen`,
   clear it when leaving the screen.
2. Second inline hook on **0x4088E0**: if `g_gamepadRedefine`, return `&this[0x60]` (mirror the
   empty→EnsureDefaults guard against `+0x60/+0x64`, seeding from `FABLE_XBOX_CONTROL_SCHEME` if
   empty); else tail to the saved original. Because `CKeyRedefiner::Redefine` writes back through
   the vector the list is bound to, this would flip DISPLAY and CAPTURE together.
3. Label the value column via `gamepad_binding_table.md` (display-only).
Risk: 0x4088E0 may have other callers while the screen is open; gate STRICTLY on the screen flag.

**BUILT + DEPLOYED (2026-08-09)** as three boot-safe inline hooks in `gamepad_redefine_hook.c`
(VS2022 x86 `cl /O2 /MT /LD /Gz` → 85504-byte PE32, KERNEL32-only import; prior single-hook DLL saved
as `gamepad_redefine.dll.singlehook.bak`):
1. **Action @0x59A238** — on action 284, resolve the gamepad screen, cache its def pointer in
   `g_gamepadScreen`, GotoNextScreen.
2. **GotoNextScreen @0x596763** — every transition sets `g_useController = (screenArg ==
   g_gamepadScreen)`; single source of truth, auto-clears on navigation.
3. **GetPrimaryInputVector @0x4088E0** (full 5-byte-jmp replacement) — faithful reimpl (primary
   `+0x54`, empty→`EnsureDefaults 0x4085F0`), EXCEPT when `g_useController` AND the return address is
   Refresh's call site (`0x55700D`): returns the passive vector `+0x60`. The retaddr guard confines
   the redirect to `CRedefinerList::Refresh`.
Revert path: restore `gamepad_redefine.dll.singlehook.bak` or remove the Mods.ini line. Open item if
+0x60 shows empty: seed with `FABLE_XBOX_CONTROL_SCHEME` (def 1099) via a ResetAssignedInputs-by-name
call in the Action-284 handler.

### Live-test session 2026-08-09 (evening) — findings and the two real blockers

Drove the patch live with a diagnostic/logging DLL. The routing infrastructure is solid, but TWO
real blockers remain, and the earlier "it opened the gamepad screen" belief was wrong:

1. **Why it kept showing keyboard values (SOLVED, data bug).** Row #810 had `Action=284` BUT
   `ActionOnLeftUnclicked=283` — copied verbatim from keyboard row #344. A mouse click fires
   `ActionOnLeftUnclicked`, so clicking "Redefine Keys (Gamepad)" fired **283** and opened the
   KEYBOARD screen #238. FIX: `#810.ActionOnLeftUnclicked = 284` (0x1C010000). Verified in the
   deployed bin (`Action=284, ActionOnLeftUnclicked=284`).
2. **The gamepad screen #811 renders BLACK when correctly routed (OPEN BLOCKER #1).** #811 is a
   BYTE-IDENTICAL clone of the working #238 (both `Type=10`, `Children=[217,342,632,120,585]`,
   `NonScrollingChildren=[0,1]`, identical States) — the DEF is fine. Opening it via a bare
   `GotoNextScreen(this, screen, 0)` is INSUFFICIENT: retail's action-283 path inside
   `CFrontEndManager::Action` @0x59A238 must do additional setup (most likely initialising list #217 /
   `CRedefinerList` / redefiner context). NEXT: RE the 283 case in the Action dispatcher and replicate
   the WHOLE handler for 284, not just the navigation call.
3. **The controller vector IS populated.** Primary `+0x54` and passive `+0x60` both hold **123**
   records — the feature only needs the gamepad screen to READ/format them.
4. **Raw vector pointer-swap is too invasive (REJECTED).** Swapping the `+0x54`<->`+0x60`
   {begin,end,cap} triples blacked the redefine render AND corrupted the profile-select screen (text
   only drew on hover). Reverted. Do NOT swap the vectors.
5. **`GetPrimaryInputVector` @0x4088E0 is NOT the row-value source.** A screen-gated redirect had
   ZERO effect on displayed values (the Phase-3 hypothesis disproved). Candidates:
   `GetAssignedInputForAction` @0x408C90 (has the `usePassive` param), called from big functions at
   0x48CAF0 / 0x642A60 (HUD/redefine builders). **OPEN BLOCKER #2** (identify the exact read site).
6. **Hook infrastructure that DOES work** (keep): Action@0x59A238 catches 284, resolves+caches the
   gamepad screen ptr, `GotoNextScreen`s (routing confirmed via logs). GotoNextScreen@0x596763
   screen-tracking flag toggles correctly. `%TEMP%\gamepad_hook.log` diagnostics + a buffered
   caller-address recorder are wired in the v3 source.

**v3 DLL (authored, NOT deployed):** non-invasive — hooks `GetAssignedInputForAction` @0x408C90 to
force the controller-vector read while on the gamepad screen and logs every caller's return address,
so the next test shows whether redefine rows flow through 0x408C90. Gated behind BLOCKER #1.

**Definitive next tool:** live tracing — start the debugger server (`python -m debugger`) so
`mcp__ghidra__debugger_trace_function` can log calls+args on 0x408C90 / the redefine builders while
the screen is shown, pinning BOTH the screen-activation gap and the value read path in one session.

### SHELVED 2026-08-10 — reverted from rebuild and install

Per user direction, the entire redefine-keys / gamepad-menu line was reverted out of the **rebuild**
and the **live install**, to refocus on the binary-wide byte-parity crawl ("byte purity and parity...
no hacks, no guesses").

**Removed from the rebuild (working tree):**
- `rebuild/integration/gamepad_patch/` (whole dir: hook `.c`, `.dll`, `.obj`, README) — deleted.
- `tools/build_gamepad_redefine_data.py` — deleted.
- Gamepad blocks in `rebuild/integration/visual_boot_d3d9.cpp`: `kGamepadKeyValueLabels`/
  `kGamepadKeyValues`, the "(Keyboard)/(Gamepad)" title split (reverted to plain "Redefine Keys"),
  `AppendRedefineGamepadValueText` + its call, screen-5 backdrop reuse, and the screen>4→>5 guard
  raises. All reverted (screens capped at 4).
- Gamepad rows in `rebuild/integration/visual_boot_checkpoint.cpp`: options-row loop `!=5`→`!=4`,
  `detailScreens[5]{1,3,2,4,5}`→`[4]{1,3,2,4}`, selection guard `>=5`→`>=4`.
- The 8 landed byte-pure `CKeyRedefiner`/`CRedefinerList` functions un-landed: catalog blocks
  (`build_candidates.ps1`), src+test files (`rebuild/{src/compiled,tests}/00/55/`), and rows in
  `retail-parity.tsv`, `vc71-compiled.tsv`, `auto-re-candidates.tsv`, `ARTIFACT_INDEX.tsv`. Addrs:
  00556580, 00557850, 00557860, 00557880, 005578a0, 00557bd0, 00557c10, 00557ca0. Manifest
  reconstruction columns blanked; addrs appended to `tools/decomp_pipeline/crawl/gen_tried.txt`.

**KEPT (intentionally):** all findings docs (now this file); the faithful **keyboard** Redefine-Keys
screen rendering (`AppendRedefineKeyText`, `AppendRedefineActionText`, `kRedefine*` constants) — real
parity; the PDB-backed **naming** identities for the 8 functions in `manifest/functions.tsv` and
`rebuild/corrections/function_overrides.tsv`.

**Live install:** stock. `Mods.ini` = FSE-only, `Mods/gamepad_redefine.dll` (+`.singlehook.bak`)
deleted, `frontend.bin`/`names.bin` confirmed byte-identical to stock (SHA-1), all
`*.gamepadbak`/`*.preclickfix` backups removed.

**To resume:** byte-exact function sources are in git history (commits before 2026-08-10); the patch
package is at commit `07c1037`. Re-land the 8 functions via the normal crawl, then re-apply the
menu/gamepad integration from git. Open blockers unchanged: #811 renders black (needs retail
283-handler setup) and the redefine value read path (candidate `GetAssignedInputForAction`
@0x408C90) — pin both via live `debugger_trace_function`.

---

## Open gaps

- **Stripped `EGameAction` int→name table for the full 31-action Redefine list.** Retail strips the
  enum-name strings; three sourcing passes (binary scan, web, community) failed to recover a complete
  human-readable name per ordinal — do not re-scan the binary or the web. What IS available: the PDB
  enum identifiers (`GAME_ACTION_NULL`…`GAME_ACTION_BETTING`, 0–114) and the 31 `TEXT_GUI_ACTION_*`
  frontend labels decoded in `tools/render_fable_frontend_subscreens.py`
  (`REDEFINE_ACTION_DISPLAY_TEXT`).
- `EXboxControllerButton` 0 and 7–12 names (trigger/thumb-click/Start/Back) remain empirical.
- `EMouseButtonControl` 6/7 (likely mouse axes) unconfirmed.
- Runtime 28-byte `CUserProfileManager` record form vs the scheme-side `CActionInputControl` form —
  not confirmed identical.
- `usePassive` polarity in `GetAssignedInputForAction` 0x408C90 — two contradictory readings on
  record (see the binding-store section).
- Gamepad patch blockers #1 (#811 black) and #2 (row-value read site) — shelved.

## Superseded

- **Empirical `EXboxControllerButton` face-button names (2026-07-19: A=1/B=2/X=3/Y=4/Black=5/White=6)**
  — superseded 2026-08-09 by the `FableWin.pdb` enum: `X=1, Y=2, BLACK=3, A=4, B=5, WHITE=6`.
- **"EGameAction names are an empirical correlation"** — superseded 2026-08-09: the PDB gives the
  enum identifiers 0–114 (`GAME_ACTION_COUNT`=115) directly.
- **Gamepad patch build order steps 1–8 (reconstruction-mockup plan)** — superseded 2026-08-09 by
  the retail patch architecture (data clones + Action/Init2 detours), then the whole line shelved
  2026-08-10.
- **"Clicking the gamepad row opened screen #811" (2026-08-09 first live test)** — wrong; the click
  fired `ActionOnLeftUnclicked=283` and opened keyboard screen #238. Correctly routed, #811 is black.
- **`GetPrimaryInputVector` 0x4088E0 redirect as the way to show controller values** — disproved
  live 2026-08-09; it is not the row-value source.
- **Init2 @0x00598A1C hook as part of the retail patch** — the 2-hook build crashed at frontend init;
  replaced by a single on-demand resolve inside the Action-284 handler.
