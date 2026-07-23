#!/usr/bin/env python3
r"""Native, dependency-free LEV terrain reader + WRITER for Fable: The Lost Chapters.

Route B pattern (mirrors tools/blender_addon/io_scene_fable/mesh_rw.py): parse a real
.lev into a struct, serialize it back, and gate on a BYTE-IDENTICAL round-trip for the
edit-nothing case. LEV is fixed-layout, so byte-exact IS achievable and required.

Byte layout (cross-checked against 3 sources: FableForge forge::lev
[libs/forgecore/src/lev.cpp], the FableMod.LEV decompiled reader it cites, and a scan of
all 398 real FinalAlbion LEVs which parse with 0 failures):

  LEVHeader   (25B): u32 headerSize(==25), u16 version(==6404), u8 pad[3],
                     u32 reserved1, u32 obsOffset, u32 reserved2, u32 navOffset
  LEVMapHeader(22B): u8 size, u8 mapVersion(==8), u8 mpad[3] (mpad[2] is the
                     sub-version, 8 or 9), u32 uidLo, u32 uidHi,
                     i32 width, i32 height, u8 flag
  256 ground themes: char name[128] + u32 value                     (256*132 = 33792B)
  u32 cellVersion, u32 themeCount, u8 palette[33792],
  (sub-version 9 only) u32 extra,
  (themeCount-1) x { u32 len, char name[len] } theme strings,
  cells: (height+1)*(width+1) x 21B                                  <-- the EDITABLE grid
         float heightRaw at +5 (world height = raw * 2048),
         theme indices at +10..12, strengths at +13..14, walkable byte at +15,
  opaque obstacle/brush region up to navOffset (obsOffset points inside it),
  navigation sections from navOffset (8B nav header + { u32 len, name, u32 off } TOC,
         then section payloads).

WRITER strategy for byte-exactness: the file is captured as three spans --
  prefix  = everything before the cell grid, PARSED into named fields and re-emitted
            (reconstruction is verified byte-identical to the original prefix),
  cells   = the (h+1)*(w+1) x 21B grid, the mutable payload,
  suffix  = obstacle/brush region + nav TOC + section payloads, preserved VERBATIM.

Out of scope (left for a later Ghidra job -- CMap::SaveToFile): regenerating the nav
mesh / region graph / obstacle-brush data, and brand-new-level creation. Those are
preserved opaque; editing cell height/theme/walkable does not touch them.

Cell-field editing preserves cell size, so navOffset/obsOffset stay valid. This writer
never changes grid dimensions; doing so would require rewriting the nav/obstacle data.
"""
import struct
import sys

LEV_VERSION = 6404
MAP_VERSION = 8
CELL_SIZE = 21
PALETTE_SIZE = 33792
GROUND_THEME_COUNT = 256
GROUND_THEME_NAME_LEN = 128
HEIGHT_SCALE = 2048.0

# Byte offsets inside a 21-byte cell.
OFF_HEIGHT = 5        # float32 raw height (world = raw * HEIGHT_SCALE)
OFF_THEME_IDX = 10    # 3 bytes: theme blend layer indices [10,11,12]
OFF_THEME_STR = 13    # stored strengths for slots 0/1; slot 2 = 255-s0-s1
OFF_WALKABLE = 15     # 1 byte: nonzero == walkable


