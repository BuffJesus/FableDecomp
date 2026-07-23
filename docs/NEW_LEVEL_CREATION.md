# New-Level Creation Recipe (Lionhead editor RE)

> **2026-07-20:** the RETAIL-engine side (level discovery/registration, `.bwd` compiled world
> map, mapwho, LEV brush blocks, minimal file-set checklist, staged ForgeTest experiment) is
> completed in **`docs/NEW_LEVEL_ASSEMBLY.md`**. Note: `NewMap <n>` in the `.wld` is the 1-based
> map SLOT index (unique per map), and retail ignores the `.wld` text entirely unless
> `UseCompiledWorldFiles FALSE` is set in `userst.ini` (it loads `FinalAlbion.bwd`).

*Reverse-engineered from **FableWin.exe** (Lionhead editor debug build, PDB-named) in the Ghidra
project `ghidra_proj/FableTLC`. All addresses are FableWin ImageBase-`0x400000` addresses unless
marked "retail". Cross-checked against the on-disk `FinalAlbion.wld` in the installed game and against
FableForge's `forge::wld` reader.*

Raw decompiles: `ghidra_out/decomp_newlevel.c` (12 functions). Companion nav/region doc:
`docs/NAVMESH_RE.md`.

## TL;DR — what "add a new level" means

A level is one entry in three coordinated containers, keyed by a **level filename** and a **u64
map UID**:

1. A **`.lev` file** (terrain + placed things + navmesh) written by the editor.
2. A **`.tng` file** (the thing manager script) written alongside the `.lev`.
3. A **`CMapInfo` record** appended to the world map (`FinalAlbion.wld`) giving the level a **2D grid
   coordinate**, a **bounding box** (in map cells), a **display/script name**, and the **UID**.

Region membership, minimaps, and the region-connectivity graph are separate registration steps (see
"Region registration" and `docs/NAVMESH_RE.md`).

## Editor call chain (verified)

```
CEditInputProcessMapPlacement::CreateAndSaveNewLevel(C2DBoxI& box)   FableWin 0x02952140
  -> GetSaveFileNameW  (user picks "<name>.lev" in the Level dir)
  -> CMapInit mi;                        // ctor 0x029524a0 : zeroes 2 dwords
     mi.width  = box.GetWidth();         // stored at CMapInit+0x00
     mi.height = box.GetHeight();        // stored at CMapInit+0x04
  -> CEditControlCentre::CreateAndSaveNewLevel(fileName, box.GetTLPos(), mi)   0x0204c3c0
       -> CEditWorldMap::CreateAndSaveNewLevel(fileName, C2DCoordI tlPos, CMapInit mi)  0x0296d2d0
       -> CEditControlCentre::InitRegionDialog()
```

`box` is a `C2DBoxI` in **world-map cell units** (the same integer grid as `MapX`/`MapY` in the WLD).
`GetTLPos()` returns the top-left `C2DCoordI`; width/height come from the box.

## `CEditWorldMap::CreateAndSaveNewLevel` — the core recipe (0x0296d2d0)

Decompiled body (`ghidra_out/decomp_newlevel.c` line ~1009). Step by step:

1. **Allocate a map slot** and mark it used:
   - `wideName.ToCharString()` → the CharString filename.
   - `CArray<CMapInfo>::operator[](slot)` — new `CMapInfo`; sets `+0x2c = 1` (**Used** flag).
2. **Set the bounding box** on the CMapInfo:
   - `C2DBoxI::Initialise(tlPos.x, tlPos.y, tlPos.x + mi.width, tlPos.y + mi.height)` — i.e. left,
     top, right, bottom from the 2D coord + `CMapInit` width/height (`0x0296d392`/`0x0296d399`).
   - sets `+0x2d = 1` (a second bool flag, persisted).
3. **Create the map object** (`operator new(0x1d78)` = a `CEditMap`):
   - `CEditMap::CEditMap(GameDefinitionManager&, CWorld&, worldSeed*, C2DCoordI&, CMapInit&)`
     (`0x0296d3f5`) — builds an empty map using `CWorld::GetWorldSeed()` and
     `CWorld::PeekDefinitionManager()`.
   - stored via `CCountedPointer<CMap>::reset(...)`.
