#!/usr/bin/env python3
"""FinalAlbion.bwd (compiled binary world map) reader/writer.

The retail engine loads the world map from `<world>.bwd` when the userst.ini
console var `UseCompiledWorldFiles` is TRUE (retail default) — the .wld TEXT is
only parsed when it is FALSE.  Evidence (Fable.exe, retail):

  CWorldMap::LoadFromFile      0x00507c30  (gate: DAT_013b8618 = "UseCompiledWorldFiles")
  LoadWorldFromBinaryFile      0x00507650  (FUN_00507650; reads the .bwd)
  CMapInfo::LoadBinary         0x004fb4f0  (map record layout)
  CRegion::LoadBinary          FableWin 0x01f70940 (donor, PDB-named; layout below)
  ghidra_out/newlevel_retail_decomp*.c, ghidra_out/newlevel_fablewin_run5_decomp.c

File layout (little-endian):
  u32 mapCount                    # slots 0..mapCount-1; slot 0 is NEVER stored
  (mapCount-1) x CMapInfo record  # slots 1..mapCount-1, in slot order
  u32 regionCount                 # slots 0..regionCount-1; slot 0 NEVER stored
  (regionCount-1) x CRegion record

CMapInfo record (CMapInfo::LoadBinary 0x4fb4f0):
  presized str  levelName        -> +0x18   ("Data\\Levels\\FinalAlbion\\X.lev")
  presized str  levelScriptName  -> +0x1c
  u8            used             -> +0x24
  u8            loadedOnPlayerProximity -> +0x28
  u8            isSea            -> +0x40
  i32           boxLeft   (MapX) -> +0x00
  i32           boxRight         -> +0x08
  i32           boxTop    (MapY) -> +0x04
  i32           boxBottom        -> +0x0c
  u8            flag2            -> +0x25   (second used/placement flag)
  u64           mapUID           -> +0x20

CRegion record (CRegion::Save/LoadBinary, FableWin donor):
  i32 numContainsMaps; i32 numSeesMaps
  numContainsMaps x i32 mapSlotIndex
  numSeesMaps     x i32 mapSlotIndex
  presized str name; presized str displayName;
  presized str regionDefName; presized str miniMapGraphic
  u8 appearOnWorldMap; u8 creatureGenerationEnabled; u8 soundThemesEnabled
  f32 miniMapScale
  i32 miniMapOffsetX; i32 miniMapOffsetY; i32 worldMapOffsetX; i32 worldMapOffsetY
  i32 numExitTextOffsets x { presized str name; f32 x; f32 y }

A "presized string" is u32 length + raw bytes (no NUL).

Oracle: `python tools/wld_bwd.py roundtrip <file.bwd>` proves
serialize(parse(x)) == x byte-for-byte.
"""

import struct
import sys


class Reader:
    def __init__(self, data):
        self.d = data
        self.o = 0

    def u32(self):
        v = struct.unpack_from('<I', self.d, self.o)[0]
        self.o += 4
        return v

    def i32(self):
        v = struct.unpack_from('<i', self.d, self.o)[0]
        self.o += 4
        return v

    def u64(self):
        v = struct.unpack_from('<Q', self.d, self.o)[0]
        self.o += 8
        return v

    def u8(self):
        v = self.d[self.o]
        self.o += 1
        return v

    def f32(self):
        v = struct.unpack_from('<f', self.d, self.o)[0]
        self.o += 4
        return v

    def pstr(self):
        n = self.u32()
        if n > 0x10000:
            raise ValueError(f'string length {n} at {self.o - 4}: layout drift')
        s = self.d[self.o:self.o + n]
        self.o += n
        return s.decode('latin1')


class Writer:
    def __init__(self):
        self.parts = []

    def u32(self, v): self.parts.append(struct.pack('<I', v))
    def i32(self, v): self.parts.append(struct.pack('<i', v))
    def u64(self, v): self.parts.append(struct.pack('<Q', v))
    def u8(self, v): self.parts.append(bytes([v]))
    def f32raw(self, b): self.parts.append(b)  # preserve exact float bytes

    def pstr(self, s):
        b = s.encode('latin1')
        self.u32(len(b))
        self.parts.append(b)

    def bytes(self):
        return b''.join(self.parts)


class MapInfo:
    __slots__ = ('level_name', 'script_name', 'used', 'loaded_on_proximity',
                 'is_sea', 'left', 'right', 'top', 'bottom', 'flag2', 'map_uid')

    @classmethod
    def parse(cls, r):
        m = cls()
        m.level_name = r.pstr()
        m.script_name = r.pstr()
        m.used = r.u8()
        m.loaded_on_proximity = r.u8()
        m.is_sea = r.u8()
        m.left = r.i32()
        m.right = r.i32()
        m.top = r.i32()
        m.bottom = r.i32()
        m.flag2 = r.u8()
        m.map_uid = r.u64()
        return m

    def write(self, w):
        w.pstr(self.level_name)
        w.pstr(self.script_name)
        w.u8(self.used)
        w.u8(self.loaded_on_proximity)
        w.u8(self.is_sea)
        w.i32(self.left)
        w.i32(self.right)
        w.i32(self.top)
        w.i32(self.bottom)
        w.u8(self.flag2)
        w.u64(self.map_uid)


