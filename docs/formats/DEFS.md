# Compiled definitions — names.bin / game.bin, crc0, Transfer<T> orders, schema, load contract

*Merged reference (2026-09-07) of `DEF_LOAD_CONTRACT.md` (2026-07-24, EgoCore cross-ref 2026-07-31),
`DEF_SCHEMA_COVERAGE.md` (2026-07-19/20), `DEF_SCHEMA_GAP_FILLED.md` (2026-07-19),
`TRANSFER_FIELD_ORDERS.md` (2026-08-10/11), `COMPILED_DEFINITION_INDEX.md` (2026-07-18) and
`DEFINITION_HEADER_INDEX.md` (2026-07-18). Each source is now a stub pointing here. Related:
[QUEST_CARD_BYTE_LAYOUT.md](QUEST_CARD_BYTE_LAYOUT.md) (a concrete game.bin def edit),
[FRONTEND_FORMAT.md](FRONTEND_FORMAT.md) (frontend.bin), [../engine/QUEST_CARDS.md](../engine/QUEST_CARDS.md),
[../journal/2026-07/EGOCORE_ASSESSMENT_20260731.md](../journal/2026-07/EGOCORE_ASSESSMENT_20260731.md).*

## Overview

Fable TLC ships its game-object definitions ("defs") pre-compiled under `data\CompiledDefs\`:

| File | Entries | Unique definition types | Named entries |
|---|---:|---:|---:|
| `game.bin` | 14,761 | 249 | 8,888 |
| `script.bin` | 611 | 3 | 611 |
| `frontend.bin` | 810 | 8 | 810 |

`names.bin` is the shared string+CRC table (13,593 names) that every `.bin` references by byte
offset. A def entry's payload is the concatenated output of its class's whole inheritance chain of
`Transfer(CPersistContext&)` calls; each field is tagged with `crc0(fieldName)` and the order of
`Transfer<T>` calls IS the binary field order.

**State of knowledge (all confirmed):**
- On-disk layout of both containers is byte-proven against the pristine retail base
  (14761 entries / 13593 names) — [container](#namesbin--gamebin-container).
- The one hash, `crc0`, keys names.bin CRCs, game.bin field tags and the engine's
  `map<unsigned_long, CDefClassInfo>` class registry; 13593/13593 names verified — [crc0](#crc0--the-one-hash).
- Field orders for **268** def classes / **5,133** fields extracted from the debug-build `Transfer`
  functions; 267/268 exact-match `jamen/fable-defs` — [Transfer<T> field orders](#transfert-field-orders).
- Schema coverage is **CLOSED at 100.00%** (14,761/14,761 clean decode, 0 no-schema, 0 dirty) —
  [schema coverage](#schema-coverage-and-gaps-filled).
- Appending a new def works only if BOTH append bugs are fixed (names.bin CRC must be crc0 AND
  self global-entry-index back-refs retargeted); in-place field edits sidestep both —
  [load contract](#load-contract-append-rules).
- Installed-game indexes of compiled entries and the 63,540 header enum constants —
  [indexes](#definition-and-header-indexes).

## names.bin / game.bin container

On-disk layout, byte-exact vs pristine retail base = 14761 entries / 13593 names. (Independent
agreement: NIKO's `DEFBIN_FORMAT.md` AGREES on every field — 20-B header, `0xa8e36c34`, 12-B entry,
zlib chunks, NULLDEF, component prefix — see `docs/journal/2026-07/NIKO_DOC_COMPARISON.md` row 13.)

### names.bin (shared string+CRC table)

```
+0  u32  0x0007AB1E   version/tag (preserved verbatim)
+4  u32  0xA8E36C34   MAGIC (PC); echoed in game.bin @+5
+8  u32  nameCount    = 13593        ← append: +1 per new name
+12 u32  tableSize    = 396904 = record_bytes + 4 (writer nextOffset-4 convention) ← append: grow
+16 u32  0            reserved
+20 ...  nameCount × { u32 crc0(name) ; ASCIIZ name }
```

A "name reference" stored in game.bin = byte offset of the name TEXT relative to +20, i.e.
`(pos_of_crc - 20) + 4`. `-1 / 0xFFFFFFFF` = "no name" (sub-defs).

### game.bin (compiled def store)

```
+0  u8   0x00
+1  u8   FORMAT FLAG: 0xAA=Xbox (rejected), 0x21=PC   (part of a 9-byte header blob, verbatim)
+5  u32  0xA8E36C34   MAGIC
+9  u32  entryCount   = 14761        ← append: +1 per new entry
+13 ...  MAIN TABLE: entryCount × 12-byte rows {
             i32 defNameOffset   (names.bin ref → class name, e.g. "CREATURE")
             i32 nameOffset      (names.bin ref → instance name, or -1)
             i32 indexInDefinition }
     u32  chunkCount (= realChunks + 1)
          (chunkCount-1) × { i32 firstEntryIndex ; i32 cumulativeCompressedOffset }
          + end sentinel { i32 entryCount ; i32 totalCompressed } + u32 totalCompressed
     ...  zlib chunks (each ≤ ~32–64 KiB inflated). Each inflated chunk begins with
          u16 payloadOffset[nInChunk] (biased by nInChunk*2), then concatenated payloads.
