# Fable: The Lost Chapters — Save HEADER field map (tag → name → type)

**Status:** HEADER section CRACKED and validated. 23/23 HEADER fields CONFIRMED + 2/2 PLAYER
fields CONFIRMED against 4 real saves.
**Date:** 2026-07-19.
**Companion to** `docs/SAVEGAME_FORMAT.md` (container/CRC/zlib spec) — read that first for the
`FableSave!` file layout, zlib chunk chain, and the persist-stream `[section][field]` framing.
This doc closes SAVEGAME_FORMAT.md **Open Item #2** (the HEADER field roster / tag→name map — the
save-editor payload).

Tools produced: `tools/save_tools/savecrc.py` (GetCRC + inflater), `tools/save_tools/decode_header.py`
(typed HEADER decoder). Decomp logs: `ghidra_out/decomp_save_header_pass{1..5}.log`,
`ghidra_out/decomp_save_hero_entity.c`, `ghidra_out/decomp_save_header_body.c`.

---

## 0. ★ CRITICAL CORRECTION to SAVEGAME_FORMAT.md: the tag seed is **0**, not 0xFFFFFFFF

SAVEGAME_FORMAT.md §4.2/§6 states the save-stream field tag is `GetCRC` with **seed 0xFFFFFFFF**.
**Empirically that is wrong.** All 23 HEADER field tags and both PLAYER tags in 4 real saves match
`CCRC::Calc(seed=**0**, name, len)` — i.e. the **same seed as game.bin**, NOT 0xFFFFFFFF.

Proof (first four HEADER tags, ground-truth bytes vs. computed):

| field | on-disk tag (LE) | Calc(seed=0) | Calc(seed=0xFFFFFFFF) |
|---|---|---|---|
| `WorldName`          | `22 4f de a8` = a8de4f22 | **a8de4f22 ✓** | b128a473 ✗ |
| `WorldFrame`         | `a0 ca 0f 5b` = 5b0fcaa0 | **5b0fcaa0 ✓** | 477a5d29 ✗ |
| `TeleportingEnabled` | `a7 e6 24 d3` = d324e6a7 | **d324e6a7 ✓** | 4bc0d615 ✗ |
| `SavingEnabled`      | `20 62 75 70` = 70756220 | **70756220 ✓** | 80fedb5d ✗ |

The `0xffffffff` seen all over `CheckCRC`/`GetCRC` in the donor decompile is the **`CCharString`
constructor's length argument (`-1` = "call strlen")**, not a CRC seed — that is the source of the
confusion in SAVEGAME_FORMAT.md. The algorithm is otherwise exactly as documented: reflected CRC-32,
poly 0xEDB88320, **no final inversion**, table at donor `0x0449BE30`
(`ghidra_out/crc_table_full.txt`). Net effect: **save tags and game.bin tags use the identical
`Calc(0, name)`.** (The three-way "seed table" in SAVEGAME_FORMAT.md §6 collapses: save = game.bin =
seed 0.)

`GetCRC` computes over the raw ASCII field name, no NUL terminator.

---

## 1. Where these come from (evidence, ≥2 sources per field)

- **Writer (authoritative order + type):** `CWorld::SaveGameStateInternal` retail `0x0049F4C0`,
  HEADER block decompiled in `ghidra_out/decomp_savegame_retail2.log` lines 171–341. Field order =
  `Transfer<T>(ctx,"Name",…)` call order; `T` = on-disk value type.
- **PLAYER section writer:** `CPlayerManager::SaveGameState` donor `0x01805EF8` / retail `0x00445A20`
  (`ghidra_out/decomp_save_hero_entity.c` lines 441–567) — writes exactly two fields.
