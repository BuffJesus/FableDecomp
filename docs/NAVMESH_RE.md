# Navmesh & Region-Graph Generation (Lionhead editor RE)

*Reverse-engineered from **FableWin.exe** (editor debug build, PDB-named). Addresses are FableWin
base-`0x400000`. Cross-checked against the shipped `FinalAlbion.wld`,
`data/Misc/FinalAlbion_StartingRegionGraph.txt`, and the `.lev` container structure in
`docs/NEW_LEVEL_CREATION.md`.*

Raw decompiles: `ghidra_out/decomp_navmesh.c` (18 fns incl. `SaveToFile`/`LoadFromFile`),
`ghidra_out/decomp_navmesh2.c` (node writers + ctors), curated index in
`ghidra_out/decomp_navmesh2_key.c`. **Parser + byte-exact round-trip validator:
`tools/parse_navmesh.py`** — round-trips both nav sections of `BarrowFields.lev` (89,970 bytes
each, all four node kinds) to identical bytes. **VALIDATED: data-diff Y.**

There are **two distinct navigation artifacts** in Fable, do not conflate them:

1. **Per-level navmesh** = an adaptive **quad-tree** (`CNavQuadTree`) embedded inside each `.lev`
   file. Fine-grained walkability for creature pathfinding (A*).
2. **Region-connectivity graph** = a coarse inter-region routing table (`CRegionConnectionGraph`)
   built from region-exit portals and stored as a small text file (`*RegionGraph.txt`). Used for
   "which region do I go to next" high-level travel.

---

## 1. Per-level navmesh: `CNavQuadTree`

Class file (from asserts): `lib_nav_quad_tree.cpp`. The mesh is an **adaptive quadtree of navigable
cells** organized into one or more `CNavigationLayer`s (vertical layers, e.g. bridges over ground),
plus a list of `CNavRegion`s and `CNavigationPosition`s (action points).

### Node model (`CANavQuadTreeNode`, base 0x031a75f0 dtor)

Per-node queries recovered (all virtual, `CANavQuadTreeNode`):

| Method | Addr | Meaning |
|---|---|---|
| `IsNavigable()` | `0x02973240` | cell is walkable |
| `IsLeafNode()` | `0x02973260` | leaf vs subdivided |
| `GetNavLayer()` | `0x031a6570` | which vertical layer |
| `GetRegion()` → `CNavRegion*` | `0x031a7660` | owning nav region |
| `GetPreferabilityCost()` (u8) | `0x031a77f0` | terrain cost bias for A* |
| `IsNeighbourOf(node)` | `0x031a7640` | adjacency |
| `IsSwitchable()/IsSwitchableOn(key)` | `0x031a7980/90` | dynamically blockable (doors/gates) |
| `SetUpNeighbours(ENavQuadTreeNeighbours, node)` | `0x031a79c0` | link neighbours |
| `CalculateChildrenNeighbours()` | `0x031a79d0` | recompute after subdivision |

Node serialization is via two virtual slots on the node vtable, used by `SaveToFile`:
- vtable **`+0x50`** = write/register node index (first pass).
- vtable **`+0x54`** = write node payload (second pass).
`CNavBlockedQuadTreeNode` is a special singleton "blocked" node (`GetBlockedNode` `0x031c91c0`,
`SetBlockedNode` `0x032948a0`), referenced by a sentinel index in the file.

### On-disk navmesh format — `CNavQuadTree::SaveToFile` (0x03289b50) / `LoadFromFile` (0x0328a760)

**PROVEN, DATA-DIFF VALIDATED.** The exact byte layout below is cross-checked against the writer,
the reader (ground truth), and a byte-exact round-trip of `BarrowFields.lev` in `parse_navmesh.py`.
All little-endian.

**Block header** (`CNavQuadTree` block, one per nav section):
```
long   reservedSize    // back-patched: ABSOLUTE file offset of the block END
                       //   == next section's offset; for the LAST section == nav-block/EOF end.
long   version = 8     // QUAD_TREE_VERSION_NUMBER; LoadFromFile asserts ==8, treats <5 as stale
float  mapWidth        // this+0x10; asserted == map_area.GetWidth()  (128.0 in BarrowFields)
float  mapHeight       // this+0x14; asserted == map_area.GetHeight() (128.0)
uint   numRegions      // list<CNavRegion>.size(); regions carry NO per-region bytes (rebuilt from index)
uint   numPositions    // vector<CNavigationPosition>.size()
numPositions x {       // CSaveNavigationPositionToFile (0x0328a5f0); 12 bytes each
    float x            //   +worldTL (map top-left) applied on load
    float y
    long  data
}
uint   numLayers       // vertical navigation layers (2 in BarrowFields)
long   totalNodeCount  // written as (indexCounter - 1); flat count of records that follow
totalNodeCount x <node record>   // see per-node layout below (pass-2 payloads, flat/linear on disk)
long   endSymbol       // == 0xACBDEF12 (signed -0x534210ee); assert "test_end_symbol==END_QUAD_TREE_SYMBOL"
```