class Cell:
    """A live view into the 21-byte record for one grid cell.

    Backed by the shared bytearray in LevFile.cells so mutations write through.
    """
    __slots__ = ("_buf", "_base")

    def __init__(self, buf, base):
        self._buf = buf
        self._base = base

    @property
    def raw(self):
        """The 21 raw bytes of this cell (copy)."""
        return bytes(self._buf[self._base:self._base + CELL_SIZE])

    # --- height ---
    @property
    def height_raw(self):
        return struct.unpack_from("<f", self._buf, self._base + OFF_HEIGHT)[0]

    @height_raw.setter
    def height_raw(self, value):
        struct.pack_into("<f", self._buf, self._base + OFF_HEIGHT, float(value))

    @property
    def height(self):
        """World-space height (raw * 2048), matching FableMod/FableForge."""
        return self.height_raw * HEIGHT_SCALE

    @height.setter
    def height(self, world_height):
        self.height_raw = float(world_height) / HEIGHT_SCALE

    # --- theme blend (3 layers) ---
    def theme_index(self, layer):
        assert 0 <= layer < 3
        return self._buf[self._base + OFF_THEME_IDX + layer]

    def set_theme_index(self, layer, value):
        assert 0 <= layer < 3
        self._buf[self._base + OFF_THEME_IDX + layer] = value & 0xFF

    def theme_strength(self, layer):
        # Retail corpus invariant: +13/+14 are slots 0/1; slot 2 is the
        # implicit remainder. BarrowFields has s0+s1<=255 for every cell and
        # exactly 255 in 16,586/16,641 cells.
        assert 0 <= layer < 3
        if layer < 2:
            return self._buf[self._base + OFF_THEME_STR + layer]
        stored = (self._buf[self._base + OFF_THEME_STR] +
                  self._buf[self._base + OFF_THEME_STR + 1])
        return max(0, 255 - stored)

    def set_theme_strength(self, layer, value):
        assert 0 <= layer < 2
        self._buf[self._base + OFF_THEME_STR + layer] = value & 0xFF

    # --- walkability ---
    @property
    def walkable(self):
        return self._buf[self._base + OFF_WALKABLE] != 0

    @property
    def walkable_byte(self):
        return self._buf[self._base + OFF_WALKABLE]

    @walkable_byte.setter
    def walkable_byte(self, value):
        self._buf[self._base + OFF_WALKABLE] = value & 0xFF


class GroundTheme:
    __slots__ = ("name", "value", "raw_name")

    def __init__(self, raw_name, value):
        self.raw_name = raw_name  # exact 128 bytes, preserved for byte-exact re-emit
        self.name = raw_name.split(b"\x00", 1)[0].decode("latin-1")
        self.value = value


class LevFile:
    """Parsed LEV. `cells` is a flat bytearray of (cellsY*cellsX*21) bytes; the header
    fields and the opaque suffix are preserved so serialize() is byte-exact."""

    def __init__(self):
        # LEVHeader
        self.header_size = 25
        self.version = LEV_VERSION
        self.header_pad = b"\x00\x00\x00"
        self.reserved1 = 0
        self.obs_offset = 0
        self.reserved2 = 0
        self.nav_offset = 0
        # LEVMapHeader
        self.map_size = 0
        self.map_version = MAP_VERSION
        self.map_pad = b""          # 3 bytes; map_pad[2] is the sub-version
        self.uid = 0
        self.width = 0
        self.height = 0
        self.flag = 0
        # ground themes / palette / theme strings
        self.ground_themes = []     # list[GroundTheme], len 256
        self.cell_version = 0
        self.theme_count = 0
        self.palette = b""          # 33792 bytes, opaque, preserved
        self.subver9_extra = None   # u32 present iff sub-version == 9
        self.themes = []            # list[bytes] length theme_count-1, exact bytes
        # cell grid (mutable)
        self.cells = bytearray()
        # opaque suffix: obstacle/brush region + nav TOC + payloads, verbatim
        self.suffix = b""
        # provenance / verification
        self.source = ""
        self._prefix_len = 0        # byte length of everything before the cells

    # ---- geometry ----
    @property
    def sub_version(self):
        return self.map_pad[2] if len(self.map_pad) >= 3 else None

    @property
    def cells_x(self):
        return self.width + 1

    @property
    def cells_y(self):
        return self.height + 1

    @property
    def cell_count(self):
        return self.cells_x * self.cells_y

    def cell(self, x, y):
        """Live Cell view at grid (x, y). Row-major, stride = cells_x."""
        if not (0 <= x < self.cells_x and 0 <= y < self.cells_y):
            raise IndexError("cell (%d,%d) out of %dx%d" % (x, y, self.cells_x, self.cells_y))
        base = (y * self.cells_x + x) * CELL_SIZE
        return Cell(self.cells, base)

    # ---- convenience edit helpers ----
    def set_height(self, x, y, world_height):
        self.cell(x, y).height = world_height

    def get_height(self, x, y):
        return self.cell(x, y).height

    def set_walkable(self, x, y, walkable):
        self.cell(x, y).walkable_byte = 1 if walkable else 0

    def get_walkable(self, x, y):
        return self.cell(x, y).walkable

    def set_theme(self, x, y, layer, index, strength=None):
        c = self.cell(x, y)
        c.set_theme_index(layer, index)
        if strength is not None and layer < 2:
            c.set_theme_strength(layer, strength)


