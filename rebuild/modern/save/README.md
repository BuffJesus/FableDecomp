# Save files (`.sav`, `Profile.bin`)

Portable C++23 codec for the retail `FableSave!` container: the fork-side piece of
MODERNIZATION step 2 ("retail saves load; a save round-trips"). It decompresses and recompresses
the two persist-stream sections, enforces the retail load gates, and decodes HEADER, the body's
section list, PLAYER, ENTITIES down to the inflated SAVED_ENTITIES cells and SAVED_NPC_NAMES,
REGIONS, and the QUESTS and FACTIONS persist text (LZO). The entity streams inside the cells
stay opaque bytes.

## Evidence boundary

- Layout and gates: [SAVE.md](../../../docs/formats/SAVE.md) "Container and header layout" and
  "Signature enforcement". The decoder accepts exactly what retail
  `CUserProfileManager::VerifySignature` (0x00409730) accepts: the magic, `total_data_len + 4 <=
  size`, the seed-0 `CCRC::Calc` trailer, size exactly 0x4B000, all-zero padding. It then applies the
  chunk framing `CWorld::LoadGameStateInternal` (0x004A21F0) reads.
- Compression: retail links zlib **1.2.1** (`deflate 1.2.1 Copyright` in `Fable.exe`), driven by
  `CDataOutputStream::WriteZlibCompressed` (0x009967E0). Level 9, 15-bit window, memLevel 8 and the
  default strategy reproduce retail's compressed bytes. That holds with upstream 1.2.1 and with
  classic zlib 1.3.1. **zlib-ng does not reproduce them** (Python's `zlib` here is
  `1.3.1.zlib-ng`), which is why SAVE.md §2.4 originally concluded byte identity was unreachable.
- Text blobs: QUESTS and FACTIONS hold persist text compressed by `lzo1x_999_compress`
  (0x00C08130, called from the blob writer 0x00996610 on all but the last 3 bytes, which are
  stored raw). Upstream LZO 2.10 reproduces 8,698 of the corpus's 8,700 blobs (288 of 290 QUESTS,
  all 8,410 FACTIONS records) byte for byte. The other two are QUESTS streams our Python editor
  wrote in store mode.

## Licensing (LZO)

This tree is MIT. LZO is GPL-2.0-or-later, so it is never committed here. The decoder in
`../codec` holds the tree's own LZO1X decoder and an MIT LZO1X encoder ported from FableForge.
Without upstream LZO, the writer compresses text blobs with that encoder: a valid stream the
retail reader accepts, but not retail's exact bytes. It falls back to retail's stored form
(`i16 0` then the raw text) when compression does not help. All 8,698 corpus text blobs
re-encode with it and decode back to the same text. Configure with
`-DFABLE_SAVE_WITH_LZO=ON` to download upstream LZO 2.10 (SHA-256 pinned) or build it from
`FABLE_LZO_SOURCE_DIR`, and get byte-exact re-encoding. A binary built that way is a combined
work under the GPL, so whoever distributes it must also offer its source. Either way, everyone
gets the full source and can modify it.

## Verified

`fable_save_check` over every `FableSave!` file in the local save folder (409 files, 2026-10-17),
built x86 and x64:

- 402 **exact**: the container, the typed HEADER, the body's section list, the typed PLAYER
  section, ENTITIES (all 1,921 cells inflated and re-deflated at level 1, plus the NPC-name map)
  and the QUESTS/FACTIONS text all decode, and every layer re-encodes byte for byte (the text
  blobs only with LZO linked; without it they must decode back to the same text) (`encode(decode(file)) == file`,
  all 307,200 bytes). This is every save the game itself wrote.
- 2 **sections**: valid at every layer, but recompressed by our Python tools (zlib-ng, and a
  store-mode LZO stream in QUESTS), so the compressed bytes differ.
- 5 **malformed**, all backups made by our Python save editors. Three have body sections that
  don't tile the stream (`body:decode_error`). Two have ENTITIES one byte short, so the next
  section reads as `LAYER` (`body:unexpected_sections`).

All 153 `Profile.bin` files (`FablePro`) also decode and re-encode byte for byte
(`profile:exact`). The same results come from MSVC x86, MSVC x64 and GCC 15 builds, with and
without LZO.

Body shapes in the corpus: 291 full (game clock + ENTITIES, PLAYER, QUESTS, REGIONS, FACTIONS)
and 113 minimal (`.qs.hs`, ENTITIES only), matching the writer's two branches.

