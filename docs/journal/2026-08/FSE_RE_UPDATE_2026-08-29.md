# FSE_RE Developer Pack — 2026-08-29 Correction Update

This note corrects and updates claims in the FSE_RE developer pack assembled on
2026-08-28. The corrections below are based on retail `Fable.exe` assembly,
`FableWin.pdb` type data, and a successful live ForgeFSE test in ForgeTest.

## 1. Retail and FableWin `CHitParameters` layouts differ

The original pack incorrectly said that the PDB-backed FableWin layout and the
retail layout agreed, and it placed the retail augmentation vector at `+0x40`.

### Retail `Fable.exe` (VC7.1)

Retail constructor and copy-constructor assembly establish this layout:

```cpp
// Retail sizeof = 0x60
struct CHitParameters {
    // Strike state                         // +0x00 .. +0x2F
    std::map<EDamageAttribute, float> DamageMap; // +0x30, 0x0C bytes
    std::vector<EObjectAugmentationType> Augmentations; // +0x3C
    CHitLocation* PHitLocation;             // +0x48
    CDefPointer<const CMaterialDef> PHitMaterialDef; // +0x4C
    CIntelligentPointer<CThing> PThingHitting; // +0x50
    CIntelligentPointer<CThing> PWeapon;    // +0x58
};
```

The retail VC7.1 vector is the ordinary three-pointer representation:

- `begin` at `CHitParameters + 0x3C`
- `end` at `CHitParameters + 0x40`
- `capacity` at `CHitParameters + 0x44`

Relevant retail evidence:

- `CHitParameters` constructor at `0x00789238` initializes the vector pointers
  at `+0x3C/+0x40/+0x44`.
- `CHitParameters` copy constructor at `0x00812170` copies the vector beginning
  at `+0x3C`, then copies the fields at `+0x48`, `+0x4C`, `+0x50`, and `+0x58`.
- `CHitParameters::SetWeapon` at `0x00868CE0` operates on `PWeapon` at `+0x58`.

### PDB-backed `FableWin.exe`

`FableWin.pdb` reports `sizeof(CHitParameters) = 0x70`. Its newer debug STL
layout adds a container proxy and therefore must not be copied directly into
retail tooling:

- vector object at `+0x44`
- `_Myproxy` at `+0x44`
- `_Myfirst/_Mylast/_Myend` at `+0x48/+0x4C/+0x50`
- `PThingHitting` at `+0x60`
- `PWeapon` at `+0x68`

The former retail `+0x40` read actually treated the real vector end and
capacity as its first two fields and `PHitLocation` as the third. Live behavior
therefore alternated between false empty results and invalid-vector failures.

## 2. Resolve the native thing through virtual `GetPThing`

The original bridge directly cast the wrapped entity implementation and read a
local `PThing` member. ForgeFSE currently contains scripted-thing declarations
that place `PThing` at both `+0x0C` and `+0x14`; a cast alone cannot select the
correct runtime layout.

The corrected bridge calls the engine's virtual `GetPThing` accessor (entity
vtable slot 11) and resolves `CTCEntityEvents` from the returned `CThing`.

## 3. Live augmentation results

The corrected bridge was tested against a solid scripted bandit in ForgeTest.
The target used `MsgWhoHitMe()` as an independent hit-delivery control.

Observed structured log results:

```text
SM_HIT_PROBE|phase=primary|found=1|types=Flame|typeIds=4|mask=4
SM_HIT_PROBE|phase=repeat_same_window|found=1|types=Flame|typeIds=4|mask=4
SM_HIT_PROBE|phase=post_yield|found=0|types=|typeIds=|mask=0

SM_HIT_PROBE|phase=primary|found=1|types=|typeIds=|mask=0
SM_HIT_PROBE|phase=repeat_same_window|found=1|types=|typeIds=|mask=0
SM_HIT_PROBE|phase=post_yield|found=0|types=|typeIds=|mask=0
```

This live test confirms:

- Flame/Fire is `EObjectAugmentationType 0x04`.
- An unaugmented strike returns an empty vector and mask zero.
- Event lookup is non-destructive within the same script execution window.
- The event is no longer visible after one coroutine yield advances the window.
- The corrected target and bridge run without a Lua error or native crash.

The final log contains two independent Flame primary events and two independent
unaugmented primary events with those same results, ruling out a one-off stale
event or a single lucky read.

The pack's former statement that the bridge was built but not deployed or
live-tested is now obsolete.

## 4. Hitter-name filtering is live-validated

The initial successful layout-validation build obtained the matched hit through
retail's basic time-window specialization at `0x008D47A0`. The production bridge
was then restored to the hitter-script-name specialization at `0x008D4840` and
retested after correcting `GetPThing` resolution.

The `0x008D4840` path successfully returned Flame type `4`/mask `4` and an empty
vector/mask zero for unaugmented Hero strikes. Same-window repetition and
post-yield expiry remained correct. The public `hitterName` argument is therefore
live-validated for `SCRIPT_NAME_HERO`; `MsgWhoHitMe()` supplied an independent
positive control during the test.

## 5. Xbox executable availability correction

The original StatueMaster report incorrectly stated that the Xbox executable
was unavailable. The extracted executable exists locally at:

```text
D:\tmp\fable_xbox\extracted\default.xbe
```

Established first-pass facts:

- The XBE contains 66 `S_Q*` quest-code overlays.
- The correct StatueMaster overlay is `S_VSM` at virtual address `0x0081D4E0`;
  it contains 17 contiguous functions. `S_QSMS` is actually `QS_MeetSister`.
- Xbox and PC contain the same known `SM_` state strings.
- Xbox and PC text data retain `TEXT_QST_061_STATUE_GUILD = 3497`.

The complete overlay-level comparison is now finished. Xbox `S_VSM` and retail
PC implement the same three-way presentation: Bowerstone, Greatwood, and a
fallback to Nowhere. Direction 2/Guild falls through to Nowhere on both, and
neither executable implements the five augmentation inscriptions. Therefore
those behaviors were already removed or disconnected before the original Xbox
retail master; they were not PC-port cuts. A search limited to resident XBE
`.text` would still be incomplete—the verified result includes all overlays.

## Status summary

The event-local augmentation design is now live-validated. Consumers should use
the matched `CEventHitBy` payload and the corrected retail
`CHitParameters + 0x3C` vector, not the Hero's currently equipped weapon. Hero
identity filtering, payload extraction, same-window repeat behavior, and
post-yield expiry are now live-validated together.
