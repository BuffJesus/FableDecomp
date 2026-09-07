# Systems Analysis

*Started 2026-07-18 after FableWin donor pipeline and installed-game indexing.*

## Chest / Key / Reward System

### Evidence

Installed-data fixtures:

- `ghidra_out/installed_game/chest_key_reward_entities.csv`: 391 placed gameplay entities.
- `ghidra_out/installed_game/gameplay_entity_definition_validation.csv`: all 391 resolve to
  `game.bin`.
- `ghidra_out/installed_game/compiled_game_entries.csv`: concrete object definitions and binary
  payload previews.
- `docs/formats/COMPILED_DEFINITION_INDEX.md`: chest/key proof points.
- `docs/formats/WORLD_AND_CHEST_INDEX.md`: placement counts and first targets.

Native symbol fixtures:

- `ghidra_out/native_gameplay_targets.tsv`
- `docs/engine/NATIVE_GAMEPLAY_TARGETS.md`
- `ghidra_out/decomp_chest_core.log`
- `ghidra_out/decomp_reward_container.log`

### Data Findings

Placed gameplay entity categories:

| Category | Count |
|---|---:|
| shop/trader | 211 |
| chest | 148 |
| silver-key/silver-key-chest | 16 |
| key | 16 |

All currently indexed chest/key/shop/container-like TNG placements resolve against `game.bin`; there
are zero unresolved `DefinitionType` references in this slice.

Concrete compiled object definitions include:

- `OBJECT_CHEST_OPENABLE`
- `OBJECT_CHEST_OPENABLE_TPL`
- `OBJECT_SILVERKEY_CHEST_5`
- `OBJECT_SILVERKEY_CHEST_10`
- `OBJECT_SILVERKEY_CHEST_25`
- `OBJECT_SILVERKEY_CHEST_50`
- `OBJECT_SILVERKEY_CHEST_75`
- `OBJECT_SILVERKEY_CHEST_100`
- `OBJECT_CHEST_REWARD_ON_DEATH`
- `OBJECT_SILVER_KEY`
- `OBJECT_CHOCOLATE_BOX_01`
- `OBJECT_CHOCOLATE_BOX_UNGIVEABLE`

### Native Targets

First functions/classes to inspect in Ghidra:

- `CScriptThing::MsgOpenedChest`
- `CGameScriptThing::MsgOpenedChest`
- `CTCChest::Open`
- `CTCChest::Close`
- `CTCChest::GetNumberOfKeysToUnlock`
- `CTCChest::DisplayLockedMessage`
- `CTCChest::OnSerialise`
- `CTCChest::OnCreate`
- `CChestDef::Transfer`
- `CChestDef::Copy`
- `CEventOpenedChest`
- `CCreatureAction_OpenChest::PerformAction`
- `CCreatureAction_OpenChest::FinishAction`
- `CGameScriptInterface::IsChestOpen`
- `CGameScriptInterface::OpenChest`
- `CGlobal_OpenChestScript`
- `CGlobal_GiveHeroItemsFromRewardChestScript`
- `CCheckTheSilverKeyChest::Main`

### Working Model

The chest stack appears to split cleanly into:

- `CChestDef`: compiled definition payload for chest behavior and inventory/reward data.
- `CTCChest`: runtime component on a `CThing`; owns open/close state, key requirements, animation,
  targetability, serialization, and frame behavior.
- `CEventOpenedChest`: event emitted into the entity/script system.
- `CGameScriptInterface` and `CScriptThing`: script-facing helpers and messages.
- quest/global scripts: special-case chest behavior such as silver-key checks and reward-chest
  item grants.

Confirmed retail function map:

