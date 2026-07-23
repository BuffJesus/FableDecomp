# Fable: The Lost Chapters — Hero live stats inside SAVED_ENTITIES

**Status:** The hero's live editable stats are LOCATED and the primary ones (gold, morality,
age, renown-level) are **CONFIRMED with real save values** and a **working round-trip editor**.
**Date:** 2026-07-19.
**Companion to** `docs/SAVEGAME_FORMAT.md` (container/zlib/CRC), `docs/SAVE_HEADER_MAP.md`
(HEADER/PLAYER tag map). Read those first. This doc closes SAVE_HEADER_MAP.md §4 (the hero
live-stats follow-up: gold / morality / experience / renown).

Seed reminder: **all persist-stream field tags = `CCRC::Calc(seed 0, name, len)`** (reflected
CRC-32, poly 0xEDB88320, no final inversion). Seed 0, not 0xFFFFFFFF. (See SAVE_HEADER_MAP.md §0.)

---

## 0. TL;DR verdict — TAGGED, not positional (but you must know each field's type)

The hero's live stats are **NOT** flat HEADER/PLAYER fields. They live in the hero `CThing`
entity inside the **`SAVED_ENTITIES`** entity graph. That entity graph has THREE nesting layers:

1. **SAVED_ENTITIES section** (inside chunk1) → a small u32 header + a run of **independently
   zlib-compressed spatial "cells"** (one per loaded region/cell). The hero lives in exactly one
   cell (in the sample: the largest, index 10 of 25).
2. **Inside an inflated cell:** a sequence of **entities**, each framed positionally as
   `[instance-name ASCII\0][pad][u64 UID][pad][DEF-name ASCII\0][u32 componentBlockLen][components…]`.
3. **Inside each component:** `[component-class-name ASCII\0][pad][u32 dataLen]` then a
   **tagged field stream** `[u32 seed0-CRC(fieldName)][value]…` — the SAME `[tag][value]`
   shape as the HEADER section, **but the value width is per-field (type-driven), NOT
   self-describing** (a `Money` field is a 4-byte int, a bool field is 1 byte, `Age` is a
   4-byte float). So a save editor finds a stat by (a) walking to the hero entity, (b) walking
   to the right component by class-name, (c) matching the seed-0 CRC tag of the field name, and
   (d) knowing that field's type to read/overwrite the right number of bytes.

So: **tagged at the field level** (recover name→seed0-tag→type per stat), **positional at the
entity/component level** (walk by name). It is emphatically not a flat brute-forceable tag blob.

---

## 1. Locating the hero entity (reproducible)

- **Hero UID** = `PlayerCharacterUID` from the PLAYER section (SAVE_HEADER_MAP.md §3, tag
  `getcrc("PlayerCharacterUID",0)` = `96adc827`, stored LE on disk as `27 c8 ad 96`), a u64. In the samples the hero UID is
  `0xffffff0000003ae3` (Cornelio) / `0xffffff00000000c2` (Test/Default). The UID is the low
  bytes + a `0xffffff` high-word marker; it is written verbatim in the entity header.
- The hero entity is **unique and trivially found**: in the inflated SAVED_ENTITIES cell there
  is exactly ONE `PlayerCreature\0` (instance name) → `[u64 UID]` → `CREATURE_HERO\0` (def
  name), and exactly ONE `CTCHeroStats\0` component. Confirmed: counts = 1/1/1 in the sample.
- Practical locator: inflate the SAVED_ENTITIES cells (§4), find the cell containing
  `CTCHeroStats\0`; that cell is the hero's. The stats are in a ~0x120-byte window right after
  the `CTCHeroStats\0` component header.