`fable_save_tests` covers the CRC check value, layout, determinism, each retail gate, and the
HEADER round trip, including the minimal-context form and the tag check. It also covers a
hand-assembled LZO1X stream and the text-blob framing (empty, short, stored, and over 0x7FFF).

## Layers

- `SaveContainer.hpp`: `FableSave!` container, zlib sections, retail gates, `ccrc`.
- `PersistStream.hpp`: binary `CPersistContext` primitives. Each field is `[u32 field_tag(name)]`
  then its value. The tag is checked the way retail `CheckCRC` asserts. Widths are pinned:
  `long` → `int32_t`, `CWideString` → UTF-16 `char16_t`, UIDs → `uint64_t`, `CFloatRange`/vectors
  → 3 × `float`.
- `SaveHeader.hpp`: the HEADER section (SAVE.md §3.4).
- `SaveBody.hpp`: the body. It holds the game clock (`Time`, `TimeStep`, `DayCount`) and the
  named sections (`name\0`, i32 length, bytes; `CPersistContext::StartSection`/`EndSection`
  0x009BAB60/0x009BABF0), plus the typed `PlayerSection` (SAVE.md §3.5).
- `Entities.hpp`: ENTITIES (head, SAVED_ENTITIES, SAVED_NPC_NAMES) and the SAVED_ENTITIES slot
  table: empty slots, cell records (zlib level 1) and the dead-entity table (SAVE.md §3.6, the
  2026-10-17 grammar). The head is the format version (2) and `CThingManager::CurrentUniqueID`.
  The tail is `PersistedDeadEntities`: the UID of each killed game-persistent entity, plus the
  seed-0 CRC of the quest script it belonged to. `SavedNpcNames` is `CThingManager::NPCNameMap` (UID → group, name; writer 0x00524440).
  An empty group is written as `UNATTACHED`. The section is empty in the minimal `.qs.hs` form.
- `Regions.hpp`: REGIONS, the region manager's `vector<CRegion>` (SaveGameState 0x004FDE80).
  For elements 1 onwards it stores `Name` and the `AppearOnWorldMap`, `CreatureGenerationEnabled`
  and `SoundThemesEnabled` flags. Every field is `Transfer("")`, so every tag is 0. All 290
  REGIONS sections re-encode exactly.
- `TextSections.hpp`: the LZ text blob (`read_lz_text`/`write_lz_text`; its `clen` is a
  signed i16, so above 0x7FFF it is -1 followed by a u32), `QuestsSection` (text plus the accepted
  boasts: `NumAcceptedBoasts`, then `BoastScriptName<i>`, `BoastId<i>`, `BoastFailed<i>` and
  `BoastCompleted<i>`) and `FactionsSection` (`NumberOfFactions`, then a `Faction<i>` text each).
  Every field tag in the save is now named.
- `Profile.hpp`: `Profile.bin` (`FablePro`, text-mode persist script). Entries are kept in
  order with their raw values; `save_game_names()` gives the 50-slot manual-save registry.
  `set_value` plus `encode` re-signs the file. Retail `VerifySignature` also has a
  0x4000-size / 0x20-pad branch for this magic, but no real profile satisfies it (all are
  variable-length and unpadded). The decoder applies the CRC rule the files actually follow.

## Build

```
cmake -S rebuild/modern -B build -G Ninja             # find_package(ZLIB), else pinned zlib 1.3.1
cmake -S rebuild/modern -B build -DFABLE_ZLIB_SOURCE_DIR=<classic zlib tree>   # offline
cmake -S rebuild/modern -B build -DFABLE_SAVE_WITH_LZO=ON                       # byte-exact text blobs (GPL build)
cmake --build build && ctest --test-dir build
build/save/fable_save_check "<Documents>/My Games/Fable/Saves"
```

## Intentional modernization

- Sections are `std::vector<std::byte>` and every on-disk field is an explicit little-endian
  `u32`, so nothing depends on pointer width or on the retail struct images.
- Oversized output is an error (`too_large`), not a silently truncated file.

## Not covered

- Typed profile settings (values stay raw text; `CActionInputControl(...)` bindings unparsed).
- The entity and component records inside each cell (SAVE.md "Entity graph"). Those are the
  reconstructed engine's own `Transfer` code, not a separate model.
- A typed model of the quest and faction persist text (it stays one string per blob).
- A size-0 text blob is written as a bare `u32 0`, as the writer does. No save in the corpus has
  one, so how the reader handles it is unchecked.