| Address | Function | Notes |
|---|---|---|
| `00750050` | `CTCChest::GetNumberOfKeysToUnlock` | Reads the chest sub-definition, writes a string name, returns the required key count, or `0` when no key gate is active. |
| `00750110` | `CTCChest::DisplayLockedMessage` | Uses `TEXT_GUI_CHEST_LOCKED_SINGLE_KEY` or `TEXT_GUI_CHEST_LOCKED_MULTIPLE_KEYS`, formatting the count for multi-key locks. |
| `00750540` | `CTCChest::Open` | Sets open/animation state, forces frame updates, persists opened chests when the definition flag at `+0x25` is set, and can spawn a particle emitter from definition field `[10]`. |
| `007506c0` | `CTCChest::Close` | Reverses open state and forces frame updates for close animation. |
| `00750780` | `CTCChest::FrameUpdate` | Runs `COpenCloseHelper::FrameUpdate`; on the opening transition it calls `CTCContainerRewardHero::RewardHeroWithItems`. |
| `0074e500` | `CTCContainerRewardHero::HasReward` | Returns true if the item vector is non-empty or attached `CTCGold` contains gold. |
| `0074e860` | `CTCContainerRewardHero::GetRewardItems` | Resolves static reward vector plus dynamic potion/resurrection dummy objects; may convert failed resurrection resolution into `20` gold. |
| `0074ec50` | `CTCContainerRewardHero::CreateItemPickupContainer` | Spawns `OBJECT_CHEST_REWARD_ON_DEATH`, inserts item rewards into its container, and spawns `OBJECT_GOLDBAG_SMALL_WITH_COINS` for gold rewards. |
| `0074ee90` | `CTCContainerRewardHero::RewardHeroWithItems` | Gives items directly via `CTCHeroReceiveItems::GiveItems`, adds money via `CTCHeroStats::AddMoney`, and displays `TXT_SIGN_NOTHING` when appropriate. |
| `0074e240` | `CTCContainerRewardHero::OnSerialise` | Persists `RewardEnabled` at component offset `+0x18`. |
| `00d91320` | `CCheckTheSilverKeyChest::Main` | Quest-specific check for script object `FiveKeyChest`, with `TEXT_QST_036_BANDIT_OPENED_SILVER_KEY_CHEST`. |
| `00eec410` | `CGlobal_GiveHeroItemsFromRewardChestScript::Main` | Global script path using `OBJECT_CHEST_REWARD_ON_DEATH`. |
| `00eec890` | `CGlobal_OpenChestScript::Main` | Script-level open path; emits/uses `CHEST_OPEN_FAIL` on failure. |

Important interpretation:

- Rewards are not one flat list. The runtime treats item rewards, gold, dynamic potion picks, and
  dynamic resurrection picks differently.
- Static reward definitions use `CContainerRewardHeroDef::ObjectFamilies` at `+0x28`; runtime
  `CTCContainerRewardHero::GetRewardItems` expands those families into concrete reward objects.
- `RewardEnabled` is persisted separately from the static object definition, so an editor should
  distinguish "definition reward contents" from "save/runtime consumed state".
- The object spawned for deferred/on-death rewards is a real compiled definition:
  `OBJECT_CHEST_REWARD_ON_DEATH`.
- Gold pickup visuals use `OBJECT_GOLDBAG_SMALL_WITH_COINS`; direct hero reward uses
  `CTCHeroStats::AddMoney`.
- Silver-key tiers are encoded in `CChestDef`: `OpenerObject` (`+0x34`) gives the required
  opener/key object def name, and `OpenersRequired` (`+0x38`) gives the required count.

Retail `CChestDef` addresses + field map (2026-07-18, RTTI port + decompile-confirmed):

- Retail methods (RTTI vtable-slot port, `tools/rtti_map.py`): `CChestDef::Transfer` = `0x004DE204`
  (slot 18), `Copy` = `0x004E0564`, `GetSizeofClass` = `0x004D3840`, vec-dtor = `0x004D8072`.
  Labels: `ghidra_out/labels_chestdef_rtti.tsv`.
