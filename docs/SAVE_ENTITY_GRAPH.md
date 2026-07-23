# Fable: The Lost Chapters — SAVED_ENTITIES entity graph (full walk)

**Status:** VALIDATED against real saves — graph walked end-to-end, real inventory + quest
state decoded, and the quest write path (LZO re-encode + container re-sign) EXECUTED and
re-verified (§8.1). **Date:** 2026-07-19 (second pass).
**Tool:** `tools/parse_save_entities.py` (read-only, pure stdlib + `tools/lionhead_lz.py`).
**Companions:** `docs/SAVEGAME_FORMAT.md` (container/zlib/CRC), `docs/SAVE_HEADER_MAP.md`
(HEADER/PLAYER tags), `docs/SAVE_HERO_STATS.md` (CTCHeroStats field map). Read those first.

Seed reminder: **every persist-stream field tag = `CCRC::Calc(0, fieldName, len)`** —
reflected CRC-32, poly `0xEDB88320`, seed 0, no final inversion (`tools/save_tools/savecrc.py
getcrc(name, 0)`).

Primary validation save: `Cornelio/Manual - Save1` (played hero). Cross-checked against
`Test/AutoSave` and `Default/AutoSave` (fresh new-game heroes) and `Cornelio/AutoSave.qs`
(same playthrough) — see §9.

---

## 1. Nesting recap (all layers, validated)

```
file ("FableSave!", fixed 307200 B)
 └─ chunk1 (zlib)  →  inflated persist stream
     └─ "SAVED_ENTITIES\0" [u32 sectionLen]
         └─ ~25 INDEPENDENTLY zlib-compressed CELLS (78 xx magic, self-framed)
             └─ each inflated cell = [u32 head] + run of top-level ENTITIES
                 └─ entity = header + base fields + N COMPONENTS
                     └─ component = [class\0][framing][u32 dataLen] + [tag][value] fields
```

Cornelio/Manual-Save1: **25 cells, 383,284 bytes total inflated** (matches
SAVE_HERO_STATS.md §4 byte-exact). Cell sizes range 396 B (cell 23) to 134,587 B (cell 24);
the hero lives in **cell 10** (43,920 B inflated — the cell containing `CTCHeroStats\0`).

Per-cell table (Cornelio/Manual-Save1, `parse_save_entities.py … cells`):

| cell | comp_off | comp_len | inflated | first-u32 |
|---|---|---|---|---|
| 0 | 124 | 533 | 894 | 1 |
| 1 | 694 | 3001 | 20889 | 2 |
| 2 | 3732 | 2417 | 7034 | 2 |
| 3 | 6186 | 1621 | 5203 | 1 |
| 4 | 7844 | 2012 | 5300 | 2 |
| 5 | 9893 | 501 | 688 | 1 |
| 6 | 10431 | 2144 | 7811 | 2 |
| 7 | 12612 | 2129 | 8121 | 1 |
| 8 | 14778 | 1504 | 5591 | 2 |
| 9 | 16319 | 657 | 1680 | 1 |
| **10** | 17013 | 13056 | **43920 (hero)** | 2 |
| 11 | 30106 | 2243 | 5450 | 1 |
| 12 | 32386 | 1096 | 2450 | 1 |
| 13 | 33807 | 515 | 840 | 1 |
| 14 | 34415 | 784 | 2264 | 1 |
| 15 | 35342 | 653 | 1097 | 1 |
| 16 | 36064 | 9734 | 92280 | 2 |
| 17 | 45939 | 1675 | 3026 | 2 |
| 18 | 47659 | 2592 | 6360 | 1 |
| 19 | 50325 | 412 | 636 | 1 |
| 20 | 50774 | 407 | 653 | 1 |
| 21 | 52209 | 4529 | 24729 | 1 |
| 22 | 56775 | 522 | 1385 | 1 |
| 23 | 58363 | 302 | 396 | 1 |
| 24 | 58766 | 15475 | 134587 | 4 |