def parse(data):
    """Parse raw LEV bytes into a LevFile. Raises ValueError on malformed input."""
    if isinstance(data, bytearray):
        data = bytes(data)
    lev = LevFile()
    o = 0

    # LEVHeader (25B)
    (lev.header_size,) = struct.unpack_from("<I", data, o)
    (lev.version,) = struct.unpack_from("<H", data, o + 4)
    lev.header_pad = data[o + 6:o + 9]
    (lev.reserved1, lev.obs_offset, lev.reserved2, lev.nav_offset) = \
        struct.unpack_from("<IIII", data, o + 9)
    if lev.version != LEV_VERSION:
        raise ValueError("bad LEV version %d (expected %d)" % (lev.version, LEV_VERSION))
    o += 25

    # LEVMapHeader (22B)
    lev.map_size = data[o]
    lev.map_version = data[o + 1]
    lev.map_pad = data[o + 2:o + 5]
    lo, hi = struct.unpack_from("<II", data, o + 5)
    lev.uid = (hi << 32) | lo
    lev.width, lev.height = struct.unpack_from("<ii", data, o + 13)
    lev.flag = data[o + 21]
    if lev.map_version != MAP_VERSION:
        raise ValueError("bad map version %d" % lev.map_version)
    sub = lev.map_pad[2]
    if sub not in (8, 9):
        raise ValueError("bad map sub-version %d" % sub)
    if not (0 <= lev.width <= 4096 and 0 <= lev.height <= 4096):
        raise ValueError("implausible map size %dx%d" % (lev.width, lev.height))
    o += 22

    # 256 ground themes (name[128] + u32)
    for _ in range(GROUND_THEME_COUNT):
        raw_name = data[o:o + GROUND_THEME_NAME_LEN]
        o += GROUND_THEME_NAME_LEN
        (value,) = struct.unpack_from("<I", data, o)
        o += 4
        lev.ground_themes.append(GroundTheme(raw_name, value))

    (lev.cell_version,) = struct.unpack_from("<I", data, o); o += 4
    (lev.theme_count,) = struct.unpack_from("<I", data, o); o += 4
    lev.palette = data[o:o + PALETTE_SIZE]; o += PALETTE_SIZE
    if sub == 9:
        (lev.subver9_extra,) = struct.unpack_from("<I", data, o); o += 4

    # (theme_count - 1) counted strings
    n_theme_str = lev.theme_count - 1 if lev.theme_count > 0 else 0
    for _ in range(n_theme_str):
        (ln,) = struct.unpack_from("<I", data, o); o += 4
        if ln > 4096:
            raise ValueError("implausible theme string length %d" % ln)
        lev.themes.append(data[o:o + ln]); o += ln

    lev._prefix_len = o

    # cell grid
    n = lev.cell_count
    cells_bytes = n * CELL_SIZE
    if o + cells_bytes > len(data):
        raise ValueError("truncated cell grid")
    lev.cells = bytearray(data[o:o + cells_bytes]); o += cells_bytes

    # everything else (obstacle/brush + nav) preserved verbatim
    lev.suffix = data[o:]
    if lev.nav_offset and lev.nav_offset > len(data):
        raise ValueError("navOffset past EOF")
    return lev


def _serialize_prefix(lev):
    """Rebuild the pre-cell bytes from parsed fields. Verified byte-identical to source
    for the edit-nothing case (see roundtrip_report)."""
    out = bytearray()
    # LEVHeader
    out += struct.pack("<I", lev.header_size)
    out += struct.pack("<H", lev.version)
    out += lev.header_pad
    out += struct.pack("<IIII", lev.reserved1, lev.obs_offset, lev.reserved2, lev.nav_offset)
    # LEVMapHeader
    out.append(lev.map_size)
    out.append(lev.map_version)
    out += lev.map_pad
    out += struct.pack("<II", lev.uid & 0xFFFFFFFF, (lev.uid >> 32) & 0xFFFFFFFF)
    out += struct.pack("<ii", lev.width, lev.height)
    out.append(lev.flag)
    # ground themes
    for gt in lev.ground_themes:
        out += gt.raw_name
        out += struct.pack("<I", gt.value)
    out += struct.pack("<I", lev.cell_version)
    out += struct.pack("<I", lev.theme_count)
    out += lev.palette
    if lev.sub_version == 9:
        out += struct.pack("<I", lev.subver9_extra if lev.subver9_extra is not None else 0)
    for s in lev.themes:
        out += struct.pack("<I", len(s))
        out += s
    return bytes(out)