- **Decompiled retail `Transfer` @ `0x4DE204` confirms it persists exactly six OWN fields**, in this
  order (Ghidra shows BSim-stale helper names, but the struct offsets + donor field-name order are
  authoritative — `ghidra_out/decomp_chestdef_transfer_retail.log`):

  | retail off | field (donor name) | note |
  |---|---|---|
  | `+0x25` | `PersistOnOpening` | bool (1 byte) |
  | `+0x28` | `OpenParticleEffect` | ref/def field |
  | `+0x2C` | `DisplayMessageOnEmpty` | ref/def field |
  | `+0x30` | `OpenAnimationForCreature` | distinct helper (`FUN_00431143`) — string/anim field |
  | `+0x34` | `OpenerObject` | ref/def field |
  | `+0x38` | `OpenersRequired` | count/int |

  Donor (Anniversary) offsets differ slightly (OpenerObject +0x34, OpenersRequired +0x3C) — struct
  layout drifted between builds, so ALWAYS take offsets from the retail body, names from the donor
  order. Field types are inferred from which transfer helper each call uses (three distinct helpers
  seen); exact types still need the helper functions named.
- **Silver-key requirement IS in these fields**: `CTCChest::GetNumberOfKeysToUnlock` reads the
  chest sub-def from the owning thing definition, requires both `OpenerObject` (`+0x34`) and
  `OpenersRequired` (`+0x38`) to be positive, writes the `OpenerObject` def name to its output
  string, and returns `OpenersRequired`. The donor `Transfer` also chains sub-def transfers for
  `CTCActionUseChest`, `CTCDiscovery`, `CTCExplodingObject` (proximity/explosion fields) - those
  are separate defs, not CChestDef own fields.
- ⚠ **Apply gap discovered:** the conservative `LabelApplyDefaultOnly` did NOT rename `0x4DE204` —
  a low-confidence BSim guess (`...InitialiseDefaultCameraTags`, sim 0.79) already occupied it. RTTI
  vtable-slot identity is stronger evidence than a sub-0.8 BSim hit; see FINDINGS for the override
  analysis of the 2,180 RTTI addresses BSim had pre-named.

### Editor Implications

The first chest editor should be read-only plus safe validation:

- list chest/key placements by level with UID and PositionX/Y/Z.
- show resolved `game.bin` entry index, definition class, and payload length.
- distinguish ordinary chests from silver-key chests by `DefinitionType`.
- show required opener/key object from `CChestDef::OpenerObject` and required count from
  `CChestDef::OpenersRequired`.
- show reward model columns separately: direct items, gold, dynamic potion/resurrection, and
  deferred pickup container.
- expose `OBJECT_CHOCOLATE_BOX_01` and `OBJECT_CHOCOLATE_BOX_UNGIVEABLE` in the object definition
  picker now; they are already proven compiled definitions.
- validate object definitions through `compiled_game_entries.csv`, not hardcoded names.

Mutation should wait until `CTCChest::OnSerialise`, `CChestDef::Transfer`, and bridge round-trips are
understood well enough to avoid corrupting TNG/BIN data.

### CChestDef retail addresses (2026-07-18, RTTI vtable-slot port)

`tools/rtti_map.py` mapped the donor's PDB-named `CChestDef` vtable onto retail via RTTI
(method + evidence in `FINDINGS.md`; labels in `ghidra_out/labels_chestdef_rtti.tsv`):

- `004de204` `CChestDef::Transfer(CPersistContext&)` — slot 18; body enumerates fields at
  `this+0x25/+0x28/+0x2C/+0x30/…`
- `004e0564` `CChestDef::Copy` — slot 19
- `004d3840` `CChestDef::GetSizeofClass` — slot 20
- `004d8072` `CChestDef` vector-deleting dtor — slot 0