**Per-node record** (mirrors the `LoadFromFile` node loop; ground truth). Prologue = 3 flag bytes
(the `blockedFlag` byte is present only when `version>5`, so always for v8):
```
u8   blockedFlag       // writer byte1 = node->IsBlocked() vtbl+0x24 result
u8   isRealNode        // writer byte2 = the "blocked" bool arg; !=0 => node occupies a layer-grid slot
u8   payloadFlag       // writer byte3 = node+0xf
if (blockedFlag==0 && payloadFlag!=0):        // BLOCKED singleton  -> NO further bytes (3 bytes total)
else:                                         // real node, common body:
    u8    leafKind     // node+0xe : 0 => internal CNavQuadTreeNode ; !=0 => a leaf
    u8    detailLevel  // node+0xc : quadtree level 0..7 (size = 32/2^level map units)
    u8    navLayer     // node+0xd : vertical navigation-layer index (0 = ground)
    f32   posX ; f32 posY   // C2DVector centre (+worldTL on load)
    i32   selfIndex    // node's own file index (from the pointer->index map)
    // ---- leafKind/blocked-specific tail ----
    if leafKind==0:                                  // INTERNAL (ctor 0x032834d0)
        i32 child[4]                                 // 16 bytes: 4 child node indices (+0x10.. +0x1c)
    else:                                            // LEAF (nav-leaf body; ctor 0x0327fed0)
        i32 regionIndex                              // CNavRegion internal index (0 => none)
        if version>6: u8 preferability               // A* terrain cost (node+0x1c; default 0x40;
                                                     //   observed {0,128} layer 0, {64} layers 1+)
        u32 neighbourCount
        i32 neighbour[neighbourCount]                // neighbour node indices
        if blockedFlag!=0:                           // SWITCHABLE leaf (ctor 0x032823a0) adds:
            u32 switchStateCount
            u64 switchState[switchStateCount]        // pair<unsigned __int64,bool> keys (8 bytes each)
```

Node-kind selection on read: `blockedFlag==0 && payloadFlag!=0` → blocked singleton; else if
`leafKind==0` → internal; else if `blockedFlag==0` → navigable leaf; else → switchable leaf.
BarrowFields section 0 histogram (round-trip-confirmed): 19 blocked, 949 internal, 1375 navigable
leaves, 10 switchable leaves (2353 total).

- The writer uses a `std::map<const CANavQuadTreeNode*, long>` node→index table; the shared blocked
  node is inserted with **index 0** and the index counter starts at **1**. Two passes: pass 1
  (`vtbl+0x50`) assigns indices, pass 2 (`vtbl+0x54`) writes payloads — so child/neighbour references
  (stored as indices) resolve after every node has an index.
- On completion the editor logs `"... nav info takes N kB) on disk."`, confirming the size path.
- `LoadFromFile` clamps reconstructed geometry to the passed `C2DBoxI` map bounds and adds `worldTL`
  (map top-left, `this+7`) to every stored point/position.

### Where it sits in the `.lev`: `CWorldMap::SaveNavigationMapToFile` (0x01c8e300)

**PROVEN, VALIDATED vs BarrowFields.lev.** The nav section is the FINAL block of the `.lev`.
`navOffset` comes from the 25-byte LEVHeader at **file offset 21** (`u32`). At `navOffset`:
```
long  navDirEnd            // back-patched = file offset just past this directory (== first section offset)
uint  numNavSections
numNavSections x {
    long  nameLength
    char  name[nameLength] // map sub-section name (e.g. "NULL", "V_SickChildBarrowFields")
    long  sectionFileOffset// back-patched ABSOLUTE file offset of that section's CNavQuadTree block
}
// then, contiguously, each section's CNavQuadTree::SaveToFile block, in directory order.
```
BarrowFields: `navOffset=0x71370`, `navDirEnd=0x713a3` (== section[0] offset, verified), sections
`NULL`@`0x713a3` and `V_SickChildBarrowFields`@`0x87315`. Each section's `reservedSize` field points
at the NEXT section's offset (`0x87315`, then `0x9d287`==EOF), giving a self-describing chain with no
padding between sections.

### Where it sits in the `.lev` (editor build order): `CWorldMap::SaveNavigationMapToFile` (0x01c8e300)

This is step 5 of `SaveSingleLevelToFiles` (see NEW_LEVEL_CREATION). It writes a **navigation section
directory** into the LEV, then the quad-tree(s):

```
long   navDirOffsetPlaceholder = 0            // back-patched
// select which sub-sections of the map get a navmap:
for each candidate section name:
    if CWorldMap::IsSectionToHaveNavigationMap(name, mapno): keep it
uint   numNavSections
for each nav section:
    long   sectionNameLength
    CCharString sectionName                    // WriteVar<CCharString>
    long   sectionFileOffsetPlaceholder        // filled after the tree is written
// then, for each section:
    CNavigationAreaInit init;                  // built from map box + section
    GetMapNavigationAreaInit(mapno, &mapBox, sectionName, &init)
    NavigatorManager.RemoveNavigationMap(mapno)
    NavigatorManager.AddNavigationMap(mapno, init)          // (re)builds the tree in memory
    NavigatorManager.SaveNavigationMapToFile(mapno, file)   // -> CNavQuadTree::SaveToFile
    NavigatorManager.ActivateNavigationMap(mapno)
// back-patch each section's file offset into the directory
```

So a LEV can carry **multiple named nav sections**, each an independent `CNavQuadTree`. `CFileFormatLevel+0x15`
holds the running section offset the framing back-patches.

### How the tree is BUILT (from geometry): `CNavQuadTree::Initialise(CNavigationAreaInit&)` (0x03290030)

This is the actual generator. Honest depth: it is a large function (~600 lines of pseudo-C) and is
**not fully algorithm-recovered here**. What is established:

- Input is a `CNavigationAreaInit` describing the map box + terrain/collision source for a section
  (produced by `CWorldMap::GetMapNavigationAreaInit`).
- It samples walkability over the area and **recursively subdivides** the quadtree down to
  `smallestNodeSize`, merging uniform-preferability regions up to `largestNodeSize`
  (`IsAreaAllSamePreferability` `0x0328d710`, `GetPreferabilityCostForNode` `0x0328d710`), then:
  - `SetUpRegions(...)` (`0x0328f270`) flood-fills contiguous navigable areas into `CNavRegion`s.
  - `JoinNavigationLayers()` (`0x03291790`) stitches vertical layers.
  - `VerifyTransitionPoints()` (`0x03291a20`) validates layer transitions.
