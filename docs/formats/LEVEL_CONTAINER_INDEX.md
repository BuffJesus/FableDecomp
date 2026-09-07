# Level Container Index

*Generated from the installed TLC data path on 2026-07-18.*

## Containers

The installed game exposes four complementary views of Albion's level data:

| Container | Indexed Output | Count | Role |
|---|---|---:|---|
| `FinalAlbion.wld` | `ghidra_out/installed_game/wld_maps.csv`, `wld_regions.csv` | 398 maps, 141 regions | world map, region grouping, visibility/loading relationships |
| `FinalAlbion.wad` | `ghidra_out/installed_game/finalalbion_wad_entries.csv` | 398 LEV + 398 TNG | editable level/entity archive view |
| `FinalAlbion_RT.stb` | `ghidra_out/installed_game/finalalbion_stb_entries.csv` | 398 LEV + 26 static sea map entries | runtime level bank view |
| loose `FinalAlbion\*.tng` | `ghidra_out/installed_game/loose_tng_summary.csv` | 397 TNG files | current modding/editing surface |

## Editor Meaning

- The world browser should start from WLD, not directory scanning: it has map coordinates, script
  names, display names, minimaps, and region membership.
- The level browser should reconcile WLD map names against WAD/STB entries. Mismatches are validation
  warnings.
- The terrain/scene view should use STB/WAD LEV extraction plus `SilverChest.LevBridge` exports.
- Entity editing should use loose TNGs by default, with WAD TNGs as the original/reference layer.
- Static sea map entries in STB are runtime-only level bank data and should be visible in diagnostics
  even if not directly editable at first.

## Verified Bridges

- `SilverChest.WadBridge --list` indexes WAD entries without mutating the install.
- `SilverChest.StbBridge --list` indexes STB entries without mutating the install.
- `SilverChest.TngBridge --list` agrees with raw `NewThing` counts on the BarrowFields sample.
- `SilverChest.LevBridge --export-theme-grid` produces a CSV terrain/theme grid from extracted LEV.

## First Validation Queries

1. Every WLD `.lev` map should have a matching WAD LEV and STB LEV.
2. Every loose TNG should map to a WLD level script name or known special case.
3. Every chest/key entity should resolve its `DefinitionType` through compiled definitions.
4. Every LEV opened in the editor should round-trip through the same bridge path used for export.

## Current Consistency Check

`tools/TestLevelContainerConsistency.ps1` writes
`ghidra_out/installed_game/level_container_consistency.csv`.

Current result:

- WLD maps checked: 398.
- Missing WAD LEV: 0.
- Missing WAD TNG: 0.
- Missing STB LEV: 0.
- Missing loose TNG: 1 (`creature_hub`, which still exists in WAD/STB).

## Verified facts (from FINDINGS log)

- **2026-08-21 — ForgeTest teleport fault relocates to CBankFile's packed entry table (2026-08-21).**
  Live pybag/dbgeng capture on retail Fable.exe with the regenerated ForgeTest64 package:
  `Access violation c0000005` at `Fable+0x662a21` (`mov edx, dword ptr [ebp]`), fault VA
  0x00A62A21 inside `CPackedUIntArray::operator[]` (0x00A629F0) — not the historical 0x00BDD1D3.
  Same run logged `[StaticMapProbe] opening 'Data\Levels\FinalAlbion\ForgeTest64.lev'` and
  `GoToMapSlotPrepare: slot 399 -> resolved region 55`.
  - `operator[]`: `bl = bitsPerEntry [this+8]` (0 -> return bias `[this+0xC]`); bit offset =
    bitsPerEntry*index; dword index = >>5; `edx = *(data [this+0] + idx*4)` — no bounds/null check.
  - All 50 direct callers of 0x00A629F0 are `CBankFile` methods (`EntryExists`, `ReadEntryData`,
    `GetMaxEntryIndex`, `CountEntries`, `GetTotalDataSize`, `CreateSymbolMap`, `FindIndexByFilename`,
    `FindEmptyBlockOffset`, `GetAllocatedBlockSize`, `UpdateEntry`, `CompareDataOffset::operator()`,
    `InsertionSort_PackedEntry`, `QuicksortPartition`, heap helpers) => bank-file packed entry table.
  - Offset-order lead: retail index 424/425 `PrisonCells_3.lev` / `__STATIC_MAP_COMMON_HEADER__` at
    597,426,176 -> 597,624,832 (ascending); ours 425/426 `__STATIC_MAP_COMMON_HEADER__` /
    `ForgeTest64.lev` at 598,794,240 -> 597,981,184 (non-monotonic).
  - **Update (same session): WAD alignment ruled out; stack attribution corrected.** All 796 retail
    `FinalAlbion.wad` entries are 2048-aligned (0 exceptions); `forge world install-level` output had
    one misaligned entry (`ForgeTest64.tng`) + misaligned footer; realigning reproduced a bit-identical
    fault (same 0x00A62A21, index -1, 798-entry object). Outer frames 0x008F7219 / 0x008F7245 were
    wrong (no unwind info): 0x008F71A0 = `CMap::InitializePalette`, 0x008F70F0 =
    `CMap::AddThemeDefIndexToPalette`, neither calls `CAFile::ReadVar<int>`. Solid: `bitsPerEntry =
    22`, `22 * 0xFFFFFFFF = 0xFFFFFFEA` => index is -1; entry count `0x31E = 798` = modified WAD;
    immediate caller 0x00A39C2C = `CWADFile::GetLength`. Probe: BP `CBankFile::FindIndexByFilename`
    (0x009CCDF0).
  - **ROOT CAUSE: the WAD header carries two entry counts; forge updated only one (2026-08-21).**
    `CCachedFile::OpenFromFile` first arg is a `CWADFile` with entry index preset; last three opens
    `0129cf8c 12e46640 00000049` (73), `... 00000143` (323), `... 00000000` (ForgeTest64) — index 0 =
    1-based not-found sentinel. Header offset 20 (entryCount) retail `0x31C` = 796 / forge `0x31E` =
    798; offset 24 (second count) retail `0x31C` / forge `0x31C` (stale); footer stats pair
    `(type,count)` `(0, 796)` stale. Then: `CMap::LoadFromFile` never checks the `OpenFromFile`
    return; `CBankFile::GetEntryDataSize` does `dec eax` (1-based) -> -1; `CPackedUIntArray::operator[]`
    dereferences `data + (22 * -1 >> 5) * 4` -> AV at 0x00A62A21.
  - Fixed in `libs/forgecore/src/wad.cpp`: write the count to offsets 20 and 24, bump the per-type
    stats entry, pad every appended payload (and footer) to 2048 bytes. Ruled out (real invariants,
    not the fault): 2048-byte entry alignment (796/796 retail), per-map loose `.tng` (397/397 retail).
    The 32-alignment finding moved the failure past `CEngineLandscapeMap::OpenStaticMap` (25/105 ->
    105/105).