The donor Transfer's ordered field-name sequence starts: `OpenParticleEffect`,
`PersistOnOpening`, `DisplayMessageOnEmpty`, `OpenAnimationForCreature`, `OpenerObject`,
`OpenersRequired`, … (Anniversary-era donor — retail may lack trailing fields). Retail strips
field-name strings entirely, so `game.bin` chest payloads decode by field order; pairing donor
names with retail offsets in a side-by-side Ghidra read of both Transfer bodies is the direct
route to answering Next Question 1 below.

### Answered Questions (2026-07-18 late session; evidence in FINDINGS.md)

1. **Reward contents encoding:** `CContainerRewardHeroDef::ObjectFamilies` at `+0x28` is a
   `vector<long>` of family def indices. Each `CObjectFamilyDef` persists one own field at `+0x3C`:
   a `vector<pair<long objectDefIdx, int weight>>`. Reward = random family from the list, then
   weighted-random member via `CObjectFamilyDef::GetRandomObject` (`0x8ED590`). Both `Transfer`s
   are single-field thunks into a shared persist-vector helper that CRC-checks an EMPTY field name
   (retail strips def field-name strings; decode is by field order).
2. **Inventory check:** `NScript::CGlobal_OpenChestScript::Main` (`0xEEC890`) calls
   `CGameScriptInterface::GetNumberOfKeysNeededToUnlockChest` (vtbl `+0x6D4`, `0x8966C0`); if the
   required count ≥ 1 it calls `CGameScriptInterface::GetNumberOfItemsOfTypeInInventory`
   (vtbl `+0x2DC`, `0x897190`), which resolves the `OpenerObject` name via
   `CDefinitionManager::GetDefGlobalIndexFromName` (`0x9AD410`) and counts matching items in the
   hero's `CTCInventoryBase` (`NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory`,
   `0x5BDF08`). Opens if `required <= have`; else `"CHEST_OPEN_FAIL"` +
   `DisplayLockedChestMessage`. **Keys are counted, not consumed.**
3. **Give-items script:** `CGlobal_GiveHeroItemsFromRewardChestScript::Main` (`0xEEC410`) has no
   item tables; it special-cases the container def name `OBJECT_CHEST_REWARD_ON_DEATH` and
   otherwise drives the generic reward-container machinery.
4. **Where rewards live:** entirely in compiled definitions (`CContainerRewardHeroDef` /
   `CObjectFamilyDef` payloads in `game.bin`) plus attached TCs (`CTCGold`, dynamic
   potion/resurrection dummies). TNG placements select the definition; changing reward contents
   means editing `game.bin` def payloads (or swapping `DefinitionType`), not TNG properties.

### Next Questions

1. Locate the `game.bin` payload byte ranges for `CContainerRewardHeroDef` and `CObjectFamilyDef`
   instances (single binary vector each) and confirm against `compiled_game_entries.csv` previews.
2. Enumerate `OBJECT_FAMILY_*` def names and their global indices for the FableForge reward picker.
3. Confirm the runtime reward list at `CTCContainerRewardHero+0xC` is save-game state only
   (populated by `OnSerialise`) vs. also def-seeded at spawn.

## Controller / Input System

**Moddability verdict:** Key remapping = **data edit (game.bin CControlsDef)**, blocked only on
the human-readable enum value tables. Controller-motor rumble = **dead on retail PC** (vtable
stubbed to nops); no data edit or "disable" needed. Screen-quake rumble = **data edit (CRumbleDef)**.

**Binding table.** `CControlsDef` (game.bin def; retail `Transfer` `0x004330F0`) holds a
`Controls` vector at retail +0x34 of 28-byte `CActionInputControl` records, plus 6 toggle bools
(+0x48..+0x4D: ZTarget/Spells/Sneak/ExpressionMenu/ExpressionShift/FlourishNeedsAttackButtonHeld).
Each binding record: `+0x00 EGameAction`, `+0x04 EControllerType` (1=Xbox pad, 2=keyboard,
3=mouse), then a tagged-union device value: `+0x08 EInputKey` (kbd), `+0x0C EXboxControllerButton`
(pad), `+0x10 EMouseButtonControl` (mouse); `+0x14/+0x18` C2DVector direction (runtime-only). The
persist writer `CPersistTraits<CActionInputControl>::TransferOut` (donor `0x017E149A`) writes only
the device field matching `ControllerType`. All device values are plain 4-byte ints. Source-path
literal in the donor confirms this is retail `fablelib\defs\controls_def.hpp`.

