#!/usr/bin/env python3
r"""Parser + byte-exact round-trip validator for the Fable: TLC per-level navmesh
(CNavQuadTree), the adaptive quadtree embedded in every .lev.

Reverse-engineered from FableWin.exe (editor debug build, PDB names). Every field below is
taken from the decompiled writer AND the decompiled reader and gated by a byte-identical
round-trip of a real shipped .lev nav section (see round_trip and __main__).

  CWorldMap::SaveNavigationMapToFile     0x01c8e300   LEV nav-section directory framing
  CNavQuadTree::SaveToFile               0x03289b50   quadtree writer
  CNavQuadTree::LoadFromFile             0x0328a760   quadtree reader (ground truth)
  CANavQuadTreeNode::SaveToFile          0x0327f5c0   shared node prologue (vtbl+0x54 base)
  CNavQuadTreeNode::SaveToFile           0x032841e0   internal-node writer
  CNavNavigableLeafNode::SaveToFile      0x03280b60   navigable-leaf writer
  CNavSwitchableLeafNode::SaveToFile     0x03283270   switchable-leaf writer
  CNavBlockedQuadTreeNode::SaveToFile    0x032889a0   blocked-node writer (3-byte record)
  CNavQuadTreeNode ctor                  0x032834d0   internal-node reader
  CNavNavigableLeafNode ctor             0x0327fed0   navigable-leaf reader
  CNavSwitchableLeafNode ctor            0x032823a0   switchable-leaf reader
  CSaveNavigationPositionToFile ctor     0x0328a5f0   position writer  (C2DVector + long)
  CNavigationPosition ctor               0x01c90330   position reader  (this+8 = long)

Docs: docs/NAVMESH_RE.md.  Evidence: ghidra_out/decomp_navmesh.c, decomp_navmesh2_nodes.txt.

CLI:
  python parse_navmesh.py [file.lev]            round-trip validate (byte-identical oracle)
  python parse_navmesh.py correlate [file.lev]  decoded walkability vs LEV terrain walkable bytes
  python parse_navmesh.py regen     [file.lev]  subdivision-algorithm oracle: rasterize shipped
                                                leaves, re-run the recovered subdivision rule,
                                                compare node-for-node (398/398 retail LEVs EXACT)
  python parse_navmesh.py generate  <in.lev> [out.lev] [--tng file.tng] [--compare]
                                                build a TERRAIN-ONLY navmesh from the LEV cell
                                                grid (walkable byte +15, preferred-path byte +20,
                                                unreachable-island removal) and write a new .lev
                                                with the generated nav section(s). Existing nav
                                                (if any) is replaced; a navless LEV gains one
                                                "NULL" section. --tng supplies NAVIGATION_SEED
                                                placements for the island keep-set (else the
                                                largest walkable component is kept). --compare
                                                diffs generated layer-0 walkability vs the
                                                shipped navmesh (thing-blocking delta report).

Subdivision algorithm (recovered from CNavQuadTreeNode::Initialise 0x032851b0, validated by
the regen oracle): the map is tiled with 32-unit root cells; a cell subdivides while its area
is non-uniform (mixed walkability or mixed preferability), to depth 5 (1-unit cells) normally
or depth 6 (0.5-unit) inside "detailed areas"; uniform-clear+same-pref cells become navigable
leaves early, all-blocked subtrees collapse into the shared blocked singleton, switchable
leaves are only emitted at max depth (never merged). Walkability inputs (IsAreaClear
0x0328c7e0): TopologyWeights u8 grid (0xFF=blocked, sampled per map unit; derived from the
LEV cell walkable byte +15 and preferred-path byte +20: 0xFF/0x00/0x80) + per-layer blocking
lines in an 8x8-unit bucket grid + switchable blockage lines keyed by u64 (door uids).

===========================================================================================
PER-NODE ON-DISK LAYOUT (little-endian), for a CNavQuadTree of file version 8.

Every node record starts with a 3-byte discriminator head written by the shared base
prologue (CANavQuadTreeNode::SaveToFile) / CNavBlockedQuadTreeNode::SaveToFile:
    u8  isSwitchable      # node->IsSwitchable()   (vtbl+0x24)
    u8  blockedArg        # the 'blocked' arg for this node (1 for the shared blocked node)
    u8  switchFlag        # node+0x0f  (switchable/blocked flag; 1 on the blocked singleton)

  BLOCKED node (the shared CNavBlockedQuadTreeNode singleton) is written as EXACTLY those 3
  bytes and nothing else. The reader's blocked test is  isSwitchable==0 && switchFlag!=0
  (LoadFromFile: local_b2==0 && local_b1!=0) -> maps to the blocked sentinel. Children /
  neighbours pointing at "blocked" serialise as its sentinel index.

  Otherwise (a REAL node) the base prologue continues:
    u8    isLeaf          # node+0x0e  (0 => internal CNavQuadTreeNode; nonzero => leaf)
    u8    detailLevel     # node+0x0c  quadtree subdivision level 0..7 (MAX_DETAIL_LEVEL=7);
                          #   node size = SIZE_TABLE[detailLevel] = 32/2^level map units
    u8    navLayer        # node+0x0d  vertical navigation-layer index (0=ground)
    f32   posX ; f32 posY # node CENTRE position MINUS the map top-left origin (C2DVector)
    i32   myFileIndex     # this node's own index in the save-order table
  then a per-subclass tail:

  INTERNAL node (CNavQuadTreeNode, isLeaf==0):
    i32 child[4]          # file indices of the 4 quadrant children (TL,TR,BL,BR at
                          #   pos + (-q,-q),(+q,-q),(-q,+q),(+q,+q), q=size*0.25)
                          #   blocked children serialise as the blocked sentinel index

  NAVIGABLE leaf (CNavNavigableLeafNode, isLeaf!=0, isSwitchable==0):
    i32   regionIndex     # CNavRegion internal index (0 if node has no region)
    u8    preferability   # node+0x1c A* terrain-cost bias (version>6; default 0x40=64;
                          #   from CNavQuadTree::GetPreferabilityCostForNode/TopologyWeights)
    i32   numNeighbours
    i32   neighbour[numNeighbours]   # file indices of adjacency neighbours

  SWITCHABLE leaf (CNavSwitchableLeafNode, isSwitchable!=0): the navigable-leaf tail, then:
    u32   numSwitchKeys
    u64   switchKey[numSwitchKeys]   # 8-byte switch keys (door/gate group ids); bool=false

After all nodes:  i32 END_QUAD_TREE_SYMBOL (0xACBDEF12).
===========================================================================================
"""
import struct
import sys

