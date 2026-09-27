# In-engine modding environment: a Source-style toolchain for the modernized engine

Design doc, 2026-09-26. Status: **proposal**, nothing built. Scope: the post-recreation
modernization fork (see [ARCHITECTURE](../ARCHITECTURE.md) and the "x64 / broad modernization" row in
[ROADMAP](../ROADMAP.md#parked--shelved)). The byte-exact rebuild is untouched and stays retail: every
feature here lives behind the modernization switch or in a separate build target.

Companion: [SCRIPTING_REDESIGN](SCRIPTING_REDESIGN.md) covers the script runtime (engine-native Lua
host, generated bindings, async model, hot reload, REPL, debugger, headless test host). This doc links
to it and does not repeat it.

**The question.** Modding Fable today needs four tools that do not know about each other: EgoCore, the
leaked Lionhead dev/editor builds, FSE/ForgeFSE, and FableForge. Could the recreated engine carry its
own modding environment instead, the way Source does? "Garry's Mod" here means the **Source modding
experience** that modders praise: a console that reaches everything, mods as folders layered over the
base game, Hammer and its entity contract, the model/choreography/particle tools, hot reload,
developer overlays, and an SDK that ships. The GMod sandbox game (spawn menu, physgun) is one
section, not the frame.

Short answer: yes, and Lionhead already built half of it. Their editor was an **in-engine game
component** (`CEditComponent` / `CEditControlCentre`), the same idea as Source's `-tools` mode. It was
compiled out of retail. The recreation can bring the concept back as clean, documented code, with
FableForge's format library underneath and a Source-style mod folder model around it.

Contents

- [1. The tools today and where their power comes from](#1-the-tools-today-and-where-their-power-comes-from)
- [2. What the engine itself already has](#2-what-the-engine-itself-already-has)
- [3. Runtime-editable vs bake-only](#3-runtime-editable-vs-bake-only)
- [4. The Source toolchain mapped to Fable](#4-the-source-toolchain-mapped-to-fable)
- [5. s&box: what Source 2's modern successor changes](#5-sbox-what-source-2s-modern-successor-changes)
- [6. The sandbox (GMod-style) layer](#6-the-sandbox-gmod-style-layer)
- [7. Design for the recreation](#7-design-for-the-recreation)
- [8. Constraints and dependencies](#8-constraints-and-dependencies)
- [9. Sequencing](#9-sequencing)
- [10. Open questions and unverified claims](#10-open-questions-and-unverified-claims)

---

## 1. The tools today and where their power comes from

| Tool | What it gives a modder | Where the capability comes from | What it cannot do |
|---|---|---|---|
| **EgoCore** (`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master`) | Bank editing and recompiling (`.big`, `.lut`, `.lug`), glTF mesh import/export, def editor + compiler, FSE Lua tab with autosuggest, WAD decompile, mod manager with load order and DLL mods (`mods.ini`) | Its own format code (`Banks/`, `Meshes/`, `Animations/`, `Lipsync/SpeechAnalyzer.h`, `Particles/`, `Levels/WADBackend.h`, `Mods/ModManagerBackend.h`). **Def compile is not its own:** `Definitions/CompilerBackend.h` patches `dbugst.ini` (`AllowDataGeneration TRUE`, ...), launches **`ego_r.exe -build_retail_static_maps`** hidden, and pokes one byte at `0x00C90613` in the suspended process. The engine is the compiler. | Anything live. It edits files the game reads at boot. DLL mods need FSE's launcher to inject. |
| **Lionhead dev/editor builds** (`debug_build/FableWin.exe` 55.5 MB, `debug_build/ego_r.exe` 15.9 MB, both Dec 2012, with `FableWin.pdb`, `Ego_r.pdb`, `Ego_d.pdb`) | The real in-engine editor: place/select things, paint heights and themes, rivers, fractal terrain, regions, camera tracks, quest dialogs, animation events, nav generation, STB bake, level merge | Native code in those binaries (`ghidra_out/fablewin_editor_symbols.tsv`: 80,059 symbols in 8 categories; `editor_classes` alone 2,503). Used today as an RE oracle for nav, water and foliage ([NAVIGATION](NAVIGATION.md), [WATER_RE](WATER_RE.md), [FOLIAGE_LOCAL_DETAIL_RE](FOLIAGE_LOCAL_DETAIL_RE.md)) and as a bake tool ([TERRAIN_RENDER_FIX §3](TERRAIN_RENDER_FIX.md)). | These are **Anniversary-era (2012) builds, not TLC**. They are GUI + D3D9 only, cannot run headless, crash-prone, need XP-SP3 compatibility mode ([TERRAIN_RENDER_FIX](TERRAIN_RENDER_FIX.md)). Leaked, unlicensed: nothing to ship or build on. |
| **FSE / ForgeFSE** (`D:\Code\ForgeFSE-retail-shadow`) | A Lua host inside the running game: custom quests, entity control, spawning, teleport, 933 reversed API functions (`refs/fse_api_manifest.json`) | A DLL detoured into retail at `0xCDB355` that calls engine functions by raw address. The quest must also be armed with `AddQuest(name, TRUE)` in `FinalAlbion.qst` ([QST_FORMAT](../formats/QST_FORMAT.md), [QUEST_SCRIPTS](QUEST_SCRIPTS.md)). | No hot reload, no debugger, no entity inspection. Semantics are recovered binding by binding (blocking `Speak`, retry-loop `AcquireControl`, see SCRIPTING_REDESIGN "What goes wrong today"). Steam verify wipes its `.qst` arming. |
| **FableForge** (`D:\Code\FableForge`, MIT, public) | Level/world editor: things, terrain sculpt + theme paint, foliage, water brush, nav patching, new levels and regions, overworld layout, creatures, villages, spawners, presets, textures tab, static mesh import (in-game verified 2026-09-20), quest node editor, mod packs (branch `modpacks`) | `libs/forgecore` (49 source files: `lev`, `tng`, `wad`, `stb`, `stbbake`, `navmesh`, `navpatch`, `defedit`, `defschema`, `meshcompose`, `questnodes`, `modorder`, `egocore`, `fse`, ...). Every writer is byte-checked against retail. Live link through ForgeFSE (`src/livelink.cpp`): teleport and spawn in the running game. | **Nothing hot-reloads** (`docs/ENGINE_RULES.md`): rewriting the WAD, STB, `textures.big` or `game.bin` under a running game crashes it. Saves cache a map's entities and the region table. Nav is patched per cell, not regenerated. |

The pattern: every tool reimplements a slice of the engine from the outside, and the one that edits
live (FSE) has the least structure. Source's toolchain works because the engine *itself* exposes the
console, the file system layering, the entity I/O and the reload hooks, and the tools sit on those.

---

## 2. What the engine itself already has

### 2.1 Retail `Fable.exe`

- **A console and config system**, fully mapped in [CONSOLE_COMMAND_SYSTEM](CONSOLE_COMMAND_SYSTEM.md):
  `CConsole::Initialise` `0x009ED190` (constructed in the shipping build), one registry insert
  `0x009EC5E0` with 43 callers, `RunTextCommand` / `RunCommandLine`, config-file execution
  (`userst.ini`, `default_userst.ini`, nested `RunScript`), and `BindKey` / `BindString` /
  `RunBoundString`. Registered commands are data-pipeline switches (`UseCompiledDefs`,
  `AllowDataGeneration`, ...), memory pool sizes, `ActivateQuest`, `SetTimeOfDay`, `SetDaySpeed`, and
  the console built-ins (`CommandList`, `VarList`, ...).
- **Input processes for a console, debug controls and a free camera**: the RTTI names
  `.?AVCInputProcessConsole@@`, `.?AVCInputProcessDebugControls@@`,
  `.?AVCInputProcessControlFreeCamera@@` are in the retail binary (RTTI scan of the Steam
  `Fable.exe`, 1,973 class names). Whether a retail key reaches them is **not verified**.
- **No editor.** The same RTTI scan finds zero `CEdit*` classes, no `CTCEditor`, no
  `CEditControlCentre` string, and no `BuildRetailStaticMaps` string. `AllowDataGeneration` is
  present (one occurrence). The editor was compiled out; there is no retail target for it.

### 2.2 The Lionhead dev/editor builds

From `ghidra_out/struct_layouts_egor.tsv` (Ego_r.pdb) and `ghidra_out/fablewin_editor_symbols.tsv`
(FableWin.pdb):

- **The editor is a game component.** `CEditComponent` (`Init`, `PostInit`, `Update`, `Render`,
  `EditInitWorld`, `EditInitDisplayEngine`, `IsEditorActive`, `SetAsEditingLevel`,
  `SetAsEditingWorld`) owns a `CEditControlCentre`, a `CEditWorld`, a `CEditDisplayEngine` and an edit
  GUI. It runs in the same process as the game, like Source's `-tools` mode.
- **`CEditControlCentre`** holds `GlobalMode`, `ViewMode`, `EditMode`, 2D and 3D cameras, a
  `CInputProcessManager`, and one input process per tool: `Main`, `View3D`, `View2D`, `PaintMap`,
  `ScriptBrushes`, `EditThings`, `MapPlacement`, `Region`, `EditTracks`, `CopyPaste`, `Bone`,
  `Animation`, `SetPolygonalArea`, `EditCameraPoint`, `EditShapes`, `AddTrackPoints`,
  `SurveyPassability`, `SurveyThemes`, `SurveyEngine`, `SurveySounds`, `SurveyReflections`,
  `SurveyMinimap`, `PreviewSpline`, `Console`. Dialog pointers include `PEditThingPropertiesDialog`,
  `PEditBrushLibraryDialog`, `PEditMapsAndRegionsDialog`, `PEditFractalDialog`, `PEditQuestDialog`,
  `PEditInitialQuestsDialog`, `PEditAnimationDialog`, `PEditAnimationEventsDialog`.
- **Undo and transactions**: `CEditTransactionManager`, `CEditTransactionComposite`,
  `CEditTransactionSetHeight`, `CEditTransactionSetEngineTheme`, `CEditTransactionSetEngineBlend`,
  `CRepeatTransaction`, `CReverseTransaction`; `EditUndo` / `EditRedo` / `InsertUndoPartition`.
- **Brushes = prefabs as diffs**: `CFileFormatBrush` with `CFileFormatBrushThingToAdd`,
  `...ThingToDelete`, `...ThingToMove`, `CFileFormatEditMapBrushCell`; `CScriptedMapBrush` (applied
  by scripts at runtime through `CMap::CScriptedMapBrushUpdateArea`); `CBrushLibraryDialog`.
- **Multi-user merge**: `CEditLevelMerger` (3,667 symbols) with `CConflict`, `CThingText`,
  `SetOurText` / `SetTheirText`. Lionhead merged level edits from several designers.
- **Generators and bakes**: `CEditControlCentre::GenerateNavigationInformation`,
  `CWorldMap::GenerateRegionConnectivityGraph` (FableWin `0x01c8a2c0`), `CEditFractal`, `CEditRiver`,
  `CWaterGenerator` / `CWaterSeaGenerator`, `CStaticMapGenerationVisibiilityInfo`,
  `CLocalDetailCacheMap::GenerateStaticMapEntry`; `BuildRetailStaticMaps` in both dev binaries.
- **A much larger console**: about 70 `Console*` handlers in FableWin, among them
  `CWorld::ConsoleSetFreeCamera*` (8 variants), `CWorld::ConsoleSetDrawNavigationInfo`,
  `CWorld::ConsoleSetDrawScriptNames`, `CWorld::ConsoleSetReactionDebug`,
  `CWorldMap::ConsoleToggleShowPassability`, `CNewFrontendGameComponent::ConsoleReloadFrontEndDef`,
  `CEnvironment::ConsoleSetEnvironmentTheme*`, `CGameTimeManager::ConsoleFastForwardTimeTo`,
  `CEngineLocalDetailGenerator::ConsoleAddLocalDetail*`, `CEngineCamera::ConsoleRunDebugScript`,
  `CEditControlCentre::ConsoleEditSetZ` / `ConsoleEditRaiseZ`. The PDB also has
  `CConsoleCommand<T>` for `CThingManager`, `CWorld`, `CWorldMap`, `CCombatManager`,
  `CEngineWaterSettings`, `CEngineWeatherRenderer`, `CEditControlCentre`,
  `CEditTransactionManager` and others.
- **Debug draw and diagnostics**: `CDebugManager` (log file, errors, asserts), `CEngineDebug`
  (`Draw3DSphere`, `DrawTextW`, user stat variables), `CDebugBox` / `CDebugSphere` / `CDebugCone`,
  `CCombatDebugInfo`, `CPhysicsDebugInfo`, `CThingPhysicalDebugInfo`, `CDebugObjectTracker`.
- **Editor state on things**: `CTCEditor` (`LockedInPlace`, `ReasonsToNotDraw`,
  `ReasonsToNotBeSelectable`, `Selected`), `CThingFilter_IsSelectableBasedOnEditorMode`,
  `CTCEditorAnimationThing` / `CTCEditorAnimationCreature` for previewing animations on a thing.

**What this means.** The shape of an in-engine editor for this engine is known: a component, an input
process per tool, transactions for undo, brushes as thing diffs, survey overlays, and generators that
run inside the engine. We do not reuse Lionhead code (none of it is in retail, and the builds are
leaked); we reuse the structure, the file formats it writes (which our tools already round-trip), and
the algorithms we have already recovered from it (nav subdivision 398/398 in
[NAVIGATION](NAVIGATION.md), the water bake in [WATER_RE](WATER_RE.md)).

---

## 3. Runtime-editable vs bake-only

This decides what can be live in an in-engine editor and what needs a bake step.

| Data | Source form | Shipped form | Editable at runtime today? | Evidence |
|---|---|---|---|---|
| Things (objects, creatures, markers) | TNG text | TNG inside `FinalAlbion.wad` | **Yes, create/move/destroy**: FSE `CreateCreature`, teleport; the editor did it through `CThingManager`. Persisting needs a TNG write. Saves cache each map's entities. | FableForge `docs/ENGINE_RULES.md`; live link `src/livelink.cpp` |
| Thing wiring | TNG fields | same | Data only: `CTCActivationTrigger` (`ReceptorUID`) to `CTCActivationReceptorBase` (`Triggers`, `LogicType`, deactivate timer); `CTCSwitchableNavigation` | `struct_layouts_egor.tsv` |
| Defs | `.def` text (Lionhead), compiled `game.bin` / `names.bin` / `frontend.bin` / `script.bin` | compiled bins | **No** in retail (loaded at boot, rewriting crashes). The dev build has `ConsoleReloadFrontEndDef`, so per-def-type reload existed for at least one type. | [CONSOLE_COMMAND_SYSTEM](CONSOLE_COMMAND_SYSTEM.md); FableWin symbols |
| Terrain heights + theme paint | LEV heightfield + theme grid | LEV in the WAD, render chunk in `FinalAlbion_RT.stb` | **In the editor yes** (`CEditTransactionSetHeight`, `SetEngineTheme`, `SetEngineBlend`); in retail no | FableWin symbols; [TERRAIN_RENDER_FIX](TERRAIN_RENDER_FIX.md) |
| Render terrain, foliage, water surface | derived from LEV + themes | STB chunks (static map, local detail, `CWaterPatchMesh`) | **Bake-only**: `BuildRetailStaticMaps`; retail only loads | [WATER_RE](WATER_RE.md), [FOLIAGE_LOCAL_DETAIL_RE](FOLIAGE_LOCAL_DETAIL_RE.md); FableForge `stbbake.cpp` writes these itself |
| Navigation | LEV + thing collision outlines + nav-info things | `CNavQuadTree` sections inside the LEV | **Bake**, but a fast one: `CNavQuadTree::Initialise` regenerates all 398 retail LEVs exactly; only collision-mesh-to-line extraction is not lifted | [NAVIGATION](NAVIGATION.md) "Status of gaps" |
| Region graph | TNG region exits | `FinalAlbion_StartingRegionGraph.txt`, WLD | Bake (`GenerateRegionConnectivityGraph`, editor-only). Saves cache the region table. | [NAVIGATION](NAVIGATION.md) |
| World layout | WLD/BWD | same | Load-time only; moved maps need a new game | FableForge `docs/ENGINE_RULES.md` |
| Textures, meshes, animations, effects | PNG/glTF/3DAF sources | `.big` banks | **No**: banks are held open; rewriting crashes | ENGINE_RULES; [CAPABILITY_INDEX](CAPABILITY_INDEX.md) |
| Text, dialogue, audio, lipsync | text / WAV | `text.big`, `.lut`, `dialogue.big` | No | CAPABILITY_INDEX |
| Quest logic | C++ (retail), Lua (FSE) | compiled classes registered by `RegisterAllScripts` `0x00CD52D0`; FSE Lua files | Retail no; FSE Lua is loaded per session, no reload | [QUEST_SCRIPTS](QUEST_SCRIPTS.md), SCRIPTING_REDESIGN |
| Cutscenes | `[Actor.]Verb args` text commands, 184 verbs | `CCutsceneDef` payload in defs | No (def-bound) | [CUTSCENES](CUTSCENES.md), [SCRIPT_VM_MAP](SCRIPT_VM_MAP.md) |
| Packaging | loose LEV/TNG | `FinalAlbion.wad` | No: the engine streams from the WAD and holds it open | ENGINE_RULES |

Two conclusions. First, the things that crash on reload today crash because **the engine holds the
container open and streams from it**, not because the data cannot change. A modern engine that reads
through a virtual file system with per-asset handles can reload them (§7.4). Second, the real bake
steps are few: STB (render terrain, foliage, water), nav, region graph. Nav is already fast and exact
in our tools. The STB is the vbsp/vrad of this engine and the main thing to make incremental (§4.3).

---

## 4. The Source toolchain mapped to Fable

Each row: what Source does, the Fable equivalent that exists (retail, dev build, or our tools), and
what the recreation needs. Details for the bigger rows follow the table.

| Source feature | Fable equivalent today | Recreation needs |
|---|---|---|
| Developer console, convars, concommands | Retail `CConsole`, 43 commands; dev build about 70 more | Open registry, typed vars with flags, autocomplete, one registration macro (§4.1) |
| `sv_cheats` gating | None | `Cheat` flag; cheats off in normal play, on in dev/tools mode and in a "modded" save state |
| `bind`, `exec`, `.cfg` | `BindKey`, `BindString`, `RunScript("x.ini")`, `userst.ini` at boot | Keep these, including the retail ini syntax |
| Launch options `-dev`, `-tools`, `-game <moddir>`, `+map` | ini only (`SetLevel`, `ActivateQuest`) | Command-line `-dev`, `-tools`, `-mod <dir>`, `+<command>` |
| Mod = folder + `gameinfo.txt` search paths | FableForge mod packs (load order, conflict report); EgoCore `mods.ini`; FSE mod folders | Virtual file system that layers mod folders over the retail WAD and banks, never patching them (§4.2) |
| Hammer (brushes, entities, 2D/3D views) | Lionhead `CEditComponent`; FableForge standalone | Tools mode in the engine + FableForge offline, sharing forgecore (§4.3, §7.5) |
| FGD entity definitions | Defs: `refs/transfer_field_orders.json` (268 classes / 5,133 fields from ego_r), 249 def types field-level decoded ([DEFS](../formats/DEFS.md)) | Generate the editor's property sheets from def schema + TC layouts, with docs and ranges |
| Entity I/O (outputs to inputs, no code) | `CTCActivationTrigger` to `CTCActivationReceptorBase` (UID link, `ELogicType`), switchable nav | Generalize into named outputs/inputs on things, authored in the editor (§4.3) |
| `vbsp` / `vvis` / `vrad` compile | STB bake (static map, local detail, water), nav gen, region graph, WAD pack | Incremental per-map bake service, run from the editor, never a full-world rebake (§4.3) |
| `map <name>` to test instantly | FSE `GoToMapSlotRetailTransition`; FableForge "Go here in game" | `map <level> [x y z]`, plus play-from-here in tools mode |
| studiomdl + QC | `forge mesh-import` (glTF/OBJ, physics hull), EgoCore `MeshCompiler.h` / `GltfMeshImporter.h`, Blender addon | A text "model recipe" file (the QC role) compiled by the asset pipeline (§4.4) |
| HLMV model viewer | FableForge `meshpreview`; dev build `CTCEditorAnimationThing` | In-engine model/animation viewer (§4.4) |
| Faceposer (choreography, lip sync) | Cutscene verbs ([CUTSCENES](CUTSCENES.md)); EgoCore `SpeechAnalyzer.h`; dev build animation events dialog | Timeline editor over the cutscene verb stream + lipsync generation (§4.5) |
| Particle editor | effects.big decoded (1,165/1,165 in FableForge); EgoCore `ParticleCompiler.h` | Live particle editor in tools mode |
| VMT materials + `mat_reloadallmaterials` | Textures in GBANK; terrain look in `ENGINE_THEME` defs | Text material/theme sources, hot reload (§4.6) |
| `-tools` mode (SFM lineage) | `CEditComponent` inside the game process | Same, as a component of the modern build (§7.3) |
| `ent_fire`, `ent_text`, `ent_create`, `picker`, `cl_showpos`, overlays | Dev build `DrawScriptNames`, `DrawNavigationInfo`, `ToggleShowPassability`, survey modes, free camera; FSE entity bindings | Entity inspection commands and overlays (§4.7) |
| `nav_edit` | Survey passability; FableForge walkable paint + `navpatch` | In-game nav view and edit, with regeneration (§4.7) |
| VScript | FSE / ForgeFSE Lua | [SCRIPTING_REDESIGN](SCRIPTING_REDESIGN.md) |
| Source SDK shipped and open | This repo + FableForge (MIT) | Ship the SDK: headers, API docs, example mods, format docs (§4.8) |

### 4.1 Console, convars and launch options

The retail console already has the parts Source's has: a registry, a command line parser, config file
execution with comments, key binding, and a list command. What it lacks is reach (43 commands) and
metadata. The modern build should:

- register commands and variables with one macro next to the code, carrying a name, help text,
  range and flags. The flag set can follow s&box's `ConVarFlags` (`Saved`, `Cheat`, `Hidden`,
  `Protected`, `ChangeNotice`, ...; see §5), minus the network ones;
- restore the dev-build commands we have names for (§2.2): free camera, draw navigation info, draw
  script names, show passability, environment theme override, fast-forward time, local detail
  add/remove, camera debug scripts;
- add the Source staples: `map`, `give <def>`, `spawn <def>`, `noclip`, `god`, `ent_*` (§4.7),
  `reload <asset|defs|script>`, `find`, `help`, history and autocomplete;
- gate cheats with a `Cheat` flag and a `sv_cheats` equivalent. Turning it on in a save marks the save
  as modded, so achievements and "retail" saves stay honest;
- keep `userst.ini` / `RunScript` syntax working so existing configs and the `ActivateQuest` idiom
  carry over;
- add launch options: `-dev` (console, overlays, verbose errors), `-tools` (editor component),
  `-mod <folder>` (repeatable, in load order), `+<command>` (run at boot).

Script access to the console (running Lua from it) is SCRIPTING_REDESIGN §7.

### 4.2 Mods as folders layered over the base game

Source's best idea: a mod is a folder, `gameinfo.txt` lists search paths, and the file system resolves
each path through them in order. Base files are never modified, so uninstalling is deleting a folder.

Fable today is the opposite: the game reads `FinalAlbion.wad`, the `.big` banks and compiled def bins,
and every tool rewrites those in place (with backups). FableForge's mod packs (0.20, branch
`modpacks`) and EgoCore's mod manager both compose mods by *writing a merged install*.

The recreation should add a virtual file system under every loader:

- **Mount order**: retail install (read-only), then each enabled mod folder, then the user's working
  folder. Later mounts win per file.
- **Container-aware mounts**: a mod provides a loose `Levels/FinalAlbion/<map>.lev` or `.tng`, a bank
  entry by name (`textures/<NAME>.png` or a compiled entry), or a def patch; the VFS presents it as if
  it were inside the WAD or bank. The retail containers are indexed once and never written.
- **Field-level def merge**: defs merge per field, not per file. FableForge already does this at 100%
  coverage of `game.bin` ([CAPABILITY_INDEX](CAPABILITY_INDEX.md)); the engine would do it at load.
- **One manifest per mod** (name, version, dependencies, load-after, content types, script entry
  points), which is also where SCRIPTING_REDESIGN §6 registers quests and cutscenes.
- **A conflict report** at boot and in the editor, reusing FableForge's one-JSON conflict report and
  namespaced picks.
- **Compatibility importers** for the existing ecosystem (`.fmp`, loose trees, bsdiff, `.qst`, EgoCore
  `Mods/<Name>/`, GB packs; corpus in FableForge `work/nexus_mods/CATALOGUE.md`, plan in its
  `docs/ROADMAP_1.0.md` "0.20") that convert them into mod folders.

This alone removes the most common failure today (a Steam verify or another tool overwriting a
patched file) and makes the dev loop "save file, reload asset" instead of "repack WAD, restart".

### 4.3 Hammer: the level editor, the entity contract, and the compile step

**The editor.** Lionhead's editor was in-engine; Hammer is a separate program with its own renderer.
The recreation should do what Lionhead did (tools mode inside the engine, §7.3), because Fable's look
depends on engine-only systems (theme blending, local detail, water, lighting) that a separate
renderer would have to duplicate. FableForge keeps the offline role (§7.5).

Tools, taken from the `CEditControlCentre` mode list and FableForge's current feature set:

- select, move, rotate, scale things with gizmos (FableForge's ImGuizmo gizmo with Q/W/E/R and
  multi-select, `D:\Code\FableForge\docs\EDITOR.md` "Transform");
- place from a searchable def browser (every `OBJECT_*`, `CREATURE_*`, marker);
- terrain sculpt and theme paint as transactions (`SetHeight`, `SetEngineTheme`, `SetEngineBlend`);
- foliage paint, water paint (depth themes), rivers;
- regions and polygonal areas, camera points and tracks, copy/paste across maps;
- brushes: save a selection as a thing diff (add/delete/move), reuse it as a prefab;
- undo/redo for everything, grouped into partitions.

**The entity contract (the FGD role).** Hammer knows what an entity can be from the FGD. Fable already
has a stronger contract: every def type's fields in `Transfer` order with names and wire types
(`refs/transfer_field_orders.json`, 268 classes / 5,133 fields), the def schema FableForge uses for
field-level edits, and PDB layouts of every thing component (`CTC*` classes in
`struct_layouts_egor.tsv`). The editor's property sheets should be generated from these, with help
text, ranges and enum names added in one annotation file. That file is the Fable FGD.

**Entity I/O.** Source's outputs-to-inputs wiring lets designers build doors, traps and sequences
without code. Fable has the seed of it: an activation trigger holds its receptor's UID, and the
receptor holds its triggers with a logic type (`CTCActivationTrigger.ReceptorUID`,
`CTCActivationReceptorBase.Triggers` / `LogicType` / `DeactivateAfterSetTime`), and switchable nav is
keyed by thing UID (407/407 proven in [NAVIGATION](NAVIGATION.md)). The modern build can generalize
this into named outputs (`OnActivated`, `OnKilled`, `OnEnteredArea`, `OnSpeechDone`, ...) wired to
named inputs on other things (`Open`, `Enable`, `Spawn`, `PlayCutscene`, `StartQuest`, ...), with a
delay and a parameter, stored as extra TNG fields in the mod's layer. Retail things keep their retail
wiring. The input set is the same list as the script API, so a wire and a Lua call do the same thing.

**The compile step.** Source's pain is known: `vbsp`/`vvis`/`vrad` on a whole map, leaks that stop the
compile, minutes to hours of waiting. Fable's equivalents:

| Source | Fable | Today | Target |
|---|---|---|---|
| `vbsp` (geometry) | STB static map chunk from LEV + themes | ego_r full-world rebake 20 to 40+ min; FableForge per-map `stbbake` / `stbheightbake` | Per-map, background, on save |
| `vrad` (lighting) | local detail + water patch meshes in the STB | editor bake; FableForge water writer (not seen in-game yet) | Per-map, background |
| `vvis` (visibility) | `CStaticMapGenerationVisibiilityInfo` | editor bake | Per-map |
| nav (`nav_generate`) | `CNavQuadTree::Initialise` | exact regen oracle (398/398); FableForge patches per cell | Regenerate the touched map on save, seconds |
| - | region graph | editor-only | Regenerate on region edit |
| packing | WAD | FableForge repack | Gone in dev: the VFS reads loose files; pack only for distribution |

Fable has no BSP leaks (terrain is a heightfield, there is no sealed-world requirement), so the worst
Hammer failure mode does not exist here. The design goals are: never a full-world rebake, bake in the
background while the editor stays usable, show a clear "render terrain stale" marker instead of a
crash, and let the game run with a stale bake (retail degrades gracefully on missing per-map data in
some paths; the white-out work in [TERRAIN_WHITEOUT_FIX](TERRAIN_WHITEOUT_FIX.md) shows where it does
not).

**`map <name>`.** Testing a change should be one command: `map StartOakValeWest` or, in tools mode,
"play from here" (spawn the hero at the camera, like Unreal's PIE and s&box's in-editor play, §5).
Leaving play mode restores the edited world. Because saves cache region entities and the region table,
the dev build needs the switch SCRIPTING_REDESIGN §5 already proposes: refresh cached region entities
on load, so an existing save sees edits.

### 4.4 Models: the QC role and the model viewer

Source: a QC text file names the meshes, skeleton, animations, hitboxes and materials; `studiomdl`
compiles it; HLMV previews it. Fable's pieces exist but are spread out: FableForge's `forge
mesh-import` composes static meshes and physics hulls (in-game verified 2026-09-20; hull winding must
be the engine's), EgoCore has skinned import (`ImportType5`) and animation retargeting
(`GltfAnimImporter.h`), the Blender addon round-trips meshes, and the formats are in
[MESH_COMPOSE](../formats/MESH_COMPOSE.md), [BIG_ANIM_FORMAT](../formats/BIG_ANIM_FORMAT.md) and
[ANIM_WRITER](../formats/ANIM_WRITER.md).

The recreation should:

- define a small text recipe per model (source glTF, mesh type 1/2/3/4/5, donor or custom skeleton,
  LODs, physics hull, textures, the def it produces), compiled by the asset pipeline into the mod
  folder;
- ship an in-engine model and animation viewer in tools mode (Lionhead had
  `CTCEditorAnimationThing` / `CEditInputProcessAnimation` / the animation events dialog), including
  animation event editing (footsteps, hit frames, sounds) that today lives in compiled animation event
  data;
- reload a model when its source changes (§7.4).

### 4.5 Choreography and lip sync (the Faceposer role)

Faceposer maps well onto Fable. Cutscenes are a text stream of `[Actor.]Verb args` commands (184
verbs, [CUTSCENES](CUTSCENES.md), [SCRIPT_VM_MAP](SCRIPT_VM_MAP.md)); dialogue joins text, audio and
lipsync on one `<N>` ordinal ([CAPABILITY_INDEX](CAPABILITY_INDEX.md)); EgoCore's `SpeechAnalyzer.h`
generates lipsync from a WAV (5 phonemes, 512-sample frames, at least 3 zero-weight keys on silent
frames or the mouth hangs open). A modern tool is a timeline over the verb stream: actor tracks, camera
tracks (the editor's camera track tools), speech clips with generated lipsync, expressions, and
scrub-and-preview inside the running engine. Its output is the same text command stream, so retail
cutscenes open in it and new ones play in retail-compatible form. SCRIPTING_REDESIGN §6 makes the verb
table extensible from Lua.

### 4.6 Materials, themes and asset hot reload

Source's VMT is a text file per material, and `mat_reloadallmaterials` picks up edits without a
restart. Fable's appearance data is split between texture banks (GBANK, LZO1X, DXT) and defs
(`ENGINE_THEME` for terrain, particle and mesh material fields). The recreation should read textures
from loose source images in a mod folder (compiled and cached on first load) and theme/material
settings from text defs, and reload both in place. The hard part is engine-side: every texture and
mesh reference must go through a handle the resource manager can repoint (§7.4).

### 4.7 Entity reporting, overlays and nav editing

Source's everyday debugging tools are `ent_text` (overlay an entity's state), `ent_fire` (send an
input), `ent_create`, `picker`, `cl_showpos`, the `developer` overlays and `nav_edit`. Lionhead had
most of the Fable equivalents in the dev build (§2.2). The modern build should have:

- `ent_list`, `ent_text <name|picked>` (def, TC list, script name, owning quest, AI state, scheduler
  resources, which ForgeFSE work showed is where quests hang), `ent_fire <thing> <input> [arg]`,
  `ent_create <def>`, `ent_remove`, a crosshair picker;
- `showpos` (map, region, world position, ground height, nav cell);
- overlays: script names (retail `DrawScriptNames`), navigation (`DrawNavigationInfo`), passability,
  themes, sounds, reflections, minimap zones (the survey modes), activation wiring, trigger volumes,
  quest thread state (SCRIPTING_REDESIGN §7);
- `nav_edit`: show the quad tree, paint walkable/blocked, place layer transitions, regenerate the map's
  nav in seconds from the recovered `CNavQuadTree::Initialise`.

### 4.8 Shipping the SDK

Source's SDK shipped with the game; that is why its modding scene lasted. The equivalent here is legal
only as *our* code and documentation: the recreated engine source (never retail bytes; see
[CONTRIBUTING](../../CONTRIBUTING.md) and the git rules in CLAUDE.md), FableForge (MIT), the format
docs in `docs/formats/`, the generated script API reference (SCRIPTING_REDESIGN §2), the entity
annotation file (§4.3), and example mods. Players supply their own retail data.

---

## 5. s&box: what Source 2's modern successor changes

s&box (Facepunch, `github.com/Facepunch/sbox-public`) is Garry's Mod's successor: a C# engine layer and
editor on Valve's Source 2. Read via the GitHub API on 2026-09-26.

**Licence (verified).** `LICENSE.md` is MIT, copyright 2025 Facepunch Studios Ltd, for the source in the
repo (the managed engine, editor, tooling and game content). The README states that "certain native
binaries in `game/bin` are not covered by the MIT license" and are under the s&box EULA, and third-party
components keep their own licences (`game/thirdpartylegalnotices`). The GitHub API reports the licence
as "Other" / `NOASSERTION` because of that mix. **Consequence:** we may reuse MIT C# code with
attribution, but it is C# on .NET 10 and our engine is C++, so in practice we reuse **ideas and
designs**, not code. The native Source 2 core is not in the repo and not reusable.

What it does, with the files that show it:

- **Scene / GameObject / Component** (`engine/Sandbox.Engine/Scene/`): a scene is a tree of game
  objects with components, serialized as data. **Prefabs** (`Resources/Scene/PrefabFile.cs`,
  `Scene/GameObject/GameObject.Prefab.cs`, `PrefabInstanceData`) store instance overrides as diffs
  against the prefab, with tests for GUID stability, reparenting and undo.
- **Play in the editor** (`engine/Sandbox.Tools/Scene/Session/SceneEditorSession.Game.cs`):
  `SetPlaying(scene)` creates a `GameEditorSession` from the edited scene and carries the selection
  over; `StopPlaying()` destroys it and returns to the untouched edit scene. No compile step.
- **Code hot reload** (`engine/Sandbox.Hotload/`): new assemblies are swapped in and live instances are
  upgraded in place (`InstanceUpgrader.cs`, `UpdateReferences.cs`, per-type upgraders), so objects keep
  their state across a code change. Game code is compiled from source by the engine
  (`engine/Sandbox.Compiling/`).
- **Sandboxed mod code** (`engine/Sandbox.Access/`): every assembly is checked with Mono.Cecil against
  an allow-list of namespaces and members (`Rules/BaseAccess.cs`: engine assemblies, collections,
  math, LINQ, selected `System.IO` stream types, ...) before it loads. AGENTS.md: exposing a new .NET
  type to game code means adding it to the allow-list, not bypassing it.
- **Console variables with flags** (`Sandbox.System/ConVar/ConVarAttributes.cs`): `Saved`,
  `Replicated`, `Cheat`, `UserInfo`, `Hidden`, `ChangeNotice`, `Protected` (console/tools only, not
  game code), `Server`, `Admin`, `GameSetting` (exposed to the platform UI).
- **Asset system** (`engine/Sandbox.Tools/Assets/`, `Resources/Compiling/`): source assets compile to
  engine resources through per-type compilers (texture, sound, ...), with a compile cache.
- **Packages** (`Systems/Project/ProjectConfig.cs`, `Services/Packages/`,
  `Sandbox.Tools/Utility/ProjectPublisher/`): a project has `Org`, `Ident` (full ident
  `org.ident`), type, schema version, extra resource paths and a dependency list; the publisher
  uploads compiled assets (and optionally sources); the package manager installs dependencies.
- **Action graphs** (`Systems/ActionGraphs/`): node graphs as a resource, the non-programmer path.
- **Mounting other games** (`engine/Mounting/`): `BaseGameMount` lets a plugin present another
  installed game's assets (GoldSrc, Quake, NS2, SE3 mounts ship) as read-only resources, detected by
  install and Steam ownership.

**Where s&box beats classic Source, and what suits Fable:**

| Topic | Classic Source | s&box | For the Fable fork |
|---|---|---|---|
| Testing a level | compile, then `map` | play inside the editor, stop returns to the edit state | **Adopt.** "Play from here" in tools mode (§4.3); world state restored on stop |
| Prefabs | `func_instance` VMFs | prefab diffs with per-instance overrides | **Adopt.** Lionhead brushes are already thing diffs (add/delete/move); extend with property overrides |
| Code iteration | recompile DLL, restart | hot reload with live instance upgrade | **Adopt the idea for Lua** (SCRIPTING_REDESIGN §7, declared state survives reload). Native C++ mods stay restart-only |
| Mod code safety | server plugins are native DLLs, fully trusted | allow-listed managed code | **Adopt.** Mod Lua gets an allow-listed API surface; native DLL mods stay possible but are marked unsafe and off by default (§7.6) |
| Content distribution | Workshop over `gameinfo.txt` folders | packages with `org.ident`, versions, dependencies | **Adopt the manifest shape** for mod folders (§4.2); hosting is out of scope |
| Retail content access | ship your own content | mount installed games read-only | **This is exactly the Fable situation.** The retail install is a read-only mount; mods never copy retail data |
| Convars | `FCVAR_*` flags | same idea, attribute-based | Same flag set minus networking (§4.1) |
| Networking | built in | built in | Not relevant (single-player; co-op was cut, [CUT_COOP_MULTIPLAYER](CUT_COOP_MULTIPLAYER.md)) |

What does not fit: .NET as the mod language (we have a Lua ecosystem: FSE mods, Aeon's ports, our
converted quests), a platform-hosted package service, and the scene-first model for everything. Fable's
world is regions of LEV terrain plus TNG things, and the recreation must load retail data unchanged, so
the scene model stays "map + things + components", not a rewrite into game objects.

---

## 6. The sandbox (GMod-style) layer

Kept to one section, as asked. With tools mode, the console and entity I/O in place, a GMod-style
sandbox is a thin layer:

- a **spawn menu** in play mode: searchable def browser (objects, creatures, effects), spawns at the
  crosshair through the same path as `ent_create`;
- a **tool gun**: pick a thing, then move, rotate, delete, wire (entity I/O), set a property, freeze;
- a **physgun**: carry and throw physical things. Fable's physics (`CThingPhysical`, Havok-era physics
  meshes) needs to be reconstructed first; this is the least certain item;
- **save/load a contraption** as a brush (thing diff) and share it as a mod folder;
- all of it gated by the `Cheat` flag and marked as a modded session.

This is fun and a good showcase, but it is a consumer of the real work (§4), not a driver of it.

---

## 7. Design for the recreation

### 7.1 Principles

1. **Retail stays retail.** The byte-exact build has none of this. The modern build adds it as new
   code, behind `-dev` / `-tools` at runtime and a build flag at compile time.
2. **Retail data is read-only.** Everything a modder makes lives in a mod folder layered over the
   install (§4.2). No tool writes the WAD, the banks or the def bins in dev.
3. **Source forms in dev, compiled forms for distribution.** The engine reads loose LEV/TNG, text
   defs, glTF and PNG in dev and compiles/caches them; a mod can ship either.
4. **One format library.** forgecore is the reader/writer for every format, used by FableForge, the
   engine's tools mode and the asset pipeline, so formats are implemented once and byte-checked once.
5. **Everything a tool can do, the console and scripts can do.** Editor actions are transactions with
   console names; the editor UI is a front end to them.

### 7.2 Layers

```
 mod folders + manifests ──> VFS (retail mounts read-only, mods in load order, conflict report)
                                │
 asset pipeline (source -> compiled cache, per type; forgecore writers) ──> resource manager (handles, reload)
                                │
 engine (reconstructed systems)  ├── console / convar registry (flags, cheats, bind, cfg, launch options)
                                 ├── entity inspection + I/O wiring (ent_*, outputs -> inputs)
                                 ├── script runtime  ── see SCRIPTING_REDESIGN
                                 ├── bake service (per-map STB, nav, region graph; background)
                                 └── tools component (edit/play modes, transactions, overlays, viewers)
 FableForge (standalone) ───── uses forgecore + talks to a running engine over the dev IPC
```

### 7.3 Tools mode

A `CEditComponent`-style component added by `-tools`: it owns the edit camera, selection, gizmos, the
transaction manager and the tool modes (§4.3), and switches the world between **edit** (simulation
paused, things selectable) and **play** (simulation running, hero spawned, edits frozen). Stopping play
restores the pre-play world snapshot, as s&box does. UI is Dear ImGui (FableForge already uses it,
with ImGuizmo and Unreal-style viewport controls). Transactions are serializable, so the same edit can
come from the UI, the console, a script or FableForge.

### 7.4 Hot reload

What reloads, and what it needs from the engine:

| Asset | Reload means | Engine requirement |
|---|---|---|
| Lua quests / region scripts | re-run, keep declared state | SCRIPTING_REDESIGN §5, §7 |
| Defs | re-read changed def, repoint `CDefPointer`s | def manager keyed by name/CRC with stable indices; dev build's `ReloadFrontEndDef` shows it was done per type |
| Textures | re-decode, swap the GPU resource behind the handle | resource manager with handle indirection (not raw pointers into banks) |
| Meshes, animations | swap mesh/anim data under the handle; live instances rebind | same, plus anim instance rebinding |
| Things in the current map | re-read TNG layer, diff by UID, apply adds/moves/deletes | thing manager create/destroy (exists) + UID-keyed diff |
| Terrain | re-read LEV heights, re-run the per-map bake, swap the chunk | per-map streaming unit that can be unloaded and reloaded; the retail streamer holds the STB open |
| Nav | regenerate the touched map, swap the tree | nav tree per map, swap-safe for agents mid-path |

The container-held-open crash (§3) disappears once the VFS owns file access and nothing streams from a
file that a tool rewrites; in dev, bakes write new cache files and the engine switches to them.

### 7.5 FableForge's role: both, on one library

Recommendation: **keep FableForge as the standalone editor and make the engine's tools mode share its
core**, rather than choosing one.

- FableForge works **today, on retail**, for players who will never run the modernized fork; it
  must keep writing retail-compatible installs and mod packs.
- The engine's tools mode is where live editing happens (only the real engine renders Fable exactly
  and runs its systems).
- Shared: `libs/forgecore` as the single format library; the def schema and entity annotation file;
  the mod manifest and load order (`modorder.cpp`); the quest node compiler (targeting the Lua runtime
  per SCRIPTING_REDESIGN §9); the viewport control scheme.
- The live link (`src/livelink.cpp`, today a Lua file polled every 0.5 s through ForgeFSE) becomes a
  proper dev IPC (named pipe or local socket) exposed by `-dev`: FableForge sends transactions and
  console commands, the engine streams selection, hero position and logs back. That makes FableForge a
  second front end to tools mode, the way Unreal's editor and PIE share one engine.

Compared with the reference points: Source = separate Hammer + in-engine `-tools`; Skyrim's Creation
Kit = separate editor on the same engine code, plugins as data layers with load order (our §4.2 is the
same model); Unreal = editor is the engine, PIE in-process; s&box = editor is the engine, play in the
editor. Fable ends up with Unreal/s&box's in-process loop for iteration and the Creation Kit's
data-layer mod model, with FableForge as the offline companion.

### 7.6 Mod safety

- **Data mods** (folders of LEV/TNG/defs/assets) are safe by construction: read through the VFS,
  validated on load, never written back.
- **Lua mods** get an allow-listed API (no `os`, no raw file I/O outside the mod's own folder, no
  native calls), the s&box approach applied to Lua. Details belong in SCRIPTING_REDESIGN's open
  question on sandboxing.
- **Native DLL mods** (EgoCore `Mods/<Name>/<Name>.dll`, FSE plugins) stay possible for
  compatibility but are listed as unsafe, off by default, and cannot be distributed through the same
  channel as data mods without an explicit opt-in.

---

## 8. Constraints and dependencies

Everything below belongs to the modernization fork. The fork starts after the reconstructed process
reaches the game loop ([ARCHITECTURE](../ARCHITECTURE.md), [ROADMAP](../ROADMAP.md)). The current
reconstruction ([HANDOFF](../HANDOFF.md), [HERO_IN_WORLD_ROADMAP](HERO_IN_WORLD_ROADMAP.md)): boot
leaves de-baked, `CGame::Play` trap-linked and reaching graphics initialization; world construction,
tick orchestration, entity spawn, animation runtime and the 3D renderer / D3D9 device layer are not
reconstructed (the renderer is the XL item). Strict coverage is 38.02% of the function catalog
(README).

| Feature | Depends on | State of the dependency |
|---|---|---|
| Console extensions, convar flags, launch options | `CConsole` (retail, mapped), boot path | Console mapped; boot path reconstructed; **can start first in the fork** |
| VFS + mod folders | every loader: WAD, banks, defs, LEV/TNG, STB | Loaders partly reconstructed (`OpenRetailBank` byte-pure; `OpenStaticMap` landed but crashing) |
| Def hot reload | `CDefinitionManager`, `CDefPointer` | Def loading reconstructed in the seam (`LoadBinaryDefinitions` near-miss scaffold) |
| Entity inspection, `ent_*`, I/O wiring | `CThingManager`, TC system, `CWorld` | Not reconstructed (HERO roadmap rows 5, 6, 9) |
| Tools mode, edit/play switch | world, tick, renderer, input processes | Not reconstructed; renderer is XL |
| Terrain/foliage/water live edit | landscape renderer, STB streaming, bake code | Formats and bakes known (FableForge writes them); runtime renderer not reconstructed |
| Nav regen in engine | nav tree runtime + generator | Generator algorithm recovered and oracle-validated (398/398); runtime not reconstructed |
| Asset hot reload | resource manager with handles | Retail uses raw pointers into banks (inferred from the crash on rewrite); needs a modern resource layer |
| Model/anim viewer | mesh + anim runtime + renderer | Formats known; runtime not reconstructed |
| Cutscene timeline | cutscene interpreter, camera, speech | Verb stream decoded; interpreter not reconstructed |
| Physgun / physics tools | physics runtime | Least known; not reconstructed |
| Script runtime | quest manager, scheduler | See SCRIPTING_REDESIGN "Sequencing" |

Three honest limits:

- **Nothing here can run on retail `Fable.exe`.** Retail has no editor code and holds its containers
  open. Detouring an editor into retail (the FSE route) would repeat today's fragility; the only
  retail-side work worth doing is what FableForge and ForgeFSE already do.
- **The Lionhead builds are a design reference, not a base.** They are 2012 Anniversary-era builds, not
  TLC, and leaked. Their layouts, formats and algorithms guide us; their code is not reused.
- **Save compatibility bites.** Saves cache region tables and map entities. The dev refresh switch
  (SCRIPTING_REDESIGN §5) helps testing; a released mod that changes a visited map still needs a
  rule (for example: mod-layer things merge into cached entities by UID on load).

---

## 9. Sequencing

**Now, without the modernized engine** (all offline, all benefit today's modders):

1. **Entity annotation file** (the FGD): generate from `refs/transfer_field_orders.json`, the
   FableForge def schema and the `CTC*` layouts; add help text and enum names. FableForge's property
   panel uses it first.
2. **Mod manifest + folder layout**, finalized in FableForge's mod packs work (0.20), written so the
   engine VFS can read the same folders later. Include the SCRIPTING_REDESIGN §6 registration block.
3. **Model recipe format** (the QC role) wrapping `forge mesh-import` and the skinned/animation paths
   from EgoCore's importers.
4. **Per-map bake as a library call** in forgecore (STB static map, water, local detail, nav, region
   graph), so FableForge and the future engine call the same code. Nav is closest (exact oracle).
5. **Console catalogue**: list the dev-build commands from `fablewin_editor_symbols.tsv` with their
   parameters (decompile the handlers), as the spec for the modern registry.
6. SCRIPTING_REDESIGN's "now" items (API table, lint, headless host) run in parallel.

**When the fork starts (reconstruction at the game loop):**

7. Console extensions, convar flags, cheat gating, launch options (`-dev`, `-mod`, `+cmd`).
8. VFS with read-only retail mounts and mod folders; field-level def merge at load.
9. Entity inspection commands and overlays (`ent_*`, `showpos`, script names, nav view).
10. Dev IPC replacing the ForgeFSE live link; FableForge talks to the engine directly.

**After the world and renderer are running in the fork:**

11. Tools mode: select/transform/place, transactions and undo, edit/play switch.
12. Resource handles and hot reload: defs, textures, TNG layer, Lua (per SCRIPTING_REDESIGN).
13. Terrain, theme, foliage and water editing with the background per-map bake; `nav_edit`.
14. Entity I/O generalization; prefabs with overrides.
15. Model/animation viewer, particle editor, cutscene timeline with lipsync generation.
16. Sandbox layer (§6), physics tools last.

---

## 10. Open questions and unverified claims

- **Retail console reachability.** `CInputProcessConsole`, `CInputProcessDebugControls` and
  `CInputProcessControlFreeCamera` exist in retail RTTI; whether any retail key or ini path opens them
  was not checked.
- **Def reload scope.** Only `CNewFrontendGameComponent::ConsoleReloadFrontEndDef` was found; whether
  the dev build could reload other def types is unknown.
- **Retail tolerance of stale bakes.** A dev loop that runs on a stale STB assumes the engine degrades
  gracefully; [TERRAIN_WHITEOUT_FIX](TERRAIN_WHITEOUT_FIX.md) shows at least one path where it does
  not.
- **`CScriptedMapBrush` at runtime.** Its presence in `CMap` suggests scripts applied brushes to live
  maps in retail-era code; whether retail TLC does so was not checked.
- **Editor global/edit mode enums** (`GlobalMode`, `EditMode` values) were not recovered; the mode list
  in §2.2 comes from the input-process members, not the enum.
- **s&box details** were read from file listings and a handful of files through the GitHub API, not a
  full clone; behaviour claims are as those files state them.
- **Lua version** for mod sandboxing and `<close>`: see SCRIPTING_REDESIGN open questions.
