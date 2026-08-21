#!/usr/bin/env python3
"""Rebuild retail BuildLayerMesh strips from a decoded triangle set."""

from __future__ import annotations

import argparse
import csv
from collections import defaultdict
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("triangles", type=Path)
    parser.add_argument("indices", type=Path)
    args = parser.parse_args()

    with args.triangles.open(newline="", encoding="utf-8") as stream:
        triangle_rows = list(csv.DictReader(stream, delimiter="\t"))
    with args.indices.open(newline="", encoding="utf-8") as stream:
        index_rows = list(csv.DictReader(stream, delimiter="\t"))

    triangles: dict[tuple[int, int], set[frozenset[tuple[int, int]]]] = defaultdict(set)
    mappings: dict[tuple[int, int], int] = {}
    for row in triangle_rows:
        key = (int(row["frame"]), int(row["layer"]))
        mappings[key] = int(row["mapping"])
        triangles[key].add(frozenset(
            (int(row[f"{name}x"]), int(row[f"{name}y"])) for name in "abc"))

    retail: dict[tuple[int, int], list[int]] = defaultdict(list)
    retail_coords: dict[tuple[int, int], dict[int, tuple[int, int]]] = defaultdict(dict)
    for row in index_rows:
        key = (int(row["frame"]), int(row["layer"]))
        vertex = int(row["vertex"])
        retail[key].append(vertex)
        retail_coords[key][vertex] = (int(row["x"]), int(row["y"]))

    patterns = (
        (((0, 0), (1, 0), (1, 1)), ((0, 0), (0, 1), (1, 1))),
        (((0, 0), (1, 0), (0, 1)), ((1, 0), (0, 1), (1, 1))),
    )
    exact_layers = exact_indices = total_indices = 0
    for key in sorted(retail):
        wanted = triangles[key]
        if not wanted:
            continue
        min_x = min(x for triangle in wanted for x, _ in triangle) // 16 * 16
        min_y = min(y for triangle in wanted for _, y in triangle) // 16 * 16
        vertex_ids: dict[tuple[int, int], int] = {}
        generated: list[int] = []
        for x in range(min_x, min_x + 16):
            for y in range(min_y, min_y + 16):
                parity = (x ^ y) & 1
                for triangle_in_cell, offsets in enumerate(patterns[parity]):
                    coords = tuple((x + dx, y + dy) for dx, dy in offsets)
                    if frozenset(coords) not in wanted:
                        continue
                    ids = tuple(vertex_ids.setdefault(coord, len(vertex_ids)) for coord in coords)
                    if not generated:
                        generated.extend(ids if triangle_in_cell != 0
                                         else (ids[0], ids[0], ids[1], ids[2]))
                    elif triangle_in_cell != (len(generated) & 1) and generated[-2:] == list(ids[:2]):
                        generated.append(ids[2])
                    elif triangle_in_cell == (len(generated) & 1):
                        if len(generated) < 4:
                            generated.extend((ids[0], ids[0], ids[1], ids[2]))
                        else:
                            generated.extend((generated[-1], ids[0], ids[0], ids[0],
                                              ids[1], ids[2]))
                    else:
                        generated.extend((generated[-1], ids[0], ids[0], ids[1], ids[2]))

        expected = retail[key]
        matches = sum(a == b for a, b in zip(generated, expected))
        exact_indices += matches
        total_indices += max(len(generated), len(expected))
        exact_layers += generated == expected
        if generated != expected:
            mismatch = next((i for i, pair in enumerate(zip(generated, expected))
                             if pair[0] != pair[1]), min(len(generated), len(expected)))
            inverse = {vertex: coord for coord, vertex in vertex_ids.items()}
            generated_coord = inverse.get(generated[mismatch]) if mismatch < len(generated) else None
            retail_coord = retail_coords[key].get(expected[mismatch]) if mismatch < len(expected) else None
            print(f"mismatch frame={key[0]} layer={key[1]} mapping={mappings[key]} "
                  f"at={mismatch} generated={len(generated)} retail={len(expected)} "
                  f"coords={generated_coord}/{retail_coord}")
    print(f"strip_layers_exact={exact_layers}/{len(retail)}")
    print(f"strip_indices_exact={exact_indices}/{total_indices}")
    return 0 if exact_layers == len(retail) else 1


if __name__ == "__main__":
    raise SystemExit(main())