**Rumble.** `CJoystick::SetRumbleGloballyActive(bool)` is a static writing one global gate byte
(donor `DAT_04a67e31`). `CJoystick::CInitRumble` = 4 floats + bool (motor params, default 1.0f).
But retail `CJoystickDX` rumble vtable is **entirely stubbed**: `AddRumble`/`ClearAllRumbles` nop,
`Get/PeekRumbleWithID`/`GetJoystickDeviceNumber` return 0 (`0x00AB7900`-`0x00AB7940`). The
Anniversary donor `AddRumble` is also a nop -> DirectInput force-feedback never shipped on PC.
Re-enabling motor rumble would require a native XInput patch (new code). The live "rumble" on PC is
**screen quake**: `CGameScriptInterface::CreateRumble` (`0x0089FDA0`) spawns a `CTCDRumble` thing;
strength/duration from `CRumbleDef` (game.bin def, `Transfer` `0x004E676A`): `QuakeIntensities`
(map<EQuakeStrength,float> +0x28) and `QuakeDurations` (map<EQuakeLength,float> +0x34). Tunable via
data edit. `ERumbleType` = NULL/WILL/DAMAGED/HITTING/QUAKE.

**Open gap:** exact `EXboxControllerButton` / `EGameAction` / `EInputKey` / `EControllerType`
integer->name tables (retail strips the strings; not in the extracted DevHeaders). Recover from
controls_def.hpp/inputkey.h headers, an empirical game.bin CControlsDef payload dump, or the wiki.

Evidence: `ghidra_out/decomp_controller_{donor,retail,retail2}.log`, `decomp_rumbledef_donor.log`,
`def_schema.json` (CControlsDef, CRumbleDef). Full detail in [INPUT.md](INPUT.md#verified-facts-from-findings-log) (2026-07-19
controller cluster).

## Verified facts (from FINDINGS log)

