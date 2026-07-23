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
- `docs/COMPILED_DEFINITION_INDEX.md`: chest/key proof points.
- `docs/WORLD_AND_CHEST_INDEX.md`: placement counts and first targets.

Native symbol fixtures:

- `ghidra_out/native_gameplay_targets.tsv`
- `docs/NATIVE_GAMEPLAY_TARGETS.md`
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
`def_schema.json` (CControlsDef, CRumbleDef). Full detail in FINDINGS.md 2026-07-19 controller
section.
