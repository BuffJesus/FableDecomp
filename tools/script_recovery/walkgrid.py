"""Walkable world grid for hands-free playtests: the LEVs of the selected maps read out of the install's
FinalAlbion.wad (read-only), unioned in world cells, with A* between world points. `waypoints(src, dst)`
gives hops on walkable ground away from blocked cells, so followers can path to where the hero lands
(a straight-line hop put the hero inside trees and the Trader Escort traders stood still, 2026-09-24).
Parsed maps are cached under work/runner_cache."""
import pickle
import heapq
import math
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
CACHE = ROOT / 'work' / 'runner_cache'
import lev_rw  # noqa: E402

WAD = Path(r'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\Levels\FinalAlbion.wad')
FORGE = r'D:\Code\FableForge\build\forge.exe'


def _wad_entries(f):
    f.seek(20)
    count = struct.unpack('<I', f.read(4))[0]
    f.seek(28)
    footer = struct.unpack('<I', f.read(4))[0]
    f.seek(footer)
    data = f.read()
    pos = 4 + struct.unpack_from('<I', data, 0)[0] * 8
    out = {}
    for _ in range(count):
        _, _, _, size, off, _ = struct.unpack_from('<6I', data, pos); pos += 24
        nl = struct.unpack_from('<I', data, pos)[0]; pos += 4
        name = data[pos:pos + nl].decode('latin1'); pos += nl + 4
        depc = struct.unpack_from('<I', data, pos)[0]; pos += 4
        for _ in range(depc):
            pos += 4 + struct.unpack_from('<I', data, pos)[0]
        pos += 4 + struct.unpack_from('<I', data, pos)[0]
        out[name.lower()] = (off, size)
    return out


