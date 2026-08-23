#!/usr/bin/env python3
"""Check retail local-detail quadtree node spheres against BuildFromSubSpheres.

CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren (FableWin
0x02E39420) gathers non-empty child spheres first, then direct cache-group
spheres, and calls C3DBoundingSphere::BuildFromSubSpheres (0x0338B200).
"""

import argparse
import math
import struct
from pathlib import Path


def quad_header(data: bytes, at: int) -> dict:
    return {
        "sphere": struct.unpack_from("<4f", data, at),
        "fb_pos": struct.unpack_from("<I", data, at + 24)[0],
        "off_in": struct.unpack_from("<I", data, at + 32)[0],
    }


def group_header(data: bytes, at: int) -> tuple[float, float, float, float]:
    return struct.unpack_from("<4f", data, at + 0x0C)


def build_from_subspheres(items: list[tuple[float, float, float, float]]):
    items = [sphere for sphere in items if sphere[3] > 0.0]
    if not items:
        return (0.0, 0.0, 0.0, 0.0)
    lo = [min(s[a] - s[3] for s in items) for a in range(3)]
    hi = [max(s[a] + s[3] for s in items) for a in range(3)]
    centre = [(lo[a] + hi[a]) / 2.0 for a in range(3)]
    radius = max(
        math.sqrt(sum((s[a] - centre[a]) ** 2 for a in range(3))) + s[3]
        for s in items
    )
    return (*centre, radius)


def walk(chunk: bytes, node: dict, path: str, rows: list[tuple]):
    at = node["fb_pos"] + node["off_in"]
    count = struct.unpack_from("<I", chunk, at)[0]
    at += 4
    groups = [group_header(chunk, at + i * 0x28) for i in range(count)]
    at += count * 0x28
    children = []
    for quadrant in range(4):
        present = struct.unpack_from("<I", chunk, at)[0]
        at += 4
        if present:
            child = quad_header(chunk, at)
            children.append((quadrant, child))
            at += 44
    # Engine order at 0x02E39420: children, then directly attached groups.
    expected = build_from_subspheres(
        [child["sphere"] for _, child in children] + groups)
    error = max(abs(expected[i] - node["sphere"][i]) for i in range(4))
    rows.append((path, node["sphere"], expected, error, len(children), len(groups)))
    for quadrant, child in children:
        walk(chunk, child, path + str(quadrant), rows)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("chunk", type=Path)
    parser.add_argument("record", type=Path)
    parser.add_argument("--header-offset", type=lambda v: int(v, 0), default=0x7D)
    parser.add_argument("--tolerance", type=float, default=0.00035)
    args = parser.parse_args()
    chunk = args.chunk.read_bytes()
    record = args.record.read_bytes()
    rows = []
    walk(chunk, quad_header(record, args.header_offset), "R", rows)
    bad = [row for row in rows if row[3] > args.tolerance]
    worst = max((row[3] for row in rows), default=0.0)
    print(f"{args.chunk}: {len(rows) - len(bad)}/{len(rows)} node spheres match "
          f"within {args.tolerance:g}; worst error={worst:.9g}")
    for path, stored, expected, error, children, groups in bad[:10]:
        print(f"  {path}: error={error:.9g} children={children} groups={groups} "
              f"stored={stored} expected={expected}")
    return 1 if bad else 0


if __name__ == "__main__":
    raise SystemExit(main())