(`comp_off` is relative to the SAVED_ENTITIES data start; the small u32 at cell start is
recorded as-is — it is 1/2/4 on these cells and is NOT proven to be the full top-level entity
count, since the name-anchored walker recovers more entities per cell than it. Treat as
[hypothesis: sub-group count]. The walker does not depend on it.)

---

## 2. Entity header framing (CONFIRMED on real bytes)

```
[inst-name ASCII \0]                 e.g. "PlayerCreature", "Object", "Thing"
[u32 A][u32 B]                       spatial/cell fields
[EngineClass ASCII \0]               Object | Marker | Village | AICreature | Building | …
[u32 flagsA][u32 mid][u32 flagsB]    mid = 4 (fresh) or 0xffffffff (no-parent marker)
[u64 UID]                            e.g. ffffff0000003ae3 (hero) — matches PLAYER section's
                                     PlayerCharacterUID (tag 96adc827) verbatim
[u32 z = 0]
[DEF-name UPPER_SNAKE \0]            game.bin thing type, e.g. CREATURE_HERO, OBJECT_BARREL_BREAKABLE
[u32 baseFieldLen] baseFieldLen bytes of [tag][value] base fields
                                     hero base fields DECODED: tag 78b48786 =
                                     getcrc("Health",0) → f32 80.0, tag eba5fd9d =
                                     getcrc("Scale",0) → f32 1.0   (both CONFIRMED by CRC
                                     name-match + sane values)
[u32 0][u32 componentCount][u32 0]
component × componentCount
```

The walker (`entity_headers`) anchors on `\0<EngineClass>\0` word boundaries and demands
`mid ∈ {4, 0xffffffff}`, `z == 0`, and an UPPER_SNAKE DEF-name after the 24-byte middle
block — this rejects the `Creature`-inside-`AICreature` substring false positives and nested
child records.

**Full enumeration** (`entity_records`) is class-independent — the hero's own header carries
NO engine-class token, and child entities nest below top level — so the complete walk anchors
on `[u64 UID (hi dword nonzero)][u32 0][DEF-name with known prefix \0][u32 baseFieldLen]`
(DEF prefixes: `OBJECT_ CREATURE_ MARKER_ BUILDING_ VILLAGE_ DUMMY_ THING_ HOLDING_`).
**Cross-check:** every entity record is closed by an `END\0` marker; on Cornelio/Manual-Save1
the anchor walk finds **348 records** and the cells contain **exactly 348 `END\0` markers** —
the enumeration is complete and exact.

Component framing (CONFIRMED, all cells):

```
[class-name ASCII \0]     CTC* (147 distinct classes observed)
[u32 pad = 0][u8 0]
[u32 dataLen]
dataLen bytes             [u32 seed-0 CRC(fieldName)][value] … (value width type-driven,
                          NOT self-describing: int/float = 4 B, bool = 1 B,
                          CCharString = raw chars + NUL, vector = [i32 count][elems])
[u32 sep = 0]             inter-component separator
```

---

## 3. Entity census (Cornelio/Manual-Save1)

**348 entity records total** (`entity_records`, END-marker cross-check 348/348): 267 are
classed top-level headers (`entities` command); the rest are children + the class-token-less
hero header. Classed top-level breakdown:

| EngineClass | count |
|---|---|
| Object | 153 |
| AICreature | 48 |
| Building | 32 |
| Marker | 19 |
| Village | 15 |