QUAD_TREE_VERSION = 8
END_QUAD_TREE_SYMBOL = 0xACBDEF12          # LoadFromFile: test_end_symbol==END_QUAD_TREE_SYMBOL

# CANavQuadTreeNode::GetSize (0x0327f4a0) = DAT_044f344c[detailLevel]. Values confirmed
# empirically from BarrowFields child spacing (levels 0..5) and halving (6..7).
SIZE_TABLE = [32.0, 16.0, 8.0, 4.0, 2.0, 1.0, 0.5, 0.25]


# ------------------------------------------------------------------ LEV framing
def find_nav_offset(lev: bytes) -> int:
    """navOffset from the 25-byte LEVHeader (u32@21). See lev_rw.py for the full header."""
    header_size, = struct.unpack_from("<I", lev, 0)
    version, = struct.unpack_from("<H", lev, 4)
    assert header_size == 25, f"unexpected LEV headerSize {header_size}"
    assert version == 6404, f"unexpected LEV version {version}"
    nav_off, = struct.unpack_from("<I", lev, 21)
    return nav_off


def parse_nav_directory(lev: bytes, nav_off: int):
    """Nav-section directory (CWorldMap::SaveNavigationMapToFile):
        long navDirEnd; uint numSections;
        numSections x { long nameLen; char name[nameLen]; long sectionFileOffset }
    Each section's payload starts at sectionFileOffset and is one CNavQuadTree block; the
    block runs until the NEXT section's offset (== that block's back-patched reservedSize).
    """
    o = nav_off
    nav_dir_end, = struct.unpack_from("<i", lev, o); o += 4
    num_sections, = struct.unpack_from("<I", lev, o); o += 4
    sections = []
    for _ in range(num_sections):
        nlen, = struct.unpack_from("<i", lev, o); o += 4
        name = lev[o:o + nlen].decode("latin1"); o += nlen
        foff, = struct.unpack_from("<i", lev, o); o += 4
        sections.append({"name": name, "offset": foff})
    return {"nav_dir_end": nav_dir_end, "toc_end": o, "sections": sections}


# ------------------------------------------------------------------ reader
class Reader:
    __slots__ = ("buf", "pos")

    def __init__(self, buf, pos=0):
        self.buf = buf
        self.pos = pos

    def i32(self):
        v, = struct.unpack_from("<i", self.buf, self.pos); self.pos += 4; return v

    def u32(self):
        v, = struct.unpack_from("<I", self.buf, self.pos); self.pos += 4; return v

    def u64(self):
        v, = struct.unpack_from("<Q", self.buf, self.pos); self.pos += 8; return v

    def f32(self):
        v, = struct.unpack_from("<f", self.buf, self.pos); self.pos += 4; return v

    def u8(self):
        v = self.buf[self.pos]; self.pos += 1; return v


