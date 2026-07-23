# Def-schema gap closed — 53/53 missing types recovered

**Date:** 2026-07-19  **Donor:** FableWin.exe (Ghidra project `FableTLC`)
**Validated against:** retail `C:\Programs\Steam\steamapps\common\Fable The Lost
Chapters\...\game.bin` (249 def types, 14761 entries) via
`D:\Code\FableForge\build\forge.exe defs decode` (read-only).

## Result

All **53** previously-unresolved def types from `docs/DEF_SCHEMA_COVERAGE.md`
(Gap A = 46, Gap B = 7; **6431** entries) are now schema-covered and
**field-mergeable**. Every one **decodes a real retail `game.bin` payload
CLEAN** (forge `leftover == 0`, all field tags matched in Transfer call order) —
verified on the largest 3 + smallest entry of each type: **53/53 clean**.

New field-mergeable coverage:

| bucket | types | entries |
|---|---:|---:|
| was covered (from COVERAGE.md) | 196 | 8330 |
| **recovered this pass** | **53** | **6431** |
| **new total** | **249 / 249** | **14761 / 14761** |

That is **100% of def types** now field-mergeable (up from 196/249 / ~56% of
entries).

## Deliverable

`docs/re_reference/def_schema_missing.json` — additive, same structure as
`def_schema.json`. Contains 59 entries:

