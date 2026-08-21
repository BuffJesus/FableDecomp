#!/usr/bin/env python3
"""Compare retail STB foreground normals with two LEV-derived models."""

from __future__ import annotations

import argparse
import csv
import math
import statistics
from collections import defaultdict
from pathlib import Path

import lev_rw
from compare_terrain_normal_models import (
    angle_degrees,
    forge_gradient_normal,
    pack_normal,
    peek_map_normal,
)


def sign_extend(value: int, bits: int) -> int:
    sign = 1 << (bits - 1)
    return (value ^ sign) - sign


def unpack_normal(value: int) -> tuple[float, float, float]:
    x = sign_extend(value & 0x7FF, 11) / 1023.0
    y = sign_extend((value >> 11) & 0x7FF, 11) / 1023.0
    z = sign_extend((value >> 22) & 0x3FF, 10) / 511.0
    length = math.sqrt(x * x + y * y + z * z)
    return (x / length, y / length, z / length) if length else (0.0, 0.0, 1.0)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("lev", type=Path)
    parser.add_argument("vertices", type=Path, help="foregroundinfo --vertices TSV")
    parser.add_argument(
        "--neighbor", action="append", nargs=3, default=[],
        metavar=("LEV", "WORLD_X", "WORLD_Y"),
        help="adjacent LEV and its world origin; repeat as needed",
    )
    args = parser.parse_args()

    lev = lev_rw.parse(args.lev.read_bytes())
    with args.vertices.open(newline="", encoding="utf-8") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    if not rows:
        raise SystemExit("vertex TSV contains no rows")

    origin_x = min(int(row["x"]) for row in rows)
    origin_y = min(int(row["y"]) for row in rows)
    neighbors = [
        (lev_rw.parse(Path(path).read_bytes()), int(wx), int(wy))
        for path, wx, wy in args.neighbor
    ]

    def stitched_normal(x: int, y: int) -> tuple[float, float, float]:
        def engine_height(value: float) -> float:
            return math.floor(value * 128.0 + 0.5) / 128.0

        def sample(sx: int, sy: int) -> float:
            if 0 <= sx < lev.width and 0 <= sy < lev.height:
                return engine_height(lev.get_height(sx, sy))
            world_x, world_y = origin_x + sx, origin_y + sy
            for neighbor, nx, ny in neighbors:
                lx, ly = world_x - nx, world_y - ny
                if 0 <= lx < neighbor.width and 0 <= ly < neighbor.height:
                    return engine_height(neighbor.get_height(lx, ly))
            return engine_height(lev.get_height(
                min(max(sx, 0), lev.width - 1),
                min(max(sy, 0), lev.height - 1),
            ))

        def normalize2(a: float, b: float) -> tuple[float, float]:
            length = math.sqrt(a * a + b * b)
            return (a / length, b / length) if length else (0.0, 1.0)

        hx, hz = normalize2(sample(x - 1, y) - sample(x + 1, y), 2.0)
        vy, vz = normalize2(sample(x, y - 1) - sample(x, y + 1), 2.0)
        length = math.sqrt(hx * hx + vy * vy + (hz * vz) ** 2)
        return (hx / length, vy / length, hz * vz / length)
    by_coord: dict[tuple[int, int], set[int]] = defaultdict(set)
    model_functions = [
        ("gradient", lambda x, y: forge_gradient_normal(lev, x, y)),
        ("peek_map", lambda x, y: peek_map_normal(lev, x, y)),
    ]
    if neighbors:
        model_functions.append(("peek_map_stitched", stitched_normal))
    exact = {name: 0 for name, _ in model_functions}
    angles = {name: [] for name, _ in model_functions}
    in_bounds = 0

    for row in rows:
        x, y = int(row["x"]), int(row["y"])
        lx, ly = x - origin_x, y - origin_y
        if not (0 <= lx <= lev.width and 0 <= ly <= lev.height):
            continue
        stored = int(row["packed_normal"], 0)
        by_coord[(lx, ly)].add(stored)
        models = {name: fn(lx, ly) for name, fn in model_functions}
        stored_normal = unpack_normal(stored)
        for name, normal in models.items():
            exact[name] += pack_normal(normal) == stored
            angles[name].append(angle_degrees(stored_normal, normal))
        in_bounds += 1

    conflicts = {coord: values for coord, values in by_coord.items() if len(values) > 1}
    print(
        f"LEV={args.lev} size={lev.width}x{lev.height} origin=({origin_x},{origin_y}) "
        f"rows={len(rows)} compared={in_bounds} unique_coords={len(by_coord)}"
    )
    print(f"duplicate_normal_conflicts={len(conflicts)}")
    for name, _ in model_functions:
        values = angles[name]
        ordered = sorted(values)
        print(
            f"{name}: exact={exact[name]}/{in_bounds} ({exact[name] * 100.0 / in_bounds:.2f}%) "
            f"angle_mean_deg={sum(values) / len(values):.6f} "
            f"median={statistics.median(values):.6f} "
            f"p95={ordered[math.ceil(len(ordered) * 0.95) - 1]:.6f} "
            f"max={max(values):.6f} within_1deg={sum(v <= 1.0 for v in values)} "
            f"within_5deg={sum(v <= 5.0 for v in values)}"
        )

    for label, coords in (
        ("all_unique", list(by_coord)),
        (
            "interior_unique",
            [
                coord
                for coord in by_coord
                if 0 < coord[0] < lev.width and 0 < coord[1] < lev.height
            ],
        ),
        (
            "border_unique",
            [
                coord
                for coord in by_coord
                if coord[0] in (0, lev.width) or coord[1] in (0, lev.height)
            ],
        ),
    ):
        print(f"{label}: coords={len(coords)}")
        for name, normal_fn in model_functions:
            model_angles = []
            model_exact = 0
            for x, y in coords:
                stored = next(iter(by_coord[(x, y)]))
                normal = normal_fn(x, y)
                model_exact += pack_normal(normal) == stored
                model_angles.append(angle_degrees(unpack_normal(stored), normal))
            print(
                f"  {name}: exact={model_exact}/{len(coords)} "
                f"mean={sum(model_angles) / len(model_angles):.6f} "
                f"max={max(model_angles):.6f}"
            )
    if neighbors:
        worst = sorted(
            (
                angle_degrees(unpack_normal(next(iter(values))), stitched_normal(*coord)),
                coord,
            )
            for coord, values in by_coord.items()
        )
        for angle, (x, y) in reversed(worst[-4:]):
            print(f"stitched_worst x={x} y={y} angle={angle:.6f}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
