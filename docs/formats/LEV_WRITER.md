# LEV terrain reader + WRITER (`tools/lev_rw.py`)

*Created 2026-07-19. Route B (native, dependency-free) LEV read+write, mirroring the
mesh writer pattern in `tools/blender_addon/io_scene_fable/mesh_rw.py`. Concrete blocker
cleared for terrain editing in Blender (CONTENT_AUTHORING_PLAN.md B3; CAPABILITY_MAP
"Level terrain [ready] — needs a LEV writer").*

## What this delivers

`tools/lev_rw.py` — pure stdlib Python. Reads a `.lev` into a struct, serializes it back
**byte-identical** for the edit-nothing case, and exposes an edit API for cell height,
theme blend, and walkability. The obstacle/brush region and the entire navigation block
are preserved **verbatim** (opaque bytes) — this writer edits existing cell data only, it
does not regenerate nav/region/brush data (that is the `CMap::SaveToFile` Ghidra job,
left out of scope).

## Byte layout (confirmed)

Cross-checked against **3 sources** (rule: ≥2):
1. FableForge `forge::lev` — `D:\Code\FableForge\libs\forgecore\src\lev.cpp` + `lev.hpp`.
2. The decompiled **FableMod.LEV** reader that `lev.hpp` cites as the theme-grid ground
   truth (ChocolateBox `DecompiledDLLs`, used by `SilverChest.LevBridge`).
3. A scan of **all 398 real `FinalAlbion` LEVs** extracted from the retail
   `FinalAlbion.wad` — every one parses with 0 failures under this layout, and every one
   round-trips byte-identical (see oracle below).

Little-endian throughout. `ImageBase`-independent (pure file format).

```
LEVHeader    (25 B): u32 headerSize(=25), u16 version(=6404), u8 pad[3],
                     u32 reserved1, u32 obsOffset, u32 reserved2, u32 navOffset
LEVMapHeader (22 B): u8 size, u8 mapVersion(=8), u8 mpad[3], u32 uidLo, u32 uidHi,
                     i32 width, i32 height, u8 flag
                     — mpad[2] is the sub-version: 8 (1 file) or 9 (397 files)
256 ground themes  : char name[128] + u32 value                    (256 * 132 = 33792 B)
u32 cellVersion, u32 themeCount
u8  palette[33792]                                                 (opaque, preserved)
u32 extra                                                          (sub-version 9 ONLY)
(themeCount-1) × { u32 len, char name[len] }                       (theme strings)
--- cell grid (the EDITABLE payload) ---
cells: (height+1) × (width+1) cells, row-major, stride (width+1), each 21 B:
        +0 .. +4   (5 B, unclassified — preserved)
        +5         float32 heightRaw   (world height = heightRaw * 2048)
        +6 .. +9   (rest of the float)
        +10,+11,+12  u8 theme index, blend layers 0/1/2
        +13,+14      u8 blend strength, slots 0/1 (slot-2 is implicit
                     max(0, 255-strength0-strength1))
        +15          u8 walkable (nonzero = walkable)
        +16 .. +20 (5 B, unclassified — preserved)
--- opaque suffix (preserved verbatim) ---
obstacle/brush region up to navOffset (obsOffset points inside it)
navigation block from navOffset: 8 B nav header (last u32 = sectionCount),
    then sectionCount × { u32 len, char name[len], u32 offset }, then section payloads
```

Sub-version note: sub-8 has no `extra` u32 after the palette; sub-9 has it. Both handled.
`cellVersion` observed = 3. `heightRaw * 2048` matches FableMod/FableForge/SilverChest.

## Writer strategy (why it is byte-exact)

The file is captured as three spans:

- **prefix** — everything before the cell grid, fully **parsed into named fields** and
  re-emitted from those fields. Reconstruction is proven byte-identical to the source
  prefix by the oracle (so header/theme edits are possible later without guesswork).
- **cells** — the `(h+1)×(w+1)×21`-byte grid, held as a mutable `bytearray`; edits write
  through in place, so cell size and therefore `navOffset`/`obsOffset` stay valid.
- **suffix** — obstacle/brush region + nav TOC + section payloads, kept **verbatim**.

`serialize(lev) == prefix_bytes + cells + suffix`. Because LEV is fixed-layout with no
compression, offsets, or checksums that depend on cell contents, editing cell fields
never invalidates the opaque spans.

## Edit API

```python
import lev_rw
lev = lev_rw.read_file("BarrowFields.lev")

lev.set_height(x, y, world_height)     # world units; stored as world/2048
lev.set_walkable(x, y, True/False)     # writes the +15 byte
lev.set_theme(x, y, layer, index, strength=None)  # layer 0..2; stored strength for 0..1

c = lev.cell(x, y)                     # live view into the 21-byte record
c.height        # world-space height   (c.height_raw for the stored float)
c.walkable      # bool                  (c.walkable_byte for the raw byte)
c.theme_index(layer); c.theme_strength(layer)

lev_rw.write_file(lev, "edited.lev")
```

Grid is `lev.cells_x × lev.cells_y` = `(width+1) × (height+1)`; `lev.cell_count` cells.
Coordinates are `(x, y)` row-major. Out-of-range raises `IndexError`.

CLI: `python tools/lev_rw.py info|roundtrip|edittest <file.lev> [more.lev ...]`.

## Oracle results (the gate)

`roundtrip_report(bytes)` proves `serialize(parse(x)) == x`; on any mismatch it reports
the first differing byte offset and whether it lands in prefix/cells/suffix.