4. **Allocate the UID**: `CWorldMap::AllocateMapUID(slot)` (retail-equiv `AllocateMapUID` FableWin
   `0x01c950e0`); result written to `CMapInfo+0x28` (the u64 **MapUID**; `MapUIDCount` in the WLD is
   the allocator's high-water mark).
5. **Register the map into the thing/engine managers**:
   - `CThingMapManager::AddGameMap(slot)` (`0x0296d45e`).
   - `CAFile::SplitPath(...)` to derive the level/script name string into `CMapInfo+0x20`.
   - `vector<CEditMapInfo>::operator[](slot) = 0` (editor-side per-map flags).
   - `CArray<long>::push_back(slot)` — the visible-map list.
   - `CWorldMap::SetMapPlacement(box, slot)` (`0x0296d667`; retail-equiv `SetMapPlacement`) — records
     the placement box in the world-map spatial index.
   - `CEngineManager::AddMap(slot)` (`0x0296d677`).
6. **Save the level to disk**: `CEditWorldMap::SaveSingleLevel(slot, wideName, false, false)`
   (`0x0296d6a2`), then `CWorldMap::UnloadLevel(slot)`.

The `.wld` itself is NOT rewritten here — that happens when the editor saves the world map
(`CEditWorldMap::SaveToFile`, below). `CreateAndSaveNewLevel` mutates the in-memory `CMapInfo`
array + writes the `.lev`/`.tng`.

## What `SaveSingleLevel` / `SaveSingleLevelToFiles` write (the `.lev` and `.tng`)

`CEditWorldMap::SaveSingleLevel(slot, wideName, bool, bool)` (0x0296e150) resolves the paths and
optionally makes `.backup` copies (when the editor's "save to backup files" flag `this+0x17d` is set),
opens two in-memory `CMemoryFile`s, then calls:

`CEditWorldMap::SaveSingleLevelToFiles(slot, CCountedPointer<CAFile> levFile, tngFile)` (0x0296e8b0),
which writes, **in this order**:

| # | Producer | Target | Content |
|---|----------|--------|---------|
| 1 | `CThingManager::SaveToScript(script, slot, log)` | **`.tng` file** | the thing-manager text script (all placed things) |
| 2 | `CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::SaveToFile` | `.lev` | LEV format header/version block |
| 3 | `CMap::SaveToFile(CAFile&, CFileFormatLevel&)`  (0x02234c60) | `.lev` | map/terrain geometry (the LEV cell grid we already decode) |
| 4 | `CMap::SaveBrushesToFile(CAFile&, CFileFormatLevel&)` (0x02236900) | `.lev` | terrain brush layer |
| 5 | `CWorldMap::SaveNavigationMapToFile(slot, CAFile&, CFileFormatLevel&, sectionNames)` (0x01c8e300) | `.lev` | **navmesh** section index + embedded nav quad-tree(s) — see `docs/NAVMESH_RE.md` |

So the **navmesh is embedded inside the `.lev` file**, appended after the terrain/brush blocks, and
the `CFileFormatLevel` object carries a small section table (`+0x15` = a file offset it back-patches).
`SaveSingleLevelToFiles` returns a `CEditLevelSaveInfo` = the two written byte-lengths (TNG delta,
LEV delta).

**Editor takeaway:** the LEV is a container with ordered sections. A `forge level new` that only
writes terrain will still load, but the engine expects the nav section to exist for pathing (the
editor always writes it via step 5). See NAVMESH_RE for the section layout.

## `CMapInfo` record layout (WLD map entry) — binary + text confirmed

`CMapInfo::SaveBinary(CDataOutputStream&)` (0x01c9fcf0) and `LoadBinary` (0x01c9fb80) fix the field
set and order. Offsets are into the `CMapInfo` object (dword-indexed in the decompile):

| Object offset | SaveBinary primitive | Meaning | WLD text key |
|---|---|---|---|
| `+0x18` | `WritePresizedString` | level filename (e.g. `FinalAlbion\Foo.lev`) | `LevelName` |
| `+0x20` | `WritePresizedString` | level script name (e.g. `Foo`) | `LevelScriptName` |
| `+0x2c` | `WriteEBOOL` | **Used** flag | (implicit: `NewMap`/`EndMap` present) |
| `+0x30` | `WriteEBOOL` | flag (sea) | `IsSea` |
| `+0x50` | `WriteEBOOL` | flag (proximity load) | `LoadedOnPlayerProximity` |
| `+0x00` | `WriteSLONG` | box left  | `MapX` |
| `+0x08` | `WriteSLONG` | box right | (derived; width = right-left) |
| `+0x04` | `WriteSLONG` | box top   | `MapY` |
| `+0x0c` | `WriteSLONG` | box bottom | (derived; height = bottom-top) |
| `+0x2d` | `WriteEBOOL` | flag | (editor-internal) |
| `+0x28` | `WriteUHUGE` (u64) | **MapUID** | `MapUID` |

> Note: the shipping `FinalAlbion.wld` on disk is the **text** form (below). `SaveBinary` is used for
> in-memory streams / other paths but proves the exact field set. The text emitter is
> `CWorldMap::SaveMapsToString` (FableWin 0x01c7d540), called by `CEditWorldMap::SaveToFile`.

## The `.wld` text format (verified against `FinalAlbion.wld` on disk)

Header of the installed `data/Levels/FinalAlbion.wld` (read directly), then per-map blocks:

```
START_INITIAL_QUESTS;
Q_SunnyvaleMaster;
...
END_INITIAL_QUESTS;

MapUIDCount 72;              // AllocateMapUID high-water mark
ThingManagerUIDCount 1;

NewMap 1;
MapX 3232;                  // C2DCoordI.x  == CMapInfo box left
MapY 3488;                  // C2DCoordI.y  == CMapInfo box top
LevelName "FinalAlbion\LookoutPoint.lev";
LevelScriptName "LookoutPoint";
MapUID 162441;              // u64 UID
IsSea FALSE;
LoadedOnPlayerProximity TRUE;
EndMap;
... (398 maps) ...

NewRegion 1;
RegionName "...";
NewDisplayName "TXT_REGION_...";
RegionDef "REGION_...";
AppearOnWorldMap;           // present only if flagged
ContainsMap "FinalAlbion\...lev";
SeesMap "FinalAlbion\...lev";
...
EndRegion;
```

This grammar is exactly what FableForge's `forge::wld` reader parses
(`D:\Code\FableForge\libs\forgecore\src\wld.cpp`: keys `NewMap/MapX/MapY/LevelName/LevelScriptName/
MapUID/IsSea/LoadedOnPlayerProximity/EndMap`, and `NewRegion/RegionName/NewDisplayName/RegionDef/
AppearOnWorldMap/ContainsMap/SeesMap/EndRegion`). The region-block emitter is
`CEditWorldMap::EditSaveRegionsToString` (FableWin 0x0296b850, in `ghidra_out/leveleditor_decomp.c`),
which also writes minimap fields (`MiniMapGraphic`, `MiniMapScale`, `MiniMapOffset`,
`WorldMapOffset`, `NameGraphicOffset`, per-exit `MiniMapRegionExitTextOffset`) — those keys exist in
the engine but the shipped `FinalAlbion.wld` uses the compact subset above.

## Region registration (separate from map creation)

- `CEditWorldMap::CreateNewRegion()` (0x0296d920) appends a `CRegion` + `CEditRegion` pair (reuses a
  freed slot if one is not `IsUsedByEditor`). A region carries name, display name, region-def, the
  minimap block, and two map lists: **ContainsMaps** (member levels) and **SeesMaps** (visibility).
- A new level is put in a region by adding its filename to that region's `ContainsMap` list; the
  `SeesMap` list is the loaded-visibility set. `EditSaveRegionsToString` serializes both.
- The **region-connectivity graph** (routing between regions, for AI/quest travel) is a *separate
  artifact* generated from region-exit things and stored as a text file — see `docs/NAVMESH_RE.md`
  ("Region connectivity graph").

## Reproducible recipe for `forge level new <name> --at X Y --size W H`

To add a brand-new level the way the editor does:

1. **Pick a unique UID** = `wld.MapUIDCount` (or max existing +1); bump `MapUIDCount`.
2. **Append a WLD map block** with `MapX=X`, `MapY=Y`, `LevelName="FinalAlbion\<name>.lev"`,
   `LevelScriptName="<name>"`, `MapUID=<uid>`, `IsSea FALSE`, `LoadedOnPlayerProximity TRUE`.
   (`x,y` are the world-map cell coordinates; W/H set the box extent used by `SetMapPlacement`.)
3. **Write `<name>.lev`** with the section order in the table above. Minimum viable = a valid
   `CFileFormatLevel` header + `CMap` terrain grid (the LEV layout FableForge already round-trips) +
   an (optionally empty/degenerate) navigation section (NAVMESH_RE gives the byte layout so it can be
   emitted rather than left absent).
4. **Write `<name>.tng`** = a thing-manager script (FableForge already round-trips TNG byte-exact; a
   new level can start from a minimal/empty thing set).
5. **Register into the archives**: add `<name>.lev` + `<name>.tng` to `FinalAlbion.wad` (WAD repack —
   FableForge has this) and add the `.lev` to `FinalAlbion_RT.stb` (static-map bank). All 398 WLD
   maps resolve to a WAD LEV+TNG and an STB LEV today; a new map must satisfy the same three-way
   consistency `forge validate` checks.
6. **Optional**: add the level to a region (`ContainsMap`) and regenerate the region graph (below).

### Honest gaps for step 3/4

- `CMap::SaveToFile` (terrain) and the `CFileFormatLevel` section framing are **not fully byte-mapped
  here** — FableForge already reads/round-trips the shipped LEV cell grid, so the terrain body is
  known; what is *not* yet proven is the exact section-table framing the editor emits for a
  *from-scratch* LEV (offsets/order of the `CReplaceSerialise` header vs `CMap` vs brushes vs nav).
  The producer order (table above) is proven; the precise header bytes need a diff of an
  editor-saved fresh LEV vs a hand-built one.
- `CThingManager::SaveToScript` output = the TNG text FableForge already parses; a minimal new TNG is
  low-risk.

## Cited addresses (FableWin, base 0x400000)

| Symbol | Addr |
|---|---|
| `CEditInputProcessMapPlacement::CreateAndSaveNewLevel(C2DBoxI&)` | `0x02952140` |
| `CEditControlCentre::CreateAndSaveNewLevel(...)` | `0x0204c3c0` |
| `CEditWorldMap::CreateAndSaveNewLevel(CWideString, C2DCoordI&, CMapInit&)` | `0x0296d2d0` |
| `CEditWorldMap::CreateNewRegion()` | `0x0296d920` |
| `CEditWorldMap::SaveSingleLevel(...)` | `0x0296e150` |
| `CEditWorldMap::SaveSingleLevelToFiles(...)` | `0x0296e8b0` |
| `CEditWorldMap::SaveToFile(CAFile&)` | `0x0296b5e0` |
| `CEditWorldMap::EditSaveRegionsToString(CCharString&)` | `0x0296b850` |
| `CEditWorldMap::AddLevelFromFileWithUID(...)` | `0x0296ade0` |
| `CMapInit::CMapInit()` | `0x029524a0` |
| `CMapInfo::SaveBinary(CDataOutputStream&)` | `0x01c9fcf0` |
| `CMapInfo::LoadBinary(CDataInputStream&)` | `0x01c9fb80` |
| `CWorldMap::SaveMapsToString(CCharString&)` | `0x01c7d540` |
| `CWorldMap::SetMapPlacement(C2DBoxI&, long)` | `0x01c8bb50` |
| `CWorldMap::AllocateMapUID(long)` | `0x01c950e0` |
| `CMap::SaveToFile(CAFile&, CFileFormatLevel&)` | `0x02234c60` |
| `CMap::SaveBrushesToFile(CAFile&, CFileFormatLevel&)` | `0x02236900` |
| `CMap::LoadFromFile(CAFile&, CASuspendableProcess*)` | `0x022327b0` |
| `CWorldMap::SaveNavigationMapToFile(...)` | `0x01c8e300` |
| `NResourceDirectoryNames::GetLevelDir()` | `0x01912a40` |
| `NResourceDirectoryNames::GetMiscDir()` | `0x01912ac0` |
| `CWorldMap::GetThingFilenameFromLevelFilename(CWideString&)` | `0x01c794c0` |

Evidence: `ghidra_out/decomp_newlevel.c`, `ghidra_out/leveleditor_decomp.c`,
installed `data/Levels/FinalAlbion.wld`, `D:\Code\FableForge\libs\forgecore\src\wld.cpp`.