def parse_quadtree(lev: bytes, sec_off: int):
    """Parse one CNavQuadTree block; returns (header, positions, nodes, end_offset).
    Mirrors CNavQuadTree::LoadFromFile field-for-field."""
    r = Reader(lev, sec_off)
    hdr = {
        "reservedSize": r.i32(),        # back-patched: next section's file offset (block end)
        "version": r.i32(),
    }
    assert hdr["version"] == QUAD_TREE_VERSION, f"bad quadtree version {hdr['version']}"
    version = hdr["version"]
    # this+0x10 / this+0x14: reader asserts MapWidth==map_area.GetWidth() and
    # MapHeight==map_area.GetHeight(), so these are the map box dimensions (not node sizes).
    hdr["mapWidth"] = r.f32()
    hdr["mapHeight"] = r.f32()
    hdr["numNavRegions"] = r.u32()
    hdr["numNavPositions"] = r.u32()
    positions = []
    for _ in range(hdr["numNavPositions"]):
        x = r.f32(); y = r.f32(); data = r.i32()   # CNavigationPosition: C2DVector + long
        positions.append((x, y, data))
    hdr["numLayers"] = r.u32()
    hdr["totalNodeCount"] = r.i32()

    nodes = []
    for _ in range(hdr["totalNodeCount"]):
        start = r.pos
        # 3-byte discriminator (base prologue head): the reader's blocked-singleton test is
        # exactly (isSwitchable==0 && switchFlag!=0). isSwitchable = node->IsSwitchable()
        # (node+0x0f-derived); switchFlag = node+0x0f. The middle byte is the 'blocked' arg.
        is_switchable = r.u8() if version > 5 else 0
        blocked_arg = r.u8()
        switch_flag = r.u8()
        rec = {"_start": start, "isSwitchable": is_switchable,
               "blockedArg": blocked_arg, "switchFlag": switch_flag}
        if is_switchable == 0 and switch_flag != 0:
            rec["kind"] = "blocked"
        else:
            is_leaf = r.u8()          # node+0x0e  (0 => internal, nonzero => leaf)
            detail_level = r.u8()     # node+0x0c  quadtree level (size=SIZE_TABLE[level])
            nav_layer = r.u8()        # node+0x0d  vertical navigation layer
            px = r.f32(); py = r.f32()
            my_index = r.i32()
            rec.update(isLeaf=is_leaf, detailLevel=detail_level, navLayer=nav_layer,
                       posX=px, posY=py, myIndex=my_index)
            if is_leaf == 0:
                rec["kind"] = "internal"
                rec["children"] = [r.i32() for _ in range(4)]
            else:
                rec["kind"] = "switchable" if is_switchable else "navigable"
                rec["regionIndex"] = r.i32()
                rec["preferability"] = r.u8() if version > 6 else 0x40
                nnb = r.i32()
                rec["neighbours"] = [r.i32() for _ in range(nnb)]
                if is_switchable:
                    nkeys = r.u32()
                    rec["switchKeys"] = [r.u64() for _ in range(nkeys)]
        rec["_end"] = r.pos
        nodes.append(rec)

    end_sym = r.u32()
    assert end_sym == END_QUAD_TREE_SYMBOL, \
        f"bad end symbol {hex(end_sym)} at {hex(r.pos-4)} (expected {hex(END_QUAD_TREE_SYMBOL)})"
    return hdr, positions, nodes, r.pos


# ------------------------------------------------------------------ writer (round-trip)
class Writer:
    __slots__ = ("buf",)

    def __init__(self):
        self.buf = bytearray()

    def i32(self, v): self.buf += struct.pack("<i", v)
    def u32(self, v): self.buf += struct.pack("<I", v)
    def u64(self, v): self.buf += struct.pack("<Q", v)
    def f32(self, v): self.buf += struct.pack("<f", v)
    def u8(self, v): self.buf += struct.pack("<B", v)


def emit_quadtree(hdr, positions, nodes) -> bytes:
    """Re-serialise a parsed block. Byte-exactness against the source proves the layout."""
    w = Writer()
    w.i32(hdr["reservedSize"])
    w.i32(hdr["version"])
    w.f32(hdr["mapWidth"])
    w.f32(hdr["mapHeight"])
    w.u32(hdr["numNavRegions"])
    w.u32(hdr["numNavPositions"])
    for x, y, data in positions:
        w.f32(x); w.f32(y); w.i32(data)
    w.u32(hdr["numLayers"])
    w.i32(hdr["totalNodeCount"])
    version = hdr["version"]
    for rec in nodes:
        if version > 5:
            w.u8(rec["isSwitchable"])
        w.u8(rec["blockedArg"])
        w.u8(rec["switchFlag"])
        if rec["kind"] == "blocked":
            continue
        w.u8(rec["isLeaf"])
        w.u8(rec["detailLevel"]); w.u8(rec["navLayer"])
        w.f32(rec["posX"]); w.f32(rec["posY"])
        w.i32(rec["myIndex"])
        if rec["kind"] == "internal":
            for c in rec["children"]:
                w.i32(c)
        else:
            w.i32(rec["regionIndex"])
            if version > 6:
                w.u8(rec["preferability"])
            w.i32(len(rec["neighbours"]))
            for nb in rec["neighbours"]:
                w.i32(nb)
            if rec["kind"] == "switchable":
                w.u32(len(rec["switchKeys"]))
                for k in rec["switchKeys"]:
                    w.u64(k)
    w.u32(END_QUAD_TREE_SYMBOL)
    return bytes(w.buf)


def round_trip(lev: bytes, sec_off: int):
    """Parse then re-emit the block; compare byte-for-byte against the original span."""
    hdr, positions, nodes, end = parse_quadtree(lev, sec_off)
    original = lev[sec_off:end]
    rebuilt = emit_quadtree(hdr, positions, nodes)
    ok = original == rebuilt
    first_diff = None
    if not ok:
        for i in range(min(len(original), len(rebuilt))):
            if original[i] != rebuilt[i]:
                first_diff = i
                break
        if first_diff is None:
            first_diff = min(len(original), len(rebuilt))
    return ok, hdr, positions, nodes, end, first_diff, len(original), len(rebuilt)


# ------------------------------------------------------------------ semantic decode
def rasterize_walkable(hdr, nodes, layer=0, res=1.0):
    """Decode the quadtree leaves of one nav layer into a 2D walkability raster.

    Returns (grid, nx, ny): grid[y][x] over map-relative coords with cell size `res`;
    values: 1 = navigable leaf covers it, 2 = switchable leaf, 0 = blocked (no leaf).
    Leaf coverage = pos +/- SIZE_TABLE[detailLevel]/2 (GetAreaCovering 0x0327f400).
    """
    nx = int(round(hdr["mapWidth"] / res))
    ny = int(round(hdr["mapHeight"] / res))
    grid = [[0] * nx for _ in range(ny)]
    for n in nodes:
        if n["kind"] not in ("navigable", "switchable") or n["navLayer"] != layer:
            continue
        half = SIZE_TABLE[n["detailLevel"]] * 0.5
        x0 = int(round((n["posX"] - half) / res)); x1 = int(round((n["posX"] + half) / res))
        y0 = int(round((n["posY"] - half) / res)); y1 = int(round((n["posY"] + half) / res))
        v = 2 if n["kind"] == "switchable" else 1
        for y in range(max(y0, 0), min(y1, ny)):
            row = grid[y]
            for x in range(max(x0, 0), min(x1, nx)):
                row[x] = v
    return grid, nx, ny


