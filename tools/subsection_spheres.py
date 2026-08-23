#!/usr/bin/env python3
"""Flatten the retail subsection oracle into per-instance BOUNDING SPHERES.

The engine feeds CLocalDetailPrimitiveRepeatedMesh::BuildSubSectionsAndObjectRemapTable
one C3DBoundingSphere per source object, built (0x02EE1364..0x02EE13F9) as

    centre = objectMatrix.TransformPoint(mesh.boundingSphere.centre)
    radius = mesh.boundingSphere.radius * object.scale

Both mesh values are AUTHORED DATA, read from the mesh bank's Info descriptor
(u32 flags, then f32 origin[10]; origin[0..2] centre, origin[3] radius) - not
derived from the bounding box (no candidate formula reproduces origin[3] on more
than 5.3% of MBANK_ALLMESHES' 3,295 compiled meshes) and not a constant. Solving
radius/scale out of retail's own baked subsection bytes reproduces origin[3]
exactly for every collection sampled, which is what pins this interpretation.

The object matrix the writer builds is a Z rotation scaled by the instance scale
plus the placement translation, and the oracle stores A = (cos*scale, sin*scale,
0, 0) and B = (x, y, z, scale), so the transform is recoverable per instance
without re-deriving the angle.

Output TSV (one row per retail type-1 record that has a subsection table):
    map, recordId, collectionType, objectCount, spheres(4*n floats), tableHex

Usage:
    subsection_spheres.py <oracle.jsonl> <mesh-spheres.tsv> <out.tsv>
where mesh-spheres.tsv comes from mesh_sphere_table.py.
"""

from __future__ import annotations

import json
import sys
from pathlib import Path


def main() -> int:
    if len(sys.argv) != 4:
        raise SystemExit(__doc__)
    oracle_path, mesh_path, out_path = (Path(a) for a in sys.argv[1:])

    mesh: dict[tuple[str, int], tuple[float, float, float, float]] = {}
    with mesh_path.open(encoding="utf-8") as stream:
        header = stream.readline()
        for line in stream:
            if not line.strip():
                continue
            f = line.rstrip("\n").split("\t")
            mesh[(f[0], int(f[1]))] = (float(f[3]), float(f[4]), float(f[5]), float(f[6]))

    written = skipped_no_table = skipped_no_mesh = 0
    missing: set[tuple[str, int]] = set()
    with out_path.open("w", encoding="utf-8", newline="\n") as out:
        out.write("map\trecordId\tcollectionType\tobjectCount\tspheres\ttableHex\n")
        for index, line in enumerate(oracle_path.open(encoding="utf-8")):
            if not line.strip():
                continue
            record = json.loads(line)
            if not record.get("subsectionsPresent") or not record.get("tableHex"):
                skipped_no_table += 1
                continue
            key = (record["map"], record["collectionType"])
            if key not in mesh:
                skipped_no_mesh += 1
                missing.add(key)
                continue
            mx, my, mz, mr = mesh[key]
            values: list[str] = []
            for a, b in zip(record["A"], record["B"]):
                c, s = a[0], a[1]          # cos*scale, sin*scale
                px, py, pz, scale = b
                values += [
                    repr(c * mx - s * my + px),
                    repr(s * mx + c * my + py),
                    repr(scale * mz + pz),
                    repr(mr * scale),
                ]
            out.write("%s\t%d\t%d\t%d\t%s\t%s\n" % (
                record["map"], index, record["collectionType"],
                record["objectCount"], " ".join(values), record["tableHex"]))
            written += 1

    print("wrote %d record(s) to %s" % (written, out_path))
    print("skipped: %d with no subsection table, %d with no mesh sphere"
          % (skipped_no_table, skipped_no_mesh))
    if missing:
        print("missing mesh spheres for: %s"
              % ", ".join("%s/type%d" % k for k in sorted(missing)[:12]))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