class Grid:
    def __init__(self, maps=r'.', hazards=(), hazard_radius=4):
        pattern = re.compile(maps)
        self.hazards = []
        world = subprocess.run([FORGE, 'world'], capture_output=True, text=True).stdout
        self.maps = {}
        for line in world.splitlines():
            m = re.match(r'\s*(\d+)\s+(\S+)\s+(\d+)\s+(\d+)\s+(\d+)x(\d+)\s+(\S+)', line)
            if m:
                self.maps[m.group(2)] = (int(m.group(3)), int(m.group(4)), int(m.group(5)), int(m.group(6)), m.group(7))
        self.walk, self.z = {}, {}
        CACHE.mkdir(parents=True, exist_ok=True)
        entries = None
        with open(WAD, 'rb') as f:
            for name, (mx, my, w, h, region) in self.maps.items():
                if not (pattern.search(name) or pattern.search(region)):
                    continue
                cache = CACHE / f'{name}.pkl'
                if cache.exists():
                    cells = pickle.loads(cache.read_bytes())
                else:
                    entries = entries or _wad_entries(f)
                    key = f'data\\levels\\finalalbion\\{name}.lev'.lower()
                    if key not in entries:
                        continue
                    off, size = entries[key]
                    f.seek(off)
                    lev = lev_rw.parse(f.read(size))
                    cells = [(x, y, lev.cell(x, y).walkable, lev.cell(x, y).height)
                             for y in range(lev.cells_y) for x in range(lev.cells_x)]
                    cache.write_bytes(pickle.dumps(cells))
                if hazards:
                    tcache = CACHE / f'{name}.things.pkl'
                    if tcache.exists():
                        things = pickle.loads(tcache.read_bytes())
                    else:
                        entries = entries or _wad_entries(f)
                        tkey = f'data\\levels\\finalalbion\\{name}.tng'.lower()
                        things = []
                        if tkey in entries:
                            off, size = entries[tkey]
                            f.seek(off)
                            text = f.read(size).decode('latin1', errors='replace')
                            for block in re.findall(r'NewThing \w+;.*?EndThing;', text, re.S):   # (Thing, Object, Marker, ...: the spores are Objects)
                                d = re.search(r'DefinitionType "([^"]+)"', block)
                                px = re.search(r'PositionX (-?[\d.]+)', block)
                                py = re.search(r'PositionY (-?[\d.]+)', block)
                                if d and px and py:
                                    things.append((d.group(1), float(px.group(1)), float(py.group(1))))
                        tcache.write_bytes(pickle.dumps(things))
                    for d, px, py in things:
                        if d in hazards:
                            self.hazards.append((d, mx + px, my + py))
                for x, y, ok, z in cells:
                    k = (mx + x, my + y)
                    self.walk[k] = self.walk.get(k, False) or ok
                    if ok or k not in self.z:
                        self.z[k] = z

        # hazards (exploding spores) are expensive, not impassable: a hard block closed Darkwood2's chokepoint;
        # the cost keeps paths away where there is room and keeps hops (path cells every `spacing`) off them
        self.danger = {}
        for _, hx, hy in self.hazards:
            for dx in range(-hazard_radius - 2, hazard_radius + 3):
                for dy in range(-hazard_radius - 2, hazard_radius + 3):
                    d2 = dx * dx + dy * dy
                    if d2 <= (hazard_radius + 2) ** 2:
                        k = (round(hx) + dx, round(hy) + dy)
                        self.danger[k] = max(self.danger.get(k, 0), 30 if d2 <= hazard_radius ** 2 else 6)

    def ground(self, x, y):
        return self.z.get((round(x), round(y)))

    def clearance(self, k):
        """Blocked cells within 2 (0 = open ground)."""
        return sum(1 for dx in range(-2, 3) for dy in range(-2, 3) if not self.walk.get((k[0] + dx, k[1] + dy), False))

    def nearest_walkable(self, x, y, radius=6):
        best = None
        for dx in range(-radius, radius + 1):
            for dy in range(-radius, radius + 1):
                k = (round(x) + dx, round(y) + dy)
                if self.walk.get(k):
                    d = dx * dx + dy * dy + 4 * self.clearance(k) + 10 * self.danger.get(k, 0)
                    if best is None or d < best[0]:
                        best = (d, k)
        return best and best[1]

    def path(self, src, dst):
        s, t = self.nearest_walkable(*src), self.nearest_walkable(*dst, radius=4)
        if s is None or t is None:
            return None
        openq, came, cost = [(0, s)], {s: None}, {s: 0}
        while openq:
            _, k = heapq.heappop(openq)
            if k == t:
                break
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1), (1, 1), (1, -1), (-1, 1), (-1, -1)):
                n = (k[0] + dx, k[1] + dy)
                if not self.walk.get(n):
                    continue
                step = (1.4142 if dx and dy else 1.0) * (1 + 0.5 * self.clearance(n)) + self.danger.get(n, 0)
                c = cost[k] + step
                if c < cost.get(n, 1e18):
                    cost[n], came[n] = c, k
                    heapq.heappush(openq, (c + math.dist(n, t), n))
        if t not in came:
            return None
        out, k = [], t
        while k is not None:
            out.append(k)
            k = came[k]
        return out[::-1]

    def waypoints(self, src, dst, spacing=7):
        p = self.path(src, dst)
        if not p:
            return None
        pts, i = [], spacing
        while i < len(p):
            j = i
            while j < len(p) - 1 and self.danger.get(p[j], 0) >= 30 and j - i < spacing - 1:
                j += 1                           # slide a hop that would land in a spore's reach along the path
            pts.append(p[j])
            i = j + spacing
        if not pts or pts[-1] != p[-1]:
            pts.append(p[-1])
        return [(x, y, self.z[(x, y)]) for x, y in pts]


if __name__ == '__main__':
    g = Grid(r'^(?:Darkwood|BarrowFields$)', hazards={'OBJECT_EXPLODING_SPORE_MEDIUM', 'OBJECT_EXPLODING_SPORE_LARGE'})
    print('hazards', len(g.hazards))
    print('cells', len(g.walk), 'walkable', sum(g.walk.values()))
    exits = [((3036, 2735), (3099.94, 2705.34)), ((3137.4, 2615.7), (3042.39, 2604.53)),
             ((2959.8, 2578.6), (2971.01, 2482.49)), ((3048.4, 2479.1), (3054.62, 2436.58)),
             ((2999.4, 2419.3), (2856.24, 2384.19)), ((2772.6, 2420.2), (2698.77, 2344.96))]
    for a, b in exits:
        w = g.waypoints(a, b)
        print(a, '->', b, 'hops', None if w is None else len(w), (w or [])[:3], '...', (w or [])[-1:])
