#!/usr/bin/env python3
"""Audit compiled polygon counts for mesh IDs referenced by a foliage oracle.

The oracle TSV only needs a ``meshIdx`` column (the output of
``mesh_sphere_table.py`` is suitable). The primitive grammar is shared with
``parse_mesh.find_subm_headers``; this command fails if any referenced mesh has
no descriptor or no recognized primitive.

Usage: mesh_polycount_audit.py <graphics.big> <mesh-spheres.tsv>
"""

from __future__ import annotations

import csv
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import parse_mesh


def main() -> int:
    if len(sys.argv) != 3:
        raise SystemExit(__doc__)
    bank_path, oracle_path = Path(sys.argv[1]), Path(sys.argv[2])
    bank, entries = parse_mesh.load_bigb(bank_path)
    by_id = {entry["id"]: entry for entry in entries}
    with oracle_path.open(encoding="utf-8", newline="") as src:
        mesh_ids = sorted({int(row["meshIdx"])
                           for row in csv.DictReader(src, delimiter="\t")})

    missing: list[str] = []
    decoded: list[tuple[int, str, int, int]] = []
    for mesh_id in mesh_ids:
        entry = by_id.get(mesh_id)
        if entry is None:
            missing.append(f"{mesh_id}: absent from MBANK_ALLMESHES")
            continue
        payload = bank[entry["offset"]:entry["offset"] + entry["size"]]
        desc = parse_mesh.parse_mesh_descriptor(
            entry["info"], payload, entry["name"])
        if not desc or not desc["lod"]:
            missing.append(f"{mesh_id} {entry['name']}: no compiled descriptor")
            continue
        primitives = parse_mesh.find_subm_headers(payload, desc["lod"][0])
        if not primitives:
            missing.append(f"{mesh_id} {entry['name']}: no primitive headers")
            continue
        decoded.append((mesh_id, entry["name"], len(primitives),
                        sum(p["nFaces"] for p in primitives)))

    for mesh_id, name, primitive_count, triangles in decoded:
        print(f"{mesh_id:5d}  {triangles:6d} triangles  "
              f"{primitive_count:2d} primitive(s)  {name}")
    print(f"coverage: {len(decoded)}/{len(mesh_ids)} referenced mesh IDs")
    if missing:
        for problem in missing:
            print(f"ERROR: {problem}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
