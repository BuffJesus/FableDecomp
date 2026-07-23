# Tooling integration matrix

Generated during the FableWin donor-prep wait. This records how the old community tools and the
new debug/editor donor fit together.

## Roles

| Tool/source | What it is | What it gives us | Best next use |
|---|---|---|---|
| `FableWin.exe` + `FableWin.pdb` | Lionhead debug/editor build from Fable Anniversary symbols | Internal engine/editor names: `CEditComponent`, `CEditControlCentre`, `CEditWorldMap`, `CEditLevelMerger`, `ParseTngFile`, `LoadLevFile`, `SaveSingleLevel`, region graph, navmesh/mapwho, compiled defs | BSim donor for retail `Fable.exe`; source of semantics for level authoring and extraction |
| ChocolateBox | Community Fable TLC editor lineage | Practical workflows for region/level edits, FMP packaging, definitions, FinalAlbion WAD/loose-level editing | Treat as GUI/workflow reference; use FableWin symbols to recover native meaning behind its operations |
| SilverChest / `SilverChest.WadBridge.exe` | Current FQT-bundled WAD bridge | Lists/extracts `FinalAlbion.wad` entries so FQT can scan TNGs | Keep as bridge for now; replace or extend with engine-informed LEV/TNG/WAD logic as symbols are understood |
| FQT | User's active quest editor over FSE | Downstream consumer: quest generation, chest rewards, entity browser, TNG scanning, FinalAlbion.qst deployment | Feed it extracted entity/region/chest/def data from this RE project |
| EgoCore | Local completed data-format RE codebase | Headless parsers/extractors for Fable archive/resource formats, including WAD/defs | Use as data-format implementation reference beside FableWin's engine/editor symbols |

## High-value FableWin editor clusters

See `docs/EDITOR_TOOLING_NOTES.md` and `ghidra_out/fablewin_editor_symbols.md`.

- Editor activation/mode switching: `GConsoleSetEditor`, `CMainGameComponent::IsEditorActive`,
  `CEditComponent::SetAsEditingLevel`, `CEditComponent::SetAsEditingWorld`.
- Level/world map editing: `CEditWorldMap`, `CEditControlCentre::SaveLevel`,
  `CEditControlCentre::SaveAllLevels`, `CEditWorldMap::LoadSingleLevelFromFiles`,
  `CEditWorldMap::SaveSingleLevelToFiles`.
- TNG/LEV merge path: `CEditLevelMerger::LoadLevFile`, `CEditLevelMerger::ParseTngFile`,
  `CEditLevelMerger::GetConflicts`, `CEditLevelMerger::ResolveNextFile`.
- Region/navigation tooling: `ConsoleGenerateRegionGraph`, `ConsoleLoadRegionGraphData`,
  `ConsoleSetDrawNavigationMeshes`, `ConsoleSetDrawMapwhoInfo`.
- Definitions: `CDefinitionManager::SetCompiledDefDirectory`, `SetUsingCompiledDefs`,
  `SetCreatingCompiledDefs`, `CGameDefinitionManager::InitAndCompile`,
  `CMainGameComponent::ValidateDefinitions`.
- Chests/items: `CScriptThing::MsgOpenedChest`, inventory item interfaces, chest-related definitions
  and reward/container names.

## Practical conclusion

Do not think of ChocolateBox/SilverChest as alternatives to the FableWin donor. They are complementary:

- ChocolateBox shows user-facing editing workflows and historical edge cases.
- SilverChest/FQT shows what our current tooling already needs: WAD extraction, TNG parsing, entity
  cataloging, chest reward/entity support.
- FableWin gives the names for Lionhead's own editor implementation of those same operations.

That makes the debug/editor donor valuable even when names do not port directly to retail code:
editor-only symbols document file semantics, conflict resolution, save/load paths, and data model
relationships that can improve FQT and future native modding tools.