def correlate_with_lev(lev_path):
    """Empirical cross-check (NAVMESH_RE goal 3): decode the ground-layer navmesh
    walkability and compare per-map-unit against the LEV terrain cell walkable bytes."""
    import os
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import lev_rw
    lev_bytes = open(lev_path, "rb").read()
    lev = lev_rw.read_file(lev_path)
    d = parse_nav_directory(lev_bytes, find_nav_offset(lev_bytes))
    hdr, positions, nodes, _ = parse_quadtree(lev_bytes, d["sections"][0]["offset"])
    grid, nx, ny = rasterize_walkable(hdr, nodes, layer=0, res=1.0)
    # LEV grid is (width+1)x(height+1) vertices; sample cell (x,y) for map unit (x,y).
    conf = [[0, 0], [0, 0]]   # conf[lev_walkable][nav_walkable]
    for y in range(ny):
        for x in range(nx):
            lw = 1 if lev.cell(x, y).walkable else 0
            nw = 1 if grid[y][x] != 0 else 0
            conf[lw][nw] += 1
    total = nx * ny
    agree = conf[0][0] + conf[1][1]
    print(f"map {nx}x{ny} = {total} unit cells   (nav layer 0 vs LEV cell walkable byte)")
    print(f"  LEV walkable & nav walkable : {conf[1][1]}")
    print(f"  LEV walkable & nav BLOCKED  : {conf[1][0]}")
    print(f"  LEV blocked  & nav walkable : {conf[0][1]}")
    print(f"  LEV blocked  & nav BLOCKED  : {conf[0][0]}")
    print(f"  agreement: {agree}/{total} = {100.0*agree/total:.1f}%")
    nav_w = conf[0][1] + conf[1][1]
    lev_w = conf[1][0] + conf[1][1]
    print(f"  nav-walkable total {nav_w} ({100.0*nav_w/total:.1f}%), "
          f"LEV-walkable total {lev_w} ({100.0*lev_w/total:.1f}%)")
    if lev_w:
        print(f"  P(nav walkable | LEV walkable) = {100.0*conf[1][1]/lev_w:.1f}%")
    if total - lev_w:
        print(f"  P(nav blocked  | LEV blocked)  = {100.0*conf[0][0]/(total-lev_w):.1f}%")
    return conf


# ------------------------------------------------------------------ PoC regenerator
def _leaf_raster(hdr, nodes, layer, res=0.5):
    """Exact per-res-cell class raster from the shipped leaves of one layer.
    Cell value: ('B',) blocked / ('N', pref) navigable / ('S', pref, keys) switchable."""
    nx = int(round(hdr["mapWidth"] / res)); ny = int(round(hdr["mapHeight"] / res))
    grid = [[("B",)] * nx for _ in range(ny)]
    for n in nodes:
        if n["kind"] not in ("navigable", "switchable") or n["navLayer"] != layer:
            continue
        half = SIZE_TABLE[n["detailLevel"]] * 0.5
        x0 = int(round((n["posX"] - half) / res)); x1 = int(round((n["posX"] + half) / res))
        y0 = int(round((n["posY"] - half) / res)); y1 = int(round((n["posY"] + half) / res))
        # Switchable cells record the shipped leaf level too: type-3 leaves are only
        # emitted at max depth (CNavQuadTreeNode::Initialise), so uniform switchable
        # regions must keep subdividing down to that level instead of merging.
        v = ("S", n["preferability"], tuple(sorted(n["switchKeys"])), n["detailLevel"]) \
            if n["kind"] == "switchable" else ("N", n["preferability"])
        for y in range(max(y0, 0), min(y1, ny)):
            for x in range(max(x0, 0), min(x1, nx)):
                grid[y][x] = v
    return grid, nx, ny


def regenerate_layer(hdr, grid, layer, res=0.5):
    """Re-run the recovered subdivision algorithm (CNavQuadTreeNode::Initialise 0x032851b0)
    over a class raster. Returns the list of produced real nodes as comparable tuples
    (kind, level, cx, cy, payload). Split rule: subdivide while the covered area is
    non-uniform (mixed walkability or preferability) — the raster is leaf-exact at `res`,
    so recursion terminates at the level whose cell size == res."""
    out = []

    def classify(level, cx, cy):
        size = SIZE_TABLE[level]
        x0 = int(round((cx - size / 2) / res)); x1 = int(round((cx + size / 2) / res))
        y0 = int(round((cy - size / 2) / res)); y1 = int(round((cy + size / 2) / res))
        first = grid[y0][x0]
        uniform = all(grid[y][x] == first
                      for y in range(y0, y1) for x in range(x0, x1))
        if uniform:
            if first[0] == "B":
                return "blocked"
            if first[0] == "N":
                out.append(("navigable", level, round(cx, 4), round(cy, 4), first[1]))
                return "navigable"
            # switchable: no early merge — leaf only at the recorded max-depth level
            if level == first[3]:
                out.append(("switchable", level, round(cx, 4), round(cy, 4),
                            first[1], first[2]))
                return "switchable"
        q = size * 0.25
        kinds = [classify(level + 1, cx + dx, cy + dy)
                 for dx, dy in ((-q, -q), (q, -q), (-q, q), (q, q))]
        if all(k == "blocked" for k in kinds):
            return "blocked"   # bottom-up collapse (all-blocked parent -> singleton)
        out.append(("internal", level, round(cx, 4), round(cy, 4)))
        return "internal"

    root = SIZE_TABLE[0]
    for gy in range(int(round(hdr["mapHeight"] / root))):
        for gx in range(int(round(hdr["mapWidth"] / root))):
            classify(0, root * (gx + 0.5), root * (gy + 0.5))
    return out