- **Ground truth:** 4 inflated real saves — `Test/AutoSave`, `Default/AutoSave`,
  `Cornelio/Manual - Save1`, `Cornelio/AutoSave.qs` (under
  `%USERPROFILE%\Documents\My Games\Fable\Saves\<Hero>\`). Every field's `Calc(0,name)` tag is present
  at the expected byte offset in call order in **all four** saves, and the typed decoder consumes the
  HEADER section byte-exactly (parsed_to == section end, 0 bytes remaining) in all four.

---

## 2. HEADER section — full tag → name → type map (all CONFIRMED)

Section framing on disk: `"HEADER\0" [u32 sectionByteLen] [field]...`, each field `[u32 tag][value]`.
Offsets below are into the **inflated** HEADER section of `Cornelio/Manual - Save1` (chunk0), for
orientation; every tag is confirmed in all 4 saves.

| # | tag (hex) | field name | Transfer\<T\> | on-disk type | infl.off | notes |
|--:|---|---|---|---|---|---|
| 0 | `a8de4f22` | **WorldName** | IntelPtr\<CThing\> | ASCII cstr, NUL-term | +0x00b | world/level name → `"FinalAlbion"` |
| 1 | `5b0fcaa0` | **WorldFrame** | signed_char→long | u32 LE | +0x01b | frame/tick counter (740 / 195 / 138230 / 139834) |
| 2 | `d324e6a7` | TeleportingEnabled | bool | 1 byte | +0x023 | |
| 3 | `70756220` | SavingEnabled | bool | 1 byte | +0x028 | |
| 4 | `6a28abd2` | ExperienceSpendingEnabled | bool | 1 byte | +0x02d | |
| 5 | `3f6e5950` | CreatureGenerationEnabled | bool | 1 byte | +0x032 | |
| 6 | `5899107a` | CreatureGenerationDisabledGroups | long | u32 LE | +0x037 | bitmask (0 in all samples) |
| 7 | `452dddb2` | HeroSleepingEnabled | bool | 1 byte | +0x03f | 0 early / 1 later saves |
| 8 | `2409e2a4` | MapTableShowQuestCardsOnUsed | bool | 1 byte | +0x044 | |
| 9 | `1bfb95b6` | MiniMapEnabled | bool | 1 byte | +0x049 | |
| 10 | `4c7c1d48` | MiniMapActiveBeforeDisabled | bool | 1 byte | +0x04e | |
| 11 | `ad260f50` | GuildMasterMessagesEnabled | bool | 1 byte | +0x053 | |
| 12 | `82818b2c` | SummonerDeathExplosionAffectsHero | bool | 1 byte | +0x058 | |
| 13 | `3831a50f` | MostRecentSaveType | signed_char→long | u32 LE | +0x05d | enum ESaveType (1/3 seen) |
| 14 | `6c9212bd` | MostRecentSaveTypeBeforeManualSave | signed_char→long | u32 LE | +0x065 | enum (2/3 seen) |
| 15 | `9a72a8db` | MostRecentManualSaveName | IntelPtr\<CThing\> (CWideString) | UTF-16LE cstr, NUL-term | +0x06d | `"Manual - Save1"`; empty in autosaves |
| 16 | `80926404` | SaveGameMarkerPos | CFloatRange | 3× float LE (12 B) | +0x08f | world XYZ of save marker |
| 17 | `aedf95f2` | SaveGameMarkerAngleXY | signed_char→float | float LE (4 B) | +0x09f | facing angle |
| 18 | `34b73c37` | GuildSealRecallPos | CFloatRange | 3× float LE (12 B) | +0x0a7 | 0,0,0 in samples |
| 19 | `0a111ac7` | GuildSealRecallAngleXY | signed_char→float | float LE (4 B) | +0x0b7 | |
| 20 | `fd9cf30d` | CurrentRegionName | IntelPtr\<CThing\> | ASCII cstr | +0x0bf | `"TXT_REGION_GREATWOOD_..."` |
| 21 | `eb6d5050` | CurrentRegionMinimapGraphicName | IntelPtr\<CThing\> | ASCII cstr | +0x0e1 | `"MINIMAP_GREATWOOD..."` |
| 22 | `45242440` | TotalTimePlayed | (double→) | 4 B LE | +0x0f7 | seconds played; last HEADER field |

Notes:
- Fields 2–22 are only written when `this[0x102] == 0` (i.e. not a "reload minimal" context) — that
  branch is taken by all normal player saves, so all appear in real saves.
- `IntelPtr<CThing>` names in the writer resolve to a plain NUL-terminated string on disk (the
  object's name), exactly as SAVEGAME_FORMAT.md §4.3 states.
- `TotalTimePlayed` is passed from a `double` in the writer but occupies 4 bytes here; the section
  consumes exactly, so treat it as a 4-byte trailing value (float 0.0 in the 4 samples; the true play
  time may be tracked elsewhere / written as the low dword).

### Decoded sample values (proof the map is usable)

`Cornelio/Manual - Save1`: WorldName=`FinalAlbion`, WorldFrame=138230, MostRecentManualSaveName=
`Manual - Save1`, SaveGameMarkerPos=(3114.79, 3000.x, z), CurrentRegionName=
`TXT_REGION_GREATWOOD_TELEPORT`, CurrentRegionMinimapGraphicName=`MINIMAP_GREATWOOD`.
`Test/AutoSave`: WorldName=`FinalAlbion`, WorldFrame=740, CurrentRegionName=`TXT_REGION_OAKVALE`,
save name empty. Run `python tools/save_tools/decode_header.py` for the full aligned table.

---

## 3. PLAYER section — 2 fields CONFIRMED (chunk1 body)

`CWorld::SaveGameStateInternal` opens sections in chunk1 in this order:
`ENTITIES` → `SAVED_ENTITIES` (via `CThingManager::SaveGameState`) → then, when normal save:
`PLAYER` → `QUESTS` → `REGIONS` → `FACTIONS` → text markers.

`CPlayerManager::SaveGameState` (donor `0x01805EF8`) writes **only two tagged fields**:

| tag (hex) | field name | Transfer\<T\> | type | confirmed at body offset |
|---|---|---|---|---|
| `96adc827` | **PlayerCharacterUID** | Transfer\<_K\> | u64 (8 B LE) | Test @0x29ee, Manual @0x12b27, AutoQs @0x129d7 |
| `9a7e84f7` | **PlayerRegionName** | Transfer\<CCharString\> | ASCII cstr | immediately after (e.g. `"Greatwood"`) |

`PlayerCharacterUID` is the **UID of the hero CThing** — the pointer to where the hero's live
gold/morality/experience actually live (see §4). Both confirmed present in all 3 parsed body streams.

---

## 4. Hero live stats (Gold / Morality / Experience) and Quest flags — where they are

The player-facing **live** hero stats are **NOT** flat tagged fields in PLAYER/HEADER. They are
serialized inside the **`SAVED_ENTITIES`** entity graph (chunk1, written by
`CThingManager::SaveGameState`, retail `0x00530490`), as the hero **CThing entity's** own persist
blob (the entity referenced by `PlayerCharacterUID`). That entity blob is a binary component graph
(`[type][UID][component data]…`), not a name/tag field stream you can brute-force by field name — so
the hero's gold/morality/experience field *names* are **[hypothesis / not yet recovered]** this pass.

Relevant Transfer classes located (retail addrs) for the follow-up decompile of the hero entity blob:
- `CThingManager::SaveGameState` `0x00530490` — writes SAVED_ENTITIES (entry point to the hero blob).
- `CGoldDef::Transfer` `0x004F6DBD`, `CExperienceDef::Transfer` `0x00506A30`,
  `CHeroExperienceDef::Transfer` `0x0058A900`, `CHeroStatsDef::Transfer` `0x004873BF`,
  `CCreatureStatsDef::Transfer` `0x00457202` — these are the **DEF** (game.bin) structures; they hold
  design-time thresholds/multipliers (e.g. `ExperienceLevelThresholds`, `WillExperience`,
  `SkillExperience`, `StrengthExperience`, `MoralityChange` — from `donor_def_transfers.c`), not the
  hero's live totals. The live totals mirror these categories on the runtime hero CThing.
- `CTCHeroStats` `0x0056F241` is the UI controller (GuiVarTransferStruct), not the persist path.

**Quests / Factions are TEXT, not tagged binary.** The QUESTS section (`CQuestManager::SaveGameState`
text writer, donor `0x01819106`) emits literal markers `START_QUEST_CARDS` / `END_QUEST_CARDS` /
`START_ACTIVE_QUESTS` / `START_SAVED_QUESTS` and quest names (`Q_HobbeCave`, `Q_TraderEscort`,
`CS_OakValeRevisited`, `COMPLETED`, `SCRIPT_DATA`…) as plain text — confirmed in the body-stream
string dump (offsets 0x12b41+). FACTIONS is likewise text (`Name "FACTION_HERO";` …). So quest
completion state is editable as **text sub-scripts**, not via a CRC tag. REGIONS is a plain list of
region names (LookoutPoint, BowerstoneSlums, StartOakVale, …).

Open follow-up (one focused pass): decompile `CThingManager::SaveGameState` → the hero CThing's
entity Transfer to recover the live Gold/Morality/GeneralExperience/Strength/Skill/Will/Renown field
names, then confirm their `Calc(0,name)` tags inside the hero blob region of SAVED_ENTITIES.

---

## 5. Save-editor recipe

To build a TLC save editor (or a `forge save` subcommand):

1. **Read container** (SAVEGAME_FORMAT.md §3): parse `FableSave!` + 16-byte header
   (`+0x0C` signature, `+0x10` chunk0 ulen, `+0x14` chunk1 ulen, `+0x18` chunk0 clen), inflate
   chunk0 (HEADER) and chunk1 (ENTITIES/SAVED_ENTITIES/PLAYER/QUESTS/…) with zlib (`78 DA`).
   `tools/save_tools/savecrc.py inflate <file>` does this.
2. **Locate a field:** walk `"section\0" [u32 len]` then `[u32 tag][value]`; a field is the one whose
   `tag == Calc(seed=0, name)`. Types per §2/§3 and SAVEGAME_FORMAT.md §4.3.
3. **Edit common cases:**
   - **World/level:** `WorldName` (HEADER) — string.
   - **Where the "save here" marker is:** `SaveGameMarkerPos` (3 floats) + `SaveGameMarkerAngleXY`.
   - **Current region label:** `CurrentRegionName` / `CurrentRegionMinimapGraphicName` (strings).
   - **Feature toggles:** the bool block (Teleporting/Saving/MiniMap/ExperienceSpending…Enabled).
   - **Which hero:** `PlayerCharacterUID` (PLAYER) points at the hero entity.
   - **Gold / Morality / Experience:** inside the hero entity in SAVED_ENTITIES — *not yet a named
     tag*; needs the §4 follow-up before it is safe to edit by field.
   - **Quest completion:** edit the TEXT quest sub-scripts in the QUESTS section (`COMPLETED` etc.).
4. **Write back:** re-serialise, **re-deflate each section** (zlib best-compression `78 DA`), fix the
   header dwords (chunk0/chunk1 ulen, chunk0 clen, and the chunk1 compressed-length framing dword),
   pad to the fixed **300 KB (0x4B000)** slab.

### Guard rails (from SAVEGAME_FORMAT.md §8)
- **Signature / `+0x0C` dword** [hypothesis]: governed by `UseSaveGameSignatures`
  (donor/retail string `0x0402F21C`). Per-save, per-playthrough (e.g. Test/AutoSave `0x000036A2`,
  Manual-Save1 `0x00013365`). Likely a CRC/hash over the payload (same `CCRC`/`GetCRC` family; the
  16-byte trailer after the last chunk is the likely material). If the retail loader enforces it, an
  edited save must recompute it — or flip `UseSaveGameSignatures` off via console/config. **Exact hash
  input still unconfirmed — treat as the primary editing obstacle.**
- All observed saves are exactly **307200 bytes**; keep the 300 KB slab + zero padding.

---

## 6. Reproduce

```
python tools/save_tools/savecrc.py inflate "<save file>" <outdir>   # → *.header.bin, *.body.bin
python tools/save_tools/decode_header.py                            # aligned typed HEADER table
python tools/save_tools/savecrc.py crc WorldName WorldFrame ...      # compute Calc(0xFFFFFFFF) — note
#   for SAVE tags use seed 0: savecrc.getcrc(name, 0)  (see §0)
```