- **2026-08-26 — MsgOnBoastsMade ownership and ABI.** Retail `008a9ae0` queries type `0x29` messages
  with extra data between two distinct world-frame bounds. Event `+0x3c` -> data with a long at
  `+0x00` and `CCharString` at `+0x0c`; results append as eight-byte pairs. `0099ec30` = string copy
  ctor, `0099eae0` = its dtor, `0074fce0` = receiver-based vector growth helper. Returns whether the
  temporary list was nonempty and frees every list allocation. Natural reconstruction passes its
  fixture but is `DIFFER(279v352)` (bounded, not parity).
  - Verifier COFF defect: objdump basic-block labels share the function-header syntax but nonzero
    labels are offsets within the current VC7.1 `.text` contribution; keeping them attached restored
    the 744-byte `GFHandleSystemInitError` relocation match.
  - `00c93b30 Audio_ApplyGainTableToBuffer`: five-argument fastcall (count pointer + input in
    `ECX`/`EDX`; gain indexes, output, limit callee-clean stack args); `gainTable[index] * input` up
    to the clamped limit, unrolled 4x, then float zero through the original count; 250/260-byte
    codegen bounded.
  - `0045d264 CActiveFile_AssignVector8`: callee-clean three-stack-arg allocator/copy (`count * 8`,
    half-open range of two-dword values; zero count -> null; null allocation suppresses stores);
    77/68-byte scheduling bounded.
  - 24-byte tree-node allocator grammar: exact two-dword member at `0045d901`, dword-plus-byte at
    `0045e2b1` (also `0045de2f`, `0045e797`); all return the allocation base despite `void`
    prototypes, payload at `base + 0x10`, clean one stack arg, 34-byte relocation match. Masked scan:
    34 two-dword + 4 dword-plus-byte instances; all 38 landed (28 new relocation matches).
  - `0045deba Map_EraseNode`: 47-byte member forwarding node/allocator offsets `+4/+8/+0x0c`, frees
    the returned node when nonnull, decrements count `+4`; initial 45-byte boundary missed the 3-byte
    `ret 4`. Ten exact 47-byte erase/free members, all landed.
  - 26-byte `00429fe3` node-allocation residue: eight exact bodies (incl. `00493a40`, `0053c879`,
    `0057f525`, `00592ad3`, `005f807c`), payload at `+8`, all exact-length `DIFFER(26v26)`
    (VC7.1 folds `pop/lea/test`), all explicitly bounded.
  - 22-byte `00451303` output-pointer allocator grammar: 62 authoritative starts (sizes `0x14`,
    `0x18`, `0x1c`, `0x20`, `0x28`, `0x30`, `0x40`, `0x48`, `0x58`), fastcall `ECX` output, `and
    [out],0`; 62/62 landed `RELOCATION_MATCH["s"]`.
  - 43-byte free-pointer loop rooted at `0043f510`: six instances (new `0048c820`, `007b3b50`,
    `00913690`), stdcall two-range ABI; 6/6 landed.
  - 17-byte iterator/max-node forwarder: 69 starts, 69/69 landed; RB-tree/max-node named subset = 32.
  - 42-byte list/sentinel initialization rooted at `0045170f`: 62 instances (incl. texture-format
    handler block `004dd170..004dd5c4`, `00cd2d37/00cd2d61`); 62/62 landed.
  - 32-byte list unlink/free: 7 bodies (new `0053c7b9`, `00655125`, `006553d0`, `006553fd`); 7/7 landed.
  - 58-byte buffer move/relocate: 42 retail bodies, 4 authoritative (all landed); 38 discovery-only
    (manifest gate). 37-byte list-insert splice: 34 hits, 33 authoritative, 33/33 landed (20 renamed
    `ListNode_InsertBefore_<address>`). Polymorphic range-cleanup rooted at `004437e0`: 19/19 landed
    (`PolymorphicRangeCleanup_*`). 26-byte list push-front wrapper: 33/33 landed
    (`ListPushFrontWrapper_*`). 35-byte tree-lookup accessors (`self + 0x144`, `self + 0x150`) 1 each,
    complete. 38-byte vector-copy wrapper: 26 bodies, only `00411b00` authoritative. 59-byte vector
    push-back: `0044bff0 std::vector::push_back` landed, family 2/2.

- **2026-07-18 — Retail CChestDef::Transfer = 0x004DE204 (RTTI vtable-slot port).** vtable slot 18;
  `Copy` = `0x004E0564` (slot 19), `GetSizeofClass` = `0x004D3840` (slot 20), vector-deleting dtor =
  `0x004D8072` (slot 0). Labels `ghidra_out/labels_chestdef_rtti.tsv`; tool `tools/rtti_map.py`.
  Evidence: (1) donor and retail `CChestDef` vtables both have exactly 25 slots and the override
  cluster Transfer/Copy/GetSizeofClass sits at 18–20 in both; (2) same walk on `CTCChest` puts retail
  slot 9 at `0x750780` = confirmed `CTCChest::FrameUpdate`; (3) `0x4DE204` disassembles as a
  canonical Transfer (`mov esi,ecx`, then `lea eax,[esi+0xNN]; push eax; mov ecx,edi; call …` for
  +0x28, +0x25, +0x2C, +0x30 …). Retail strips def field-name strings (0 hits for
  `OpenParticleEffect`, `PersistOnOpening`, `OpenersRequired`, `DisplayMessageOnEmpty`), so game.bin
  decodes by field ORDER; donor Transfer supplies names, retail Transfer supplies offsets.