- Neighbour links are computed per node (`CalculateChildrenNeighbours`).
- Cross-map stitching is separate: `LinkToNeighbouringMaps(CNavigatorManager&)` (`0x03291d30`) +
  `ConnectHorizantalMapEdge`/`ConnectVerticalMapEdge` (`0x03292040`/`0x03292910`) join a map's edge
  nodes to adjacent maps' navmeshes at runtime.

Runtime pathfinding over the tree: `CNavigatorAStar` (A* state at `0x031a7110`; heuristic
`0x031a7c80`, cost `0x031a7cd0`), using `GetPreferabilityCost` as terrain weighting.

### Editor trigger

`CEditControlCentre::GenerateNavigationInformation()` (0x0204b700): loops all maps, and for each used
map calls `NavigatorManager.RemoveNavigationMap(mapno)` then `CWorldMap::ActivateNavMap(mapno)` —
i.e. "regenerate nav for every level". The per-map regen ultimately runs `Initialise` + save.

---

## 2. Region-connectivity graph: `CRegionConnectionGraph`

Class file: `region_connection_graph.cpp`. This is a coarse graph over **regions** (not cells): for
every ordered pair of regions it precomputes the **next region to move to** en route (an all-pairs
next-hop table), so AI/quests can travel across Albion region-by-region.

### Generation: `CWorldMap::GenerateRegionConnectivityGraph()` (0x01c8a2c0)

Editor console command `ConsoleGenerateRegionGraph` (0x018d90c0) → this. Algorithm (verified):

1. Put the player in limbo, unload the current region.
2. **For each region** (index 1..N):
   - `LoadRegion(regionIndex)`.
   - `GetAllThings` filtered by TC interface type **100** = **`CTCDRegionExit`** (region-exit
     portals). For each region-exit thing that is *in this region*, read its target-region link
     (asserts `region_exits[c1]->PeekTC(&ptc_region_exit)` in `world_map.cpp:0x95c`).
   - Accumulate `"<RegionName>": "<Target1>", "<Target2>", ...;` into a `CCharString`.
3. Write the whole string to **`RegionGraph.txt`** via `CDiskFileWin32` + `CCharString::SaveToFile`.
4. Reload the original region, un-limbo the player.

So the graph is derived purely from **region-exit thing placements**: editing where region-exits
point changes the graph.

### On-disk format — confirmed against the shipped file

`data/Misc/FinalAlbion_StartingRegionGraph.txt` (5,126 bytes). Grammar is one record per line:

```
"RegionName": "ConnectedRegionA", "ConnectedRegionB", ..., "ConnectedRegionN";
```

Real samples (read directly from the install):
```
"LookoutPoint": "BowerstoneSlums", "DemonDoor_LookoutPoint", "GreatwoodEntrance", "HeroGuildComplexInside", "PicnicArea";
"PicnicArea": "LookoutPoint";
"BowerstoneSlums": "BowerstonePosh", "BowerstoneSlumsWarehouses", "BowerstoneTavernCellar", "LookoutPoint";
```

Adjacency is **directed** (note LookoutPoint→PicnicArea and PicnicArea→LookoutPoint are listed
separately). The list is asymmetric in general.

### Load + build routing table: `CWorldMap::LoadRegionGraph(CWideString&)` (0x01c8aec0)

Console `ConsoleLoadRegionGraphData` (0x018d9100) passes a filename; `LoadRegionGraph`:
- prefixes `GetMiscDir()` to the name, checks it exists (asserts otherwise, `world_map.cpp:0x9a0`).
- lazily `new CRegionConnectionGraph()` into a `CCountedPointer`.
- reads the whole file into a `CCharString`, then calls
  `CRegionConnectionGraph::InitialiseGraph(text, worldMap)` (0x027dabf0).

`InitialiseGraph` parses the text with a `CStringParser` (delimiters `"`, `:`, `,`, `;` — the
`DAT_042eee*` string constants):
- `reserve(0x50)` nodes.
- For each record: read the quoted **region name** symbol → `GetRegionNumberFromName` → if valid,
  `new CRegionConnectionNode(regionNumber)` and `Insert` into a `CVectorMap<long, CountedPtr<Node>>`.
- Skip past `:`, then loop reading each quoted connected-region name until `;`
  (`SkipPastNextItemIfIs(",")` / `ReadAsStringUntilString`), converting each to a region number and
  `push_back` into a temp vector; a missing `;`/`,` → **"Corrupt graph data!"** assert
  (`region_connection_graph.cpp:0x86`).
- `CRegionConnectionNode::InitialiseConnections(vector<long> neighbours)` records direct neighbours.
- After all records, `CVectorMap::Update()` sorts/builds the lookup.

### Query API (the routing table)

- `CRegionConnectionNode(long region)` (0x027da620) — one node per region.
- `SetAsConnectedToRegion(long region, bool)` (0x027da930) — marks direct connectivity.
- `CRegionConnectionGraph::SetRegionsAsConnected(long a, long b, bool)` (0x027db600).
- `CRegionConnectionGraph::GetNextRegionOnWayToRegion(long from, long to)` (0x027db2c0) → next region
  index toward `to` (BFS/Floyd next-hop). Delegates to
  `CRegionConnectionNode::GetNextNodeOnWayToRegion(long to, graph)` (0x027da6a0).
- `GetRegionNode(long region)` (0x027db500).

This is effectively a precomputed **all-pairs shortest-path next-hop** table over the region
adjacency, so travel queries are O(1) per hop.

### Recipe for `forge` region-graph regeneration

To regenerate `*RegionGraph.txt` offline from world data:
1. For each region, collect its member levels (`ContainsMap` from the WLD region blocks).
2. Scan those levels' TNGs for region-exit things (`CTCDRegionExit`, TC iface 100) and read their
   target-region link.
