# New-Level Assembly — retail engine registration & streaming (COMPLETE recipe)

*2026-07-20. Completes `docs/NEW_LEVEL_CREATION.md` (editor-side recipe) with the **retail
`Fable.exe`** side: how the shipping engine discovers/registers levels, what "mapwho" and the
LEV "brush blocks" actually are, and the full minimal file-set for a brand-new loadable region.
Evidence: headless Ghidra decompiles of retail `Fable.exe` (BSim-named) in
`ghidra_out/newlevel_retail_decomp{,2,3}.c`, `newlevel_retail_run{1..4}.log`, FableWin donor
decomp `newlevel_fablewin_run5_decomp.c`, cross-checked against the installed game data.
Addresses are retail `Fable.exe` @ base 0x400000 unless marked FableWin.*

## TL;DR

1. Retail loads the world from **`FinalAlbion.bwd`** (a compiled binary CMapInfo+CRegion
   stream), NOT the `.wld` text — gated by the **`UseCompiledWorldFiles TRUE`** console var in
   `userst.ini`. Flip it to FALSE and the engine parses the `.wld` text instead. Either
   registering path works; a new level needs its record in whichever one is active.
2. **mapwho is not a file block.** It is the runtime spatial-hash (`NThingMap::CThingMapManager`)
   built per map after world load. Nothing to author.
3. The LEV "brush blocks" are the second half of the LEV container (brush-def table + per-cell
   obstacle records) read by `CMap::LoadFromFile`; a cloned retail LEV carries them verbatim and
   an **empty TNG ("`Version 2;`") and a navless LEV are both shipping-valid** (retail precedent).
4. Minimal new-region file set = WLD map+region records (text and/or BWD), LEV+TNG in the WAD.
   No script.bin, no .qst, no .gtg changes required.
5. **A per-level entry in `FinalAlbion_RT.stb` is MANDATORY** — a registered map missing
   from the STB common header is a guaranteed CTD at world construction (§9; learned the
   hard way from the stage-1 live test).
6. Static experiment: `work/newlevel_experiment/stage/` (stage 1, no STB entry) **crashed
   live at save-load**; corrected set with rebuilt STB is `work/newlevel_experiment/stage2/`
   — statically validated, **awaits a live game test.**

---

## 1. Retail world/level load chain (decompile-confirmed)

```
CNewFrontendGameComponent::Run (0x42ec7c)  — picks "FinalAlbion.wld" (SetLevel in userst.ini)
  -> CWorld::Load(CWideString&) (0x4a1840)
       1. "Load Quests":  <world>.qst  -> LoadQuestsFromFile(file, initial=true)
                          Data\Levels\GlobalQuests.qst -> LoadQuestsFromFile(file, false)
       2. "Startup WAD":  if UseLevelWAD (DAT_01375456): CWorldMap::CreateWorldLevelWad (0x502170)
                          -> opens Data\Levels\FinalAlbion.wad as the level source bank
       3. "Set Static Map for Engine": CWorld::ConvertWorldToISMThingyPath (0x49d550)
                          -> <world>_RT.stb (+ <world>_RT.ism path variants) for the engine
       4. CWorldMap::LoadFromFile(CAFile&, long) (0x507c30)   <-- world registration
  ...
CWorldMap::LoadFromFile (0x507c30):
  if (UseCompiledWorldFiles == 0)        // DAT_013b8618, console var (userst.ini: TRUE)
      parse .wld TEXT with CStringParser (token loop, see §2)
      if (AllowDataGeneration)           // DAT_01375459, userst.ini: FALSE
          SaveWorldToBinaryFile(<world>.bwd) (0x4fbde0)
          CompileLoadableGlobalThingsToSingleFile(<world>.gtg) (0x4fe030)
  else
      LoadWorldFromBinaryFile(<world>.bwd)  (FUN_00507650)     // see §3
  NThingMap::CThingMapManager::Init(world, mapCount) (0x638540) // mapwho init, §5
  "Load GTNG": if loose <world>.gtng exists -> CThingManager::LoadFromFile + ActivateThings
  "Load global things":
      if (UseCompiledGlobalThings)       // DAT_013b8609, userst.ini: TRUE
          LoadAllLoadableGlobalThingsFromSingleFile(<world>.gtg) (0x4fe2a0)
      else LoadLoadableGlobalThings()    // per-level loose *_global tng path (0x4fdbc0)
  CWorldMap::LoadRegionGraph (0x506d40)  // Data\Misc\<world>_StartingRegionGraph.txt
```