### Hero entity framing (bytes, Cornelio/Manual-Save1, cell-relative)
```
50 6c 61 79 65 72 43 72 65 61 74 75 72 65 00   "PlayerCreature\0"   (instance name)
… 00-pad …
e3 3a 00 00 00 ff ff ff                        u64 UID = ffffff0000003ae3
… 00-pad …
43 52 45 41 54 55 52 45 5f 48 45 52 4f 00      "CREATURE_HERO\0"    (def name)
10 00 …                                        u32 component-block framing
43 52 45 41 …                                  first component ("CTCPhysicsControlled", etc.)
…
43 54 43 48 65 72 6f 53 74 61 74 73 00         "CTCHeroStats\0"     (the stats component)
00 00 00 00  46 0c 00 00                        pad + u32 dataLen (0x0c46)
79 d4 a2 79  e3 01 00 00  …                      [Morality tag][value] … tagged field stream
```

The hero entity carries several components in order (seen in the sample):
`CTCVolumeContainmentTracker`, `CTCHitLocations`, then the graphics/physics components
`CTCPhysicsControlled`, `CTCHeroMorph` (appearance), and **`CTCHeroStats`** (the stat store),
plus others. `CTCHeroMorph` and `CTCHeroStats` both carry `Strength/Skill/Will/Age/Morality`
tags — but `CTCHeroMorph`'s are **float 0..1 appearance-morph weights**, whereas
**`CTCHeroStats`'s are the real integer stats.** Do not confuse them; edit the ones in
`CTCHeroStats`.

---

## 2. CTCHeroStats field map — CONFIRMED stats

Field tag = `getcrc(name, 0)`. Values read at the tag+4 offset. Offsets below are into the
inflated hero cell for `Cornelio/Manual-Save1` (`CTCHeroStats` component @ cell-off 0x1600
region); each value cross-checked across 4 saves.

| field name | seed0 tag (LE on disk) | type | Cornelio (played) | Test/Default (new game) | status |
|---|---|---|---|---|---|
| **Morality** | `79a2d479` (`79 d4 a2 79`) | i32 (signed) | **483** | **0** | **CONFIRMED** |
| **Money** (= gold) | `b03ccbfd` (`fd cb 3c b0`) | i32/u32 | **1625** | **0** | **CONFIRMED** |
| **Age** | `663c8f40` (`40 8f 3c 66`) | f32 | **26.4** | **18.0** | **CONFIRMED** |
| **Fatness** | `b0da59fb` (`fb 59 da b0`) | f32 | 0.0 | 0.0 | **CONFIRMED** |
| **RenownLevel** | `0c4ed775` (`75 d7 4e 0c`) | i32 | **2** | **0** | **CONFIRMED** |

Cross-validation logic: Test and Default are fresh new-game heroes → Morality 0, gold 0,
Age 18.0, RenownLevel 0 (canonical starting values). Cornelio (a played hero) → Morality 483
(good-ish), gold 1625, Age 26.4, RenownLevel 2. Both Cornelio saves (Manual-Save1 and
AutoSave.qs, same playthrough) decode to the **identical** values. All sane and in range.

### Experience / renown pools — positionally CONFIRMED, name = [hypothesis]

Between `Money` and `RenownLevel` in the `CTCHeroStats` stream is a run of `[u32 tag][i32 value]`
fields that are the hero's **experience pools** (general + per-stat strength/skill/will and the
renown accumulator). They decode as regular 4-byte ints and clearly differ between a played and
a new-game hero, so their POSITION and TYPE are confirmed, but their exact field *names* were
not recovered this pass (they do not match an obvious wordlist). Observed aligned tags in order:

| tag (seed0) | Cornelio value | Test value | note |
|---|---|---|---|
| `896e293a` | 0 | 0 | [hypothesis] pool/flag |
| `c731e25b` | 3190 (`0x0c76`) | 0 | [hypothesis] an experience pool (0 at new game) |
| `695d2a96` | 1350 (`0x546`) | 1000 (`0x3e8`) | [hypothesis] an experience/starting pool |
| `37c59c98` | 0 | 0 | [hypothesis] |
| `059107c4` | 0 | (2) | [hypothesis] |

