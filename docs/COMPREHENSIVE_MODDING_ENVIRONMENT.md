# Comprehensive Fable TLC Modding Environment

*Last updated: 2026-07-18.*

## Goal

Build two products that reinforce each other:

1. A comprehensive Fable TLC modding environment, closer to a Creation Kit than a loose bundle of
   one-off tools.
2. A decompiled, named, documented source reconstruction of the retail PC game.

The modding environment should use the decompilation to become more correct over time. The
decompilation should use the modding tools and real game data as fixtures, probes, and validation
oracles.

## Installed game baseline

Game root:

- `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters`

Generated inventory:

- `ghidra_out/installed_game/summary.txt`
- `ghidra_out/installed_game/files.csv`
- `ghidra_out/installed_game/extensions.csv`
- `docs/INSTALLED_GAME_ASSET_REPORT.md`
- `docs/WORLD_AND_CHEST_INDEX.md`
- `ghidra_out/installed_game/fqt_modding_catalog.json`

Current facts:

- `Fable.exe`: present.
- `FableScriptExtender.dll`: present.
- `FSE\`: present.
- `data\Levels\FinalAlbion.wad`: present.
- `data\Levels\FinalAlbion\`: present.
- loose TNG files: 397.
- loose LEV files: 0.
- `FinalAlbion.wad` entries: 796 total, 398 `.lev` + 398 `.tng`.
- `FinalAlbion_RT.stb` entries: 424 total, 398 `.lev` + 26 static sea map bank entries.
- BIG entries indexed through EgoCore: 57,128 across 8 banks.
- `FinalAlbion.wld`: 398 maps and 141 regions.
- loose TNG entity scan: 21,764 things, 22,083 `ScriptName` entries.
- chest/key/reward/shop/container index: 391 entities, including 148 chest, 16 silver-key, and 16
  key entries.
- retail header enum constants indexed: 63,540 across 63 enums.
- compiled definitions indexed: `game.bin` 14,761 entries, `script.bin` 611 entries,
  `frontend.bin` 810 entries.
- gameplay entity definition validation: 391 checked, 0 unresolved.
- native gameplay target symbols from FableWin: 31,541 total, including 161 chest, 1,186 reward,
  4,260 inventory, and 5,898 definition-related symbols.

Interpretation: this install already supports loose TNG workflows, but the LEV terrain/level data is
still WAD-backed. A serious editor must treat WAD extraction, loose TNG edits, and LEV-derived map
metadata as separate but coordinated layers.

## Tool roles

### FableWin debug build

Role: native editor and engine semantics.

`FableWin.exe` + PDB is the answer key for Lionhead's own editor behavior. It gives us internal names
for:

- `CEditComponent`
- `CEditControlCentre`
- `CEditWorldMap`
- `CEditLevelMerger`
- TNG parsing
- LEV loading
- world-map save/load
- region graph/navmesh/mapwho tooling
- compiled definition setup
- chest/item/inventory behavior

This is the source of truth for "what did Lionhead's editor actually do?"

### EgoCore

Role: data-format ground truth and asset compiler/extractor.

Location:

- `C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master`

Validated command:

- `EgoCore.exe --list "...\data\graphics\graphics.big"` wrote
  `ghidra_out/egocore_graphics_big_list.tsv` with 8,112 entries.

Useful headless commands from `EgoCore\main.cpp`:

- `--list <bank.big>`
- `--batch <bank.big> <mesh> [<anim>] <out.gltf>`
- `--batch-many <bank.big> <plan.tsv>`
- `--batch-ue <bank.big> <mesh> [<anim>] <out.gltf>`
- `--batch-many-ue <bank.big> <plan.tsv>`
- `--extract-textures <bank.big> <outdir>`
- `--extract-audio <bank.lug> <outdir> [filter]`
- `--extract-particles <effects.big> <out.json>`

Important safety note: EgoCore GUI WAD setup can remove `FinalAlbion.wad` and patch `userst.ini`.
For this project, use non-destructive extraction into workspace/cache directories unless explicitly
doing an install mutation.

### ChocolateBox / SilverChest

Role: existing editor workflow and bridge processes.

Local source:

- `D:\Code\Fable\ChocolateBox\ChocolateBox\src\SilverChest.Modern`
- `D:\Code\Fable\ChocolateBox\ChocolateBox\src\SilverChest.WadBridge`
- `D:\Code\Fable\ChocolateBox\ChocolateBox\src\SilverChest.TngBridge`
- `D:\Code\Fable\ChocolateBox\ChocolateBox\src\SilverChest.LevBridge`

Validated:

- `SilverChest.WadBridge --list FinalAlbion.wad`: 398 LEV + 398 TNG.
- `SilverChest.WadBridge --extract ... 86/87`: extracted BarrowFields LEV/TNG into
  `work/level_samples/BarrowFields`.
- `SilverChest.TngBridge --list BarrowFields.tng`: 270 things, matching raw `NewThing` count.
- `SilverChest.LevBridge --export-theme-grid BarrowFields.lev`: wrote
  `work/level_samples/BarrowFields/BarrowFields_theme_grid.csv`.

This makes SilverChest a practical bridge layer for FQT today, while FableWin and EgoCore validate
whether the bridge behavior matches engine/editor reality.

### FQT

Role: user-facing quest/editing surface over FSE.

Current strengths:

- FSE Lua generation.
- project validation.
- quest templates.
- entity discovery from loose TNGs.
- WAD extraction through `SilverChest.WadBridge.exe`.

Main gaps to close:

- use `SilverChest.TngBridge` for definition-aware TNG mutations, not just text scanning.
- use `SilverChest.LevBridge` or EgoCore-derived data for LEV bounds/terrain/region metadata.
- import real game definitions and asset indexes from EgoCore instead of hand-curated lists.
- feed decompiled native names and FSE labels into its API/reference UI.

### FSE

Role: runtime scripting, live probes, and deploy path.

FSE is the safe runtime layer for fast experimentation. It should remain the primary way to validate
quest behavior, engine calls, live entity lookup, and mod deployment while deeper native hooks mature.

## Architecture

### Data index layer

Build a generated workspace database from the installed game:

- files and extensions from `tools/InventoryInstalledGame.ps1`.
- WAD table of contents from `SilverChest.WadBridge`.
- STB table of contents from `SilverChest.StbBridge`.
- BIG bank entries from EgoCore `--list`.
- texture dumps from EgoCore `--extract-textures`.
- audio manifests from EgoCore `--extract-audio`.
- particle JSON from EgoCore `--extract-particles`.
- TNG entity catalogs from loose files and `SilverChest.TngBridge`.
- LEV terrain metadata from `SilverChest.LevBridge --export-theme-grid`.
- compiled definitions and symbol headers from EgoCore/FableDefCompiler outputs.
- generated retail headers from `data\Defs` for enum-backed pickers and Ghidra datatype work.
- compiled definition entries from `CompiledDefs\names.bin` + `game.bin`/`script.bin`/`frontend.bin`.

This layer should be read-only by default and regenerated from the install path.

Current bootstrap catalog:

- `ghidra_out/installed_game/fqt_modding_catalog.json` is the machine-readable handoff for FQT.
- It references the BIG/WAD/WLD/TNG indexes, FableWin editor targets, and the first editor modules:
  world browser, level scene view, asset browser, chest/reward editor, and decompilation oracle.

### Authoring layer

This is the Creation Kit surface:

- region browser
- level browser
- entity browser
- TNG thing editor
- chest/reward editor
- quest graph/Lua editor
- definition editor
- mesh/texture/audio browser
- mod package builder
- install profile manager

FQT can become this shell, but it should call bridge processes or shared libraries for format-specific
work rather than reimplementing every parser.

### Validation layer

Every mutation should have checks before it reaches the game:

- TNG parse/list round trip.
- UID uniqueness checks.
- definition existence checks.
- graphic override existence checks against BIG indexes.
- region and level existence checks against WAD/WLD/STB indexes.
- FSE API reference validation for generated Lua.
- optional launch/test profile using FSE logs.

### Native RE layer

This is the decompilation track:

- Continue BSim donor expansion from `ego_r` and `FableWin`.
- Use FableWin editor symbols as the first deep-dive target set.
- Use EgoCore source as the expected behavior for resource loaders.
- Use FSE source and live logs for runtime addresses and call signatures.
- Gradually produce subsystem source in `docs/FULL_DECOMP.md` terms.

The first native systems to decompile for modding payoff:

1. TNG/LEV/editor save-load:
   - `CEditLevelMerger::ParseTngFile`
   - `CEditLevelMerger::LoadLevFile`
   - `CEditWorldMap::LoadSingleLevelFromFiles`
   - `CEditWorldMap::SaveSingleLevel`
2. definitions:
   - `CDefinitionManager::SetCompiledDefDirectory`
   - `CGameDefinitionManager::InitAndCompile`
   - `CMainGameComponent::ValidateDefinitions`
3. chests/items:
   - `CScriptThing::MsgOpenedChest`
   - inventory and reward helpers
4. region graph/nav:
   - `ConsoleGenerateRegionGraph`
   - `ConsoleLoadRegionGraphData`
   - mapwho/navmesh draw toggles

## Immediate build plan

1. Treat the FableWin donor pipeline as complete baseline: retail `Fable.exe` is 80.14% named.
2. Generate and refresh stable installed-game indexes under `ghidra_out/installed_game`.
3. Generate WAD TOC and sample LEV/TNG extracts under workspace `work/`, never destructively in the
   Steam install unless explicitly requested.
4. Feed EgoCore `--list` output into searchable asset indexes.
5. Keep FQT catalog scans read-only: WAD-extracted TNGs are temp-cache inputs, not files copied into
   the Steam install.
6. Port or wrap `SilverChest.TngBridgeClient` into FQT for real TNG edits.
7. Port or wrap `SilverChest.LevBridgeClient` into FQT for terrain/region data.
8. Use FableWin decompilation to audit where SilverChest/FableMod behavior disagrees with the native
   editor.
9. Use FSE for runtime validation and gradually expand its native modding surface with confirmed
   decompiled signatures.

First applied FQT hardening:

- `D:\Code\FQT\FQT\FableQuestTool\Services\GameDataCatalogService.cs` now scans WAD-extracted TNG
  files from temp cache and leaves the installed game directory untouched.
- Loose installed TNGs take priority; WAD cache fills missing levels such as `creature_hub`.
- Verified with `GameDataCatalogServiceTests`: 4 passing.

## Decision

The right approach is not to pick one tool. The comprehensive environment should be a layered system:

- EgoCore: archive, bank, mesh, texture, audio, particles, definitions.
- SilverChest/ChocolateBox: practical TNG/LEV/editor bridge behavior.
- FQT: user-facing quest and world-editing shell.
- FSE: runtime scripting and live validation.
- FableWin/Ghidra: native editor/engine truth.
- Retail `Fable.exe` decompilation: long-term source reconstruction and native modding expansion.

That combination is how we get both a Creation Kit-like tool and a serious decompiled source base.