Console variables (registered in `InitialiseConsoleVariables` 0x413c50, decompile-named):
| var | address | retail value (userst.ini) | effect |
|---|---|---|---|
| `UseCompiledWorldFiles` | DAT_013b8618 | **TRUE** | world map from `.bwd`, `.wld` text ignored |
| `UseCompiledGlobalThings` | DAT_013b8609 | **TRUE** | global things from `.gtg` |
| `AllowDataGeneration` | DAT_01375459 | **FALSE** | never regenerates `.bwd`/`.gtg` |
| `UseLevelWAD` | DAT_01375456 | **TRUE** | LEV+TNG read from `FinalAlbion.wad`, not loose files |

**Modder consequence (decompile-confirmed):** with the retail `userst.ini`, editing only the
`.wld` text does NOTHING — the engine reads `FinalAlbion.bwd`. Either (a) regenerate the `.bwd`
(format fully cracked, §3, `tools/wld_bwd.py`, byte-exact oracle), or (b) set
`UseCompiledWorldFiles FALSE` in `userst.ini` so the text is parsed. Ship (a) for end users.

## 2. Text `.wld` parsing (retail, decompile-confirmed)

The token loop in `CWorldMap::LoadFromFile` handles exactly: `MapUIDCount`,
`ThingManagerUIDCount`, `NewMap <slot>`, `MapX`, `MapY`, `LevelName "..."`,
`LevelScriptName "..."`, `MapUID`, `IsSea`, `LoadedOnPlayerProximity`, `EndMap`, `NewRegion`,
`RegionName`, `NewDisplayName`, `RegionDef`, `ContainsMap`, `SeesMap`, `AppearOnWorldMap`,
`MiniMapGraphic`, `MiniMapScale`, `MiniMapOffsetX/Y`, `MiniMapRegionExitTextOffsetX/Y`
(unknown tokens are skipped line-wise). Notes:

- **`NewMap <n>` is the 1-based map SLOT index** (unique per map, 1..398 in retail —
  `NEW_LEVEL_CREATION.md`'s "NewMap 1 for all" reading was wrong). The CMapInfo array is grown
  to fit; the slot number is what regions, `.gtg` `NEWMAP <n>` sections, and the engine use.
- On `EndMap` (0x5083a9 block): fills the CMapInfo slot; **the box extent is derived by opening
  the LEV** (`CWorldMap::GetLevelDimensions` 0x506e30) — box = (MapX, MapY, MapX+levW, MapY+levH).
  Then `IsMapPlacementPositionFree(box)`; if occupied the map is *dropped* (slot marked unused);
  if `MapUID == 0` one is allocated (`AllocateMapUID` 0x4fce20); then
  `SetMapPlacement` + `CEngineManager::AddMap(slot)`.
- `START_INITIAL_QUESTS` **is not parsed by retail** (string absent from Fable.exe — raw-scan
  confirmed). Initial quests come from `<world>.qst` (`AddQuest("...", TRUE/FALSE)` lines,
  `CWorld::Load` step 1). The wld header block is editor-side only.

## 3. Compiled world map `FinalAlbion.bwd` (format CRACKED, byte-exact)

`LoadWorldFromBinaryFile` (FUN_00507650) + `CMapInfo::LoadBinary` (0x4fb4f0) +
`CRegion::LoadBinary` (FableWin 0x1f70940 donor). Full layout in `tools/wld_bwd.py` header.
**Oracle: parse+serialize of the retail 64,223-byte `FinalAlbion.bwd` is byte-identical.**
Shape: `u32 mapCount(=slots, slot 0 never stored)`, 398 × CMapInfo records, `u32 regionCount`,
141 × CRegion records. CMapInfo = levelName, scriptName, u8 used, u8 loadedOnProximity,
u8 isSea, i32 L,R,T,B, u8 flag2, u64 mapUID. CRegion = contains[]/sees[] (i32 map slots),
name/displayName/regionDef/miniMapGraphic strings, 3 bools, minimap floats/offsets, exit list.
After loading, the engine runs `SetMapPlacement` + `CEngineManager::AddMap` per used slot —
no LEV probing, no UID allocation: the .bwd is authoritative.

## 4. Level streaming (LEV/TNG load, decompile-confirmed)

- `CWorldMap::LoadLevel(slot, ELoadType)` (0x502620): builds the two filenames
  (`GetThingFilenameFromLevelFilename` swaps .lev→.tng), opens **both from the WAD bank**
  (`CWADFile(this+0xa0)`) when `this[0xa8]` (UseLevelWAD) is set, else from loose disk files
  (`CDiskFileWin32`). So a new level's LEV **and** TNG must be inside `FinalAlbion.wad` in the
  retail config.
- `CMap::LoadFromFile(CAFile&, ...)` (0x821a80) reads the LEV: 25-B header
  (`CReplaceSerialise<CFileFormatEditMapBrushHeader,...>` — the header IS the "brush header",
  carrying obsOffset/navOffset), seek, then one 0x10822-byte
  `CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>` block =
  22-B map header + 256×132 ground-theme table + u32 cellVersion + u32 themeCount +
  **256×132 second def table (the "brush"/obstacle def palette)** + u32; then themeCount-1
  theme strings (resolved via `CDefinitionManager::GetDefGlobalIndexFromName` — unresolvable
  names degrade to index 1/walk-default, not a crash); then (w+1)×(h+1) 21-B cells
  (`CReplaceSerialise<CFileFormatHeightMapCell,COldFileFormatHeightMapCell6>`); then, when
  cellVersion≥2, **w×h per-cell obstacle records** `{u32 len, (len-4) payload}` decoded into
  7-B runtime cells indexed against the second def table. This region is what
  `CMap::SaveBrushesToFile` (FableWin 0x2236900) writes.
- **Navmesh**: `CWorldMap::CreateNavMap` (0x500230) re-reads the LEV header; `navOffset == 0`
  → nav map registered with empty section handling; 249/398 retail LEVs are navless
  (docs/NAVMESH_RE.md). A cloned LEV keeps whatever it had.
- **Minimal/empty validity verdicts:** empty TNG = `"Version 2;"` (Darkwood_Filler_08 ships
  exactly that, 12 bytes). Navless LEV = shipping-valid. Brush/obstacle region: preserved
  verbatim by clone (`tools/lev_rw.py` treats it as opaque suffix; 399/399 byte-exact). A
  *from-scratch* minimal brush region is NOT yet proven — clone one.

## 5. mapwho — runtime only, nothing to author (decompile-confirmed)

`NThingMap::CThingMapManager::Init(world, mapCount)` (0x638540) is called by
`CWorldMap::LoadFromFile` after map registration; `AddGameMap(slot)` (0x638310) /
`AddGameMapWithArea` (0x6381a0) build a `CThingMap` over the CMapInfo box. Things carry a
`CTCMapwho` component and are inserted at spawn (`PlaceThing` 0x637fc0, `PlaceInMapwho`
0x71a710). There is **no mapwho data in any file** — the only file trace is the console debug
toggle (`ConsoleSetDrawMapwhoInfo`). The old "mapwho/brush blocks in .lev" open item conflated
this runtime structure with the LEV obstacle region (§4).

## 6. Minimal file-set checklist for a brand-new loadable region

| # | item | required? | evidence |
|---|------|-----------|----------|
| 1 | `.wld` text: `NewMap <slot>` block (MapX/Y, LevelName, LevelScriptName, MapUID, IsSea, LoadedOnPlayerProximity) | only if `UseCompiledWorldFiles FALSE` | decompile (0x507c30 text branch) |
| 2 | `.bwd`: appended CMapInfo record (slot = count) | **yes** in retail config | decompile (FUN_00507650) + byte-exact writer |
| 3 | `.wld`/`.bwd` region record (`NewRegion`, ContainsMap/SeesMap) | yes for region semantics (region-load, creature gen, sound themes); map alone loads via slot but belongs to no region | decompile (SetRegionAsLoaded 0x4fc8a0, GetRegionNumberMapIsIn) |
| 4 | `<name>.lev` inside `FinalAlbion.wad` | **yes** (UseLevelWAD TRUE) | decompile (LoadLevel 0x502620) |
| 5 | `<name>.tng` inside `FinalAlbion.wad` | **yes**; `"Version 2;"` suffices | decompile + retail precedent (Darkwood_Filler_08) |
| 6 | loose `data/Levels/FinalAlbion/<name>.tng` | no (retail config reads WAD); ship for parity | decompile (this[0xa8] branch) |
| 7 | unique `MapUID` (u64) | yes (GetMapNumberFromMapUID lookups); explicit value avoids the text-mode allocator | decompile (0x4fb920, AllocateMapUID) |
| 8 | free 2D placement box | yes in text mode (map dropped if overlapping); .bwd path does not re-check | decompile (IsMapPlacementPositionFree) |
| 9 | `.gtg` global-things section | **no** — only 151/398 retail maps have `NEWMAP <slot>` sections | data (FinalAlbion.gtg) + decompile |
| 10 | `.qst` changes | **no** — quests are per-world startup lists, not per-map | decompile (CWorld::Load) |
| 11 | `script.bin` entry | **no** — quest logic is compiled C++ keyed by quest name (docs/QUEST_VM_RE.md); map load path never touches script.bin | decompile (whole chain) |
| 12 | `FinalAlbion_RT.stb` entry | **YES — MANDATORY. Missing entry = guaranteed CTD at world load** (§9; proven by the stage-1 crash 2026-07-20). Every registered map is opened against the STB with an *unchecked* name lookup | decompile (0xB42750→0xB41E50→0x42B467) + WER fault + stage-2 fix |
| 13 | `_StartingRegionGraph.txt` entry | **unknown/inferred** — used for inter-region routing; new region absent from graph likely means AI cannot route to it, not a load crash | inferred; OPEN |
| 14 | LEV-embedded map header uid | not the WLD MapUID (Darkwood filler stores 1, LookoutPoint 5661 vs MapUIDs 601997/162441); loader consumes without cross-check | decompile + data |
| 15 | text.big `TXT_REGION_*` display symbol | no for non-worldmap regions — retail filler regions use a plain string display name | data (141 retail regions) |

Access to the new region in-game: no retail path teleports there automatically. Options: a
region-exit thing pair in TNGs, an FSE Lua teleport, or a quest script — all existing
capabilities (out of scope here).

## 7. Experiment: ForgeTest region (STAGED, static validation passed)

`work/newlevel_experiment/assemble_forgetest.py` (tools: `tools/wld_bwd.py`,
`tools/wad_add.py`) produces `work/newlevel_experiment/stage/`:

- `Data/Levels/FinalAlbion.wld` — +`NewMap 399` (ForgeTest @ (2784,2560) 32×32, a
  verified-free hole beside Darkwood; MapUID 4193259 = retail max+1) and +`NewRegion 142`
  (ContainsMap/SeesMap itself, filler-region shape). `forge wld info`: 399 maps / 142 regions,
  **roundtrip byte-identical**.
- `Data/Levels/FinalAlbion.bwd` — regenerated with the new map+region records (64,382 bytes;
  reparse-validated; writer proven byte-exact on the retail file first).
- `Data/Levels/FinalAlbion.wad` — 798 entries (+`ForgeTest.lev` clone of Darkwood_Filler_08,
  +`ForgeTest.tng` = `"Version 2;"`). Gates: new entries extract back byte-identical; existing
  entries (spot: LookoutPoint.lev) byte-identical to source WAD.
- `Data/Levels/FinalAlbion/ForgeTest.tng` — loose copy for parity.

**Status: stage 1 FAILED live (2026-07-20)** — the game reached the save-select menu (FSE
registration OK) but **crashed on save selection** = world construction. Root cause and fix
in §9; the corrected artifact set is `work/newlevel_experiment/stage2/` (adds a rebuilt
`FinalAlbion_RT.stb`). Keep `work/smoke_test/stage/data/Levels/` as forensics for the failed
set. Stage 2 is statically validated and **awaits a live test**; not deployed.

## 9. Stage-1 crash post-mortem: the STB per-level entry is MANDATORY (2026-07-20)

**Observed:** with the 4 stage-1 files deployed, save-select menu fine, CTD on selecting a
save; reverting only those 4 files fixed it. WER Application-log: two crashes (13:09:28,
13:11:18) `Fable.exe` 0xC0000005 at **`Fable.exe+0x62428A` = VA 0xA2428A** (a third at 13:12
in `FableScriptExtender.dll+0x200599` was a follow-on FSE run, not the root).

**Faulting code (Ghidra):** 0xA24280 is a 4-instruction indexed getter (BSim-misnamed
`CTCInventoryAbilities::GetAbilityLevel`; really a def-by-global-index resolver):
`return ((long**)(this+0x380))[idx]` — AV = wild `idx`.

**Crash chain (decompile-confirmed):**
```
CWorld::Load step 3 "Set Static Map for Engine"
  -> CEngine::SetStaticMapFileForUse (0xB23DC0, vtable)
  -> CEngineWorldMap::OpenStaticMaps (0xB42750)
  -> FUN_00B420F0: reads __STATIC_MAP_COMMON_HEADER__ entry of FinalAlbion_RT.stb =
     [u32 count][count x {asciiz "Data\Levels\FinalAlbion\<name>.lev", u32 chunkOffset}]
     -> std::map<CCharString,long>; then for EVERY registered engine map:
  -> CEngineMap::OpenRetailStaticMap (0xB41E50):
       node = mapLookup(levelName)        // FUN_0042B467 = std::map lower-bound;
                                          // returns the HEAD/END node on miss
       seek(*(node+0x14))                 // NO found-check -> garbage seek for ForgeTest
       ReadMapInfoBlock (0xB3EFA0)        // garbage CStaticMapInfoBlock
  -> CLocalDetailCacheMap::OpenStaticMap (0xBDF010)
  -> CObjectTypeCollectionPalette::Load (0xBDEDD0)
  -> CLocalDetailObjectCollectionType::Load (0xBE27B0) reads garbage def indices
  -> def resolver 0xA24280 indexes defs array with wild index -> 0xC0000005.
```
So a map registered in the BWD/WLD **must** have a chunk in the STB common header; the
engine has no missing-entry guard. Suspects 2–4 (region graph, BWD new-record serialization,
WAD TOC) are exonerated for this crash.

**STB per-level chunk format** (ReadMapInfoBlock + differential diff of same-size retail
chunks; offsets relative to chunk start inside the common-header entry data):
`+0x00 ver(=1); +0x04 STB bank-entry id; +0x08 entry-relative field; +0x0C/ +0x10 w,h (cells);
+0x14/+0x18 MapX,MapY (ints); +0x20 abs offset landscape subheader (== chunk+0x5C);
+0x24 abs offset local-detail subheader (== chunk+0x71); +0x28..+0x3C entry-relative/hash;
+0x40 6 floats box minX,minY,minZ,maxX,maxY,maxZ; +0x58 abs offset chunk end.`
Landscape subheader dword0 (chunk+0x5C) and detail subheader dword0 (chunk+0x71) are also
absolute offsets (→ chunk+0x70 / chunk+0x7D). **These 5 fields are the only
position-dependent bytes in a chunk** — invariant verified on all 398 retail chunks, and
the downstream readers (CQuadTreeElement::LoadHeader 0xBDE290,
CObjectCacheGroupCollection::LoadHeader 0xBDE190, palette Load) are purely sequential.

**Fix (stage 2, `work/newlevel_experiment/assemble_forgetest_stage2.py`):** rebuild
`FinalAlbion_RT.stb` = retail bytes + (a) new bank entry id 426
`Data\Levels\FinalAlbion\ForgeTest.lev`, payload byte-cloned from `Darkwood_Filler_08.lev`;
(b) common header grown: name table 398→399 (+43 B; the chunk at the old table end,
BanditCamp_Filler_05, relocated to the tail with its 5 offsets rebased), plus a ForgeTest
chunk cloned from Darkwood_Filler_08's with entry id → 426, int coords → (2784,2560), float
box → (2784,2560)−(2816,2592), Z bounds kept (same LEV heights), 5 offsets rebased.
Header: entryCount 424→425, nextId 425→427(=426+1), tableOffset moved; common entry size
patched; all other table records byte-identical. **Static gates ALL PASS**: 423 original
payloads byte-identical at unchanged offsets; ForgeTest payload == donor payload; all 399
chunks re-parse with invariants; every WLD LevelName resolves in the STB table; 397
untouched chunks byte-identical. Independent cross-check: `forge stb list` reads the new
file as 425 entries / 399 static maps.

## 8. Open questions

1. ~~STB (`FinalAlbion_RT.stb`) — is a per-level entry mandatory?~~ **RESOLVED 2026-07-20:
   YES, mandatory — missing entry is a guaranteed 0xC0000005 at world construction (§9).
   The doc's earlier "likely benign" rating was wrong.**
2. Region-graph absence behavior (checklist #13) + regenerating the graph for new regions
   (builders mapped in docs/NAVMESH_RE.md). (Not implicated in the stage-1 crash.)
3. From-scratch minimal LEV brush/obstacle region bytes (clone workaround removes urgency).
4. Duplicate thing-UID risk when cloning a TNG *with* things (ForgeTest avoids it: 0 things).
   `AllocateMapThingUID` (0x4fb330) UIDs are top-down from 0xFFFFFE00_00000000; a UID-remap
   pass on cloned TNGs is the clean fix.
5. Live-load verdict for the whole stage (the only remaining gate for "brand-new regions are
   possible").