- **BarrowFields.lev** (project sample, 643,719 B, sub-9, 128×128) — **byte-identical**.
- **All 398 retail `FinalAlbion` LEVs** + BarrowFields = **399/399 byte-identical**,
  covering both sub-version 8 (1 file) and sub-version 9 (398 files). 0 failures.
- **Disk round-trip**: `read_file → write_file` of an unedited LEV produces a
  byte-identical file on disk.
- **Semantic edit test**: bumping one cell's height changes **exactly** that cell's 4
  height bytes (offsets verified) and re-reading returns the new value. A combined
  height+walkable+theme edit on one cell changes **only** that cell's 21-byte span
  (7 bytes, all within the cell).
- **Cross-tool**: the independent C++ reader `forge lev info` reads our edited file
  cleanly (correct dimensions, walkable counts, theme table).

Reproduce:
```
python tools/lev_rw.py roundtrip work/level_samples/BarrowFields/BarrowFields.lev
python tools/lev_rw.py edittest  work/level_samples/BarrowFields/BarrowFields.lev
```

## Honest gaps / out of scope

- **Cell byte +20 is now classified** (2026-07-20, navmesh RE): it is the per-cell
  **preferred-path flag** — navmesh TopologyWeights = 0xFF if `+15`==0, else 0x00 if `+20`==1
  (preferred, A* cost 0), else 0x80 (normal). Corpus-validated with 0 exceptions over all 149
  nav-bearing retail LEVs (201,951 cells). See `docs/engine/NAVIGATION.md`.
- **Cell bytes +0..+4 and +16..+19 remain unclassified** (9 of 21 bytes). They are
  preserved verbatim and untouched by the edit API, so byte-exactness holds, but their
  meaning is not yet reversed. FableForge doesn't read them either. Classifying them is a
  future read-side task, not a blocker for height/theme/walkable editing.
- **Nav mesh / region graph / mapwho / obstacle-brush data are NOT regenerated.** They are
  preserved opaque. Editing terrain heights does **not** update navmesh walkability or
  obstacle occupancy — the engine's runtime nav will still reflect the original terrain.
  For gameplay-correct large terrain edits, the nav rebuild (`CMap::SaveToFile` /
  `CEditWorldMap` region builders, see the level-editor recipe below) is the follow-on Ghidra job.
- **Grid resizing is not supported.** Changing `width`/`height` would move `navOffset`/
  `obsOffset` and require rewriting the opaque spans; the writer asserts nothing about
  this but the API offers no resize. Brand-new-level creation
  (`CEditWorldMap::CreateAndSaveNewLevel`) is explicitly out of scope.
- `subver9_extra`, `reserved1`, `reserved2`, `palette`, and `flag` are preserved but not
  interpreted.

## Files

- `tools/lev_rw.py` — reader + writer + edit API + oracle (`roundtrip_report`).
- `docs/formats/LEV_WRITER.md` — this document.
- Cross-check sources: `D:\Code\FableForge\libs\forgecore\src\lev.cpp`,
  `docs/formats/LEVEL_CONTAINER_INDEX.md`, retail `FinalAlbion.wad` (398 LEVs).

## Verified facts (from FINDINGS log)

- **2026-08-21 — Map origins are 32-aligned across all 398 retail maps (2026-08-21).** Measured via
  `forge world inspect` on pristine retail `FinalAlbion.wld`/`.bwd`: 0 off-grid at mod-32, 0 at mod-16.
  - `ForgeTest64` had been registered at `(3328, 2296)`; `2296 % 32 = 24`, `2296 % 16 = 8`. An
    off-grid origin slides every 16-cell landscape patch off the patch grid
    (`forge stb foregroundinfo --verify-topology`, same LEV + donor chunk): origin (3328, 2296) ->
    shift dy = -264 -> topology 25/105 exact; origin (3328, 2304) -> dy = -256 (16x16) -> 105/105.
    Control: untouched donor chunk 105/105.
  - Open question (not a claim): `CEngineLandscapeMap::OpenStaticMap` (0x00BDD0E0) faults at
    0xBDD1D3 on the landscape-block-header allocation size (`field_04` at `this+0x28`); off-grid
    origin is a plausible but undemonstrated source. Evidence
    `work/terrain_runtime_probe_20260821/README.md`.

- **2026-07-19 — Level-editor save/create recipe (FableWin editor RE).** `DecompDefTransfers.java`
  batch-decompiled 50 `CEdit*` editor functions (`ghidra_out/leveleditor_decomp.c`, 50/50 clean;
  targets `leveleditor_targets.tsv`). **New level** — `CEditWorldMap::CreateAndSaveNewLevel(name:
  CWideString, coord:C2DCoordI, mapInit:CMapInit)` (`0x0296D2D0`): adds a `CMapInfo` to the world-map
  `CArray<CMapInfo>`, initialises its 2D bounding box (`C2DBox::Initialise`) at the given world
  coordinate, sets map flags `+0x2c`/`+0x2d`, allocates the 0x1D78-byte map object → a new level = a
  new `CMapInfo` in the WLD at a free 2D coord + bbox, plus LEV+TNG. **Save level** —
  `CEditWorldMap::SaveSingleLevelToFiles` (`0x0296E8B0`): (1) TNG via `CThingManager::SaveToScript`,
  (2) LEV via `CMap::SaveToFile(file, CFileFormatLevel)` through
  `CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>` (matches decoded LEV version 6404),
  (3) brush/terrain paint data. `CMap::SaveToFile` is the authoritative LEV writer to mirror.
  **Regions / navmesh graph** — `CEditWorldMap::CreateNewRegion` (`0x0296D920`) maintains parallel
  `vector<CRegion>` / `vector<CEditRegion>` (asserts equal size). Donor = FableWin.exe.