def regen_compare(lev_path):
    """Stretch-goal oracle: shipped tree -> leaf raster -> regenerate with the recovered
    algorithm -> compare node multiset (kind, level, centre, payload) per layer."""
    lev_bytes = open(lev_path, "rb").read()
    d = parse_nav_directory(lev_bytes, find_nav_offset(lev_bytes))
    all_ok = True
    for s in d["sections"]:
        hdr, positions, nodes, _ = parse_quadtree(lev_bytes, s["offset"])
        print(f"[section {s['name']!r}]")
        for layer in range(hdr["numLayers"]):
            shipped = []
            for n in nodes:
                if n["kind"] == "blocked" or n["navLayer"] != layer:
                    continue
                t = (n["kind"], n["detailLevel"], round(n["posX"], 4), round(n["posY"], 4))
                if n["kind"] == "navigable":
                    t += (n["preferability"],)
                elif n["kind"] == "switchable":
                    t += (n["preferability"], tuple(sorted(n["switchKeys"])))
                shipped.append(t)
            grid, _, _ = _leaf_raster(hdr, nodes, layer)
            regen = regenerate_layer(hdr, grid, layer)
            ship_set, regen_set = sorted(shipped), sorted(regen)
            ok = ship_set == regen_set
            all_ok &= ok
            status = "EXACT MATCH" if ok else "MISMATCH"
            print(f"  layer {layer}: shipped {len(ship_set)} real nodes, "
                  f"regenerated {len(regen_set)} -> {status}")
            if not ok:
                miss = [t for t in ship_set if t not in set(regen_set)][:5]
                extra = [t for t in regen_set if t not in set(ship_set)][:5]
                print(f"    only-shipped (first 5): {miss}")
                print(f"    only-regen   (first 5): {extra}")
    print(f"=== REGENERATION {'REPRODUCES' if all_ok else 'DOES NOT REPRODUCE'} "
          f"THE SHIPPED TREE TOPOLOGY ===")
    return all_ok


# ------------------------------------------------------------------ terrain-only GENERATOR
#
# Builds a from-scratch CNavQuadTree for a LEV using only its terrain cell grid, mirroring
# the shipped writer's conventions (all reverse-engineered from the retail corpus in the
# 2026-07-20 session, see docs/NAVMESH_RE.md "GENERATOR" section):
#
#   TopologyWeights   cell walkable byte +15 == 0 -> hard blocked (0xFF); cell byte +20 == 1
#                     -> preferred path (pref 0x00); else normal ground (pref 0x80).
#   Island removal    walkable cells NOT 4-connected to a NAVIGATION_SEED thing (or to the
#                     largest walkable component when no seed is known) are blocked wholesale
#                     (corpus-observed: unseeded terrain islands are 100% nav-blocked).
#   Subdivision       the proven recursive classifier (CNavQuadTreeNode::Initialise), depth
#                     capped at level 5 (1-unit cells); no DetailedAreas -> no level 6.
#   Index assignment  counter starts at 1; per layer, per root cell in row-major order,
#                     POST-order over real nodes (children TL,TR,BL,BR then parent); a fully
#                     blocked ROOT cell burns one index (writes only the 3-byte blocked
#                     record). File records are written in PRE-order over the same loop.
#                     (Verified against BarrowFields: burned indices 1,2,817,2228..2233 ==
#                     the blocked root slots; totalNodeCount == final counter-1.)
#   Neighbours        exact same-layer edge adjacency (boxes sharing an edge segment with
#                     positive overlap; corner-only contact is NOT adjacency). Verified 60/60
#                     sampled shipped leaves. Cross-layer links (JoinNavigationLayers) do not
#                     apply to a single-layer terrain mesh.
#   Regions           flood fill over navigable-leaf adjacency; regions numbered 1..N by
#                     first appearance in file (pre-)order; header numRegions = N+1 when any
#                     leaf exists else 0 (matches all 148 nav-bearing retail LEVs).