class Region:
    __slots__ = ('contains', 'sees', 'name', 'display_name', 'region_def',
                 'minimap_graphic', 'on_world_map', 'creature_gen',
                 'sound_themes', 'minimap_scale_raw', 'mm_off_x', 'mm_off_y',
                 'wm_off_x', 'wm_off_y', 'exits')

    @classmethod
    def parse(cls, r):
        g = cls()
        n_contains = r.i32()
        n_sees = r.i32()
        g.contains = [r.i32() for _ in range(n_contains)]
        g.sees = [r.i32() for _ in range(n_sees)]
        g.name = r.pstr()
        g.display_name = r.pstr()
        g.region_def = r.pstr()
        g.minimap_graphic = r.pstr()
        g.on_world_map = r.u8()
        g.creature_gen = r.u8()
        g.sound_themes = r.u8()
        g.minimap_scale_raw = r.d[r.o:r.o + 4]; r.o += 4
        g.mm_off_x = r.i32()
        g.mm_off_y = r.i32()
        g.wm_off_x = r.i32()
        g.wm_off_y = r.i32()
        n_exits = r.i32()
        g.exits = []
        for _ in range(n_exits):
            name = r.pstr()
            vec = r.d[r.o:r.o + 8]; r.o += 8
            g.exits.append((name, vec))
        return g

    def write(self, w):
        w.i32(len(self.contains))
        w.i32(len(self.sees))
        for v in self.contains:
            w.i32(v)
        for v in self.sees:
            w.i32(v)
        w.pstr(self.name)
        w.pstr(self.display_name)
        w.pstr(self.region_def)
        w.pstr(self.minimap_graphic)
        w.u8(self.on_world_map)
        w.u8(self.creature_gen)
        w.u8(self.sound_themes)
        w.f32raw(self.minimap_scale_raw)
        w.i32(self.mm_off_x)
        w.i32(self.mm_off_y)
        w.i32(self.wm_off_x)
        w.i32(self.wm_off_y)
        w.i32(len(self.exits))
        for name, vec in self.exits:
            w.pstr(name)
            w.f32raw(vec)


class Bwd:
    def __init__(self):
        self.maps = []      # index 0 == slot 1
        self.regions = []   # index 0 == slot 1

    @classmethod
    def parse(cls, data):
        r = Reader(data)
        b = cls()
        map_count = r.u32()
        b.maps = [MapInfo.parse(r) for _ in range(map_count - 1)]
        region_count = r.u32()
        b.regions = [Region.parse(r) for _ in range(region_count - 1)]
        if r.o != len(data):
            raise ValueError(f'trailing bytes: consumed {r.o} of {len(data)}')
        return b

    def serialize(self):
        w = Writer()
        w.u32(len(self.maps) + 1)
        for m in self.maps:
            m.write(w)
        w.u32(len(self.regions) + 1)
        for g in self.regions:
            g.write(w)
        return w.bytes()


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        return 1
    cmd, path = sys.argv[1], sys.argv[2]
    data = open(path, 'rb').read()
    b = Bwd.parse(data)
    if cmd == 'info':
        print(f'{path}: {len(b.maps)} maps (slots 1..{len(b.maps)}), '
              f'{len(b.regions)} regions (slots 1..{len(b.regions)})')
        for i, m in enumerate(b.maps[:5], 1):
            print(f'  map {i}: {m.script_name} uid={m.map_uid} '
                  f'box=({m.left},{m.top})-({m.right},{m.bottom}) '
                  f'used={m.used} prox={m.loaded_on_proximity} sea={m.is_sea} f2={m.flag2}')
        for i, g in enumerate(b.regions[:5], 1):
            print(f'  region {i}: {g.name!r} def={g.region_def!r} '
                  f'contains={g.contains} sees={len(g.sees)} exits={len(g.exits)}')
    elif cmd == 'roundtrip':
        out = b.serialize()
        if out == data:
            print(f'ROUNDTRIP OK: {path} ({len(data)} bytes byte-identical)')
        else:
            n = min(len(out), len(data))
            diff = next((i for i in range(n) if out[i] != data[i]), n)
            print(f'ROUNDTRIP FAIL at byte {diff} (out {len(out)} vs src {len(data)})')
            return 1
    else:
        print(f'unknown command {cmd}')
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