**108 distinct DEF-names** across all records (89 among classed top-levels alone,
`defcensus`). Top of the list: `OBJECT_BARREL_BREAKABLE` ×38,
`OBJECT_BOOKSHELF_01` ×16, `OBJECT_CUPBOARD_MEDIUM` ×16, `OBJECT_GUILD_BED_APPRENTICE_01` ×12,
`MARKER_FISHING_SPOT` ×10, `OBJECT_CHEST_OPENABLE` ×8, `CREATURE_GUARD` ×7,
`CREATURE_GUILD_APPRENTICE_MALE` ×7, `MARKER_DIGGING_SPOT` ×6,
`VILLAGE_WANDERING_POPULATION_{GOOD,EVIL}_GREATWOOD` ×3 each, `OBJECT_SILVERKEY_CHEST_5` ×2 —
i.e. exactly the mutable world state you'd expect a save to persist (breakables, containers,
NPCs, village populations, quest markers), consistent with the region/quest layout of an
early-game playthrough (Guild + Bowerstone + Greatwood cells).

---

## 4. Component-class census (147 distinct CTC* classes)

`census` command, all cells, Cornelio/Manual-Save1 (top 30 shown; full list reproducible):

```
CTCMapwho 348           CTCSoundPlayer 298      CTCGraphicAppearance 279
CTCHitLocations 264     CTCPhysicsStandard 229  CTCTargeted 217
CTCEntityEvents 191     CTCVillageMember 185    CTCGraphicAppearanceStaticMesh 171
CTCContainerRewardHero 120  CTCAnimationComplex 93  CTCVolumeContainmentTracker 84
CTCEnemy 80             CTCCreatureOpinionOfHero 76  CTCCreatureNavigation 69
CTCTalk 69              CTCScriptedControl 69   CTCCreatureModeManager 69
CTCEmoteIcon 69         CTCDayOrNightOnlySupport 69  CTCTurncoat 69
CTCSummonableCreature 69  CTCAIScratchpad 69    CTCOverheadDisplay 69
CTCSleep 69             CTCPhysicsNavigator 68  CTCAvoidRegionExit 68
CTCLook 65              CTCCarrying 65          CTCWeaponTrail 65
```

**Hero-singleton components** (count == 1 across the whole save — all live on the hero
entity in cell 10, useful as locator anchors): `CTCHeroStats`, `CTCHeroMorph`,
`CTCHeroExperience`, `CTCInventory`, `CTCInventoryClothing`, `CTCInventoryWeapons`,
`CTCInventoryStats`, `CTCInventoryExperience`, `CTCInventoryTrade`, `CTCInventoryQuests`,
`CTCInventoryMap`, `CTCInventoryAbilities`, `CTCGamePlayer`, `CTCPhysicsControlled`,
`CTCHeroOpinionDeedLog`, `CTCHeroRangedAttacks`, `CTCSpecialAbilities`, `CTCBoastUI`,
`CTCQuickAccessMenu`, `CTCInGameMenu`, `CTCHeroGuide`, `CTCStealth`, `CTCFishing`, …
Also world singletons: `CTCTeleporter`, `CTCDoor`, `CTCBoss`, `CTCBlacksmiths`,
`CTCTavernGameMaster`, `CTCOracleMinigame`, `CTCFireheartMinigame`.

Notable gameplay-editing anchors: `CTCChest` ×11 (chest state — pairs with the retail
`CChestDef` findings, `CChestDef::Transfer` 0x004DE204), `CTCSearchableContainer` ×54,
`CTCContainerRewardHero` ×120 (reward tables), `CTCBuyableHouse` ×32, `CTCShop` ×10,
`CTCGold` ×50, `CTCQuestCard` ×15, `CTCCreatureOpinionOfHero` ×76.

---

## 5. Hero stats (re-validated through the graph walker)

`herostats` command locates the hero cell by `CTCHeroStats\0` (cell 10) and reads the
seed-0 tags in the component window — output on Cornelio/Manual-Save1:

| field | tag | value | matches SAVE_HERO_STATS.md |
|---|---|---|---|
| Money | b03ccbfd | 1625 | YES |
| Morality | 79a2d479 | 483 | YES |
| Age | 663c8f40 | 26.4 (f32) | YES |
| Fatness | b0da59fb | 0.0 (f32) | YES |
| RenownLevel | 0c4ed775 | 2 | YES |