```

- `indexInDefinition` = **per-def-class dense running counter**, 0..N-1 within its class.
  Verified: the CREATURE class has **517** entries with dense indexInDefinition 0..516;
  `CREATURE_TRADER_01` = global entry **1549**, indexInDefinition **270**. Entry order IS the
  runtime global definition index.
- **Untagged base-class prefix.** Entries whose class chain begins with an untagged base run carry a
  prefix before the first field tag: `THING_GROUP` / `ENGINE_THEME_GROUP` entries are exactly the
  5-byte prefix `01 00 01 00 00` (NULLDEF all-zero) and nothing else; `ENGINE_THEME` entries carry the
  same prefix before their first tag. `CThing*Def` derived types call `CThingBaseDef::Transfer` first,
  so their payloads open with the base-class run (the large untagged "prefix" seen in OBJECT/SWITCH
  etc. is really `CThingBaseDef`'s 12 base fields before the derived fields' first tag).
- **Payload self-references.** A creature payload carries **absolute global entry indices** as
  self/owner back-references: `CREATURE_TRADER_01` (global 1549) holds `u32 == 1549` at payload
  offsets **{25, 193, 301}** and nowhere else. Component/sibling refs (`10744` CCreatureDef /
  `10745` CAppearanceDef / `10746` CEnemyDef and the stride-9 table @314..638, indices 1175..1307) are
  **shared** sub-defs (10744 is referenced by 5 CREATUREs and does not back-ref 1549).
- **EgoCore parity details not yet retail-verified** (verify before relying): (a) the compressed
  stream is **zlib deflate level 1, fed one byte at a time**, to bit-match Lionhead; (b) game.bin carries
  a **`dependencyCRC = 0xE86E4CDE`** immediately after the safeBool byte, before randomID;
  (c) EgoCore emits a per-class **`NULLDEF_<name>`** string + null instance — check whether retail
  names.bin carries these and whether the append tool must preserve them.
- No game.bin-level integrity/trailer checksum was found (unlike save files, which carry a seed-0 CRC
  trailer); forge round-trips game.bin edits in other subsystems, so believed absent but not
  explicitly ruled out.

## crc0 — the one hash

`crc0` = standard **reflected CRC-32, poly `0xEDB88320`, seed `0`, NO final inversion**
(engine `CCRC::Calc(0,…)` / `CCharString::ComputeCRC32` @ `0x00404310`). This single hash keys
everything: names.bin CRCs, game.bin field tags, and the `std::map<unsigned_long, CDefClassInfo>`
def-class registry (key = `crc0(className)`). It is also the hash for text.big symbols and save
streams + trailer — no seed-`0xFFFFFFFF` variant exists on disk (FINDINGS "★ CANONICAL CRC REFERENCE").

- Proven across the entire table: **13593/13593** stored names.bin CRCs equal `crc0(name)`;
  `0/13593` equal any other variant. Case-sensitive.
- `crc0("CREATURE_TRADER_01") = 0xAA22BB08` (== retail stored CRC, byte-exact)
- `crc0("Graphic") = 0x2E6B63C8` (== the game.bin field tag)
- `crc0("CREATURE_MESHY_HUNTER") = 0x5A11F1E5` (forge's old formula wrote `0xE4EACB91`)
- It is **NOT** `0xFFFFFFFF - crc32`, NOT `~crc32` via subtraction, NOT zlib `crc32`, NOT seed-`0xFFFFFFFF`.
- Field-tag validation: reflected CRC-32 (poly `0xEDB88320`, seed 0, no final xor, LE), matching the
  corpus-validated 145/145 method in FINDINGS. `forge::defdecode::fieldTag` uses seed 0.
- **EgoCore caveat.** EgoCore's `CalculateFableCRC32` (seed 0, no invert, poly 0xEDB88320) agrees with
  crc0, but its standalone `CDefStringTable::GetCRC` uses standard CRC-32 + `tolower` — **wrong** for
  names.bin (filed as EgoCore issue #4; dead-code in the discontinued FableDefCompiler only) — and its
  standalone writer emits `classIndex = 0` for every entry, contradicting the dense per-class
  `indexInDefinition`. Do NOT adopt either; keep crc0 + the dense counter in FableForge.
- **Independent oracle:** `jamen/fable-defs` (the def compiler now inside EgoCore) is a byte-exact
  def compiler whose CRC agrees with ours (13593/13593 both verified).

## Transfer<T> field orders

**Artifact:** `refs/transfer_field_orders.json` — **268** def classes, **5,133** named+typed+ordered
field controls. Generated 2026-08-10; sub-component collection + base-class flatten + namespaced-def
recovery landed by 2026-08-11. **Status: complete.** **267/268** classes exact-match fable-defs field
order; the single remainder (`COpinionSourceDef`) is an array-unroll artifact, not a data gap.

### Why it exists

`jamen/fable-defs` derives each def class's binary layout from the game's `CDefBase::Transfer`
functions: *"the order of `Transfer<…>` calls IS the binary field order,"* with `Transfer<long>⇒i32`,
`Transfer<float>⇒f32`, `Transfer<CDefString>⇒DefString`. Its `AGENTS.md` names its field-order oracle
as a private decomp `~/git/fable-decomp`, marked **"Absent at time of writing."** We *have* that
oracle; this JSON is our contribution back (EGOCORE_ASSESSMENT §B5 for the relationship).

### Source & method

- **Binary:** `debug_build/ego_r.exe` — the Fable Anniversary (2013-02-25) **debug build**, ImageBase
  `0x400000`, which ships full PDB symbols (`ghidra_out/egor_pdb_names.tsv`, demangled).
- **Method** (`tools/transfer_extract/tx_extract.py` → `tx_finalize.py`): linear-disassemble each
  `?Transfer@C*Def@@UAEXAAVCPersistContext@@@Z`. Every call to
  `CPersistContext::Transfer<T>(const char* name, T& field, const T& deflt)` is one `game.bin` field
  control, emitted **in call order**. The **name** is the 1st-arg string literal (`push <rdata ptr>`);
  the **wire type T** is the template arg of the call-target symbol.
- **Wire-type legend:** `J`=i32(long) · `H`=i32(int) · `_N`=bool · `M`=f32 · `N`=f64 ·
  `K`=u32(ulong) · `I`=u32(uint) · `F`=i16 · `G`=u16 · `E`=u8 · `D`=i8 ·
  `VCCharString`=String/DefString · `VCDefIndex`=DefIndex · `W4X`=enum X (i32 wire) ·
  `TransferVector<T>`=Vec<T>.
- **Sub-component collection fields** (`Abilities`, `Waves`, `Creatures`, `Meshes`, ...) are
  transferred inline in `Transfer()` via
  `CPersistContext::TransferVectorOfSubComponents<T>(const char* name, vector<T>&)` — name string and
  all — under a symbol family the first regex missed. Verified on `CArenaRoundDef::Transfer @0xf93320`:
  `NumWaves` (i32) then `Waves` (`vector<subdef CArenaWaveDef>`). Family regex widened to
  `\?\?\$(Transfer\w*)@...@CPersistContext@@`; exact matches **211 -> 251**, +92 collection fields.
  No constructor pass was needed.
- **Base-class flatten** (`tools/transfer_extract/tx_flatten.py`, pure JSON transform): recursively
  expands each `{"type": "base:<Class>"}` marker into that base's field list, tagging inlined fields
  with `"from_base": "<Class>"`. The 10 base-marker classes (9x `CThing*Def` + `CTavernGameShoveHaPennyDef`)
  became **exact** (251→261). Base classes (`CThingBaseDef` @ 32 fields, `CTavernGameDef`) remain as
  their own top-level entries; `_meta.base_fields_inlined` records this. Dry-run by default; `--write`.
- **Namespaced UI/dialogue defs** (the 6 formerly "not located"): symbols carry `@N<Namespace>@@`,
  which the discovery regex skipped. `NUISystem`: `CUIDef` (@0x6758f0, 109 fields), `CUIIconsDef`,
  `CUIMiscThingsDef` (133), `CUIStateDef`, `CUILocaleGraphicsDef`; `NSpeechGainManager`:
  `CDialogueLayerDef` (@0x473179). `_CLS_RE` now accepts an optional namespace; `tx_ui_merge.py` merges
  the 6 keyed by the **binary** class name with a `"namespace"` field. Where binary casing differs from
  fable-defs (`CUIDef` vs `CUiDef`) the entry carries a `"fable_defs_class"` alias so `tx_flatten` joins.
  All 6 score **exact**.

### Validation

- **`CChestDef` 6/6** vs fable-defs `chest.rs` — names, types, order all match, **including
  `OpenerObject` at field offset `0x34`**, independently RE-verified (CLAUDE.md chest facts:
  `CChestDef::Transfer` is retail `0x004DE204`; `OpenersRequired` at `+0x38`).
- **`CAbilityDef`** → `Ability : enum EHeroAbility` = fable-defs `ability.rs` (`ability: HeroAbility`).
- **267 / 268 classes** are an exact order-preserving supersequence of fable-defs' `#[def("…")]`
  order. Each class carries `"fable_defs_order_match": "exact" | "partial(...)"`.