def build_terrain_raster(lev, tng_path=None):
    """1-map-unit class raster from the LEV cell grid: ('B',) blocked / ('N', pref).
    Applies the TopologyWeights derivation + unreachable-island removal."""
    from collections import deque
    w, h = lev.width, lev.height
    grid = [[None] * w for _ in range(h)]
    for y in range(h):
        for x in range(w):
            c = lev.cell(x, y)
            if c.walkable_byte == 0:
                grid[y][x] = ("B",)
            else:
                grid[y][x] = ("N", 0x00 if c.raw[20] == 1 else 0x80)
    # --- island removal: keep only components reachable from a NAVIGATION_SEED ---
    comp = [[-1] * w for _ in range(h)]
    comp_sizes = []
    for y in range(h):
        for x in range(w):
            if grid[y][x][0] == "N" and comp[y][x] < 0:
                cid = len(comp_sizes)
                q = deque([(x, y)]); comp[y][x] = cid; n = 0
                while q:
                    cx, cy = q.popleft(); n += 1
                    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                        px, py = cx + dx, cy + dy
                        if 0 <= px < w and 0 <= py < h and grid[py][px][0] == "N" \
                                and comp[py][px] < 0:
                            comp[py][px] = cid; q.append((px, py))
                comp_sizes.append(n)
    # keep-set anchors (corpus rule, 0 exceptions over the 148 nav-bearing retail LEVs
    # tested): a walkable component survives iff it contains a NAVIGATION_SEED or a
    # REGION_ENTRANCE_POINT / REGION_EXIT_POINT placement. Plain markers do NOT count.
    keep = set()
    if tng_path:
        import re as _re
        anchors = ('"NAVIGATION_SEED"', '"REGION_ENTRANCE_POINT"', '"REGION_EXIT_POINT"')
        txt = open(tng_path, encoding="latin-1").read()
        for t in txt.split("NewThing")[1:]:
            if not any(a in t for a in anchors):
                continue
            px = _re.search(r"PositionX ([\d.\-eE]+);", t)
            py = _re.search(r"PositionY ([\d.\-eE]+);", t)
            if px and py:
                sx, sy = int(float(px.group(1))), int(float(py.group(1)))
                if 0 <= sx < w and 0 <= sy < h and comp[sy][sx] >= 0:
                    keep.add(comp[sy][sx])
    if not keep and comp_sizes:
        keep.add(max(range(len(comp_sizes)), key=lambda i: comp_sizes[i]))
    removed = 0
    for y in range(h):
        for x in range(w):
            if grid[y][x][0] == "N" and comp[y][x] not in keep:
                grid[y][x] = ("B",); removed += 1
    return grid, removed


class _GenNode:
    __slots__ = ("kind", "level", "cx", "cy", "pref", "children", "index",
                 "region", "neighbours")

    def __init__(self, kind, level, cx, cy, pref=0x40):
        self.kind = kind          # 'internal' | 'navigable' | 'blocked'
        self.level = level
        self.cx = cx; self.cy = cy
        self.pref = pref
        self.children = None      # internal: list of 4 (_GenNode or 'blocked')
        self.index = 0
        self.region = 0
        self.neighbours = []


def _gen_subdivide(grid, level, cx, cy, max_level=5):
    """The recovered classifier over the 1-unit terrain raster; returns a _GenNode
    or the string 'blocked' for a fully blocked area."""
    size = SIZE_TABLE[level]
    x0 = int(round(cx - size / 2)); x1 = int(round(cx + size / 2))
    y0 = int(round(cy - size / 2)); y1 = int(round(cy + size / 2))
    first = grid[y0][x0]
    uniform = all(grid[y][x] == first for y in range(y0, y1) for x in range(x0, x1))
    if uniform:
        if first[0] == "B":
            return "blocked"
        return _GenNode("navigable", level, cx, cy, first[1])
    # non-uniform -> subdivide (raster is 1-unit so level 5 cells are always uniform)
    assert level < max_level
    node = _GenNode("internal", level, cx, cy)
    q = size * 0.25
    node.children = [_gen_subdivide(grid, level + 1, cx + dx, cy + dy, max_level)
                     for dx, dy in ((-q, -q), (q, -q), (-q, q), (q, q))]
    if all(c == "blocked" for c in node.children):
        return "blocked"
    return node


def generate_tree(grid, map_w, map_h):
    """Terrain-only single-layer tree. Returns (roots row-major, file_records,
    total_node_count, num_regions) with indices/neighbours/regions assigned per the
    shipped writer's conventions."""
    root = SIZE_TABLE[0]
    roots = []
    for gy in range(int(round(map_h / root))):
        for gx in range(int(round(map_w / root))):
            roots.append(_gen_subdivide(grid, 0, root * (gx + 0.5), root * (gy + 0.5)))
    # pass 1: post-order index assignment; blocked roots burn one index
    counter = 1
    def assign(node):
        nonlocal counter
        if node.kind == "internal":
            for c in node.children:
                if c != "blocked":
                    assign(c)
        node.index = counter; counter += 1
    for r in roots:
        if r == "blocked":
            counter += 1
        else:
            assign(r)
    total_node_count = counter - 1
    # pass 2 order: pre-order file records
    records = []
    def emit(node):
        records.append(node)
        if node.kind == "internal":
            for c in node.children:
                if c != "blocked":
                    emit(c)
    for r in roots:
        if r == "blocked":
            records.append("blocked")
        else:
            emit(r)
    # neighbours: exact edge adjacency via a leaf-id raster (all boundaries on unit lines)
    w = int(round(map_w)); h = int(round(map_h))
    leaf_at = [[None] * w for _ in range(h)]
    leaves = [n for n in records if n != "blocked" and n.kind == "navigable"]
    for n in leaves:
        half = SIZE_TABLE[n.level] * 0.5
        for y in range(int(round(n.cy - half)), int(round(n.cy + half))):
            for x in range(int(round(n.cx - half)), int(round(n.cx + half))):
                leaf_at[y][x] = n
    nbset = {id(n): set() for n in leaves}
    for y in range(h):
        for x in range(w):
            a = leaf_at[y][x]
            if a is None:
                continue
            for dx, dy in ((1, 0), (0, 1)):
                px, py = x + dx, y + dy
                if px < w and py < h:
                    b = leaf_at[py][px]
                    if b is not None and b is not a:
                        nbset[id(a)].add(id(b)); nbset[id(b)].add(id(a))
    byid = {id(n): n for n in leaves}
    for n in leaves:
        n.neighbours = sorted(byid[i].index for i in nbset[id(n)])
    # regions: flood fill over leaf adjacency, numbered by first appearance in file order
    nreg = 0
    for n in leaves:
        if n.region == 0:
            nreg += 1
            stack = [n]; n.region = nreg
            while stack:
                m = stack.pop()
                for i in nbset[id(m)]:
                    o = byid[i]
                    if o.region == 0:
                        o.region = nreg; stack.append(o)
    # numRegions = N+1 whenever a nav layer exists (region 0 = "none" is counted);
    # shipped all-blocked one-layer trees carry numRegions=1 (NorthernWastes1_Leadout_01).
    num_regions = nreg + 1
    return roots, records, total_node_count, num_regions