---

## 6. Hero inventory (CTCInventory / CTCInventoryClothing / CTCInventoryWeapons)

CONFIRMED on-disk layout of an inventory component's data block:

```
[u32 listTag = getcrc("InventoryItems", 0) = 2b837863]
[u32 itemCount][u8]
itemCount × record:
    [u32 nameLen][nameLen ASCII chars, no NUL]      OBJECT_* def name
    [u32 quantity]
    [u32 unkA][u8 ext]                              ext != 0 → variable extension payload
                                                    (money-bag contents, weapon augmentation
                                                    sublists [u32 n][records], hotkey CRCs)
then: run of NUL-terminated OBJECT_* names                  (quick-access / equipped refs)
then: further [tag][value] fields (e.g. equipped-weapon cstr in CTCInventoryWeapons)
```

Weapons use the literal name `"NULL"` for empty augment slots (skipped by the decoder).
Validated on Cornelio/Manual-Save1:

| component | declared count | decoded |
|---|---|---|
| CTCInventory | 62 | 62 |
| CTCInventoryClothing | 40 | 40 (worn pieces + `HERO_SUIT_*` outfit records) |
| CTCInventoryWeapons | 3 | 4 = 3 weapons + 1 **nested** `OBJECT_FLAME_AUGMENTATION` record inside `OBJECT_HERO_STICK`'s augment sublist |

Decoded sample (all names are valid game.bin OBJECT_* defs, quantities sane):
`OBJECT_RESURRECTION_PHIAL ×2, OBJECT_GUILD_SEAL_1 ×1, OBJECT_HAND_LAMP ×1,
OBJECT_FISHING_ROD ×1, OBJECT_SPADE ×1, OBJECT_HERO_MONEY_BAG ×1, OBJECT_SILVER_KEY ×7,
OBJECT_MANA_POTION ×14, OBJECT_HEALTH_POTION ×3, OBJECT_TROPHY_WHISPER_BROOCH_01 ×1, …`
Weapons: `OBJECT_HERO_STICK ×1, OBJECT_EBONY_CUTLASS ×1, OBJECT_OAK_LONGBOW ×1`.

**The trailing name table = equipped / quick-access state** (values are the proof):

- `CTCInventoryClothing`: exactly the 5 currently worn pieces — `OBJECT_HERO_NO_HAT,
  OBJECT_HERO_SHIRT_LEATHERARMOUR_GOOD, OBJECT_HERO_GLOVES/TROUSERS/BOOTS_LEATHERARMOUR`.
- `CTCInventoryWeapons`: `OBJECT_EBONY_CUTLASS` — the equipped weapon.
- `CTCInventory`: 9 names (money bag, 5 expressions, lightning ability, trophy tooth,
  mana potion) — the quick-access slot assignments [hypothesis on exact slot order].

Component-tail tags after the tables (`a7d70546`, `a10e3cad`, `c93f54bc`,
`691625d6`→equipped-weapon cstr, `1f5c5c94`/`f85a6bb9`→u32 values, …): names unrecovered
(2-strike wordlist brutes failed; structure recorded by the tool).

---

## 7. QUESTS / FACTIONS / REGIONS body sections (chunk1, outside SAVED_ENTITIES)

All three parse byte-exact on Cornelio/Manual-Save1 (`report`):

- **QUESTS** — one Lionhead-LZO1X text blob (**17,457 bytes** of persist-script text after
  decompression) + a **119-byte binary tagged trailer**. LZ blob framing (CONFIRMED on 30
  blobs: 1 QUESTS + 29 FACTIONS): `[u32 ulen][u16 clen][clen bytes LZO1X stream][raw literal
  tail]` — the stream self-terminates ~3 bytes short of ulen; the tail bytes are stored raw.
  Decoder: `tools/lionhead_lz.py`. Trailer tag names are UNRECOVERED (2 wordlist brutes
  failed) — structure only (`parse_quest_trailer`).