- **Residual `COpinionSourceDef`**: all field names present in our order; fable-defs'
  `opinion_source.rs` unrolls two array fields — `#[def("BinaryReaction")]` ~79x and
  `#[def("BinaryOpinion")]` 5x — whereas linear disasm sees the single `Transfer` call inside the
  emitting loop **once** (`BinaryReaction`:bool@off11 loop-temp, `BinaryOpinion`:f32). Flag reads
  `partial(array-unrolled:BinaryReactionx79,BinaryOpinionx5)`. The compressed single entry is the
  faithful Transfer-order record.

### Regenerate

`python tools/transfer_extract/tx_extract.py --all` (writes `scratchpad_out.json`) → move to the
scratchpad as `transfer_orders.json` → `python tools/transfer_extract/tx_finalize.py` (writes
`refs/transfer_field_orders.json`) → `python tools/transfer_extract/tx_ui_merge.py --write` →
`python tools/transfer_extract/tx_flatten.py --write` (inlines base markers + re-scores).
`tx_compare.py` prints the fable-defs agreement report. Needs `debug_build/ego_r.exe` +
`ghidra_out/egor_pdb_names.tsv` + the `fable-defs` clone at
`C:\Users\Cornelio\Documents\EgoCoreInspect\fable-defs`. `pip install capstone`.

