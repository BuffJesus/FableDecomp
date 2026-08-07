# Fable TLC input-binding & redefine system (RE map for patching)

Byte-level reverse engineering of the retail `Fable.exe` input-binding and
key-redefine subsystem, so it can be understood and **patched** (gamepad
redefinition, controller hotbar cycling, custom menu entries). All addresses are
retail VAs (ImageBase 0x400000). Disassembly is from the shipped exe; function
list is `rebuild/manifest/functions.tsv`.

## 1. Where bindings live: `CUserProfileManager`

The active profile owns **two parallel binding vectors** (STL `vector<CInputBinding>`,
begin/end pointer pairs):

| field | meaning |
|-------|---------|
| `this+0x54` / `this+0x58` | **primary** binding vector (begin/end) |
| `this+0x60` / `this+0x64` | **passive / alternate** binding vector (begin/end) |

Each record is **0x1C (28) bytes** — proven by the `imul 0x92492493; sar 4`
divide-by-28 count idiom in `GetAssignedInputForAction` (0x408C90):

```
CInputBinding {                 // 0x1C bytes
    +0x00  EGameAction action;  // dword key; linear-searched by action
    +0x04  <input record>       // 0x18 bytes: the assigned physical input(s)
}
```

`GetAssignedInputForAction(EGameAction, bool usePassive)` @ **0x408C90**:
- `usePassive == 0` → search the `+0x54` vector; `!= 0` → search `+0x60`.
- If the chosen vector is **empty** (begin == end) it lazily calls
  `ResetAssignedInputs` (0x4085F0) to populate defaults, then linear-scans
  28-byte records comparing `record->action == target`.
- `GetAssignedInputForMovementAction` (0x408D90) and
  `GetAssignedInputForContextSensitiveItemAction` (0x408F50) are the same shape
  over the passive vector — movement/context actions live in the alternate set.

## 2. Named binding schemes: reset applies a scheme by name

`ResetAssignedInputsWASD` @ **0x408820** and `ResetAssignedInputs` @ **0x4085F0**
do NOT hardcode key codes — they look up a **named input scheme** and apply it:

- WASD (0x408820): loads scheme-name string const `0x122E1FC` (or `0x122E1D8`
  under a global flag), constructs a `CWideString` (0x99EBF0, len −1), then via
  globals `0x13B86A0`/`0x13B8760` calls `0x44C6B0` or `0x43368D` to resolve the
  scheme, and applies it to BOTH binding maps (`this+0x54` and `this+0x60`) via
  `0x411B90`.
- This is the GDD/config-driven control-scheme system: schemes are data, keyed
  by name. **Adding a new scheme (e.g. a gamepad default) is a data + one
  apply-call change, not a rewrite.**

The reconstruction's authored visual checkpoint mirrors this with
`kVisualRedefineDefaults` (WASD `[31,9,27,12,...]`) vs
`kVisualRedefineArrowDefaults` (`[41,42,43,44,...]`); only the first four
(movement) rows differ between schemes.

## 3. Input records are DEVICE-TYPE-AWARE (the gamepad foundation)

`CKeyRedefiner::GetSubTypeForAction` @ **0x557CA0** branches on the input
record's **type field** (`record[0]`):

| type (`record[0]`) | handling |
|--------------------|----------|
| `0x37` | increments global counter `0x13B8AD0` (device/event class A) |
| `0x38` | increments global counter `0x13B8ACC` (device/event class B) |
| `0x3C` | **analog/controller movement** — calls `0x40ED10`, then a 4-way jump table returning subtypes **0x0A–0x0D** (four directions) |
| other | returns 0 |

The `0x3C` case with four directional subtypes is analog-stick / control-axis
movement. **The engine already models controller-class input records** — the
binding store and redefiner are not keyboard-only. What's missing at retail is a
*frontend screen* that captures gamepad input into these records.

### Byte-exact reconstruction (verified RELOCATION_MATCH, 120/120 bytes)

