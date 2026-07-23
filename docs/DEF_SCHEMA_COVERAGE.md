# game.bin def-schema coverage

> **★★ CLOSED 2026-07-20 — 100.00% clean decode (14,761/14,761, 0 no-schema, 0
> dirty).** The last 2 types, `THING_GROUP` (85) + `ENGINE_THEME_GROUP` (20), are
> now specified: both classes' `Transfer` methods are **EMPTY** (donor decompiles
> `0191da25`/`0191e286` in `ghidra_out/donor_def_transfers.c` = bare `ret`; retail
> stubs `0x0045954D`/`0x00459764`) — they serialize ZERO fields. Every entry is the
> 5-byte untagged base-class prefix (`01 00 01 00 00`; NULLDEF all-zero), the same
> prefix `ENGINE_THEME` entries carry before their first field tag. Since no field
> tag exists to anchor the prefix empirically, `def_schema.json` gained an optional
> per-type `"prefix_len"` (set to 5 on both types + their class keys) which the
> FableForge decoder uses directly. FableForge commit `11aabb9`; mirror
> `ghidra_out/def_schema.json` re-synced (338 types).

> **★ UPDATE 2026-07-19 (VALIDATED): with `def_schema_missing2.json` merged, clean
> decode = 14,656 / 14,761 = 99.29%** (verified: `forge defs decode <root> <canonical
> + missing2> --all game.bin` → "14656 clean (99.29%), 105 no-schema, **0 dirty**").
> The 27 types in `missing2` include 16 **field-set replacements** of previously-
> incomplete types (CREATURE/CPhysicsDef/CTargetingDef/CWeaponDef derived chains) +
> 11 new — fold-in is **update-semantics, not pure-append**. `missing2` was produced
> by an agent that was interrupted before self-validating; the 99.29%/0-dirty check
> above was run afterward, so it is safe to fold into canonical `def_schema.json`.
> **Only 2 types still unresolved:** `THING_GROUP` (85 entries) + `ENGINE_THEME_GROUP`
> (20) = 105 entries. The table below is the pre-missing2 (86%) state, kept for history.

**Two distinct metrics — do not conflate them:**

| metric | value | meaning |
|---|---|---|
| **name-resolution** | 249/249 types (100%) | every bin `definition` string maps to *some* schema key (direct, or `C`+CamelCase+`Def`) |
| **clean decode** (the real one) | **99.29%** with missing2 (was 86.19% canonical-only) | the resolved schema actually decodes the payload with all field tags present and zero leftover (`forge defs decode --all`) |

Field-level def merge keys off **clean decode**: an entry that doesn't decode
clean falls back to whole-record merge. So field-merge covers **99.3%** of game.bin
by entry once `missing2` is folded in (canonical-only today is ~86%).

## Why the 14% gap

CamelCase name-resolution produces a schema key that *exists* but is the **wrong or
merely-base class** for derived Thing-hierarchy types. Example: `CREATURE`→
`CCreatureDef` resolves by name, but a real `CREATURE` payload's tags match only
`CThingBaseDef`'s 12 **base** fields, then continue with creature-specific fields
whose full class isn't in the schema. A game.bin entry for a derived type is the
**concatenation of its whole inheritance chain's** Transfer output, so it needs a
schema entry with the complete field set (as the decomp agent produced for
`OBJECT`→`CThingObjectDef`), not just one class layer.

`def_schema_missing.json` (folded in 2026-07-19) closed OBJECT/BUILDING/THING/UI +
the special-abilities family — raising field-merge from 39→43 field-merged records
on Aeon+LostContent (whole-record fallbacks 14→10). The types below are the
remaining gap.

## Decomp handoff — types needing their full derived-class field set

These bin def types don't decode clean; they need the complete field list of their
real (derived) class added to `def_schema.json`, keyed by the literal definition
string (so `resolveType` Candidate-1 picks it up). By entry payoff:

| bin def type | entries | note |
|---|---:|---|
| `CREATURE` | 517 | matches CThingBaseDef base only; needs full creature class |
| `CPhysicsDef` | 315 | class-named but field list incomplete for real entries |
| `CTargetingDef` | 254 | " |
| `CWeaponDef` | 190 | " |
| `THING_GROUP` | 85 | |
| `CContextSensitiveItemDef` | 79 | |
| `COMBAT_SEQUENCE` | 68 | |
| `CHasNameDef` | 62 | |
| `CBuyableHouseDef` | 60 | |
| `SOUND_THEME` | 57 | |
| `CShopDef` | 55 | |
| `CPerceivedThingDef` | 50 | |
| `VILLAGE` | 48 | |
| `CAMERA_MODE` | 46 | |
| `CSummonableCreatureDef` | 45 | |
| `HERO_ABILITY` | 20 | |
| `ENGINE_THEME_GROUP` | 20 | |
| `MESSAGE_EVENT` | 16 | |
| `OPINION_REACTION_MASK` | 13 | |
| `CAugmentationDef` | 11 | |
| `CTavernGameDef` | 9 | |
| `CDecapitationDef` | 6 | |
| `CShipDef` | 5 | |
| `CShopItemDef` / `CDegradableDef` / `CBettingDef` | 2 each | |
| `PLAYER_INVENTORY` | 1 | |

Regenerate with: `forge defs decode <root> docs/re_reference/def_schema.json --all
game.bin` (the corpus regression guard; exits non-zero while any entry is
unresolved/dirty).

## CRC note

Engine name→tag hash is **seed-0 reflected CRC-32** everywhere (game.bin fields,
text.big symbols, save streams + trailer) — no seed-`0xFFFFFFFF` variant on disk
(FINDINGS.md "★ CANONICAL CRC REFERENCE"). `forge::defdecode::fieldTag` already
uses seed 0.
