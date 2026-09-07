# Local ChocolateBox / SilverChest Sources

*Last updated: 2026-07-18.*

## Source locations

Modern source tree:

- `D:\Code\Fable\ChocolateBox\ChocolateBox\src\SilverChest.Modern`
- `D:\Code\Fable\ChocolateBox\ChocolateBox\src\SilverChest.WadBridge`
- `D:\Code\Fable\ChocolateBox\ChocolateBox\src\SilverChest.TngBridge`
- `D:\Code\Fable\ChocolateBox\ChocolateBox\src\SilverChest.LevBridge`
- `D:\Code\Fable\ChocolateBox\ChocolateBox\src\SilverChest.ModPackageBridge`

Older copied tree also exists:

- `D:\Code\Fable\ChocolateBox\ChocolateBox - Copy`

FQT currently vendors only the WAD bridge binary:

- `D:\Code\FQT\FQT\FableQuestTool\tools\SilverChest.WadBridge.exe`

## What each local bridge does

`SilverChest.WadBridge` wraps `FableMod.BBB`:

- `--list <wadPath>`
- `--extract <wadPath> <index> <outputPath>`
- `--extract-all <wadPath> <outputDir>`

FQT already calls this through `FableQuestTool\Services\WadBridgeClient.cs`.

`SilverChest.TngBridge` wraps `FableMod.TNG` and `FableMod.ContentManagement`:

- `--list <tngPath>`
- `--update <tngPath> <index> <name> <uid> [definitionsPath]`
- `--update-props <tngPath> <index> <name> <uid> <definitionType> <graphicOverride> [definitionsPath]`
- `--duplicate <tngPath> <index> <name> <uid> [definitionsPath]`
- `--update-position <tngPath> <index> <x> <y> <z> [definitionsPath]`
- `--list-vars <tngPath> <index> [definitionsPath]`
- `--update-var <tngPath> <index> <ctcIndex> <varName> <value> [definitionsPath]`
- `--apply-ctc <tngPath> <index> <ctcName> [definitionsPath]`
- `--add-createtc <tngPath> <index> <ctcName> [definitionsPath]`
- `--apply-def-graphic <tngPath> <definitionName> <graphic> [definitionsPath]`
- `--delete <tngPath> <index> [definitionsPath]`
- `--move-section <tngPath> <index> <sectionIndex> [definitionsPath]`
- `--import-thing <targetTng> <sourceTng> <sourceIndex> <sectionIndex> <name> <uid> [definitionsPath]`
- `--create <tngPath> <sectionIndex> <name> <uid> <definitionType> [definitionsPath]`

`SilverChest.Modern\IO\TngBridgeClient.cs` already has C# wrappers for those operations:

- `TryListThings`
- `TryListThingVariables`
- `TryUpdateThingVariable`
- `TryApplyThingCtc`
- `TryAddCreateTcEntry`
- `TryUpdateThing`
- `TryCreateThing`
- `TryDuplicateThing`
- `TryDeleteThing`
- `TryMoveThingSection`
- `TryImportThing`
- `TryUpdateThingProperties`
- `TryApplyDefinitionGraphicOverride`
- `TryUpdateThingPosition`

`SilverChest.LevBridge` wraps `FableMod.LEV` and graphics/content helpers:

- `--export-obj <levPath> <outputObj> [baseDir]`
- `--export-splat <levPath> <outPng>`
- `--export-themes <levPath> <outDir> [baseDir]`
- `--export-theme-grid <levPath> <outCsv>`

`SilverChest.Modern\IO\LevBridgeClient.cs` currently wraps the OBJ export path.

## Why this matters for FQT

FQT's active code has its own lightweight `TngParser`, but the ChocolateBox/SilverChest tree already
contains a richer TNG bridge with edit operations and definition-aware loading. FQT can reuse or port
that bridge instead of growing a second partial TNG implementation.

FQT currently extracts `FinalAlbion.wad` and then scans loose TNG files. It still lacks native-level
understanding of LEV/world-map semantics, region bounds, and editor save/load behavior. The local
SilverChest LEV bridge can help with terrain/preview/export, while `FableWin.exe`'s editor symbols
identify Lionhead's real logic for:

- `CEditWorldMap::LoadSingleLevelFromFiles`
- `CEditWorldMap::SaveSingleLevel`
- `CEditWorldMap::LoadFromFile`
- `CEditWorldMap::SaveToFile`
- `CEditLevelMerger::LoadLevFile`
- `CEditLevelMerger::ParseTngFile`

That makes the practical path:

1. Keep FQT's `WadBridgeClient` for WAD extraction.
2. Add or port the existing `TngBridgeClient` where FQT needs real TNG editing.
3. Add a narrow `LevBridgeClient` surface for LEV-derived bounds/terrain metadata.
4. Use FableWin decompiler output to validate bridge behavior against Lionhead's editor behavior.
5. Use EgoCore for independent archive/format confirmation where bridge behavior is unclear.

## Immediate follow-up targets

- Compare FQT's lightweight `TngParser` output against `SilverChest.TngBridge --list` on the same
  sample TNGs.
- Check whether FQT can vendor the source projects or should keep bridge binaries as tool-process
  dependencies.
- After FableWin names are applied, inspect `CEditLevelMerger::ParseTngFile` and compare its parse
  model against `FableMod.TNG`/`SilverChest.TngBridge`.
- Use `SilverChest.LevBridge --export-theme-grid` on known LEV files to replace FQT's hard-coded
  region/world-map heuristics with file-derived data.

## Quick loose-TNG check

Sample:

- `D:\Code\FQT\SourceFilesToReference\FSE\FSEBeta\data\Levels\FinalAlbion\BarrowFields.tng`

Result:

- Raw `NewThing` block count: 270.
- `SilverChest.TngBridge --list ... tngdefinitions.xml`: 270 entries.

That suggests FQT's lightweight loose-TNG scanner is probably good enough for entity discovery on
plain text TNGs. The higher-value SilverChest integration is not basic listing; it is definition-aware
editing and validation:

- list variables and CTCs
- update variables
- update positions
- apply/add CTCs
- create, duplicate, import, move, and delete things
- apply definition graphic overrides

So the FQT priority should be:

1. Keep the current simple parser for fast catalog/search.
2. Add `TngBridgeClient` for mutating operations and validation.
3. Add `LevBridgeClient` for LEV-derived terrain/region metadata.
4. Use FableWin editor decompilation to validate where FableMod/SilverChest behavior diverges from
   Lionhead's native editor behavior.