In-memory (runtime) the stat-experience array is a 4-long block: `CTCHeroStats+0x118` →
`[general, strength, skill, will]` (from `CTCHeroStats::GetHeroStatExperience`/`AddHeroStatExperience`,
retail `0x0057a…`, index 0/1/2/3). The four serialized experience tags above mirror that array;
matching each tag to strength/skill/will/general by name is the only remaining [hypothesis].

**Health / Physique:** the hero's `Health` (tag `78b48786`, f32 = 80.0) appears in the entity
header block (near the UID/`CREATURE_HERO`), value 80.0 in Cornelio — the creature's current
health, CONFIRMED present but living on the base creature record, not in `CTCHeroStats`.

---

## 3. Evidence (≥2 sources per confirmed stat)

**Decompiled code (retail Fable.exe, base 0x400000):**
- `ghidra_out/decomp_hero_stats_classes.c` — the `CTCHeroStats` accessors that pin the runtime
  layout: `CTCHeroStats::SetMorality` (morality stored at **`this+0x28`**),
  `GetHeroStatExperience`/`AddHeroStatExperience` (stat-exp array at **`this+0x118`**, 4 longs),
  `GetPercentageThroughRenownLevel`/`GetMaxBoastingCrowdForRenownLevel` (renown level at
  **`this+0x70`**, renown value at **`this+0x78`**). `SetMorality` clamps to
  `±*(def+0xd0)` and mirrors morality → `CTCHeroMorph::SetAge` (why `CTCHeroMorph` carries a
  parallel float `Age`/`Morality`).
- `ghidra_out/decomp_save_hero_stats.log` — `LoadEntitiesBinary` (retail `0x00521240`),
  `CThingManager` child-walk (`0x00530460`), Def transfers. Confirms the entity graph is loaded
  from the binary SAVED_ENTITIES stream.