3. Emit `"Region": "Target1", "Target2", ...;` lines (directed, one per region).
Place the file in `data/Misc/` (or the map's Misc dir); the engine loads it via `LoadRegionGraph`.

> Gap: the exact TNG property/CTC field on a region-exit thing that names the *target region* is not
> yet byte-mapped here (it comes from `CTCDRegionExit::PeekTC` runtime state). Practical path:
> diff a region-exit thing in a known level's TNG against the graph line it produces. FableForge
> already parses TNG things/CTC blocks, so this is a data-mapping task, not new RE.

---

## Cited addresses (FableWin, base 0x400000)

### Navmesh (quad-tree)
| Symbol | Addr |
|---|---|
| `CNavQuadTree::SaveToFile(CAFile&)` | `0x03289b50` |
| `CNavQuadTree::LoadFromFile(CAFile&, long, C2DBoxI&, CASuspendableProcess&)` | `0x0328a760` |
| `CNavQuadTree::Initialise(CNavigationAreaInit&)` | `0x03290030` |
| `CNavQuadTree::SetUpRegions(long)` | `0x0328f270` |
| `CNavQuadTree::JoinNavigationLayers()` | `0x03291790` |
| `CNavQuadTree::LinkToNeighbouringMaps(CNavigatorManager&)` | `0x03291d30` |
| `CNavQuadTree::ConnectHorizantalMapEdge / ConnectVerticalMapEdge` | `0x03292040` / `0x03292910` |
| `CNavQuadTree::GetSmallestNodeSize / GetLargestNodeSize` | `0x03294880` / `0x03294890` |
| `CNavQuadTree::SetBlockedNode / GetBlockedNode` | `0x032948a0` / `0x031c91c0` |
| `CWorldMap::SaveNavigationMapToFile(...)` | `0x01c8e300` |
| `CNavigatorManager::SaveNavigationMapToFile(long, CAFile&)` | `0x031c6660` |
| `CEditControlCentre::GenerateNavigationInformation()` | `0x0204b700` |
| `CANavQuadTreeNode::IsNavigable / IsLeafNode` | `0x02973240` / `0x02973260` |
| `CNavigatorAStar::CAStarState(...)` | `0x031a7110` |

### Region graph
| Symbol | Addr |
|---|---|
| `NGlobalConsole::ConsoleGenerateRegionGraph(...)` | `0x018d90c0` |
| `NGlobalConsole::ConsoleLoadRegionGraphData(...)` | `0x018d9100` |
| `CWorldMap::GenerateRegionConnectivityGraph()` | `0x01c8a2c0` |
| `CWorldMap::LoadRegionGraph(CWideString&)` | `0x01c8aec0` |
| `CRegionConnectionGraph::InitialiseGraph(CCharString&, CWorldMap&)` | `0x027dabf0` |
| `CRegionConnectionGraph::GetNextRegionOnWayToRegion(long, long)` | `0x027db2c0` |
| `CRegionConnectionGraph::SetRegionsAsConnected(long, long, bool)` | `0x027db600` |
| `CRegionConnectionGraph::GetRegionNode(long)` | `0x027db500` |
| `CRegionConnectionNode::CRegionConnectionNode(long)` | `0x027da620` |
| `CRegionConnectionNode::GetNextNodeOnWayToRegion(long, graph)` | `0x027da6a0` |
| `CRegionConnectionNode::SetAsConnectedToRegion(long, bool)` | `0x027da930` |
| `CWorldMap::LoadTopologyFromFile(...)` | `0x01c89d20` |
| `CWorldMap::ActivateTopology(long)` | `0x01c8e7d0` |

Evidence: `ghidra_out/decomp_navmesh.c`, installed
`data/Misc/FinalAlbion_StartingRegionGraph.txt`, `docs/NEW_LEVEL_CREATION.md`.

## Status of the four navmesh gaps

1. ✅ **Per-node byte payload** — **DONE, data-diff validated.** Exact per-node layout is proven above
   (blocked / internal / navigable-leaf / switchable-leaf) and `tools/parse_navmesh.py` re-serializes
   both shipped nav sections to byte-identical output. Derived from the writer (`CANavQuadTreeNode`/
   `CNavQuadTreeNode`/`CNavNavigableLeafNode`/`CNavSwitchableLeafNode`/`CNavBlockedQuadTreeNode`
   `SaveToFile`) AND the reader (`LoadFromFile` + node ctors), then confirmed against real bytes.
2. ✅ **LEV nav-section framing offsets** — **DONE, validated.** navOffset@LEVHeader+21; directory with
   back-patched `navDirEnd` + per-section name/offset; each block's `reservedSize` chains to the next
   (last→EOF). Confirmed against `BarrowFields.lev` (offsets `0x71370 / 0x713a3 / 0x87315 / 0x9d287`).
3. ✅ **`CNavQuadTree::Initialise` subdivision algorithm** — **DONE, oracle-validated 2026-07-20.**
   Fully lifted (root tiling, recursive classifier, IsAreaClear two-source predicate, preferability
   sampling, depth gating, switchable no-merge rule) and validated by exact topology regeneration of
   all 398 retail LEVs (`parse_navmesh.py regen`). See "SUBDIVISION ALGORITHM — RECOVERED" below.
   Remaining upstream gap: `CWorldMap::GetMapNavigationAreaInit` (how blocking lines / switchable
   blockages / detailed areas are gathered from placed things) — TopologyWeights is pinned to LEV
   terrain bytes (+15 walkable, +20 preferred-path), lines are thing-collision-derived.
   **2026-07-20: empirically mapped (thing footprints + island rule + switchKey=UID low32) and a
   terrain-only generator shipped — see "THING-COLLISION → BLOCKING" and "TERRAIN-ONLY NAVMESH
   GENERATOR" sections below.**
4. **Region-exit → target-region field** (region *graph*, not navmesh) — still runtime-derived
   (`CTCDRegionExit::PeekTC`); map by diffing a known TNG against its graph line. Unchanged.

### 3. `CNavQuadTree::Initialise(CNavigationAreaInit&)` (0x03290030) — proven vs inferred

PROVEN (from the decompile + node model above):
- **Cell geometry is a fixed per-level size table, not free-form.** `CANavQuadTreeNode::GetSize()`
  (0x0327f4a0) returns `DAT_044f344c[QuadTreeLevel]` where `QuadTreeLevel` is the node's `+0x0c` byte
  (`<= 7 = MAX_DETAIL_LEVEL`, asserted in the base ctor 0x0327f1f0). So subdivision depth is bounded at
  8 levels and every node's covered area is `size[level]` centred on `pos` (`GetAreaCovering`
  0x0327f400 = `pos ± size*0.5`). This is the concrete meaning of the on-disk `navLayer`/level byte.