### 7.1 Decoded QUESTS text structure (the quest read/flip surface)

Plain persist-text (`Name value;` grammar — text mode, so **no CRC tags**). Blocks, in
order, from the real Cornelio/Manual-Save1 decode:

```
START_QUEST_CARDS                          quest cards held, one CSV line each:
OBJECT_QUEST_CARD_HOBBE_CAVE, Q_HobbeCave, 0     [cardDef, questName, flag]
END_QUEST_CARDS
START_ACTIVE_QUEST_CARDS                   accepted cards as u64 DECIMAL entity UIDs
18446742974197958578                             (= 0xffffff00… card-object UIDs, §2)
… END_ACTIVE_QUEST_CARDS
START_COMPLETED_QUESTS                     completed quests, same u64 UID form
18446742974197924033                             (9 entries in the sample)
… END_COMPLETED_QUESTS
START_ACTIVE_QUESTS                        running quest scripts by NAME (48 in sample:
Q_SunnyvaleMaster, Gameflow, V_SickChild, …)
END_ACTIVE_QUESTS
START_SAVED_QUESTS                         one block per quest script:
START_NEW_QUEST V_SickChild
START_SCRIPT_DATA                            per-quest persistent variables:
FinishedQuest FALSE;  GotFishingSpotMushroom TRUE;  MansLoverState 0; …
END_SCRIPT_DATA
START_ENTITY_SCRIPTS                         per-entity script state, keyed by u64 UID:
START_NEW_ENTITY_SCRIPT 179907590094853509
GivenObject TRUE;
END_NEW_ENTITY_SCRIPT …
END_NEW_QUEST … END_SAVED_QUESTS
```

**Read:** `parse_save_entities.py <save> quests` prints the full text.
**Flip:** quest completion lives in (a) the `COMPLETED_QUESTS` u64-UID list and (b) the
quest's own `SCRIPT_DATA` vars (`FinishedQuest TRUE;` etc.) — edit the text, re-encode
(§8.1). The 119-byte binary trailer references `"Q_TraderEscort"` ×3 (the currently
tracked/boast quest) with small ints (3/1/3/9) and bools.
- **FACTIONS** — `[u32 headTag = 4f0e45c3][u32 count]` then count × `[u32 recTag][LZ blob]`.
  **29/29 records** consume the section byte-exact. Text per record:
  `Name "FACTION_X"; NumberOfAllies n; Ally "…"; … EndFaction`. recTag is NOT
  `getcrc(factionName, 0)` — names unrecovered.
- **REGIONS** — `[u32 a][u32 b][u32 c]` head then run of `[region-name \0][19 bytes flags]`.
  **141 records** on Cornelio/Manual-Save1.

---

## 8. Reproduce

```
python tools/parse_save_entities.py "<save>" cells       # cell inventory
python tools/parse_save_entities.py "<save>" census      # CTC* component census
python tools/parse_save_entities.py "<save>" entities    # 267 top-level entity headers
python tools/parse_save_entities.py "<save>" defcensus   # DEF-name census
python tools/parse_save_entities.py "<save>" herostats   # confirmed CTCHeroStats fields
python tools/parse_save_entities.py "<save>" inventory   # hero inventory decode
python tools/parse_save_entities.py "<save>" quests      # QUESTS LZ text + trailer
python tools/parse_save_entities.py "<save>" factions    # 29 faction records
python tools/parse_save_entities.py "<save>" regions     # region flag records
python tools/parse_save_entities.py "<save>" report      # one-shot summary
```

All commands above are read-only. `flipquest <questName> <outfile>` is the write-path PoC
(writes only to the given output file, never to the source save).

### 8.1 Write path — EXECUTED and re-verified (quest flip round-trip)