**Real save files (ground truth, byte-exact):** `Test/AutoSave`, `Default/AutoSave`,
`Cornelio/AutoSave.qs`, `Cornelio/Manual - Save1` (under
`%USERPROFILE%\Documents\My Games\Fable\Saves\<Hero>\`). Every confirmed stat's seed-0 CRC tag
sits at the expected place in the hero's `CTCHeroStats` window in all four, decoding to the sane
values in §2. The tag identities are independently proven by `getcrc(name,0)` matching the
on-disk 4 bytes (e.g. `getcrc("Morality",0)=79a2d479`, `getcrc("Money",0)=b03ccbfd`).

---

## 4. SAVED_ENTITIES container layout (for the editor)

Inside the inflated chunk1 stream:
```
"SAVED_ENTITIES\0"
[u32 se_payload_len]          <- compressed byte length of all cells (header→PLAYER). PATCH on edit.
[u32 0][u32 ?][u32 0]…        <- small header of per-cell metadata (u64-ish fields; e.g. the first
                                 cell's uncompressed size 0x37e=894 and compressed size 0x215=533
                                 appear here). Cells are self-framed zlib so the loader can also
                                 walk them by the 78 xx magic.
[cell 0: zlib 78 01 …]        <- inflates to one region's entity records
[cell 1: zlib …]
…                             <- N cells (25 in Cornelio/Manual-Save1)
```
Each cell inflates to a run of entities (§1 framing). Concatenating all inflated cells gives the
full entity graph (383,284 bytes in the sample). The hero is in the cell that contains
`CTCHeroStats\0`.

---

## 5. Save-editor recipe: edit gold / morality (VALIDATED round-trip)

> **★ 2026-07-20 CORRECTION:** step 5 below is INCOMPLETE — each cell is framed by a 36-byte
> descriptor `[u64 0][u64 recLen=29+clen][u64 cellId][u64 clen][u32 ulen]` whose
> `recLen`/`clen`/`ulen` must ALSO be patched on any cell edit (SAVE_ENTITY_GRAPH.md §9.5.1).
> Use `save_edit.py set-gold / set-stat / set-qty / add-item`, which do this correctly.

This composes with `tools/save_edit.py` (which already solves the container: zlib chunks +
the **seed-0 trailer signature** over `file[0:trailer_pos]` — that "signature" open item is
CLOSED; `read_save` verifies it and `build_save` re-signs it). Steps:

1. `sf = save_edit.read_save(raw)` — inflates chunk0||chunk1 and **verifies the trailer sig**.
2. In `sf.payload`, find `SAVED_ENTITIES\0`; walk the zlib cells between it and `PLAYER\0`,
   inflating each; pick the cell containing `CTCHeroStats\0` (the hero cell).
3. In the inflated hero cell, find `CTCHeroStats\0`, then within the next ~0x120 bytes find the
   seed-0 tag (`getcrc("Money",0)` for gold, `getcrc("Morality",0)` for morality) and overwrite
   the 4-byte value at tag+4 (Money = u32 gold; Morality = **signed** i32, negative = evil).
4. Re-deflate the edited cell (`zlib.compress(cell, 9)` → `78 DA`; the loader accepts it).
5. Splice the new compressed cell back into `sf.payload` in place of the old one. The
   uncompressed cell size is unchanged (fixed-width int edit) but the **compressed** size shifts,
   so patch the `SAVED_ENTITIES [u32 se_payload_len]` field (`+0x00`) by the compressed delta.
6. `new_c1_ulen = sf.chunk1_ulen + compressed_delta` (SAVED_ENTITIES lives in chunk1's
   *uncompressed* stream, so its length tracks the spliced compressed bytes). chunk0 is unchanged.
7. `out = save_edit.build_save(sf.chunk0_ulen, new_c1_ulen, new_payload)` — re-deflates chunks,
   fixes header dwords, **recomputes the seed-0 trailer signature**, pads to 300 KB.
8. Re-read with `save_edit.read_save(out)` to confirm the trailer re-verifies.

**Validated:** the PoC `hero_edit_test.py` did exactly this on a COPY of `Cornelio/Manual-Save1`,
setting gold 1625→999999 and morality 483→-1000. The rebuilt 307,200-byte file re-read cleanly,
the **trailer signature re-verified** (new sig `cf9b3395`), PLAYER/QUESTS/REGIONS/FACTIONS and
`PlayerCharacterUID` stayed intact, and the hero cell re-decoded to the edited values.
(Whether the retail loader *enforces* the trailer sig on load is still the one live-fire unknown
per SAVEGAME_FORMAT.md §8, but the editor recomputes it correctly regardless.)

> Note on `save_edit.py` seeds: that file's `get_crc` default seed is `0xFFFFFFFF`, but the
> confirmed field-tag seed is **0** (SAVE_HEADER_MAP.md §0). When locating hero-stat tags use
> `getcrc(name, 0)` (as in `tools/save_tools/savecrc.py`), not `save_edit.get_crc`. The trailer
> signature `save_edit.sig_crc` (seed 0) is correct and unaffected.

---

## 6. Reproduce

```
# inflate a save, get chunk0/chunk1
python tools/save_tools/savecrc.py inflate "<save file>" <outdir>

# compute a hero-stat tag (seed 0):
python -c "import sys;sys.path.insert(0,'tools/save_tools');from savecrc import getcrc;print('%08x'%getcrc('Money',0))"
#   Money=b03ccbfd  Morality=79a2d479  Age=663c8f40  RenownLevel=0c4ed775

# container read/verify + rebuild pipeline (trailer sig included):
python tools/save_edit.py "<save file>"        # prints "trailer sig=… VERIFIED"
```

Scratchpad artifacts used this pass (not committed): inflated `*.entities.bin`, the cell
inflater, cross-validator, and the validated round-trip editor `hero_edit_test.py`.
