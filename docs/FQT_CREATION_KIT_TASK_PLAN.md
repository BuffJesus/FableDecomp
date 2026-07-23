# FQT Creation Kit Task Plan

*Grounded in installed-game indexes generated on 2026-07-18.*

## Principle

FQT should become the shell, but it should not own every format parser. The practical architecture is:

- FQT UI and project model.
- SilverChest bridges for WAD/STB/TNG/LEV operations.
- EgoCore for BIG/resource extraction and compilation.
- FSE for runtime probes and deploy/test loops.
- FableWin/Ghidra for native behavior validation.

## Milestone 1: Read-Only Albion Browser

Inputs:

- `ghidra_out/installed_game/fqt_modding_catalog.json`
- `wld_maps.csv`
- `wld_regions.csv`
- `finalalbion_wad_entries.csv`
- `finalalbion_stb_entries.csv`
- `loose_tng_summary.csv`
- `level_container_consistency.csv`

Tasks:

1. Add an installed-game indexing command in FQT that runs the project-local exporters.
2. Add a world map/region tree from WLD regions and map coordinates.
3. Add level detail view showing WLD map, WAD LEV/TNG, STB LEV, loose TNG, and consistency status.
4. Add diagnostics panel for the one known special case: `creature_hub` has WAD/STB data but no loose TNG.
5. Add open/extract actions routed through `SilverChest.WadBridge` and `SilverChest.StbBridge`.

Acceptance:

- All 398 WLD maps are visible.
- All WAD/STB presence checks match `level_container_consistency.csv`.
- No Steam install mutation is performed.

## Milestone 2: Entity and Chest/Key Browser

Inputs:

- `chest_key_reward_entities.csv`
- `gameplay_entity_definition_validation.csv`
- `compiled_game_entries.csv`
- `header_enums.csv`

Tasks:

1. Show placed chest/key/shop/container entities grouped by level.
2. Display UID and PositionX/Y/Z for each placement.
3. Resolve each `DefinitionType` to `game.bin` index/definition class.
4. Add filters for normal chests, silver-key chests, silver keys, prison key racks, and shops/traders.
5. Add read-only hex/ASCII preview from `compiled_game_entries.csv` until structure decoding is implemented.

Acceptance:

- All 391 indexed gameplay entities resolve against `game.bin`.
- Silver-key chest tiers 5/10/25/50/75/100 are discoverable.
- `OBJECT_CHOCOLATE_BOX_01` is available in the object definition picker.

## Milestone 3: Safe TNG Mutation

Inputs:

- `SilverChest.TngBridge`
- `TngBridgeClient.cs`
- loose TNG files under installed `data\Levels\FinalAlbion`

Tasks:

1. Wrap `TryListThings`, `TryListThingVariables`, and `TryUpdateThingVariable`.
2. Wrap `TryUpdateThingPosition`.
3. Wrap `TryDuplicateThing`, `TryCreateThing`, `TryDeleteThing`, and `TryMoveThingSection`.
4. Require backup/staging output by default; direct install writes need an explicit profile setting.
5. Validate post-write by re-running bridge list and project consistency checks.

Acceptance:

- Move a test entity in a workspace copy, re-list it, and see unchanged entity count.
- Duplicate/delete round trip works on a copied TNG.
- No direct edits happen in the Steam install unless explicitly enabled.

## Milestone 4: LEV Terrain/Scene Preview

Inputs:

- `SilverChest.LevBridge`
- extracted LEV samples under `work/level_samples`
- `FinalAlbion_RT.stb` and `FinalAlbion.wad`

Tasks:

1. Add LEV extract action from WAD/STB.
2. Export theme grid for selected LEV.
3. Render a simple 2D height/theme preview from `*_theme_grid.csv`.
4. Add region and WLD metadata beside the preview.
5. Compare WAD LEV and STB LEV metadata for selected levels.

Acceptance:

- BarrowFields opens from indexed data and renders a theme/height preview.
- Large levels such as HeroGuildComplexInside remain responsive.

## Milestone 5: Asset Browser

Inputs:

- `big_entries.csv`
- `big_banks.csv`
- `big_entry_types.csv`
- EgoCore `--list`, `--batch`, `--extract-textures`, `--extract-audio`, `--extract-particles`

Tasks:

1. Add searchable BIG entry browser grouped by bank and EgoCore type name.
2. Add mesh/animation export command to workspace cache.
3. Add texture extraction/preview command for selected entries.
4. Add text/dialogue lookup using header enums and text/dialogue BIG entries.
5. Add mod package staging references, not raw install overwrites.

Acceptance:

- Graphics, textures, text, dialogue, fonts, effects, and shaders are all browsable.
- Exports land under workspace/cache paths.

## Milestone 6: Native Behavior Oracle

Inputs:

- `fablewin_fqt_editor_targets.tsv`
- BSim-renamed retail `Fable.exe`
- FSE labels

Tasks:

1. Inspect FableWin `CEditLevelMerger::ParseTngFile`.
2. Inspect `CEditLevelMerger::LoadLevFile`.
3. Inspect `CEditWorldMap::LoadSingleLevelFromFiles` and `SaveSingleLevel`.
4. Inspect `CScriptThing::MsgOpenedChest` and related inventory/reward helpers.
5. Turn confirmed behavior into validation rules and structure decoders in FQT/SilverChest.

Acceptance:

- At least one editor workflow has a native-behavior-backed validator, not just parser round-trip.