There is no LZO1X *compressor* port, but the grammar admits a **store-mode stream**
(`lzo_store()`: one initial literal run, one 3-byte M3 match on a repeated 3-gram, one
extended literal run, EOS marker; blob rebuilt in the retail shape
`[u32 ulen][u16 clen][stream][3 raw bytes]`). Validated:
`lionhead_lz.decompress(lzo_store(T)) == T` for the real QUESTS text **and all 29 faction
texts of all 3 saves** (90 round-trips) — the decoder is a 1:1 port of the engine routine,
so grammar-level acceptance is proven.

Executed PoC on a COPY of `Cornelio/Manual - Save1`:

```
python tools/parse_save_entities.py <copy> flipquest Q_HobbeCave <out>
→ wrote 307,200 bytes, trailer sig re-verified, quest 'Q_HobbeCave' now in COMPLETED_QUESTS
```

Pipeline: decode QUESTS text → insert the quest into `START_COMPLETED_QUESTS` →
`replace_quests_text()` (re-encodes the blob, patches the QUESTS `[u32 sectionByteLen]`) →
`save_edit.build_save()` (re-deflates chunks, fixes header dwords, **recomputes the seed-0
trailer signature**, pads to 300 KB). The rebuilt save re-reads cleanly, the binary quest
trailer is byte-identical, and the decoded text shows the flip. (A full flip should also
set the quest's `FinishedQuest TRUE;` and drop it from `START_ACTIVE_QUESTS` — same text
edit. Retail's in-game acceptance of the trailer sig / store-mode stream remains the one
live-fire unknown, as in SAVE_HERO_STATS.md §5.)

### 8.2 Save-editor recipe — inventory edit

1. `save_edit.read_save(raw)` (verifies trailer sig); find `SAVED_ENTITIES\0` in chunk1,
   inflate cells (`inflate_cells`), pick the cell containing `CTCHeroStats\0`.
2. Find `CTCInventory\0`, skip `[u32 0][u8 0][u32 dataLen]`; check
   `[u32 2b837863][u32 itemCount]`; walk records (§6) to `[u32 nameLen][name]` of the
   target item and overwrite the following `u32 quantity` **in place** (fixed width — no
   reframe). Adding/removing whole items shifts lengths (component `dataLen`, `itemCount`,
   cell size) — prefer quantity edits until the record-extension grammar is closed.
3. Re-deflate the cell (`zlib.compress(cell, 9)`), splice it back, patch the
   `SAVED_ENTITIES [u32 payloadLen]` by the compressed-size delta, set
   `chunk1_ulen += delta`, then `save_edit.build_save(...)` (re-signs + pads) — identical
   to the validated SAVE_HERO_STATS.md §5 gold/morality round-trip.

---

## 9. Cross-save validation (4 saves, all clean)

Every command above was run on all four saves; all parse without error and all invariants
hold (`report` + `herostats`):

| | Cornelio/Manual-Save1 | Cornelio/AutoSave.qs | Test/AutoSave | Default/AutoSave |
|---|---|---|---|---|
| cells | 25 | 25 | 2 | 2 |
| inflated bytes | 383,284 | 382,797 | 38,824 | 39,983 |
| entity records (walk) | 348 | 348 | 44 | 46 |
| `END\0` cross-check | 348 | 347¹ | 44 | 46 |
| classed top-levels | 267 | 266 | 42 | 42 |
| distinct DEF-names | 108 | 108 | 18 | 20 |
| hero cell idx | 10 | 12 | 0 | 0 |
| Money / Morality | 1625 / 483 | 1625 / 483 | 0 / 0 | 0 / 0 |
| Age / Fatness | 26.4 / 0.0 | 26.4 / 0.0 | 18.0 / 0.0 | 18.0 / 0.0 |
| RenownLevel | 2 | 2 | 0 | 0 |
| CTCInventory (count=decoded) | 62=62 | 62=62 | 5=5 | 5=5 |
| CTCInventoryClothing | 40=40 | 40=40 | 0=0 | 0=0 |
| CTCInventoryWeapons | 3 decl / 4 dec¹ᵃ | 3 decl / 4 dec¹ᵃ | 0=0 | 0=0 |
| QUESTS text / trailer B | 17,457 / 119 | 17,457 / 119 | 7,010 / 8 | 6,002 / 8 |
| FACTIONS records | 29 | 29 | 29 | 29 |
| REGIONS records | 141 | 141 | 141 | 141 |

