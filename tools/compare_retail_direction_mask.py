#!/usr/bin/env python3
"""Compare retail CliffU/CliffV with BuildMapDirMask's face smoothing."""

from __future__ import annotations

import argparse
import csv
import math
from pathlib import Path

import lev_rw


def normalize(v: tuple[float, float, float]) -> tuple[float, float, float]:
    length = math.sqrt(sum(c * c for c in v))
    return tuple(c / length for c in v) if length else (0.0, 0.0, 1.0)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("lev", type=Path)
    parser.add_argument("vertices", type=Path)
    parser.add_argument("--origin", nargs=2, type=int, required=True, metavar=("X", "Y"))
    parser.add_argument("--neighbor", action="append", nargs=3, default=[],
                        metavar=("LEV", "WORLD_X", "WORLD_Y"))
    args = parser.parse_args()

    lev = lev_rw.parse(args.lev.read_bytes())
    origin_x, origin_y = args.origin
    neighbors = [(lev_rw.parse(Path(p).read_bytes()), int(x), int(y))
                 for p, x, y in args.neighbor]

    def height(x: int, y: int) -> float:
        if 0 <= x < lev.width and 0 <= y < lev.height:
            value = lev.get_height(x, y)
        else:
            wx, wy = origin_x + x, origin_y + y
            value = None
            for neighbor, nx, ny in neighbors:
                lx, ly = wx - nx, wy - ny
                if 0 <= lx < neighbor.width and 0 <= ly < neighbor.height:
                    value = neighbor.get_height(lx, ly)
                    break
            if value is None:
                value = lev.get_height(min(max(x, 0), lev.width - 1),
                                       min(max(y, 0), lev.height - 1))
        return math.floor(value * 128.0 + 0.5) / 128.0

    def mask_normal(x: int, y: int) -> tuple[float, float, float]:
        center = height(x, y)
        left, right = height(x - 1, y) - center, height(x + 1, y) - center
        up, down = height(x, y - 1) - center, height(x, y + 1) - center
        if ((origin_x + x) ^ (origin_y + y)) & 1:
            faces = [(-right, up, 1.0), (-right, -down, 1.0),
                     (left, -down, 1.0), (left, up, 1.0)]
        else:
            ul = height(x - 1, y - 1) - center
            ur = height(x + 1, y - 1) - center
            dl = height(x - 1, y + 1) - center
            dr = height(x + 1, y + 1) - center
            faces = [(up - ur, up, 1.0), (-right, ur - right, 1.0),
                     (-right, right - dr, 1.0), (down - dr, -down, 1.0),
                     (dl - down, -down, 1.0), (left, left - dl, 1.0),
                     (left, ul - left, 1.0), (ul - up, up, 1.0)]
        total = [0.0, 0.0, 0.0]
        for face in faces:
            face = normalize(face)
            weight = math.prod(0.5 / (abs(c) + 0.0625) + 0.5294118 for c in face)
            for i in range(3):
                total[i] += face[i] * weight
        return normalize(tuple(total))

    def pack_component(value: float) -> int:
        # BuildMapDirMask converts the positive 0..255 float directly to int;
        # despite Ghidra's ROUND p-code spelling, retail truncates here.
        return math.floor((value * 0.5 + 0.5) * 255.0) & 0xFF

    with args.vertices.open(newline="", encoding="utf-8") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    by_coord: dict[tuple[int, int], tuple[int, int]] = {}
    conflicts = 0
    for row in rows:
        coord = (int(row["x"]) - origin_x, int(row["y"]) - origin_y)
        stored = (int(row["cliff_u"]), int(row["cliff_v"]))
        if coord in by_coord and by_coord[coord] != stored:
            conflicts += 1
        by_coord[coord] = stored

    direct = swapped = 0
    component_error = [0, 0]
    worst: list[tuple[int, tuple[int, int], tuple[int, int], tuple[int, int]]] = []
    for coord, stored in by_coord.items():
        normal = mask_normal(*coord)
        packed = (pack_component(normal[0]), pack_component(normal[1]))
        direct += packed == stored
        swapped += packed[::-1] == stored
        error = (abs(packed[0] - stored[0]), abs(packed[1] - stored[1]))
        component_error[0] += error[0]
        component_error[1] += error[1]
        worst.append((sum(error), coord, stored, packed))

    worst.sort(reverse=True)
    print(f"rows={len(rows)} unique_coords={len(by_coord)} conflicts={conflicts}")
    print(f"direct_exact={direct}/{len(by_coord)} swapped_exact={swapped}/{len(by_coord)}")
    print(f"mean_abs_error_u={component_error[0]/len(by_coord):.6f} "
          f"mean_abs_error_v={component_error[1]/len(by_coord):.6f}")
    for error, coord, stored, packed in worst[:8]:
        print(f"worst x={coord[0]} y={coord[1]} error={error} "
              f"stored={stored} computed={packed}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