- **6 class definitions** newly recovered by fresh decompile of their
  `Transfer(CPersistContext&)` in the donor:
  - `CUIDef` (NUISystem, 109 fields), `CUIMiscThingsDef` (133),
    `CUIIconsDef` (32), `CUILocaleGraphicsDef` (3), `CUIStateDef` (14 — helper
    sub-component of `CUIDef`).
  - `CCombatAbilityBlockDefBase` (5 fields) — **OVERRIDE**: this key already
    exists in `def_schema.json` but with **0 fields** (the original donor sweep
    couldn't parse its "args-set-up-before-a-bare-call" decompile shape). Fold
    this in as a REPLACEMENT, not just an add.
- **53 bin-def-type aliases** keyed by the game.bin `definition` STRING (e.g.
  `"OBJECT"`, `"UI"`, `"SPECIAL_ABILITIES_BERSERK_DEF"`, and the 7 Gap-B class
  names). Each copies the resolved donor class's ordered field list. Keying by
  the literal definition string makes FableForge's `resolveType` **Candidate 1**
  (`schema.find(binDefinition)`) hit directly — **no resolver code change
  needed**. Each alias carries `"alias_of": "<DonorClass>"` for provenance.

Supporting artifacts:
- `ghidra_out/decomp_schema_gap.c` — the raw pseudo-C decompiles (11 Transfer
  methods) this was read from.
- `ghidra_out/decomp_schema_gap.log` — headless log (DecompDefTransfers ok=11
  fail=0) + the `ReadStr` pass that resolved `&DAT_` field names.

## The key finding: the COVERAGE.md class guesses were wrong

`resolveType` guesses `C` + CamelCase(CATEGORY) + `Def`. For most missing types
that guess names a class **that does not exist**; the real donor class has a
different name. Almost all of Gap A was a **naming/alias** problem, not a
missing decompile:

- **Thing hierarchy** — `OBJECT`→`CThingObjectDef` (not `CObjectDef`),
  `BUILDING`→`CThingBuildingDef`, `THING`→`CThingBaseDef`,
  `MARKER`→`CThingMarkerDef`, `SWITCH`→`CThingSwitchDef`,
  `PHYSICAL_SWITCH`→`CThingPhysicalSwitchDef`, `HOLY_SITE`→`CThingHolySiteDef`,
  `NOISE`→`CThingNoiseDef`, `SHOT`→`CThingShotDef`. These already existed in the
  schema. The `CThing*` derived types call `CThingBaseDef::Transfer` first, so
  their alias field lists **prepend `CThingBaseDef`'s 12 base fields**
  (`fields_include_base: true`) — that is why OBJECT decodes 19 fields, SWITCH
  13, etc., and why the payload has a large untagged "prefix" (it is really the
  base-class run before the derived fields' first tag).
- **Special abilities (23 types)** — COVERAGE.md guessed a **double `Def`**
  (`CSpecialAbilitiesBerserkDefDef`). Real class is single-`Def`
  (`CSpecialAbilitiesBerserkDef`), already in the schema. Pure alias.
- **Others (alias to existing class):** `INVENTORY_TYPE`→`CInventoryDef`,
  `CONTROL_SCHEME`→`CControlsDef`, `SOUND_SETUP`→`CSoundDef`,
  `HERO_MELEE_COMBAT_ABILITY`→`CMeleeCombatAbilityDef`,
  `VILLAGER_INTERACTION`→`CVillagerInteractionsDef`,
  `LOCAL_DETAIL_GENERATOR`→`CEngineLocalDetailGeneratorDef`,
  `MELEE_COMBAT_KNOCKDOWN_EFFECTS`→`CMeleeCombatKnockdownEffects`,
  `CONFIG_OPTIONS_DEFAULTS_DEF`→`CConfigOptionsDefaultsDef`,
  `COMBAT_DIALOGUE_DEF`→`CCombatDialogueDef`,
  `ENVIRONMENT_THEME_DAY`→`CEnvironmentThemeDaySetDef`.

## Gap B — combat-ability blocks inherit, they do not define

The 7 Gap-B `CCombatAbility*AttackDef` classes have **no `Transfer` of their
own** (they have ctor/Copy/GetSizeofClass but inherit the base's Transfer). They
split by base:

- Block subtypes → `CCombatAbilityBlockDefBase` (5 fields: `BlockAngle`,
  `BlockCanBeBroken`, `ForceRecoilWhenBlocking`, `CombatAbility`,
  `ValidBlockWeaponTypes`): `Unarmed`, `LightWeapon`, `HeavyWeapon`,
  `ProjectileWeapon`.
- Counter/GetHit subtypes → `CCombatAbilityAttackBase` (3 fields: `Blockable`,
  `Knockdown`, `Damage`): `GetHitCounter`, `BlockCounter`, `FlourishCounter`.

## `&DAT_` field-name recovery

The original donor sweep dropped fields whose name string the decompiler left as
`&DAT_xxxxxxxx` instead of inlining. A `ReadStr` pass resolved them, which fixed
otherwise-incomplete lists:
- `CCombatDialogueDef` +0x90 = **`Flee`** (int32) — this was the 8-byte leftover
  that kept COMBAT_DIALOGUE_DEF from decoding clean.
- `CEnvironmentThemeDaySetDef` leading = **`Time`** (fresh decompile has 10
  fields incl. this; base schema had 9 — updated).
- `CUIDef`: `Type`, `MinX/MinY/MaxX/MaxY`, `UpX/UpY/UpZ`, `Font`.
- `CUIMiscThingsDef`: `Plus`.

## Validation method

Field-tag = reflected CRC-32 (poly `0xEDB88320`, seed 0, no final xor, LE),
matching the corpus-validated 145/145 method in FINDINGS.md. Pass bar = forge
`defs decode` returns `leftover == 0` with all tags matched, on real payloads.

## Unresolved

**None.** All 53 types resolve and decode clean.

Lower-confidence-but-structurally-correct (single directly-named field, noted for
transparency): `LOCAL_DETAIL_GENERATOR` / `CEngineLocalDetailGeneratorDef` has
exactly one field, `Layers` (`vector<CEngineLocalDetailLayerDef>`); the element
class is fully in the schema, so field-merge recurses into rich sub-structs. It
decodes clean on all 65 entries.

## How FableForge should fold this in

1. Load `def_schema_missing.json` and merge into the schema map.
2. Treat `CCombatAbilityBlockDefBase` as a **replacement** (base had it empty).
3. The 53 definition-string-keyed entries are picked up automatically by
   `resolveType` Candidate 1 — no code change. (Optionally, an explicit
   category→class alias table using each entry's `alias_of` would be cleaner
   than duplicating field lists, but the duplicated-fields form works as-is.)
