#!/usr/bin/env python3
"""Compare retail terrain layers with reconstructed theme/direction membership."""

from __future__ import annotations

import argparse
import csv
import math
from collections import defaultdict
from pathlib import Path

import lev_rw


def normalize(v: tuple[float, float, float]) -> tuple[float, float, float]:
    length = math.sqrt(sum(c * c for c in v))
    return tuple(c / length for c in v) if length else (0.0, 0.0, 1.0)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("lev", type=Path)
    parser.add_argument("triangles", type=Path)
    parser.add_argument("--origin", nargs=2, type=int, required=True, metavar=("X", "Y"))
    parser.add_argument("--neighbor", action="append", nargs=3, default=[],
                        metavar=("LEV", "WORLD_X", "WORLD_Y"))
    parser.add_argument("--theme", action="append", nargs=7, required=True,
                        metavar=("SLOT", "BASE", "BASE_BG", "BASE_BUMP",
                                 "CLIFF", "CLIFF_BG", "CLIFF_BUMP"))
    args = parser.parse_args()

    lev = lev_rw.parse(args.lev.read_bytes())
    origin_x, origin_y = args.origin
    neighbors = [(lev_rw.parse(Path(p).read_bytes()), int(x), int(y))
                 for p, x, y in args.neighbor]
    themes = {int(values[0]): (tuple(map(int, values[1:4])),
                               tuple(map(int, values[4:7])))
              for values in args.theme}

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
            for component in range(3):
                total[component] += face[component] * weight
        return normalize(tuple(total))

    directions = ((0.0, 0.0), (0.0, -1.0), (0.0, 1.0),
                  (-1.0, 0.0), (1.0, 0.0))

    def active(direction: int, x: int, y: int) -> bool:
        normal = mask_normal(x, y)
        topness = min(1.0, max(0.0,
            (math.asin(max(-1.0, min(1.0, normal[2]))) / (math.pi / 2) - 0.5) * 4))
        if direction == 0:
            return topness > 0.0
        if topness == 1.0:
            return False
        length = math.hypot(normal[0], normal[1])
        if length == 0.0:
            return False
        dx, dy = directions[direction]
        dot = max(-1.0, min(1.0, normal[0] / length * dx + normal[1] / length * dy))
        sideness = min(1.0, max(0.0, 1.0 - 2.0 *
                       (math.acos(dot) / (math.pi / 2) - 0.25)))
        return (1.0 - topness) * sideness > 0.0

    contributions: dict[tuple[tuple[int, int, int], int], set[tuple[int, int]]] = defaultdict(set)
    for y in range(lev.height + 1):
        for x in range(lev.width + 1):
            cell = lev.cell(x, y)
            slots = [cell.theme_index(i) for i in range(3)]
            weights = [cell.theme_strength(i) if slots[i] in themes else 0 for i in range(3)]
            total = sum(weights)
            if not total:
                continue
            weights = [weight * 255 // total for weight in weights]
            merged: dict[tuple[tuple[int, int, int], int], int] = defaultdict(int)
            for slot, weight in zip(slots, weights):
                if slot not in themes:
                    continue
                base, cliff = themes[slot]
                merged[(base, 0)] += weight
                for direction in range(1, 5):
                    merged[(cliff, direction)] += weight
            for key, weight in merged.items():
                if weight > 0x10:
                    contributions[key].add((x, y))

    patterns = (
        (((0, 0), (1, 0), (1, 1)), ((0, 0), (0, 1), (1, 1))),
        (((0, 0), (1, 0), (0, 1)), ((1, 0), (0, 1), (1, 1))),
    )
    generated: dict[tuple[int, int, tuple[int, int, int]], set[frozenset[tuple[int, int]]]] = defaultdict(set)
    for patch_y in range(0, lev.height, 16):
        for patch_x in range(0, lev.width, 16):
            frame = patch_y // 16 * (lev.width // 16) + patch_x // 16
            for material, points in contributions.items():
                texture, direction = material
                for y in range(patch_y, patch_y + 16):
                    for x in range(patch_x, patch_x + 16):
                        for offsets in patterns[(x ^ y) & 1]:
                            triangle = tuple((x + dx, y + dy) for dx, dy in offsets)
                            if (any(point in points for point in triangle) and
                                    any(active(direction, *point) for point in triangle)):
                                world = frozenset((origin_x + px, origin_y + py)
                                                  for px, py in triangle)
                                generated[(frame, direction, texture)].add(world)

    retail: dict[tuple[int, int, tuple[int, int, int]], set[frozenset[tuple[int, int]]]] = defaultdict(set)
    with args.triangles.open(newline="", encoding="utf-8") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            key = (int(row["frame"]), int(row["mapping"]),
                   (int(row["texture0"]), int(row["texture1"]), int(row["texture2"])))
            retail[key].add(frozenset((int(row[f"{name}x"]), int(row[f"{name}y"]))
                                      for name in "abc"))

    keys = set(generated) | set(retail)
    exact = 0
    for key in sorted(keys):
        missing = retail[key] - generated[key]
        extra = generated[key] - retail[key]
        shared_full_patch = not retail[key] and len(generated[key]) == 512
        exact += (not missing and not extra) or shared_full_patch
        print(f"frame={key[0]} mapping={key[1]} texture={key[2]} "
              f"retail={len(retail[key])} generated={len(generated[key])} "
              f"missing={len(missing)} extra={len(extra)}"
              f"{' shared_full_patch=1' if shared_full_patch else ''}")
    print(f"layer_membership_exact={exact}/{len(keys)}")
    return 0 if exact == len(keys) else 1


if __name__ == "__main__":
    raise SystemExit(main())