`GetSubTypeForAction` is `__stdcall` (single stack arg, `ret 0x4`). Reconstructed
source — proven byte-identical to retail via direct COFF-section extraction +
relocation masking (10 reloc slots; the harness's objdump path can't auto-verify
this one, see the jump-table gotcha in CLAUDE.md):

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

The outer `switch` compiles to the retail subtract-chain
(`sub 0x37; je / dec; je / sub 4; jne`); the inner `switch` on the analog
direction becomes the embedded `dec; cmp 3; ja; jmp [eax*4+table]` jump table
(subtypes 0x0A–0x0D). This is the exact device-type dispatch a gamepad-redefine
screen drives — see `GAMEPAD_REDEFINE_PATCH.md`.

## 4. The redefine UI

- `CRedefinerList` @ **0x5566A0** (`Initialise`), **0x557000** (`Refresh`),
  **0x5570F0** (`OnActivate`), **0x556810** (`UpdateUndefinedWarning`),
  **0x5567B0** (`ClearList`): builds/refreshes the scrollable list of
  redefinable action rows. This is the class whose **live scroll rendering** the
  visual checkpoint currently fakes with a baked page atlas — reconstructing
  `Refresh` + the row draw is the byte-pure native-scroll path.
- `CKeyRedefiner` @ **0x558770** (`Initialise`), **0x557D20** (`Redefine`),
  **0x557EB0** (`ProcessEvent`), **0x557A10** (`UpdateKeyText`),
  **0x557BD0** (`CancelSelection`), **0x5580B0** (`ClearDuplicateDefinitions`),
  **0x557C10** (`ChangeState`): the per-row capture control. State lives in
  `this+0x1A8`; the captured candidate binding at `this+0x1A4` (fields at
  +0x08/+0x10 compared against the incoming event in `Redefine`). It is a
  type-aware state machine — it matches the event's type (`event+0x4`) against
  the expected type before accepting.
- `AreAllowedToCoexist` @ **0x5578A0** and `IsRedefinableKey`
  (`CKeyRedefiner::IsRedefinableKey`, referenced from the visual bridge) gate
  which keys can share/replace a binding.

## 5. Action → input dispatch at runtime

- `CGamePlayerInterface::IsEventGameAction` @ **0x445BD0**,
  `GetControlMovementFromGameActionEvent` @ **0x445C30**,
  `IsGameActionEventInQueue` @ **0x445CB0/0x446B10**,
  `CInputProcessGameBase::IsGameActionEventInQueue` @ **0x687D80**: the runtime
  consumes bound inputs → `EGameAction` events. `EGameAction` is the enum keyed
  through `CVectorMap<EGameAction,…>` (0x447CE0) and persisted via
  `CPersistContext::TransferMap<EGameAction,CCharString>` (0x463334/0x475026).

## 6. Patch surface for the stated goals

- **Gamepad key-redefine screen.** The store (§1) and records (§3) already
  support controller-class inputs. A patch adds: (a) a gamepad control scheme
  (§2, data + one apply call), and (b) a frontend redefine screen that drives
  `CKeyRedefiner` capturing `0x3C`-type (and button-type) records into the
  binding vectors. No new storage layout needed — reuse the 28-byte record.
- **Controller hotbar cycling.** Add an `EGameAction` (the enum is a persisted
  `CVectorMap`), bind it in the passive/context vector (`+0x60`), and dispatch
  it in the runtime action consumer (§5) to advance the hotbar index.
- **Custom menu items.** Separate subsystem (`CFrontEndManager` /
  `Init2` component list @ 0x00598A1C, see docs); the redefine work here does
  not block it.

## 7. Byte-pure reconstruction status

These functions are mapped and slated for byte-exact reconstruction
(RELOCATION_MATCH + behavior) via the crawl methodology; the small
accessors/leaves land first, the larger `Refresh`/`ProcessEvent`/`Redefine`
bodies get dedicated passes. This document is the architecture the
reconstruction fills in — and the reference a patch author reads first.