def emit_generated_block(records, total_node_count, num_regions, map_w, map_h,
                         positions, block_end_abs):
    """Serialize one generated CNavQuadTree block (single nav layer)."""
    w = Writer()
    w.i32(block_end_abs)              # reservedSize back-patch = absolute block end
    w.i32(QUAD_TREE_VERSION)
    w.f32(float(map_w)); w.f32(float(map_h))
    w.u32(num_regions)
    w.u32(len(positions))
    for x, y, data in positions:
        w.f32(x); w.f32(y); w.i32(data)
    w.u32(1)                          # numLayers: terrain-only ground layer
    w.i32(total_node_count)
    for n in records:
        if n == "blocked":
            w.u8(0); w.u8(1); w.u8(1)        # blocked record head (isSwitchable, blockedArg, switchFlag)
            continue
        w.u8(0)                              # isSwitchable
        w.u8(1 if n.level == 0 else 0)       # blockedArg: 1 marks root-grid-slot records
        w.u8(0)                              # switchFlag
        w.u8(0 if n.kind == "internal" else 1)   # isLeaf
        w.u8(n.level)                        # detailLevel
        w.u8(0)                              # navLayer (ground)
        w.f32(n.cx); w.f32(n.cy)
        w.i32(n.index)
        if n.kind == "internal":
            for c in n.children:
                w.i32(0 if c == "blocked" else c.index)
        else:
            w.i32(n.region)
            w.u8(n.pref)
            w.i32(len(n.neighbours))
            for i in n.neighbours:
                w.i32(i)
    w.u32(END_QUAD_TREE_SYMBOL)
    return bytes(w.buf)


def generate_lev(lev_path, out_path=None, tng_path=None, compare=False):
    """Terrain-only navmesh generation: replace (or create) the nav sections of a LEV."""
    import os
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import lev_rw
    lev_bytes = open(lev_path, "rb").read()
    lev = lev_rw.parse(lev_bytes)
    nav_off = find_nav_offset(lev_bytes)
    old_dir = parse_nav_directory(lev_bytes, nav_off)
    # section names + per-section shipped positions (kept: they are action points,
    # not derived from terrain); a navless LEV gains a single "NULL" section.
    sections = []
    for s in old_dir["sections"]:
        hdr, positions, nodes, _ = parse_quadtree(lev_bytes, s["offset"])
        sections.append((s["name"], positions))
    if not sections:
        sections = [("NULL", [])]

    grid, removed = build_terrain_raster(lev, tng_path)
    walk = sum(1 for row in grid for c in row if c[0] == "N")
    print(f"terrain raster {lev.width}x{lev.height}: {walk} walkable cells, "
          f"{removed} island cells removed")
    roots, records, total, nreg = generate_tree(grid, lev.width, lev.height)
    kinds = {}
    for r in records:
        k = "blocked" if r == "blocked" else r.kind
        kinds[k] = kinds.get(k, 0) + 1
    print(f"generated tree: {len(records)} records {kinds}, totalNodeCount={total}, "
          f"numRegions={nreg}")

    # frame the nav suffix: directory then one block per section
    dir_buf = Writer()
    name_bytes = [(n.encode("latin1"), pos) for n, pos in sections]
    dir_len = 8 + sum(4 + len(nb) + 4 for nb, _ in name_bytes)
    first_block = nav_off + dir_len
    # blocks are identical trees per section (shipped files duplicate the map tree
    # per section) but positions can differ per section
    blocks = []
    off = first_block
    for i, (nb, positions) in enumerate(name_bytes):
        blk = emit_generated_block(records, total, nreg, lev.width, lev.height,
                                   positions, 0)
        blocks.append((off, positions, len(blk)))
        off += len(blk)
    # re-emit with correct reservedSize (absolute end of each block)
    out_blocks = []
    for i, (boff, positions, blen) in enumerate(blocks):
        out_blocks.append(emit_generated_block(records, total, nreg, lev.width,
                                               lev.height, positions, boff + blen))
    dir_buf.i32(first_block)          # navDirEnd
    dir_buf.u32(len(sections))
    for i, (nb, _) in enumerate(name_bytes):
        dir_buf.i32(len(nb))
        dir_buf.buf += nb
        dir_buf.i32(blocks[i][0])
    new_lev = lev_bytes[:nav_off] + bytes(dir_buf.buf) + b"".join(out_blocks)

    # ---- structural self-validation on the produced file ----
    d2 = parse_nav_directory(new_lev, find_nav_offset(new_lev))
    assert d2["nav_dir_end"] == d2["toc_end"] == d2["sections"][0]["offset"]
    ok_all = True
    for s in d2["sections"]:
        ok, hdr2, pos2, nodes2, end2, fd, lo, lr = round_trip(new_lev, s["offset"])
        ok_all &= ok
        # index integrity: every child/neighbour reference resolves; neighbour symmetry
        byidx = {n["myIndex"]: n for n in nodes2 if n["kind"] != "blocked"}
        for n in nodes2:
            if n["kind"] == "internal":
                for c in n["children"]:
                    assert c == 0 or c in byidx, f"dangling child index {c}"
            elif n["kind"] != "blocked":
                for i in n["neighbours"]:
                    assert i in byidx and byidx[i]["kind"] == "navigable"
                    assert n["myIndex"] in byidx[i]["neighbours"], "asymmetric neighbours"
    print(f"self-validation: directory OK, {len(d2['sections'])} section(s) round-trip "
          f"byte-identical={ok_all}, indices resolve, neighbours symmetric")
    hdr_last, _, _, end_last = parse_quadtree(new_lev, d2["sections"][-1]["offset"])
    assert hdr_last["reservedSize"] == len(new_lev) == end_last, \
        "last block reservedSize must chain to EOF"

    if compare and old_dir["sections"]:
        hdr0, _, nodes0, _ = parse_quadtree(lev_bytes, old_dir["sections"][0]["offset"])
        g_old, nx, ny = rasterize_walkable(hdr0, nodes0, layer=0, res=1.0)
        hdrn, _, nodesn, _ = parse_quadtree(new_lev, d2["sections"][0]["offset"])
        g_new, _, _ = rasterize_walkable(hdrn, nodesn, layer=0, res=1.0)
        conf = [[0, 0], [0, 0]]
        for y in range(ny):
            for x in range(nx):
                conf[1 if g_old[y][x] else 0][1 if g_new[y][x] else 0] += 1
        total_cells = nx * ny
        agree = conf[0][0] + conf[1][1]
        print(f"compare vs shipped (layer 0, 1-unit): agreement {agree}/{total_cells} "
              f"= {100.0*agree/total_cells:.1f}%")
        print(f"  shipped-walkable & gen-walkable : {conf[1][1]}")
        print(f"  shipped-walkable & gen-BLOCKED  : {conf[1][0]}   (should be ~0: "
              f"terrain-only never blocks more than shipped)")
        print(f"  shipped-BLOCKED  & gen-walkable : {conf[0][1]}   (= thing-collision "
              f"blocking the terrain-only mesh cannot know about)")

    if out_path:
        with open(out_path, "wb") as f:
            f.write(new_lev)
        # the written file must still parse as a LEV
        lev_rw.parse(new_lev)
        print(f"wrote {out_path} ({len(new_lev)} bytes; was {len(lev_bytes)})")
    return new_lev