- **Node taxonomy is fixed** (matches on-disk `leafKind`): internal `CNavQuadTreeNode` (4 children),
  navigable leaf `CNavNavigableLeafNode` (region + neighbour list), switchable leaf
  `CNavSwitchableLeafNode` (adds key→bool switch states, e.g. doors/gates), and the shared
  `CNavBlockedQuadTreeNode` singleton for impassable cells.
- **Build order** (call sequence, established): `Initialise` → recursive subdivide down to
  `smallestNodeSize`, merging uniform-preferability areas up to `largestNodeSize`
  (`IsAreaAllSamePreferability` / `GetPreferabilityCostForNode` 0x0328d710) → `SetUpRegions`
  (0x0328f270, flood-fills contiguous navigable cells into `CNavRegion`s) → `JoinNavigationLayers`
  (0x03291790) → `VerifyTransitionPoints` (0x03291a20). Neighbour links via
  `CalculateChildrenNeighbours` (0x031a79d0) / `SetUpNeighbours` (0x031a79c0). Cross-map stitching is
  separate: `LinkToNeighbouringMaps` (0x03291d30) + `ConnectHorizantalMapEdge`/`ConnectVerticalMapEdge`
  (0x03292040 / 0x03292910).

INFERRED / NOT byte-lifted:
- ~~The exact walkability sampling resolution and the precise "all same preferability" merge
  predicate~~ — **now lifted, see the "SUBDIVISION ALGORITHM — RECOVERED" section below (2026-07-20).**

---

## SUBDIVISION ALGORITHM — RECOVERED (2026-07-20 session)

*Evidence: `ghidra_out/decomp_navmesh.c` (`CNavQuadTree::Initialise` @ 0x03290030),
`ghidra_out/decomp_navmesh2.c` (`CNavQuadTreeNode::Initialise` @ 0x032851b0, builder ctor
@ 0x032833a0, `IsAreaClear` @ 0x0328c7e0, `IsAreaAllSamePreferability` @ 0x0328d400,
`GetPreferabilityCostForNode` @ 0x0328d710), cross-checked empirically against the parsed
`BarrowFields.lev` nav section (histogram + size-table inference below). All FableWin addrs.*

### Corrected on-disk field semantics (supersedes the field names above)

The three common-body bytes of a real node record are, in file order:

| file byte | object offset | TRUE meaning (was mislabeled) |
|---|---|---|
| `leafKind` | `+0x0e` | 0 = internal, nonzero = leaf (unchanged) |
| ~~navLayer~~ **detailLevel** | `+0x0c` | **quadtree subdivision level 0..7** (`MAX_DETAIL_LEVEL=7`); node size = `sizeTable[level]` |
| ~~preferability~~ **navLayer** | `+0x0d` | **vertical navigation-layer index** (0 = ground, 1.. = upper layers) |
| leaf-tail u8 (~~edgeFlags~~) | `+0x1c` | **preferability cost** (A* terrain bias; default 0x40=64) |

Empirical confirmation (BarrowFields section 0, 2353 nodes): byte`+0xc` histogram =
{0:13, 1:41, 2:112, 3:306, 4:665, 5:836, 6:361}; byte`+0xd` = {0:2224, 1:110} with
`numLayers==2`; leaf-tail byte = {64:58, 128:1327}. Child-spacing inference gives
`sizeTable = [32, 16, 8, 4, 2, 1, 0.5, 0.25]` (map units; `size[L] = 32 / 2^L`,
`DAT_044f344c` float table indexed by `+0xc`). Level-0 node centres sit on the 4×4 grid
{16,48,80,112}² of a 128×128 map — i.e. the **root tiling is 32-unit cells**, `mapWidth/32 ×
mapHeight/32` of them.

### The build pipeline (`CNavQuadTree::Initialise` @ 0x03290030)