Delivery: issue opened https://github.com/jamen/fable-defs/issues/1 (offers the JSON; awaiting the
maintainer's preferred form — gist/PR/file). A bulk gist upload was blocked by the harness data-exfil
guard; deliver the 456KB artifact only with explicit user sign-off.

## Schema coverage and gaps filled

**★★ CLOSED 2026-07-20 — 100.00% clean decode (14,761/14,761, 0 no-schema, 0 dirty).**
Regression guard: `forge defs decode <root> docs/re_reference/def_schema.json --all game.bin`
(exits non-zero while any entry is unresolved/dirty). Mirror `ghidra_out/def_schema.json` (338 types).

**Two distinct metrics — do not conflate them:**

| metric | value | meaning |
|---|---|---|
| **name-resolution** | 249/249 types (100%) | every bin `definition` string maps to *some* schema key (direct, or `C`+CamelCase+`Def`) |
| **clean decode** (the real one) | **100.00%** (was 99.29% with missing2, 86.19% canonical-only) | the resolved schema actually decodes the payload with all field tags present and zero leftover (`forge defs decode --all`) |

Field-level def merge keys off **clean decode**: an entry that doesn't decode clean falls back to
whole-record merge.

### Why there was a gap

CamelCase name-resolution (`resolveType` guesses `C` + CamelCase(CATEGORY) + `Def`) produces a schema
key that *exists* but is the **wrong or merely-base class** for derived Thing-hierarchy types, or names
a class that does not exist. A game.bin entry for a derived type is the **concatenation of its whole
inheritance chain's** Transfer output, so it needs a schema entry with the complete field set, keyed by
the literal definition string (so `resolveType` Candidate 1, `schema.find(binDefinition)`, hits directly).

### Pass 1 — `def_schema_missing.json` (2026-07-19): 53/53 types recovered

Donor FableWin.exe (Ghidra project `FableTLC`); validated against retail `game.bin` (249 types,
14761 entries) via `D:\Code\FableForge\build\forge.exe defs decode` (read-only). All **53** unresolved
types (Gap A = 46, Gap B = 7; **6431** entries) became schema-covered and field-mergeable; each decodes
a real payload CLEAN (`leftover == 0`, all tags in Transfer order) on the largest 3 + smallest entry of
its type: **53/53 clean**. Raised field-merge from 39→43 field-merged records on Aeon+LostContent
(whole-record fallbacks 14→10).

| bucket | types | entries |
|---|---:|---:|
| was covered | 196 | 8330 |
| **recovered this pass** | **53** | **6431** |
| **new total** | **249 / 249** | **14761 / 14761** |