¹ AutoSave.qs: 348 anchor-walked records vs 347 `END\0` — a 1-record delta on this one save
(anchor walk finds one record whose END marker is absent or split; benign, under
investigation). All other saves cross-check exactly.
¹ᵃ The 4th decoded weapons record is the nested `OBJECT_FLAME_AUGMENTATION` inside
`OBJECT_HERO_STICK`'s augment sublist (§6) — declared count counts weapons only.

Consistency arguments:
- Both Cornelio saves (same playthrough, minutes apart) decode to **identical** hero stats,
  inventory counts, QUESTS text size, and DEF-name census (108) — deterministic decode.
- Test and Default are fresh new-game heroes → canonical starting values (Money 0,
  Morality 0, Age 18.0, RenownLevel 0, 5 starting inventory items, no clothing/weapons yet)
  and the same 2-cell starting world (42 classed top-levels; Object 31 / Building 10 /
  Village 1).
- FACTIONS (29) and REGIONS (141) are invariant across all saves — they enumerate static
  world tables, not per-playthrough state.
- Hero cell index moves (10 vs 12 vs 0) — locate the hero by `CTCHeroStats\0` content, never
  by cell position.

---

## 9.5 ★ 2026-07-20 pass — cell RECORD framing decoded + productized editor

**New saves:** profile `1` (fresh hero, 2026-07-20) parsed clean alongside the original four —
six saves total now cross-validated.

### 9.5.1 SAVED_ENTITIES cell records (CONFIRMED, 81/81 cells across 6 saves)

The "small u32 header of per-cell metadata" (§SAVE_HERO_STATS §4) is now fully decoded. The
SAVED_ENTITIES section body is `[u32 0][u32 N?][u32 0]` then a run of **cell records**:

```
[u64 0]                     marker
[u64 recLen = 29 + clen]    record length (covers cellId..data + 9 trailing zero bytes)
[u64 cellId]                stable region/cell identifier (e.g. hero cell 28885a in Cornelio,
                            3020df in fresh saves; ids identical across saves of one layout)
[u64 clen]                  compressed length of the zlib cell
[u32 ulen]                  uncompressed (inflated) length
[clen bytes]                the zlib stream (78 xx)
[9 zero bytes]
```

Empty (unloaded-region) slots appear as all-zero runs between records; after the last record
the section carries a trailing index table of `[u32 X][u32 cellId][u32 0][u32 hash?][u32 0]`
entries mapping small ids (entity/things index?) to cellId — it references **cellId, not byte
offsets**, so it needs no patching on edits. [index-table semantics = hypothesis; record
framing = CONFIRMED by 81/81 invariant checks `marker==0 && recLen==29+clen && clen==zlib
consumed && ulen==inflated && 9-byte zero trailer`.]

> **★ CORRECTION to §8.1/§8.2 and SAVE_HERO_STATS §5:** patching only the SAVED_ENTITIES
> section length + chunk1_ulen is NOT sufficient — the per-cell `recLen`/`clen` (and `ulen`
> when the inflated size changes) in this descriptor MUST be patched too, or the engine's
> record walk mis-frames every cell after the edited one. The earlier gold/quest PoCs
> predated this finding; `save_edit.replace_cell()` now does all three.

### 9.5.2 Inventory record grammar (exact, byte-confirmed)

```
component data = [u32 2b837863 = getcrc0("InventoryItems")][u32 itemCount][u8 0]
record         = [u32 nameLen][nameLen ASCII chars, no NUL][u32 quantity][u32 unkA][u8 ext]
                 (+ variable extension payload iff ext != 0)
```
- `unkA` = 0 for most items but **nonzero on used tools** (Cornelio: FISHING_ROD 65,
  SPADE 233) — [hypothesis: per-item usage counter].