The tree is **not** grown from a single root. Per navigation layer `L` (from the
`CNavigationAreaInit`'s `vector<CNavigationLayerUpdate>`):

1. `InitialiseLines` (0x0328xxxx via `CNavigationLayer+0x14`): the layer's **blocking lines**
   (`vector<C2DLineF>` from the update) are bucketed into an **8×8-map-unit spatial grid**
   (`vector<list<C2DLineF>>`, index = `floor(x/8) + floor(y/8)*ceil(MapWidth/8)`), plus a
   `CVectorMap<u64, list<C2DLineF>>` of **switchable blockages** (key = door/gate u64 uid) at
   `CNavigationLayer+0x28`.
2. A flat array of `(MapWidth/32)×(MapHeight/32)` **root cells** is allocated; cell centres start at
   `worldTL + 32*0.5` stepping by 32 (the `_DAT_044f344c * _DAT_04022eb8(=0.5) + worldTL` code).
3. Each root cell runs the recursive classifier (below). Result per cell: internal node kept as-is,
   or replaced by `CNavNavigableLeafNode` / blocked singleton / `CNavSwitchableLeafNode`.
4. `CalculateNeighbours(rootCells)` links the leaf adjacency lists.
5. After all layers: `JoinNavigationLayers()` → `SetUpRegions()` (flood-fill `CNavRegion`s) →
   `VerifyTransitionPoints()` → unreachable `CNavigationPosition`s are **erased**
   (`IsActionPointReachable` filter).

### The recursive classifier (`CNavQuadTreeNode::Initialise` @ 0x032851b0)

```
classify(node{pos, level, layer}, tree, out type, out switchKeys):
    canSubdivide = (level < 5)
                or (level < 6 and GoToHigherDetail(tree.DetailedAreas))
                   # i.e. level==5 only if node box intersects any "detailed area" C2DBoxF
    clear = tree.IsAreaClear(node, &switchableOnly, switchKeys)
    if clear:
        if not canSubdivide:                 return type=1   # NAVIGABLE leaf
        if tree.IsAreaAllSamePreferability(node): return type=1  # NAVIGABLE leaf (merge)
        # clear but preferability varies -> subdivide anyway
    else:
        if not canSubdivide:
            return type = switchableOnly ? 3 : 2   # SWITCHABLE leaf : BLOCKED
        # blocked-ish and can go deeper -> subdivide
    type = 0                                        # internal
    q = sizeTable[level] * 0.25
    children (TL,TR,BL,BR) at pos + (±q, ±q), each with level+1, same layer:
        childType = classify(child...)
        childType==1 -> replace with CNavNavigableLeafNode(pos, level+1, layer,
                                        tree.GetPreferabilityCostForNode(child))
        childType==2 -> point at the shared blocked singleton
        childType==3 -> replace with CNavSwitchableLeafNode(pos, level+1, layer, pref, childKeys)
        childType==0 -> keep the internal child
    if all 4 children blocked: return type=2        # bottom-up collapse to blocked
```

So the subdivision rule is exactly: **split while the cell is non-uniform (mixed
walkability OR mixed preferability), down to 1-unit cells (level 5), or 0.5-unit cells
(level 6) inside "detailed areas"**. Leaves are emitted early wherever a whole cell is
uniformly clear+same-preferability; fully-blocked subtrees collapse into the shared
blocked singleton.

### The walkability predicate (`CNavQuadTree::IsAreaClear` @ 0x0328c7e0)

An area (the node's covering box, `pos ± size/2`) is CLEAR iff all three pass:

1. **TopologyWeights test** (ground layer 0 only, and only if the `vector<u8> TopologyWeights`
   is non-empty): iterate integer coords over the box (1-map-unit steps, map-relative);
   `TopologyWeights[x + y*MapWidth]` **== 0xFF means hard-blocked** → not clear
   (`switchableOnly=false` → blocked).
2. **Static line test**: gather the 8×8-bucket line lists overlapping the box (bucket range
   computed from `pos ± size/2` divided by 8) and run `IsAreaBlockedByLines(box, lists)`
   (0x0328c0a0 region): any blocking line intersecting the box → not clear.
3. **Switchable line test**: for each `(u64 key, list<C2DLineF>)` in the layer's switchable
   map, if that key's lines block the box → `switchableOnly=true` and the key is appended to
   the out `vector<u64>` (these become the leaf's on-disk `switchKeys`). Clear only if no
   switchable key blocks either.

`IsAreaAllSamePreferability` (0x0328d400): for layer 0 with non-empty TopologyWeights, every
integer-coord sample of `TopologyWeights` inside the box must be equal (else the cell
subdivides). `GetPreferabilityCostForNode` (0x0328d710) returns **0x40 (64)** for non-ground
layers or empty TopologyWeights, else the (uniform) sampled `TopologyWeights` byte —
matching the on-disk leaf byte values {64, 128} observed in BarrowFields.

### Inputs still upstream (the remaining gap for a from-scratch generator)

The classifier consumes a `CNavigationAreaInit` = per-layer `{blocking lines, switchable
blockages}` + `TopologyWeights` + `DetailedAreas`, produced by
`CWorldMap::GetMapNavigationAreaInit(mapno, C2DBoxI*, sectionName, CNavigationAreaInit&)`
(ego_r `0x004df950`; retail-equiv BSim hit `0x0050a650`). How THAT function derives lines
from placed-thing collision hulls and TopologyWeights from the LEV terrain walkable grid is
the one remaining un-lifted stage. **Empirically mapped 2026-07-20 — see "THING-COLLISION →
BLOCKING" below; only the exact collision-mesh→line extraction remains un-lifted.**

**Empirical correlation vs LEV terrain** (`python tools/parse_navmesh.py correlate`,
BarrowFields, layer 0 rasterized at 1 map unit vs the LEV cell walkable byte):

```
LEV walkable & nav walkable : 3369      P(nav walkable | LEV walkable) = 80.6%
LEV walkable & nav BLOCKED  :  811      P(nav blocked  | LEV blocked)  = 100.0%
LEV blocked  & nav walkable :    0      agreement 15573/16384 = 95.1%
LEV blocked  & nav BLOCKED  : 12204
```

`P(nav walkable | LEV blocked) = 0.0%` — navmesh walkability is a **strict subset** of the
LEV terrain walkable bytes. Interpretation (matches the two-source `IsAreaClear` model):
the LEV per-cell walkable byte feeds the hard-block source (TopologyWeights 0xFF where
unwalkable), and placed-thing obstacle outlines (the blocking lines) carve a further ~19%
out of walkable terrain. A terrain-only generator therefore over-approximates walkability
exactly where things stand.

**TopologyWeights derivation from LEV terrain — PINNED (corpus-validated, 0 exceptions).**
Checked over all 149 nav-bearing retail LEVs (201,951 layer-0 leaf-covered unit cells):

```
TopologyWeights[x + y*mapWidth] =
    0xFF  if LEV cell(x,y) walkable byte (+15) == 0        # hard-blocked
    0x00  if LEV cell(x,y) byte +20 == 1                   # preferred (paths/roads)
    0x80  otherwise                                        # normal ground
```

- 0 violations of "nav-walkable ⊆ LEV-walkable" corpus-wide.
- 0 violations of "leaf preferability == f(cell byte +20)" corpus-wide (pref values observed:
  layer 0 ∈ {0, 128}; layers 1+ always 64 = the empty-TopologyWeights default).
- This **classifies LEV cell byte +20** (one of `lev_rw.py`'s "unclassified" bytes): it is the
  per-cell **preferred-path flag** feeding nav preferability.

### PoC regeneration oracle — ALGORITHM VALIDATED (398/398)

`python tools/parse_navmesh.py regen [file.lev]` decodes the shipped leaves into an exact
0.5-unit class raster, re-runs the recovered subdivision rule over it
(`regenerate_layer`), and compares the produced node multiset (kind, level, centre, pref,
switchKeys) against the shipped tree per layer.

Result: **all 398 retail LEVs (every nav section, every layer) regenerate to the EXACT
shipped topology** — same internal/leaf structure, same node centres, same payloads. One
behavioural detail was discovered by the oracle: **switchable leaves never merge** (the
type-3 path in `CNavQuadTreeNode::Initialise` fires only at max depth), whereas uniform
navigable regions merge via the clear+same-preferability early-out. Corpus detailLevel
histogram: {0:2204, 1:5517, 2:13783, 3:38095, 4:93827, 5:155238, 6:104346} — never >6,
confirming the depth gating (5 normally, 6 in detailed areas).

Also: 249 of the 398 retail LEVs have an EMPTY nav directory (0 sections) — interior/cutscene
maps ship without a navmesh, so a custom level without one loads (but creatures cannot path).

---

## THING-COLLISION → BLOCKING — empirically mapped (2026-07-20 session)

*Method: Ghidra lock was taken, so this is pure data correlation over all 148 nav-bearing
retail LEVs (extracted from `FinalAlbion.wad`) against their TNGs. Working definition:
**delta mask** = 1-unit cells that are LEV-terrain-walkable (byte +15 != 0) but nav-blocked
on layer 0. Terrain-only walkability (the pinned TopologyWeights model) explains everything
else, so the delta mask is exactly what `GetMapNavigationAreaInit`'s line sources carve.*

### Decomposition of the delta mask (all 148 nav levels, ~119k m² total)

1. **Placed-thing collision footprints** — blobs of delta cells centred on `NewThing
   Object;` / `NewThing Building;` TNG placements. Per-blob nearest-thing matching on
   BarrowFields/LookoutPoint/PicnicArea: 83/101 blobs land on a placed OBJECT_* (fences,
   walls, rocks, benches, streetlamps, statues, pillars, gates…); the rest are (2)/(3).
   Corpus nearest-blocking-thing distances for delta cells (excl. islands/edges): p50=3.6m,
   p75=10m — the long tail is **buildings**: e.g. `KrakenChamber`'s 55k unexplained-at-4m
   cells are the interior shell of the single `BUILDING_KRAKEN_CHAMBER` placement, i.e. one
   collision mesh covering the whole map. Footprints follow the placement's rotation
   (`RHSetForward*`), so the line source is the def's **physics collision mesh** footprint
   transformed by the thing's frame (see "geometry source" below).
2. **Unreachable-island removal** — walkable terrain components (4-connected, unit cells)
   that contain **no NAVIGATION_SEED and no REGION_ENTRANCE_POINT / REGION_EXIT_POINT
   placement** are nav-blocked WHOLESALE. Checked per component: kept components are 100%
   partially-navigable, dropped components are 100% blocked. Plain MARKER_* things do NOT
   anchor a component (LookoutPoint comp with a marker is still dropped). Single corpus
   exception: `NorthernWastes2` (2 nav layers) keeps a layer-0 component that is only
   reachable via a **layer-1 bridge**, i.e. reachability is evaluated after
   `JoinNavigationLayers`.
3. **Map-border artifacts** (~1.5% of delta) — half-unit slivers hugging x/y=0 and the far
   edges; nav generation runs with the whole world loaded (`ConnectHorizantal/VerticalMapEdge`
   stitching), so seam cells pick up blocking from adjacent maps' geometry.

### Switchable blockages: key = thing UID low dword — PROVEN (407/407)

Every switchable-leaf `switchKey` (u64) in the corpus equals **`UID & 0xFFFFFFFF` of a TNG
thing in the same level** (editor-placed UIDs are `0xFFFFFE00_xxxxxxxx`, so low32 is the
per-thing serial). 407/407 keys match; the keyed things are:
- doors with `StartCTCDoor` (142 keys: `OBJECT_BUILDING_DOOR_*`, `OBJECT_DEMON_DOOR_01`,
  prison cell doors, …). Case study: BarrowFields' 10 switchable leaves cluster around the
  `OBJECT_DEMON_DOOR_01` `DD_EatPies` placement at (99.2, 25.7); key 3202 == its UID low32.
- degradable blockers (`OBJECT_DEGRADABLE_THORN_VINES_01`, 49 keys) — matches the decoded
  `CDegradableDef.InitiallyBlocksNavigation` def field.
- buyable/removable furniture (`OBJECT_BHF_*` cabinets/bookshelves/beds/chairs, Fresco
  pillars) — things that can exist or not at runtime.
So: a thing whose collision is dynamic (door state / degradable / creatable) contributes its
outline as a **switchable blockage keyed by its UID low32** instead of static lines; the leaf
under it becomes `CNavSwitchableLeafNode` carrying that key.

### Footprint geometry source (partial — the one remaining gap)

- Def-side evidence: `CDoorDef.OpenCollisionMesh` / `ClosedCollisionMesh` (mesh ids),
  `CTrapDef.(Un)TriggeredCollisionMesh` + `PhysicalObstructionDefIndex`,
  `CCameraCollisionDef.CameraCollisionMesh`, `CKickableDef.IgnoreNavigationTest`. graphics.big
  type-3 tagged-BBM entries are the `[PHYSICS]` collision meshes.
- The blocking lines are therefore the def's physics-mesh 2D footprint (outline edges)
  transformed by the placement — consistent with observed rotated, def-consistent blob
  shapes — but the exact extraction (convex hull vs raw edge soup, Z clipping vs walk
  height) is NOT byte-lifted.
- **Ghidra follow-ups** (flagged, lock was taken this session):
  1. `CWorldMap::GetMapNavigationAreaInit` (ego_r `0x004df950` / retail `0x0050a650`) —
     the lines/switchables/DetailedAreas gathering.
  2. `CANavQuadTreeNode::CalculateChildrenNeighbours` (`0x031a79d0`) — the neighbour-list
     insertion ORDER (the only bytes the offline generator cannot reproduce, see below).
  3. The physics-mesh footprint routine feeding (1).

### Negative result: the LEV "obstacle" block carries nothing

The region at LEVHeader `obsOffset` is `u32 size(=33801), u32 count(=1)` + a 33793-byte
payload that is **all zeros in every retail LEV** (397 checked, size constant regardless of
map dims). The 11-byte records between the cell grid and `obsOffset`
(`(w/4)*(h/4)` × `{u32 0x0B, u32 1, u8 FF, u8 00, u8 idx}`) vary but are sound/brush-related,
not navigation. Blocking lines are NOT baked into the LEV — they are regenerated from the
live editor world each time nav is rebuilt.

---

## TERRAIN-ONLY NAVMESH GENERATOR — `parse_navmesh.py generate` (2026-07-20)

`python tools/parse_navmesh.py generate <in.lev> [out.lev] [--tng file.tng] [--compare]`
builds a complete, valid nav suffix for any LEV from its terrain cell grid alone (replaces
existing nav sections, or gives a navless LEV a single `"NULL"` section), using the proven
subdivision algorithm + the pinned TopologyWeights model + the island rule above (with
`--tng`, keep-components are anchored by seeds/entrances/exits; without, the largest
walkable component is kept). Shipped per-section positions are preserved on replace.

### Shipped-writer serialization conventions (recovered, validated byte-exactly)

- **Index assignment (pass 1)**: counter starts at 1; per layer, per 32-unit root cell in
  row-major order, **post-order** over real nodes (children TL,TR,BL,BR then parent). A fully
  blocked ROOT cell **burns one index** (its slot writes only the 3-byte blocked record);
  blocked children inside a subtree burn nothing (they are the shared singleton, index 0).
  Verified on BarrowFields: burned indices {1,2,817,2228..2233,2344..2353} == the blocked
  root slots, `totalNodeCount == final counter - 1`.
- **Record order (pass 2)**: same root loop, **pre-order** (parent before children).
- **Prologue byte 2 (`blockedArg`)** = 1 exactly on root-grid-slot records (level-0 nodes and
  blocked roots), 0 elsewhere — it is a "occupies a layer-grid slot" flag.
- **Neighbours**: exact same-layer edge adjacency (boxes sharing an edge segment with
  positive overlap; corner contact is NOT adjacency) — verified 60/60 sampled shipped
  leaves. The shipped LIST ORDER is build-traversal insertion order and is not reproduced
  (semantically a set; Ghidra follow-up 2 above). Plus rare cross-layer entries from
  `JoinNavigationLayers` (4 in BarrowFields) — n/a for single-layer output.
- **Regions**: flood fill over navigable-leaf adjacency, numbered 1..N by first appearance
  in file order; header `numRegions = N + 1` whenever a nav layer exists (all-blocked
  one-layer trees ship `numRegions=1`), 0 only for the empty `numLayers=0` variant.
- Multi-section LEVs duplicate the same tree per section (BarrowFields' two sections are
  byte-identical apart from `reservedSize`), so the generator emits one tree for all names.

### Validation results (full retail corpus)

- **148/148** nav-bearing LEVs regenerate + pass structural self-validation (directory
  chaining, byte-identical round-trip of the emitted blocks, all child/neighbour indices
  resolve, neighbour symmetry, `reservedSize` chains to EOF); output still parses as a valid
  LEV (`lev_rw` round-trip OK).
- **Byte-exactness oracle**: on retail levels whose shipped nav has zero thing-delta, the
  generated bytes equal the shipped bytes **modulo neighbour-list order** — 17/17 such
  single-layer levels (9 are literally permutation-identical suffixes, e.g. all Darkwood
  leadouts, `DemonDoor_LookoutPoint`, `KnotholeGlade_Leadout_02`).
- **Documented delta where thing-blocking differs**: generated layer-0 walkability is a
  superset of shipped on 147/148 levels (never blocks less than it should from terrain);
  the extra walkable area = 7.4% of total map area = exactly the thing-collision carving a
  terrain-only generator cannot know. Sole subset violation: `NorthernWastes2` (2,198 cells)
  via the layer-1-bridge reachability noted above.
- Navless-LEV demo: `DragonCliff_Vista_01.lev` (0 nav sections, 12,192 walkable cells) gains
  a valid 288-node single-section navmesh; file round-trips in both `parse_navmesh.py` and
  `lev_rw.py`.
- Known limitations: single ground layer only (no `CNavigationLayer` 1+ / transitions), no
  DetailedAreas (depth capped at level 5; shipped level-6 splits change topology but not
  coverage — `LostBay_Leadout_01`), no switchable leaves, neighbour-list order differs.