- **2026-07-18 — Retail CChestDef::Transfer own-field map.** Six own fields
  (`ghidra_out/decomp_chestdef_transfer_retail.log`): `+0x25` `PersistOnOpening` (bool), `+0x28`
  `OpenParticleEffect`, `+0x2C` `DisplayMessageOnEmpty`, `+0x30` `OpenAnimationForCreature`,
  `+0x34` `OpenerObject`, `+0x38` `OpenersRequired` (int). Key requirement = `OpenerObject` +
  `OpenersRequired`; reward contents are NOT in these fields.
- **2026-07-18 — CTCChest::GetNumberOfKeysToUnlock reads CChestDef only** (no inventory/quest state;
  `ghidra_out/decomp_chest_core.log`, `decomp_chest_interface_after_rtti.log`): gets the chest sub-def
  from the owning thing's def pointer (`thing + 0x70`); requires `subdef[0x0d]`/`+0x34`
  (`OpenerObject`) and `subdef[0x0e]`/`+0x38` (`OpenersRequired`) both positive; resolves the
  `OpenerObject` global def index to a def name into the output `CCharString`; returns
  `OpenersRequired`, else clears the string and returns 0.
  `CGameScriptInterface::GetNumberOfKeysNeededToUnlockChest` validates the script thing, finds
  `CTCChest` via interface type `0x3c`, and forwards.
- **2026-07-18 — CContainerRewardHeroDef::Transfer static reward field.** One own field,
  `ObjectFamilies`, at retail `+0x28`. RTTI port: `Transfer` `0x004E8987`, `Copy` `0x004E249B`,
  `GetSizeofClass` `0x004E2497` (returns `0x34`; `Copy` assigns the tail field at `param2 + 0x28`).
  Retail bytes at `0x004E8987` begin `add ecx,0x28; push ecx; ...`; donor Transfer carries the single
  field-name string `ObjectFamilies` on donor `this + 0x28`.
- **2026-07-18 — Chest key gate: full retail runtime chain located.** `FindRefs` on
  `CTCChest::GetNumberOfKeysToUnlock` (`0x750050`) → exactly one caller,
  `CGameScriptInterface::GetNumberOfKeysNeededToUnlockChest` (`0x8966C0`)
  (`ghidra_out/findrefs_chest_callers.log`). Script bodies (`ghidra_out/chest_script_syms.log`):
  `CGlobal_OpenChestScript::Main` = `0xEEC890`, `CGlobal_GiveHeroItemsFromRewardChestScript::Main`
  = `0xEEC410`. `CGameScriptInterface` vtable base = `0x1260F0C` (slot 0 → `0x6E7D50`), proven by
  `+0x6CC` → `0x896390` `OpenChest`, `+0x6D4` → `0x8966C0`, `+0x6D8` → `0x896730`
  `DisplayLockedChestMessage` (data refs `0x12615D8/E0/E4`). `Main` decomp
  (`ghidra_out/decomp_open_chest_scripts.log`): `n = vtbl+0x6D4(keyNameOut)`; `n < 1` → open; else
  `have = vtbl+0x2DC(keyName)`; `n <= have` → open; else game text `"CHEST_OPEN_FAIL"` (`vtbl+0xAC4`)
  + `vtbl+0x6D8`. Slot `+0x2DC` = `0x12611E8` → `0x897190` =
  `?GetNumberOfItemsOfTypeInInventory@CGameScriptInterface@@UBEJABVCCharString@@@Z`
  (`ghidra_out/decomp_inventory_count_slot.log`) → `CDefinitionManager::GetDefGlobalIndexFromName`
  (`0x9AD410`) → hero `CTCInventoryBase` (TC type `0x11`) →
  `NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory` (`0x5BDF08`). No call removes key
  items in either branch: **keys are counted, never consumed**; a chest can require N of ANY object def.
