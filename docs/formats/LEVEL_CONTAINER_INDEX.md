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
