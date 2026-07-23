# Native STB write path (forge::stb) — scoped follow-up

Status (2026-07-20): **SCOPED, not built.** The ForgeTest name-registration this writer would
emit is already produced by the Python assembler `work/newlevel_experiment/assemble_forgetest_stage2.py`
and is present on disk in `work/smoke_test/stage/data/Levels/FinalAlbion_RT.stb` (verified live:
`forge stb list` → 425 entries, **399 static maps**, entry 426 = `Data\Levels\FinalAlbion\ForgeTest.lev`
@597919744, 132632 B). The task here is to let **forge own** that emission instead of the one-off
Python, on formats forge already parses. It is mechanical (~300–500 LOC), needs **no new RE**, and is
**not** on the render critical path (see "Ceiling").

## Confirmed this pass (closes prior open questions)
- **Common-header KEY form is the FULL path**, not the bare map name. Dumped the live
  `__STATIC_MAP_COMMON_HEADER__` (294216 B) from the staged STB: the ForgeTest key is
  `Data\Levels\FinalAlbion\ForgeTest.lev`, identical in form to all 398 sibling keys
  (e.g. `Data\Levels\FinalAlbion\BanditCamp_Filler_05.lev`,
  `Data\Levels\FinalAlbion\NorthernWastes3_Filler_02.lev`). A native writer MUST key by this
  full-path form. (This resolves the "bare name vs full path" blocker the investigation flagged.)
- **The ForgeTest chunk is structurally clean and forge round-trips it**: `forge stbvalidate`
  on the extracted chunk → 46 segments, 26/26 frames decode-exact, quadtree 4/4 wired, S1
  reserialize==raw. So the payload half of a write path is already proven byte-exact via
  `forge::stbbake`.
- **No theme field in CStaticMapInfoBlock** (0x5C): the InfoBlock carries no per-map theme index.
  There is nothing to register for themes. `BuildThemes` reads the global ENGINE_THEME table shared
  by every map; the per-map LEV theme table is by-NAME and degrades to walk-default. **The
  "theme-by-index registration" half of the original goal has no target surface** — do not build it.

## What the writer emits (all already prototyped in the Python assemblers; port them)
Add `forge::stb::Archive::save()` (or a free `emit()`), read-only today in `stb.hpp/stb.cpp`.
Re-serialize: `BBBB` magic + header + entry table + payloads, with a mutated entry list and a
rewritten `__STATIC_MAP_COMMON_HEADER__`, preserving byte-exact every untouched entry and rebasing
only shifted-chunk offsets.

1. **Entry table**: append `Entry{ id=next-free (426), type=0, name="Data\Levels\FinalAlbion\ForgeTest.lev",
   devName=<donor clone>, size=132632, offset=<page-aligned placement>, crc=<container rule> }`.
   Payload = the ForgeTest chunk (byte-exact via `forge::stbbake`; donor Darkwood9_Leadout_01 clone
   at ForgeTest coords). Align to `alignment_` (2048).
2. **Common header** (`__STATIC_MAP_COMMON_HEADER__`, the pos_lookup_map): `[u32 count][ (nameNUL)(u32 relOffset) ]*`.
   Bump count 398→399; append `{ name="Data\Levels\FinalAlbion\ForgeTest.lev" (FULL path — confirmed
   above), relOffset = absoluteChunkOffset - common->offset }`. The header grows ~+43 B, so every
   chunk after the old tail shifts: either relocate the old-tail chunk to the file tail and rebase, or
   (cleaner) always append the new chunk and rewrite the whole header, re-deriving every
   `StaticMap.absoluteOffset = common->offset + relOffset`.
3. **Chunk position fields** (per new/moved chunk): rebase exactly 5 absolute-offset dwords
   (invariant verified 398/398): `+0x20`→landscape-subheader (==chunk+0x5C), `+0x24`→localdetail-subheader
   (==chunk+0x71), `+0x58`→chunk-end, `+0x5C`→landscape dword0 (==chunk+0x70), `+0x71`→detail dword0
   (==chunk+0x7D). All other bytes are entry-relative or content — verbatim.
4. **CStaticMapInfoBlock** (0x5C, byte-exact via `work/stb_baker/stb_infoblock_baker.py`):
   VersionID, BankFileIndex(=426), MapWidth/Height, WorldX=2784/WorldY=2560, Landscape/LocalDetail/
   EdgeHeight/Checksum ptrs (absolute), Quality, ShorePoint, LevelChecksum, CameraMapBounds, HeaderEndPtr.
   **No theme field.**
5. **Themes**: nothing to emit.

## Acceptance gates (the Python assembler already passes these)
- Every untouched payload byte-identical at the same offset.
- All 399 chunks re-parse with the 5-field invariant.
- Every WLD LevelName resolves.
- `forge stb list` reads **399 static maps**, entry 426 = ForgeTest @ correct offset/size.
- `forge stbvalidate` on the emitted ForgeTest chunk → S1/S2/S7 PASS (as the staged chunk does now).

## Ceiling — do NOT claim this renders the hill
Name-registration fixes the **mandatory-STB-entry CRASH** (already fixed on disk). Per
`NATIVE_TERRAIN_STATUS.md`, the terrain may **still white out** via a shared-bank-state mechanism at
map-open (`CreateNewBankMap` @0x009e0360 / `CBankFile::FindIndexBySymbol`/`AddEntry`) that is
explicitly **asserted-not-proven**. That residual is **not** a registration forge can emit; it needs
the map-open shared-bank decomp spike or the editor bake to prove/cure. Native registration = own-it
(essentially shipped as data); **guaranteed render is NOT achievable by registration alone.**

Cross-refs: `libs/forgecore/src/stb.cpp` (read path to mirror), `work/newlevel_experiment/assemble_forgetest_stage2.py`,
`work/stb_baker/stb_infoblock_baker.py`, `docs/NATIVE_TERRAIN_STATUS.md`, `docs/BAKE_FORGETEST_TERRAIN.md`.