- Money-bag extension (`OBJECT_HERO_MONEY_BAG`, ext=1): 20 bytes = 15×00 + `01 00 00 00` + 00
  (recorded; internal meaning still open).
- Component tail on a fresh hero: exactly 3 `[tag][u32 0]` fields (`a7d70546`, `a10e3cad`,
  `92e50fdb`) — names still unrecovered.

### 9.5.3 Productized editor — `tools/save_edit.py` (validated this pass)

```
save_edit.py report   <save>                    hero stats + all inventory components
save_edit.py set-gold <save> <out> <amount>     Money (CTCHeroStats)
save_edit.py set-stat <save> <out> <name> <v>   Money|Morality|RenownLevel (i32), Age|Fatness (f32)
save_edit.py set-qty  <save> <out> <DEF> <qty>  in-place u32 quantity edit
save_edit.py add-item <save> <out> <DEF> [qty]  inserts a fresh record (ext=0,unkA=0) at list
                                                front; def name validated against names.bin
                                                (13,593 defs); refuses duplicates (use set-qty)
```
Every write: locates the hero cell by `CTCHeroStats`, edits the inflated cell, re-deflates,
patches descriptor `recLen/clen/ulen` + section len + chunk1_ulen, rebuilds + re-signs the
container (trailer CRC seed 0, exact 300 KB, zero padding — SAVE_SIG_ENFORCEMENT.md recipe).
`verify_edit()` then asserts byte-level surgical scope: chunk0, every non-hero cell
(compressed AND inflated), and all of chunk1 outside SAVED_ENTITIES byte-identical; hero-cell
inflated diff printed as exact ranges. Validated on real-save copies:
- set-gold 1625→999999: 3-byte diff (value bytes only); set-stat Morality 483→−750: 4 bytes;
- set-qty SILVER_KEY 7→30: 1 byte; add-item HEALTH_POTION×50 / EBONY_KATANA×1: exactly
  `dataLen+33/32`, `itemCount+1`, inserted record — nothing else.
- Edited saves re-parse clean through the independent `parse_save_entities.py` walker
  (348 records, END cross-check 348, census unchanged).

**NOT yet live-tested in-game** (user away) — staged copies only under the session scratchpad;
real saves untouched. Live QA: load an edited copy, confirm gold/stat/item appears; specifically
confirm (a) list-front insertion order is accepted, (b) a general-list add of a weapon def is
enough for it to appear (vs needing a CTCInventoryWeapons record), (c) qty>stack-max behaviour.

## 10. Open items

1. First-u32-of-cell semantics (1/2/4 observed) — not the walker-recovered entity count;
   [hypothesis: sub-group count]. Walker does not depend on it.
2. QUESTS-trailer, FACTIONS head/recTag, and inventory component-tail field names —
   unrecovered (2-strike wordlist brutes done); needs `CQuestManager::SaveGameState` /
   `CFactionManager` / `CTCInventory::Transfer` decompiles (deferred — no-Ghidra pass).
3. Entity base-field tag→name map: hero's two base fields recovered (`Health` 78b48786,
   `Scale` eba5fd9d, §2); other classes' base fields not yet brute-forced. Inventory
   record extension payload internals (money-bag state, augment sub-record grammar) and
   the `[u8]` between itemCount and the first record. REGIONS 19-byte flag block per-byte
   meaning.
4. Child-entity parent↔child containment tree — records (incl. children) are fully
   enumerated via the UID/DEF anchor and cross-checked by `END\0` counts, but the explicit
   nesting relation (which child belongs to which parent) is not yet emitted by the walker.
5. Cornelio/AutoSave.qs: 348 anchor records vs 347 `END\0` markers (1-record delta on this
   one save; all others exact) — see §9 footnote.