Deliverable `docs/re_reference/def_schema_missing.json` — additive, 59 entries:
- **6 class definitions** freshly decompiled from their `Transfer(CPersistContext&)`: `CUIDef`
  (NUISystem, 109 fields), `CUIMiscThingsDef` (133), `CUIIconsDef` (32), `CUILocaleGraphicsDef` (3),
  `CUIStateDef` (14 — helper sub-component of `CUIDef`), `CCombatAbilityBlockDefBase` (5 fields —
  **OVERRIDE/REPLACEMENT**: key existed in `def_schema.json` with **0 fields**; the original sweep
  couldn't parse its "args-set-up-before-a-bare-call" shape).
- **53 bin-def-type aliases** keyed by the game.bin `definition` STRING (e.g. `"OBJECT"`, `"UI"`,
  `"SPECIAL_ABILITIES_BERSERK_DEF"`, and the 7 Gap-B class names), each copying the resolved donor
  class's ordered field list and carrying `"alias_of": "<DonorClass>"`. No resolver code change needed.
- Supporting: `ghidra_out/decomp_schema_gap.c` (11 Transfer decompiles),
  `ghidra_out/decomp_schema_gap.log` (DecompDefTransfers ok=11 fail=0 + `ReadStr` pass).

Key finding — the class guesses were wrong; almost all of Gap A was a **naming/alias** problem:
- **Thing hierarchy** — `OBJECT`→`CThingObjectDef` (not `CObjectDef`), `BUILDING`→`CThingBuildingDef`,
  `THING`→`CThingBaseDef`, `MARKER`→`CThingMarkerDef`, `SWITCH`→`CThingSwitchDef`,
  `PHYSICAL_SWITCH`→`CThingPhysicalSwitchDef`, `HOLY_SITE`→`CThingHolySiteDef`, `NOISE`→`CThingNoiseDef`,
  `SHOT`→`CThingShotDef`. Their alias field lists **prepend `CThingBaseDef`'s 12 base fields**
  (`fields_include_base: true`) — hence OBJECT decodes 19 fields, SWITCH 13.
- **Special abilities (23 types)** — guess was a **double `Def`** (`CSpecialAbilitiesBerserkDefDef`);
  real class is single-`Def` (`CSpecialAbilitiesBerserkDef`). Pure alias.
- **Others:** `INVENTORY_TYPE`→`CInventoryDef`, `CONTROL_SCHEME`→`CControlsDef`,
  `SOUND_SETUP`→`CSoundDef`, `HERO_MELEE_COMBAT_ABILITY`→`CMeleeCombatAbilityDef`,
  `VILLAGER_INTERACTION`→`CVillagerInteractionsDef`,
  `LOCAL_DETAIL_GENERATOR`→`CEngineLocalDetailGeneratorDef`,
  `MELEE_COMBAT_KNOCKDOWN_EFFECTS`→`CMeleeCombatKnockdownEffects`,
  `CONFIG_OPTIONS_DEFAULTS_DEF`→`CConfigOptionsDefaultsDef`, `COMBAT_DIALOGUE_DEF`→`CCombatDialogueDef`,
  `ENVIRONMENT_THEME_DAY`→`CEnvironmentThemeDaySetDef`.
- **Gap B — combat-ability blocks inherit, they do not define.** The 7 `CCombatAbility*AttackDef`
  classes have no `Transfer` of their own (ctor/Copy/GetSizeofClass only). Block subtypes →
  `CCombatAbilityBlockDefBase` (5 fields: `BlockAngle`, `BlockCanBeBroken`, `ForceRecoilWhenBlocking`,
  `CombatAbility`, `ValidBlockWeaponTypes`): `Unarmed`, `LightWeapon`, `HeavyWeapon`, `ProjectileWeapon`.
  Counter/GetHit subtypes → `CCombatAbilityAttackBase` (3 fields: `Blockable`, `Knockdown`, `Damage`):
  `GetHitCounter`, `BlockCounter`, `FlourishCounter`.
- **`&DAT_` field-name recovery** (`ReadStr` pass): `CCombatDialogueDef` +0x90 = **`Flee`** (int32) —
  the 8-byte leftover that kept COMBAT_DIALOGUE_DEF dirty; `CEnvironmentThemeDaySetDef` leading =
  **`Time`** (10 fields; base schema had 9); `CUIDef`: `Type`, `MinX/MinY/MaxX/MaxY`, `UpX/UpY/UpZ`,
  `Font`; `CUIMiscThingsDef`: `Plus`.
- Lower-confidence-but-structurally-correct: `LOCAL_DETAIL_GENERATOR` / `CEngineLocalDetailGeneratorDef`
  has exactly one field, `Layers` (`vector<CEngineLocalDetailLayerDef>`); element class fully in schema;
  decodes clean on all 65 entries.
- Fold-in for FableForge: merge `def_schema_missing.json`; treat `CCombatAbilityBlockDefBase` as a
  replacement; the 53 string-keyed entries are picked up by Candidate 1 automatically (an explicit
  category→class alias table using `alias_of` would be cleaner than duplicated field lists).

### Pass 2 — `def_schema_missing2.json` (2026-07-19): 99.29%

With `missing2` merged, clean decode = **14,656 / 14,761 = 99.29%** (`forge defs decode <root>
<canonical + missing2> --all game.bin` → "14656 clean (99.29%), 105 no-schema, **0 dirty**"). The 27
types in `missing2` include 16 **field-set replacements** of previously-incomplete types
(CREATURE/CPhysicsDef/CTargetingDef/CWeaponDef derived chains) + 11 new — fold-in is
**update-semantics, not pure-append**. `missing2` was produced by an agent interrupted before
self-validating; the 0-dirty check was run afterward, so it is safe to fold into canonical.

Pre-missing2 gap list (the 86% state, kept for history — types whose derived-class field set was
still incomplete; entry counts are the retail per-type counts):

| bin def type | entries | note |
|---|---:|---|
| `CREATURE` | 517 | matched CThingBaseDef base only; needed full creature class |
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

### Pass 3 — the last 2 types (2026-07-20): 100.00%

`THING_GROUP` (85) + `ENGINE_THEME_GROUP` (20) = 105 entries: both classes' `Transfer` methods are
**EMPTY** (donor decompiles `0191da25`/`0191e286` in `ghidra_out/donor_def_transfers.c` = bare `ret`;
retail stubs `0x0045954D`/`0x00459764`) — they serialize ZERO fields. Every entry is the 5-byte untagged
base-class prefix (`01 00 01 00 00`; NULLDEF all-zero). Since no field tag anchors the prefix
empirically, `def_schema.json` gained an optional per-type `"prefix_len"` (set to 5 on both types +
their class keys) which the FableForge decoder uses directly. FableForge commit `11aabb9`.

## Load contract (append rules)

*RE'd 2026-07-24 via the Claude Workflow decomp loop (`defload-contract-re`): 8 decode agents over the
def-load cluster → synthesis → adversarial verify. Verdict **CONFIRMED** (no refutations). On-disk layout
+ both append bugs are **high confidence** (byte-proven against the retail base); the exact in-memory
per-class resolution structure is **medium** (rel32 call targets are relocation-masked). Source
artifacts: `work/def_load_re/` (bundles, `manifest.json`, `contract_md.txt`, `fix_recipe_md.txt`).*

Solved the standing blocker: a creature def **appended** to game.bin was seen by `forge` but **not**
by the engine (`CreateCreature("CREATURE_MESHY_HUNTER") → nil` + unstable load). The counts were never
the problem — **two payload/CRC bugs** were.

### Runtime resolution flow (opcode-proven structure; masked call targets = inference)

1. `CGameDefinitionManager` ctor `0x0044C6C2` — builds base `CDefinitionManager`, installs vtable
   `0x1232C24`, zeroes 4 derived dwords @ this+0xD0..0xDC, reserves a `0x80000` (512 KiB) def arena.
2. `CGameDefinitionManager::InitAndCompile` `0x0044C72B` — registers def **CLASSES** (className →
   factory-fn-ptr) into `std::map<unsigned_long, CDefClassInfo>` via registrar `0x564395`; map
   **key = `crc0(className)`**. It bakes **138** factory-fn-ptr literals here; the remaining classes
   (→249 total) register in `CStartupDefinitionManager::InitAndCompilePlatformSpecific` `0x004336BC`.
   Binds four game.bin stream readers @ this+0xD0/D4/D8/DC, then runs load/compile drivers
   (`0x190F45`, `0x233285`, `0x564195`) that stream game.bin and populate per-class def arrays.
   *(`std::pair<unsigned_long, CDefinitionManager::CDefClassInfo>` is a real demangled RTTI symbol —
   bundle `0x0047C250` — so the crc-keyed class map is confirmed to exist.)*
3. `CGameScriptInterface::CreateCreature` `0x008A9100` (by-name script path) — loads def-manager
   @ this+0x10, hashes the creature-name string, feeds the u32 to a bool-returning registry query;
   `test al,al; je` → **on miss skips the whole spawn and returns the null `CObjectRef`
   (typetag `0x1238C8C`) → Lua nil**. On hit, `[def+0x11C]` feeds the spawn. *(Control flow & offsets
   opcode-proven; that the hash IS crc0 and the query is crc-keyed rests on masked targets.)*
4. `CGameScriptInterface::CreateCreatureNearby` `0x0089F300` — computes placement, dispatches
   CreateEntity; the def name is resolved by the same by-name machinery in a callee.
5. `CDefinitionManager::GetDef<T>(long id)` `0x00410890` (numeric path) — validates `id > 0`,
   delegates to `0x59D150(id, &out)`; the class def-array is indexed by `indexInDefinition`.
   *(The `id*0x1C` record-stride allocation lives in a separate helper after this fn, likely
   `GetDefsOfType` — mis-attributing the stride to GetDef(id) itself is wrong.)*

### The two append bugs (both must be fixed; either alone still fails)

**BUG #1 — wrong names.bin CRC (fixed in FableForge `bin.cpp`).** `forge`'s `nameCrc` computed
`0xFFFFFFFF - mz_crc32(name)` — matched **0/13593** real names. Existing names round-tripped only
because the writer copies their *loaded* CRCs verbatim; every truly-NEW name got an un-resolvable CRC.
For `CREATURE_MESHY_HUNTER` it wrote `0xE4EACB91`; the correct `crc0` is `0x5A11F1E5`. **Fixed** →
seed-0 reflected CRC-32, no final inversion (verified `bin.cpp` produces `0xAA22BB08`/`0x2E6B63C8`/`0x5A11F1E5`).

**BUG #2 — stale internal global entry-index references (fixed in `02_add_creature.cpp`).** A
byte-for-byte clone of `CREATURE_TRADER_01` keeps `1549` at payload offsets {25, 193, 301}, but the
clone lands at global 14761+, so the engine wires the def to the wrong entries → instability.
**Fix**: rewrite every `u32 == donorGlobalIdx` in the cloned payload to the append landing index
(value-keyed, robust to record layout). **Leave component/sibling refs alone** — `10744`/`10745`/`10746`
and the stride-9 table @314..638 (indices 1175..1307) are shared sub-defs, already valid for a
shared-component clone. (Quest cards carry the same kind of back-refs — see
[QUEST_CARD_BYTE_LAYOUT.md](QUEST_CARD_BYTE_LAYOUT.md).)

**Header counts were never the bug** — the writer already extends `nameCount +1`, `tableSize`,
`entryCount +1`, and recomputes dense `indexInDefinition` (`bin.cpp:243 perDefinition[def]++`), all
verified by diffing base vs forge output (13593→13594, 14761→14763 for a 2-entry card set).

### Track A vs Track B

- **Track A (in-place `Graphic.modelId`, `03_inplace_modelid.cpp`)** — sidesteps BOTH bugs by
  construction: no entry count / order / name-set / new-payload-ref change, only the 4 modelId bytes of
  one existing entry. **Confirmed safe & engine-accepted.** Scope = every instance of the edited
  creature. ⚠ Rebuild from a **pristine** retail base — an install at 14781 (+20 stale appends) may carry
  the very BUG-#1 wrong-CRC names. In-place `setEntryData` field edits are the general form of this track.
- **Track B (append a distinct named creature)** — needs both fixes above **and** a FableForge rebuild
  (`libforgecore.a`) so the append tool + `forge validate` use the corrected `crc0`. For a creature with
  its OWN sub-defs, append the full coordinated set and retarget **every** self+sibling index to its new
  landing slot (see `work/append_fix/APPEND_ALGORITHM.md`).

### EgoCore cross-reference (2026-07-31)

EgoCore independently corroborates the crc0 hash, the crc0-keyed class registry
(`map<u32, DefClassInfo>`), the header layout, and the chunk offset-bias (`nInChunk*2`).
EgoCore's production def path does NOT append — it deletes game.bin/names.bin/frontend.bin/script.bin
and regenerates them by driving `Fable.exe` headless (`dbugst.ini`: `AllowDataGeneration TRUE;
UseCompiledDefs TRUE; BuildRetailStaticMaps TRUE`; one-byte force-patch at IDA VA `0x00C90613+6`).
That is a byte-exact generator to diff appended bins against — the cleanest way to settle the
"which field the engine matches" open question. Do-not-adopt items are under [crc0](#crc0--the-one-hash).

## Definition and header indexes

### Compiled definition index (game.bin / script.bin / frontend.bin)

*Generated from installed `data\CompiledDefs` on 2026-07-18.* Outputs:
`ghidra_out/installed_game/compiled_game_entries.csv`, `compiled_script_entries.csv`,
`compiled_frontend_entries.csv`, `compiled_defs_summary.csv`. Counts are in the
[Overview](#overview) table (game.bin 14,761 / 249 / 8,888; script.bin 611 / 3 / 611;
frontend.bin 810 / 8 / 810).

Chest/key proof points — concrete `game.bin` entries for object definitions that appear in TNG
placement data: `OBJECT_CHEST_OPENABLE_TPL`, `OBJECT_CHEST_OPENABLE`, `OBJECT_SILVERKEY_CHEST_5`,
`OBJECT_SILVERKEY_CHEST_10`, `OBJECT_SILVERKEY_CHEST_25`, `OBJECT_SILVERKEY_CHEST_50`,
`OBJECT_SILVERKEY_CHEST_75`, `OBJECT_SILVERKEY_CHEST_100`, `OBJECT_CHEST_REWARD_ON_DEATH`,
`OBJECT_SILVER_KEY`, `OBJECT_CHOCOLATE_BOX_01`, `OBJECT_CHOCOLATE_BOX_UNGIVEABLE`.

Placement validation: `tools/TestGameplayEntityDefinitions.ps1` joins
`ghidra_out/installed_game/chest_key_reward_entities.csv` against `compiled_game_entries.csv` —
checked gameplay entities: 391; unresolved `DefinitionType` references: 0. Every chest/key/shop/
container-like placement indexed from loose TNGs resolves to a real `game.bin` entry.

Editor meaning: chest/reward editors validate that a placed TNG chest points at a real object;
placement UI offers definitions from compiled data instead of hardcoded lists; FSE quest tooling
validates `Quest:GiveHeroObject(...)` against real objects; Ghidra work uses definition names and
object classes as search terms around inventory/reward code. Implementation: `tools/bin_dump` is a
tiny local .NET wrapper over `SilverChest.Formats.Bin`; it replaced the hardcoded
`ChestDefinitionSearch` executable (hardcoded paths) and reads only from the installed game path.

### Definition header index (`data\Defs\*.h`, `RetailHeaders\*.h`)

*Generated from installed `data\Defs\*.h` and `data\Defs\RetailHeaders\*.h` on 2026-07-18.* Outputs:
`ghidra_out/installed_game/header_enums.csv`, `header_enum_counts.csv`, `header_enum_summary.txt`.
Enum constants: **63,540**; enums: **63**. (UNIFIED_BUILD_INTEGRATION confirms counts match exactly and
RetailHeaders/DevHeaders are byte-identical.)

| Enum | Count | Editor Use |
|---|---:|---|
| `EGameText` | 26,807 | text picker, dialogue/object labels, validation |
| `ELipSync` | 12,134 | dialogue/lipsync browser |
| `ELipSync2` | 5,310 | dialogue/lipsync browser |
| `EEngineGraphic` | 3,683 | mesh/graphic references |
| `EAnimType2` | 3,435 | animation picker |
| `EMeshType2` | 3,401 | mesh picker |
| `ELipSync4` | 2,769 | dialogue/lipsync browser |
| `ETextGroup` | 2,105 | grouped text browser |
| `EParticleEmitter` | 1,165 | effects/particle picker |
| `EFrontEndGraphicBank` | 394 | frontend/UI asset picker |

Editor meaning: asset pickers for meshes, textures, animations, particles, text, lipsync; validation
for TNG `DefinitionType` and CTC fields pointing at enum-backed resources; Ghidra datatype imports and
enum application in decompiled functions; cross-checks between EgoCore BIG entries and retail headers.
The planned "stronger version" (parse names.bin + game.bin and join BIN entries to enum names) is what
the compiled index above and `forge defs decode` now deliver.

## Verified facts

- 2026-07-18 — installed `data\CompiledDefs`: game.bin 14,761 entries / 249 types / 8,888 named;
  script.bin 611/3/611; frontend.bin 810/8/810; 391 TNG chest/key placements resolve, 0 unresolved.
- 2026-07-18 — `data\Defs` headers: 63,540 enum constants across 63 enums (`EGameText` 26,807 largest).
- 2026-07-19 — `def_schema_missing.json`: 53/53 unresolved types decode clean (6431 entries); the
  class-name guesses were alias errors (`OBJECT`→`CThingObjectDef`, single-`Def` special abilities).
- 2026-07-19 — `def_schema_missing2.json`: 14,656/14,761 = 99.29% clean, 0 dirty; 16 field-set
  replacements + 11 new (update-semantics).
- 2026-07-20 — coverage CLOSED 100.00%: `THING_GROUP`/`ENGINE_THEME_GROUP` Transfer = bare `ret`
  (retail `0x0045954D`/`0x00459764`); 5-byte prefix `01 00 01 00 00`; `prefix_len` schema key;
  FableForge `11aabb9`.
- 2026-07-24 — crc0 = reflected CRC-32 poly `0xEDB88320` seed 0 no final inversion
  (`CCharString::ComputeCRC32` `0x00404310`); 13593/13593 names.bin CRCs match, 0 match any other variant.
- 2026-07-24 — game.bin/names.bin headers byte-exact (`0x0007AB1E`, `0xA8E36C34`, 13593 names,
  tableSize 396904, 14761 entries, 12-byte rows, dense `indexInDefinition`, u16 chunk offsets biased
  `nInChunk*2`); CREATURE = 517 entries; `CREATURE_TRADER_01` = global 1549 / indexInDefinition 270.
- 2026-07-24 — append needs BOTH crc0 names.bin CRC (BUG #1, `bin.cpp` fixed) AND self-index retarget
  at payload offsets {25,193,301} (BUG #2, `02_add_creature.cpp`); shared sub-defs 10744/10745/10746
  stay; header counts were never the bug; in-place edits (Track A) engine-accepted.
- 2026-07-24 — class registry `map<unsigned_long, CDefClassInfo>` keyed `crc0(className)`; 138
  factories in `InitAndCompile` `0x0044C72B` + rest in `0x004336BC` = 249; `CreateCreature` `0x008A9100`
  returns null `CObjectRef` (typetag `0x1238C8C`) on registry miss.
- 2026-07-31 — EgoCore corroborates crc0, crc-keyed registry, header layout, chunk bias; its
  `GetCRC` (std-CRC+tolower) and `classIndex=0` are wrong (issue #4, dead code); regenerates defs via
  headless `Fable.exe` (`dbugst.ini` flags, patch `0x00C90613+6`).
- 2026-08-10 — `refs/transfer_field_orders.json`: 268 classes / 5,133 fields from `ego_r.exe`
  Transfer disasm; `CChestDef` 6/6 incl. `OpenerObject` @ `0x34`; sub-component collections via
  `TransferVectorOfSubComponents<T>` (`CArenaRoundDef::Transfer @0xf93320`); exact 211→251.
- 2026-08-11 — base-class flatten (251→261 exact), 6 namespaced UI/dialogue defs recovered
  (`CUIDef` @0x6758f0, `CDialogueLayerDef` @0x473179) → 267/268 exact; `COpinionSourceDef` partial is
  an array-unroll artifact; fable-defs issue #1 opened.

## Open questions

- Which stored field the engine matches at resolve time (the names.bin CRC field, a CRC it recomputes
  from the name at load, or both) was not isolated. Fix #1 makes the appended field consistent with
  `crc0(name)` under either hypothesis. Recommended A/B/C test: append with (a) correct crc0 only,
  (b) index-rewrite only, (c) both — see which flips the nil; EgoCore's headless regeneration gives a
  byte-exact ground-truth oracle for the diff.
- The exact in-memory per-class name→def structure built by the un-decompiled load callees
  (`0x190F45`/`0x233285`/`0x564195`) — whether `indexInDefinition` alone indexes it, or an extra
  per-class count/hash is trusted — is unproven (masked targets).
- game.bin-level integrity/trailer checksum: believed absent, not explicitly ruled out.
- EgoCore parity details to retail-verify: zlib deflate level 1 fed one byte at a time;
  `dependencyCRC = 0xE86E4CDE` after the safeBool byte; per-class `NULLDEF_<name>` strings.
- Field-DEFAULT facts (EGOCORE_ASSESSMENT §B3) are not yet captured here.
- `COpinionSourceDef` array unroll (`BinaryReaction` x79, `BinaryOpinion` x5) would need the loop trip
  count recovered from the struct — optional polish.
- Container round-trip gate for a def WRITER has not been run (CAPABILITY_INDEX: field re-serialize
  proven, container gate pending).

## Superseded

- `DEF_SCHEMA_COVERAGE.md` "clean decode 86.19% canonical-only / 99.29% with missing2" and its
  "Why the 14% gap" / "Decomp handoff" list — superseded by the 100.00% close on 2026-07-20 (table kept
  above for history).
- `DEF_SCHEMA_COVERAGE.md` derived-class guesses (`CREATURE`→`CCreatureDef`,
  `CSpecialAbilitiesBerserkDefDef`, `CObjectDef`) — wrong; real classes are `CThingObjectDef`,
  single-`Def` special abilities, etc. (see Pass 1).
- `DEFINITION_HEADER_INDEX.md` "Next: parse names.bin/game.bin with `SilverChest.Formats.Bin`" and the
  hardcoded `ChestDefinitionSearch` proof — superseded by `tools/bin_dump` and `forge defs decode`.
- `TRANSFER_FIELD_ORDERS.md` resume items (base-chain flatten, 6 UI defs "not located", ship-it) — all
  DONE 2026-08-11; the 6 defs were namespaced, not missing.
- Forge `nameCrc = 0xFFFFFFFF - mz_crc32(name)` — wrong (0/13593); replaced by crc0 in `bin.cpp`.
- "Header counts / entry count are why appended defs fail" — refuted; the two payload/CRC bugs are.
- `CDefinitionManager::GetDef<T>(long id)` owning the `id*0x1C` record stride — mis-attribution; the
  stride lives in a separate helper (likely `GetDefsOfType`).
- EgoCore `CDefStringTable::GetCRC` (std-CRC + `tolower`) and `classIndex = 0` — contradicted by
  byte-proven crc0 + dense `indexInDefinition`; do not re-litigate (issue #4 resolved as dead code).