def serialize(lev):
    """Serialize a LevFile back to LEV bytes: prefix + cells + opaque suffix."""
    return _serialize_prefix(lev) + bytes(lev.cells) + lev.suffix


def read_file(path):
    with open(path, "rb") as f:
        lev = parse(f.read())
    lev.source = path
    return lev


def write_file(lev, path):
    with open(path, "wb") as f:
        f.write(serialize(lev))


def roundtrip_report(data, source=""):
    """Prove serialize(parse(x)) == x for the edit-nothing case. Returns a dict; on
    mismatch it reports the first differing byte offset and which span it falls in."""
    if isinstance(data, bytearray):
        data = bytes(data)
    lev = parse(data)
    reser = serialize(lev)
    ok = reser == data
    result = {
        "source": source,
        "ok": ok,
        "orig_len": len(data),
        "reser_len": len(reser),
        "sub_version": lev.sub_version,
        "width": lev.width,
        "height": lev.height,
        "cells": lev.cell_count,
        "theme_count": lev.theme_count,
        "prefix_len": lev._prefix_len,
        "suffix_len": len(lev.suffix),
    }
    if not ok:
        first = None
        for i in range(min(len(data), len(reser))):
            if data[i] != reser[i]:
                first = i
                break
        if first is None:
            first = min(len(data), len(reser))
        cells_start = lev._prefix_len
        cells_end = cells_start + len(lev.cells)
        span = ("prefix" if first < cells_start else
                "cells" if first < cells_end else "suffix")
        result["first_diff_off"] = first
        result["first_diff_span"] = span
        result["orig_byte"] = data[first] if first < len(data) else None
        result["reser_byte"] = reser[first] if first < len(reser) else None
    return result


def _cli(argv):
    if len(argv) < 2:
        print("usage: lev_rw.py <info|roundtrip|edittest> <file.lev> [more.lev ...]")
        return 2
    cmd = argv[1]
    files = argv[2:]
    if cmd == "info":
        for p in files:
            lev = read_file(p)
            print("%s: %dx%d grid (%d cells), sub-ver %d, %d themes, "
                  "prefix=%d suffix=%d, uid=%d"
                  % (p, lev.cells_x, lev.cells_y, lev.cell_count, lev.sub_version,
                     lev.theme_count, lev._prefix_len, len(lev.suffix), lev.uid))
        return 0
    if cmd == "roundtrip":
        allok = True
        for p in files:
            with open(p, "rb") as f:
                r = roundtrip_report(f.read(), p)
            allok = allok and r["ok"]
            if r["ok"]:
                print("OK   %-40s %d bytes byte-identical (sub%d %dx%d)"
                      % (p.split("/")[-1].split("\\")[-1], r["orig_len"],
                         r["sub_version"], r["width"], r["height"]))
            else:
                print("FAIL %-40s first diff @%d in %s (orig=%s reser=%s)"
                      % (p, r.get("first_diff_off"), r.get("first_diff_span"),
                         r.get("orig_byte"), r.get("reser_byte")))
        return 0 if allok else 1
    if cmd == "edittest":
        p = files[0]
        lev = read_file(p)
        x, y = lev.cells_x // 2, lev.cells_y // 2
        before = lev.get_height(x, y)
        before_raw = serialize(lev)
        lev.set_height(x, y, before + 500.0)
        after_raw = serialize(lev)
        # exactly the 4 height bytes of that cell changed?
        diffs = [i for i in range(len(before_raw)) if before_raw[i] != after_raw[i]]
        cell_base = lev._prefix_len + (y * lev.cells_x + x) * CELL_SIZE + OFF_HEIGHT
        expected = list(range(cell_base, cell_base + 4))
        lev2 = parse(after_raw)
        print("edited cell (%d,%d) height %.3f -> %.3f" % (x, y, before, lev2.get_height(x, y)))
        print("bytes changed: %s (expected exactly %s)" % (diffs, expected))
        print("PASS" if diffs == expected and len(before_raw) == len(after_raw) else "FAIL")
        return 0
    print("unknown command", cmd)
    return 2


if __name__ == "__main__":
    sys.exit(_cli(sys.argv))
