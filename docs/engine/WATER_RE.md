# Water — how retail decides, bakes and draws it (2026-09-19)

Question answered: *can FableForge add water to a map, and can the debug build tell us how?*
Yes on both. Everything below is read from `FableWin.exe` (debug build, full PDB names) plus the
retail label set; decompiles in `ghidra_out/decomp_water_fablewin.c` (21 functions, headless
`DecompFuncs.java`). Nothing here is in-game verified yet.

## The model in one paragraph

Water is **painted as ground themes**. An `ENGINE_THEME` carries `WaterType` (enum, 0 = none) and
`WaterHeight` (a depth above the ground); retail ships them as depth ladders, and the engine's
surface height at a vertex is `ground + Σ(slot blend × WaterHeight)` over the LEV's three splat
slots (`CEngineMap::PeekWaterHeight` 0x02d5dd80 = ground + `PeekWaterDepth`; `PeekHasWaterFast`
0x02d5d620 = any slot's type != 0). Gameplay reads that directly (`CMap::HasWaterAt`,
`GetWaterSizeZOffsetAt`, `CTCWaterWader`). **The visible surface is not generated at runtime**:
the bake turns the painted heights into meshes stored in the STB, and retail only loads them
(`CEngineWaterRenderer::LoadPatch` 0x02dfcf50 → `CWaterPatchMesh::Load`; the retail label
"CEngineWaterRenderer::GenerateWaterMesh" at 0x00b6edf0 is an autoname guess — it stripifies the
fixed 17x17 index buffer in the renderer's constructor, FableWin has no such generator).

The Lionhead editor's paint dialog confirms the tool shape: `CPaintMapDialog::InitWaterThemeListBox`
(pick a water body theme family), `GetWaterFillAltitude` (a surface altitude), `GetWaterAutoremoveFlag`;
`CEditWorldMap::GetWaterTypeAtBlock / GetRelWaterHeightAtBlock / GetWaterThemeGroupAtBlock`.

## Retail water themes (game.bin, WaterType / WaterHeight)

| type | meaning (from names + renderer lists) | themes |
|---|---|---|
| 1 | lake (foreground water + background) | `WATER_LAKE_{0,1,2,4,8,16}`, `WATER_BWLAKE_*`, `WATER_DWLAKE_*`, `WATER_WWLAKE_*`, `WATER_KRAKEN_LAKE_*` |
| 2 | river | `WATER_RIVER_{0,1,2,4,8,16}` |
| 3 | sea, old ("IsOldSea") | `WATER_SEA_{0,1,2,4,8,16}` |
| 4 | sea, reflective | `SEA_OAKVALE_{0,1,2,3,4,8,16}` |
| 5 | sea, non-reflective | `SEA_HOOKCOAST_{0,1,2,3,4,8,16}` |
| 6 | test | `WATER_TEST_*` |
| 8 | ice | `WATER_HCICE_{0,0_25,1_00,16_00}` |

`CWaterPatchMesh::Load` maps 0/1→1, 2→2, 3→3 (asserts), 4, 5, 6, 8; anything else → 1.
Sea types also need a **sea body** entry (`__ENGINE_SEA_STATIC_MAP_BANK_FILE__<Region>`, 26 in
retail, ~125 KB each: `CEngineWaterRenderer::BuildAndSaveSea` 0x02d54f90, `CWaterSeaGenerator`)
for the far disc; lakes/rivers do not.

## Bake pipeline (FableWin VAs)

| step | function | what it does |
|---|---|---|
| heights | `CEngineLandscapeMeshBuilder::BuildLayersFromThemes` 0x02cb0660 | per 16x16 patch: `Heights[x][y] = CEngineMap::PeekInterpolatedWaterHeight(x, y, 2)` for the 17x17 vertices; returns a `CWaterPatchDescriptors` (0x484 = 17x17 floats) only if any height > 0.001, else null (patch has no water) |
| foreground | `CEngineWaterRenderer::AddForegroundPatch` 0x02d54b40 → `CWaterPatchMesh::Build` 0x02e689b0 | copies heights; a vertex ≤ 0.001 takes `FindCorrectWaterLevel` 0x02e67af0 (mean of non-zero heights in ±2 cells) or, failing that, `ground − 1.0` (sunk); `WaterType` = the patch's dominant type |
| vertices | `CWaterPatchMesh::BuildVertexBuffer` 0x02e68320 | fills 289 x `CTVertexWaterForeground` (below) incl. shore data from `CWaterGenerator::ConstructVertexDistanceToShoreArray` |
| save | `CWaterPatchMesh::Save` 0x02e68bf0 | inside the foreground frame after the layer meshes: `u8 hasWater` (patch), then the block below |
| background | `CWaterGenerator::BuildStaticMapBackgroundBuffers` 0x02e067c0 + `CEngineWaterBackgroundSubPatch::Save` 0x02e055e0 | the distant water per background patch (and the `CPatchTesselationEdgeStrip` bridge buffers in the patch trailer) |
| shore | `CWaterGenerator::FindShorePointsInMap` 0x02e07490, `SortShorePointsIntoWaterBodies` 0x02e083b0, `GenerateShoreMapUCoords` 0x02e09e30 | shore points → water bodies → per-vertex 12-direction shore distances (foam); `GetZeroedShoreLookUpArray` exists for "no shore" |
| non-patch | `CEngineWorldMap::BuildAndSaveWaterData` 0x02d68be0 → `CEngineWaterRenderer::BuildAndSaveNonPatchData` 0x02d55d40 | a world-level bank entry (info + data streams; `CWaterStaticMapInfoBlock` = version + stream position), loaded by `LoadWaterData` 0x02d5fe00 / `LoadNonPatchData` |

## `CWaterPatchMesh::Save` — byte layout (foreground frame)

```
i32 Offset.x, i32 Offset.y          // patch origin in map cells
f32 WaterBodySpan
i32 WaterType
i32 compressedLen
u8  block[compressedLen]            // CRangeCompressor, 289 records x 0x42 (66 B) = 0x4a82 raw
```
Record (the on-disk 66 B of a 0x44 `CTVertexWaterForeground`), all little-endian:
```
u16 x, u16 y        // world x/y rounded (map origin + cell)
f32 z               // floor(max(h - 0.1, 0) * 256) / 256   (ice: - 0.001 instead of 0.1); h = the vertex's water level (below)
i16 waveS           // round((sin(a) + sin(b)) * 32767 / 2),  a = wx * f32(1/2pi) * f32(2pi), b likewise with wy, in double
i16 waveC           // same with cos                          (world units -> turns -> radians with the float constants)
i16 depth           // round(clamp((z + 0.1) - ground, 0, 2) * 32767 / 2)   from the QUANTISED z; ground = the baked vertex height
f32 distToShore     // ConstructVertexDistanceToShoreArray out-param -- 0.0 in every retail record (975,086 checked)
f32 shore[12]       // ShoreLookUpArray (12 directions); GetZeroedShoreLookUpArray = all zero; retail records all carry data
```
**Audited against every retail water patch (FableForge `forge water-audit --all`, 2026-09-20; 177 maps,
3,374 patches, 975,086 records):** the wave columns are exact with the formula above (the `/2pi` first
read was wrong: the code goes units -> turns -> radians with float constants, the products kept in x87
double); `depth` is exact once `ground` is the STB foreground layer's vertex height rather than the LEV's
(the bake ran on the in-memory map; the two differ by millimetres); `z` is `floor`, not `round`, and matches
96-99.8 % per map with the same STB ground inside the level average -- the residue (1-2 steps of 1/256 at
shore vertices) is window-membership detail in `PeekInterpolatedWaterHeight` still to pin. `h` for a vertex
with no interpolated level is `FindCorrectWaterLevel` = the mean of the non-zero heights within +-2 cells
**inside the patch's own 17x17 array** (never across the patch border), else `ground - 1.0` (sunk), and the
same `- 0.1` / floor then applies (so a sunk vertex reads `ground - 1.1`). Our CRangeCompressor port
re-encodes 65 % of the retail blocks byte-exact (the rest differ late in the stream by one column choice,
same length +-6 B); the raw form always decodes, which is what a new lake needs.
`Load` (0x02e673b0) trusts every field (only `depth` goes back through `SetTargetVertexDepth`
= i16 → float clamp 0..2). Constants: `0x0401df60` = 2.0 (max depth), `0x04022078` = 0.1,
`0x0417cc80` = 256, `0x0434bb98` = 32767, `0x04071768` = 2pi, `0x0443b590`/`0x0447621c` = 0.001.

## `CEngineWaterBackgroundSubPatch::Save` — byte layout (background patch trailer)

```
u16 vertexCount, u16 polyCount, i32 WaterType
i32 vertexStride     // 0x38 for types 1/2/6/8 (CTVertexWaterBackground), 0x0c for 3/4/5 (sea: position only)
i32 len, u8 vb[len]  // range-compressed vertexCount x stride
[i32 len, u8 ib[len]]// only if polyCount: range-compressed u16 x 3 x polyCount
```
`Load` 0x02e052b0 takes (stream, w, h, mapX, mapY, minZ, maxZ) from the patch header.
**Read 2026-09-20** (`CWaterGenerator::BuildStaticMapBackgroundBuffers` 0x02e067c0 + the retail
data): the 0x38 `CTVertexWaterBackground` is `u16 x, u16 y, f32 z, f32 shore[12]` (no
distToShore); the sea vertex is `f32 x, y, z`. The generator walks the background patch's own
landscape triangle list; a triangle is kept when any vertex has `PeekInterpolatedHasWaterFast(c, 1)`
(painted water within one cell); its vertices are emitted in first-touch order with
`z = PeekInterpolatedWaterHeight(c, window 2)` (unquantised; a dry vertex takes a triangle mate's
level, two retries), the index buffer is the remapped triangle list, and `WaterType` is the most
frequent `PeekInterpolatedWaterType(c, 1)` over the touched vertices (0 excluded). Retail lake
patches carry shore data here too (zero for the sea). The bridge (edge strip) water arrays are
still unread; FableForge writes the sub-patch with none.

## What FableForge has today, and the gap

- Paint: `forge::terrain::applyThemeBrush` can already paint any theme, water ones included
  (the LEV side of the model is complete, so gameplay water follows a paint). No water-specific
  brush (surface altitude → depth-theme mix) yet.
- STB: the deploy keeps retail water trailers **verbatim** on height edits and writes
  `hasWater = 0` for regenerated/new patches (`stbbake.cpp` ~3250), so a painted lake has no
  visible surface until a writer exists. `stbrelocate` already parses the 289 x 0x42 block to
  translate it; `rangecodec.hpp` has the CRangeCompressor.
- Preview: the exporter's own water surface (`--no-water` to drop it) is the same LEV formula, so
  the editor can preview a paint before any bake.

## Plan (smallest path to water in-game)

1. **Water brush** in the Terrain tab: surface altitude + body family (lake/river/sea/ice); per
   cell with ground < altitude write a two-theme mix from the family's depth ladder so
   Σ blend × WaterHeight = altitude − ground (3 splat slots; keep the ground theme in the third).
   Preview = the existing water surface. This alone gives wading/collision water.
2. **Foreground writer**: `CWaterPatchMesh::Save` per touched patch from the LEV heights with
   the formulas above and zeroed shore arrays (`distToShore` = large). Oracle: retail patches
   in the four-map corpus must round-trip byte-exact through the same encoder before a new
   one is trusted. Expected in-game: surface visible within the foreground radius, no foam.
3. **Background sub-patch + bridge strips** (RE `BuildStaticMapBackgroundBuffers`, the 0x38
   vertex, `TesselateEdge`) so the surface persists at distance; then the shore generator for
   foam. Sea bodies last (a lake never needs one).
