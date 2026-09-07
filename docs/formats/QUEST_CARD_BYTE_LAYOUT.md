# Quest-Card byte layout — CQuestCardDef + OBJECT_QUEST_CARD (for from-scratch authoring)

*RE'd 2026-08-16 (`quest-card-re-prereqs` workflow, byte-exact measurement). Confidence:
**HIGH** for the CQuestCardDef field set + Transfer order; **HIGH** for the two OBJECT
back-refs; **PARTIAL** for the rest of the 458 B OBJECT wrapper (still engine-internal).*

This pins the wire format needed to **synthesize a quest card from scratch** rather than
clone a donor. It lifts the blocker documented in FableForge `questcard.hpp:20-26`
("the engine writer `CQuestCardDef::Transfer` byte order for a full payload is
unrecovered") — for the **card def** it is now recovered.

## The two-entry model (unchanged)
A usable custom card is a PAIR of game.bin entries:
1. an unnamed **`CQuestCardDef`** sub-def (~136 B) — the card CONTENT, and
2. a named **`OBJECT` "OBJECT_QUEST_CARD_<X>"** (a `CObjectDef`, ~458 B) — the inventory
   object the runtime resolves by NAME (`Quest:AddQuestCard("OBJECT_QUEST_CARD_<X>", …)`).

The OBJECT links to its card def by GLOBAL ENTRY INDEX (`u32` @ offset **81**), and the
embedded `CQuestCardDef` component record carries a self-back-ref to the OBJECT's own
global index (`u32` @ offset **85**). Both must be retargeted on append (see
`DEF_LOAD_CONTRACT.md` BUG #2).

## CQuestCardDef — full field set (Transfer order, crc0-tag-anchored)

The def payload is **tag-anchored**, not fixed-offset: each field is preceded by its
`crc0(fieldName)` tag, then the value. `defdecode`/`defencode` round-trip retail bytes
exactly. To author from scratch, emit the 18 fields **in this order**, each as
`[u32 crc0-tag][value]`. Offsets below are the measured tag positions in the shipped
`OBJECT_QUEST_CARD_WASP_MENACE` card def (payload ~136 B); values are read at tag+4.

| # | Field | crc0 tag | Type | WASP_MENACE value | Notes |
|---|-------|----------|------|-------------------|-------|
| 1 | QuestName | `0x06A69EC7` | int32 (TextID) | (valid) | text.big TextID; **0 renders blank** — reject 0 |
| 2 | QuestSummary | `0x62A8C71D` | int32 (TextID) | (valid) | text.big TextID; 0 renders blank |
| 3 | QuestObjective | `0xE8228878` | int32 (TextID) | 0 | static; runtime objective comes from `CTCQuestCard+0x1c` via `SetQuestCardObjective` |
| 4 | SuccessSummary | `0x2A8D49C6` | int32 (TextID) | 0 | |
| 5 | RegionName | `0x6FE0488D` | CDefString ref | `0x0005A238` | region name-ref; gates `GiveHeroQuestCardDirectly` path only (AddQuestCard ignores) |
| 6 | TeleporterRegionName | `0x736F110E` | CDefString ref | `0xFFFFFFFF` (null) | |
| 7 | InventoryCategory | `0x2D3AFF51` | int32 (GameBINEntryID) | `0x00000A49` | → INVENTORY_CATEGORY def; controls Logbook tab |
| 8 | RenownReward | `0x3894FDA8` | int32 | 200 | static; on-screen renown from `CTCQuestCard+0x30` via setter |
| 9 | GoldReward | `0xFFBE5968` | int32 | 500 | static; on-screen gold from `CTCQuestCard+0x2c` via setter |
| 10 | RewardObjects | `0x5A24DE1F` | Vector_J | count=1 | `[u32 count][u32 GameBINEntryID]*count`; opaque — leave empty (count=0) for from-scratch |
| 11 | IsCoreQuest | `0x3995EBC1` | bool (1 B) | 1 | **also selects card art** (see ../engine/QUEST_CARDS.md orb art) |
| 12 | IsVignette | `0x3C9E385B` | bool (1 B) | 0 | **also selects card art** |
| 13 | IsExclusive | `0xD5A09700` | bool (1 B) | 0 | |
| 14 | MakeVignetteRouteAppearOnMinimap | `0x0C32773A` | bool (1 B) | 0 | |
| 15 | Prerequisites | `0x8F43EE40` | Map | 0 entries | `Map<…,EQuestCardPrerequisite>`; opaque — emit empty for from-scratch |
| 16 | NumBoasts | `0x1601FBA4` | int32 | 0 | side-objective count |
| 17 | CanPlayerCancel | `0x3955E3BC` | bool (1 B) | 1 | |
| 18 | QuestEpilogue | `0x462674C0` | uint32 | 0 | epilogue index/state; payload ends here |

Measured tag offsets in WASP_MENACE (tag position; value at +4): QuestName 7, QuestSummary
15, QuestObjective 23, SuccessSummary 31, RegionName 39, TeleporterRegionName 47,
InventoryCategory 55, RenownReward 63, GoldReward 71, RewardObjects 79, IsCoreQuest 91,
IsVignette 96, IsExclusive 101, MakeVignetteRouteAppearOnMinimap 106, Prerequisites 111,
NumBoasts 119, CanPlayerCancel 127, QuestEpilogue 132.

**From-scratch defaults** (safe, no donor content inherited): QuestName/QuestSummary = valid
TextIDs (never 0); QuestObjective/SuccessSummary = 0 (populated at runtime by the companion
quest); RegionName/TeleporterRegionName = `0xFFFFFFFF` (null — no region binding needed for
the AddQuestCard display path); InventoryCategory = a valid category id (choose the "Current
Quests" category — TBD id, else inherit a known-good literal); RewardObjects = empty;
Prerequisites = empty; scalars per author intent; IsCoreQuest=1 for the core-orb art.

## OBJECT_QUEST_CARD (CObjectDef, ~458 B) — back-refs + invariance

Confirmed absolute-index back-refs that MUST be retargeted on append:

| Offset | Meaning | Retarget? |
|--------|---------|-----------|
| 77 | crc0('CQuestCardDef') component tag = `0xB4C3A48A` | no (constant) |
| 81 | `u32` link to the CQuestCardDef sub-def global index | **yes** → new card index |
| 85 | `u32` self-ref: OBJECT's own global index (component back-ref to parent) | **yes** → new OBJECT index |

The remaining 458 B (offsets 0–76, 89–457) are engine-internal object state
(graphics/physics/interaction defaults) not yet fully reversed. For from-scratch authoring
these bytes are **card-content-invariant** across shipped cards (to be confirmed by a
cross-card diff — see `authorFromScratch` design): all `OBJECT_QUEST_CARD_*` share the same
generic object plumbing and differ only in name + offsets 81/85 + the embedded card def.
A single **canonical OBJECT skeleton** therefore serves any from-scratch card without
inheriting a specific quest's content. This is distinct from cloning a donor's *quest*
data — the skeleton carries no quest identity.

## Consumers
- FableForge `libs/forgecore/src/questcard.cpp` — `author()` (clone) / `overwriteDonor()`
  today; `authorFromScratch()` (this doc's target) synthesizes the pair.
- `defdecode::decode`/`encode` — the tag-anchored round-trip these offsets were measured
  against.

## Evidence
- Byte-exact measurement: defdecode round-trip of `OBJECT_QUEST_CARD_WASP_MENACE` (OBJECT
  entry 3713 → CQuestCardDef entry 12298) from a real appended game.bin build; crc0 tag
  search located each field; tag order matches def schema field order.
- [../engine/QUEST_CARDS.md](../engine/QUEST_CARDS.md) (script API, display map, empty-card fix), `docs/formats/DEF_LOAD_CONTRACT.md`.
- FableForge `questcard.hpp:28-102`, `chocolatebox_def.xml` CQuestCardDef schema.