def _histogram(nodes):
    h = {}
    for n in nodes:
        h[n["kind"]] = h.get(n["kind"], 0) + 1
    return h


if __name__ == "__main__":
    args = sys.argv[1:]
    if args and args[0] == "correlate":
        correlate_with_lev(args[1] if len(args) > 1 else
                           r"D:/Documents/FableTLC/work/level_samples/BarrowFields/BarrowFields.lev")
        sys.exit(0)
    if args and args[0] == "generate":
        rest = args[1:]
        tng = None; comp = False; paths = []
        i = 0
        while i < len(rest):
            if rest[i] == "--tng":
                tng = rest[i + 1]; i += 2
            elif rest[i] == "--compare":
                comp = True; i += 1
            else:
                paths.append(rest[i]); i += 1
        if not paths:
            print("usage: parse_navmesh.py generate <in.lev> [out.lev] "
                  "[--tng file.tng] [--compare]")
            sys.exit(2)
        generate_lev(paths[0], paths[1] if len(paths) > 1 else None, tng, comp)
        sys.exit(0)
    if args and args[0] == "regen":
        ok = regen_compare(args[1] if len(args) > 1 else
                           r"D:/Documents/FableTLC/work/level_samples/BarrowFields/BarrowFields.lev")
        sys.exit(0 if ok else 1)
    path = args[0] if args else \
        r"D:/Documents/FableTLC/work/level_samples/BarrowFields/BarrowFields.lev"
    lev = open(path, "rb").read()
    nav_off = find_nav_offset(lev)
    d = parse_nav_directory(lev, nav_off)
    print(f"navOffset={hex(nav_off)}  navDirEnd={hex(d['nav_dir_end'])}  "
          f"tocEnd={hex(d['toc_end'])}  (toc==dirEnd: {d['toc_end']==d['nav_dir_end']})")
    for i, s in enumerate(d["sections"]):
        print(f"  section[{i}] name={s['name']!r} offset={hex(s['offset'])}")
    all_ok = True
    for i, s in enumerate(d["sections"]):
        ok, hdr, positions, nodes, end, fd, lo, lr = round_trip(lev, s["offset"])
        all_ok &= ok
        status = "OK (byte-identical)" if ok else \
            f"MISMATCH @ +{fd} (orig {lo}B / rebuilt {lr}B)"
        print(f"\n[section {i} {s['name']!r}] @ {hex(s['offset'])} .. {hex(end)}  "
              f"ROUND-TRIP {status}")
        print(f"  version={hdr['version']} map={hdr['mapWidth']}x{hdr['mapHeight']} "
              f"regions={hdr['numNavRegions']} "
              f"positions={hdr['numNavPositions']} layers={hdr['numLayers']} "
              f"nodes={hdr['totalNodeCount']}")
        print(f"  node kinds: {_histogram(nodes)}")
    print(f"\n=== ALL SECTIONS ROUND-TRIP BYTE-IDENTICAL: {all_ok} ===")
    sys.exit(0 if all_ok else 1)
