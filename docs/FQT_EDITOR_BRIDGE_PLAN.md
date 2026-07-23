# FQT / Editor Bridge Plan

*Last updated: 2026-07-18 while `FableWin.exe` donor analysis is in flight.*

## Current read

`FableWin.exe` is not just a larger symbol donor. Its PDB exposes Lionhead's in-house
editor layer: `CEditComponent`, `CEditControlCentre`, `CEditWorldMap`,
`CEditLevelMerger`, editor GUI classes, save/load paths, TNG parsing, LEV merge
conflict handling, region graph tooling, navigation/mapwho debug commands, and
compiled-definition setup.

That makes it the best ground truth for the same practical problem space covered by
ChocolateBox, SilverChest, and FQT:

- ChocolateBox: community GUI/workflow reference for region and level editing.
- SilverChest / `SilverChest.WadBridge.exe`: current FQT bridge for WAD list/extract.
- FQT: quest authoring tool that needs reliable entity, region, chest, def, and level
  metadata.
- FableWin editor symbols: native engine/editor semantics for how Lionhead loaded,
  modified, saved, merged, and validated that data.

## FQT gaps confirmed locally

FQT already has useful scaffolding:

- `D:\Code\FQT\FQT\FableQuestTool\Services\WadBridgeClient.cs` wraps
  `SilverChest.WadBridge.exe` for `--list`, `--extract`, and `--extract-all`.
- `D:\Code\FQT\FQT\FableQuestTool\Services\TngParser.cs` parses loose text TNG files
  into scriptable entities.
- `D:\Code\FQT\FQT\FableQuestTool\Services\GameDataCatalogService.cs` uses the WAD
  bridge first, then scans loose TNGs and BIG archives.
- `D:\Code\FQT\FQT\FableQuestTool\Services\LevelDataService.cs` can enumerate BIG
  archives and loose level metadata.

The important gap is explicit in FQT:

- `GameDataCatalogService.cs` logs that vanilla TNG data is in LEV/WAD data and that
  TNG extraction from LEV files requires FableMod libraries and is not implemented.
- `WorldMapCoordinateService.cs` currently uses hand-authored map offsets and notes
  that actual world bounds could come from LEV files later.

So SilverChest currently solves "get files out of the WAD", not "understand the
native level/editor model." The FableWin editor build is what can close that second
gap.

## High-value FableWin symbol clusters

Already extracted reports:

- `ghidra_out/fablewin_editor_symbols.tsv`
- `ghidra_out/fablewin_editor_symbols.md`
- `docs/EDITOR_TOOLING_NOTES.md`
- `docs/TOOLING_INTEGRATION_MATRIX.md`

The most valuable clusters for FQT/ChocolateBox/SilverChest work:

- Level/world map editing:
  - `CEditControlCentre::SaveLevel`
  - `CEditControlCentre::SaveAllLevels`
  - `CEditControlCentre::CreateAndSaveNewLevel`
  - `CEditWorldMap::LoadSingleLevelFromFiles`
  - `CEditWorldMap::SaveSingleLevel`
  - `CEditWorldMap::SaveToFile`
  - `CEditWorldMap::LoadFromFile`
  - `CEditWorldMap::CreateNewRegion`
  - `CEditWorldMap::DeleteRegion`
  - `CEditWorldMap::EditSaveRegionsToString`
- TNG/LEV merge:
  - `CEditLevelMerger::LoadLevFile`
  - `CEditLevelMerger::ParseTngFile`
  - `CEditLevelMerger::GetConflicts`
  - `CEditLevelMerger::ResolveNextFile`
- Region and navigation tooling:
  - `ConsoleGenerateRegionGraph`
  - `ConsoleLoadRegionGraphData`
  - `ConsoleSetDrawNavigationMeshes`
  - `ConsoleSetDrawMapwhoInfo`
- Definitions:
  - `CDefinitionManager::SetCompiledDefDirectory`
  - `CDefinitionManager::SetUsingCompiledDefs`
  - `CDefinitionManager::SetCreatingCompiledDefs`
  - `CGameDefinitionManager::InitAndCompile`
  - `CMainGameComponent::ValidateDefinitions`
- Chests and quest-relevant interactables:
  - `CScriptThing::MsgOpenedChest`
  - inventory item interfaces
  - chest/key/item definition and persistence helpers

## Practical task list while BSim waits

1. Keep the Ghidra watcher alive and avoid duplicate project-locking jobs.
2. Turn the current broad editor symbol TSV into a smaller "FQT targets" TSV:
   level load/save, TNG/LEV merge, region graph, definitions, chests/items.
3. Build a cross-reference table from FQT service gaps to FableWin symbol clusters.
4. For each FQT gap, mark the likely source of truth:
   - native editor symbol
   - SilverChest bridge behavior
   - ChocolateBox workflow
   - EgoCore data-format implementation
5. After FableWin import saves and names are applied, use Ghidra to inspect the
   decompiler output for the first targets:
   - `CEditLevelMerger::ParseTngFile`
   - `CEditLevelMerger::LoadLevFile`
   - `CEditWorldMap::LoadSingleLevelFromFiles`
   - `CEditWorldMap::SaveSingleLevel`
   - `CScriptThing::MsgOpenedChest`
6. Feed confirmed structures and constants back into FQT:
   - replace heuristic region coordinate data where LEV bounds can be recovered
   - improve vanilla entity extraction from WAD/LEV/TNG
   - make chest/reward selection rely on real entity/definition data
   - document which ChocolateBox/SilverChest behaviors are native-compatible

## Working conclusion

ChocolateBox and SilverChest are not alternatives to the FableWin debug donor. They
are complementary references:

- ChocolateBox tells us what workflows modders already expect.
- SilverChest gives FQT a usable WAD bridge today.
- FableWin tells us what Lionhead's own editor actually did.

The best path is to keep SilverChest as the extraction bridge for now, then use
FableWin's editor symbols and decompiled code to replace FQT's heuristic LEV/TNG,
region, chest, and definition logic with engine-backed behavior.
