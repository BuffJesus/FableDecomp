#!/usr/bin/env python3
"""Compare Forge's current height-gradient normal with CMap::PeekMapNormal.

The reference model is independently transcribed by OpenAlbion from
CEngineMap::PeekMapNormal: normalize each two-cell axis slope before combining
the axes, then normalize the resulting 3-vector.  This script does not edit a
LEV or STB; it quantifies whether switching the native baker would change the
packed 11/11/10 foreground normals.
"""

from __future__ import annotations

import argparse
import math
from pathlib import Path

import lev_rw


def normalize3(x: float, y: float, z: float) -> tuple[float, float, float]:
    length = math.sqrt(x * x + y * y + z * z)
    if length == 0.0:
        return (0.0, 0.0, 1.0)
    return (x / length, y / length, z / length)


def forge_gradient_normal(lev: lev_rw.LevFile, x: int, y: int) -> tuple[float, float, float]:
    x0, x1 = max(0, x - 1), min(lev.width, x + 1)
    y0, y1 = max(0, y - 1), min(lev.height, y + 1)
    dzdx = (lev.get_height(x1, y) - lev.get_height(x0, y)) / (x1 - x0)
    dzdy = (lev.get_height(x, y1) - lev.get_height(x, y0)) / (y1 - y0)
    return normalize3(-dzdx, -dzdy, 1.0)


def peek_map_normal(lev: lev_rw.LevFile, x: int, y: int) -> tuple[float, float, float]:
    def height(cx: int, cy: int) -> float:
        return lev.get_height(
            min(max(cx, 0), lev.width),
            min(max(cy, 0), lev.height),
        )

    def normalize2(a: float, b: float) -> tuple[float, float]:
        length = math.sqrt(a * a + b * b)
        return (a / length, b / length) if length else (0.0, 1.0)

    hor_x, hor_y = normalize2(height(x - 1, y) - height(x + 1, y), 2.0)
    ver_x, ver_y = normalize2(height(x, y - 1) - height(x, y + 1), 2.0)
    return normalize3(hor_x, ver_x, hor_y * ver_y)


def pack_normal(normal: tuple[float, float, float]) -> int:
    # Match forge::stbbake::packNormal's floor(v + 0.5) convention.
    def rounded(value: float) -> int:
        return math.floor(value + 0.5)

    x, y, z = normal
    ix = rounded(x * 1023.0) & 0x7FF
    iy = rounded(y * 1023.0) & 0x7FF
    iz = rounded(z * 511.0) & 0x3FF
    return ix | (iy << 11) | (iz << 22)


def angle_degrees(a: tuple[float, float, float], b: tuple[float, float, float]) -> float:
    dot = min(1.0, max(-1.0, sum(x * y for x, y in zip(a, b))))
    return math.degrees(math.acos(dot))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("lev", type=Path)
    args = parser.parse_args()

    lev = lev_rw.parse(args.lev.read_bytes())
    changed = 0
    angles: list[float] = []
    worst: list[tuple[float, int, int, int, int]] = []
    forge_unique: set[int] = set()
    peek_unique: set[int] = set()

    for y in range(lev.height + 1):
        for x in range(lev.width + 1):
            forge = forge_gradient_normal(lev, x, y)
            peek = peek_map_normal(lev, x, y)
            forge_packed = pack_normal(forge)
            peek_packed = pack_normal(peek)
            forge_unique.add(forge_packed)
            peek_unique.add(peek_packed)
            angle = angle_degrees(forge, peek)
            angles.append(angle)
            if forge_packed != peek_packed:
                changed += 1
            worst.append((angle, x, y, forge_packed, peek_packed))

    total = len(angles)
    worst.sort(reverse=True)
    print(f"LEV={args.lev} size={lev.width}x{lev.height} vertices={total}")
    print(f"packed_changed={changed}/{total} ({changed * 100.0 / total:.2f}%)")
    print(f"angle_mean_deg={sum(angles) / total:.6f} angle_max_deg={worst[0][0]:.6f}")
    print(f"unique_packed forge={len(forge_unique)} peek_map={len(peek_unique)}")
    for angle, x, y, forge_packed, peek_packed in worst[:8]:
        print(
            f"worst x={x} y={y} angle={angle:.6f} "
            f"forge=0x{forge_packed:08x} peek=0x{peek_packed:08x}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