- **2026-07-18 — Reward payload fully decoded: ObjectFamilies -> CObjectFamilyDef -> weighted random.**
  `CTCContainerRewardHero::GetRewardItems` (`0x74E860`, `ghidra_out/decomp_reward_expand_after_rtti.log`):
  when the runtime reward list at `this+0xC..0x10` (stride 8, `pair<long, CCharString>`) is empty, it
  iterates the def's `subdef+0x28..+0x2C` (stride 4), calls `GFRandom(count)`, resolves the chosen
  family via `CDefinitionManager::GetDef<...>`, then `CObjectFamilyDef::GetRandomObject` (`0x8ED590`,
  `ghidra_out/decomp_objectfamily_def.log`): iterates `this+0x3C..0x40` stride 8, sums `entry+4`
  weights, draws `rand % totalWeight` (LCG state `DAT_013B86A0+0x1613C`, mul `0x24A1`, add `0x24DF`,
  then ROR13), returns `entry+0` of the first entry crossing the draw. Transfer thunks:
  `CObjectFamilyDef::Transfer` `0x46C294` = `add ecx,0x3C; push ecx; mov ecx,[esp+8]; call 0x46C2A4`;
  `CContainerRewardHeroDef::Transfer` `0x4E8987` same shape with `+0x28`, helper `0x4E8997`; both
  helpers call `CPersistContext::CheckCRC(ctx, "")` with the EMPTY name string at `0x122D70E`, then
  `GFSerialiseVectorBinaryIn/Out` (`ctx+0x18 == 2` = read from `CMemoryDataInputStream` at `ctx+0x24`).
  Helpers renamed via `ghidra_out/labels_chest_reward_chain.tsv` (2/2; previously stale BSim
  `CCreatureAction_FireMissileWeapon::FrameUpdate`). Gold: attached `CTCGold` TC (interface type
  `0xCC`) gated by thing flag `+0x38 & 0x1000`; `GetDynamicPotionDefIndex` (`0x74E280`) picks
  health/super/mana potion by hero inventory; failed dynamic resurrection → +20 gold.
- **2026-07-18 — game.bin reward payloads decoded end-to-end; families enumerated.** `tools/bin_dump`
  hex mode → `ghidra_out/installed_game/objectfamily_payloads.tsv`, `rewarddef_payloads.tsv`.
  `OBJECT_FAMILY` payload = 9-byte header + `u32 count` + `count x (u32 objectDefIdx, f32 weight)`;
  `CContainerRewardHeroDef` payload = 7-byte header + `u32 count` + `count x u32 familyDefIdx`. Def
  indices ARE game.bin entry indices: reward value 2820 = row 2820 = `REWARD_VILLAGER_01`; family
  member 4642 = `OBJECT_GOLDBAG_MEDIUM_WITH_COINS`, 4290/4291 = the dynamic potion/resurrection
  dummies. Weights are per-mille floats (`REWARD_GUARD_01` sums to exactly 1000 with two `defIdx 0`
  entries, 100+400 = 50% no-drop). Decoder `tools/DecodeObjectFamilies.ps1` →
  `ghidra_out/installed_game/object_families.{json,csv}` (**41 families, 72 reward defs**). The 72
  reward defs cluster after `CCreatureDef` parents = creature death-loot tables. Chest OBJECT defs
  (485-byte payloads: `OBJECT_CHEST_OPENABLE` 3688, `OBJECT_SILVERKEY_CHEST_5..100` 3689-3694) embed
  their components. Fixed chest contents live in the TNG: `StartCTCChest; ContainerContents[0]
  "OBJECT_SUPER_MANA_POTION"; ChestOpen FALSE; EndCTCChest` (HobbeCaveLarder five-key chest); 162
  chest placements across the 397 loose TNGs, 115 with fixed contents (`forge chest list`).
