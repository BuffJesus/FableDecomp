#!/usr/bin/env python3
"""Flatten the retail subsection oracle into per-instance BOUNDING SPHERES.

The engine feeds CLocalDetailPrimitiveRepeatedMesh::BuildSubSectionsAndObjectRemapTable
one C3DBoundingSphere per source object, built (0x02EE1364..0x02EE13F9) as

    centre = objectMatrix.TransformPoint(mesh.boundingSphere.centre)
    radius = mesh.boundingSphere.radius * object.scale

The mesh values are AUTHORED DATA, read from the mesh bank's Info descriptor
(u32 flags, then f32 origin[10]; origin[0..2] centre, origin[3] radius) - not
derived from the bounding box and not constant. Independently confirmed by
EgoCore (MeshParser.h CMeshEntryMetadata) and OpenAlbion (mesh.rs BoundingSphere
+ BoundingBox).

THE OBJECT MATRIX HAS TWO FORMS, and which applies is a per-collection-type
property (the def's TiltToSlope). CQuadTreeElement::AddObjectsFromLayerElement
(FableWin 0x02E3C5D0) first sets a plain scaled Z rotation through
CMatrix3x4::Set at 0x02E3C988:

    [ c*s   s*s   0 ]
    [ -s*s  c*s   0 ]      c = cos(yaw), s = sin(yaw), scale on the diagonal
    [ 0     0     scale ]

and then, only if the layer flag at +0x18 is set (tested 0x02E3C990-0x02E3C996),
builds a ground basis and post-multiplies it in at 0x02E3CAED:

    x = normalise(worldY x n)      cross 0x02E3C9F9, normalise 0x02E3CA1E
    y = n x x                      cross 0x02E3CA37, normalise 0x02E3CA5C
    z = n                          n = that instance's landscape normal

That tilt CANNOT survive into the file: a serialised instance is only
A = (cos*scale, sin*scale, 0, 0) and B = (x, y, z, scale). So retail's baked
subsection spheres were built from a transform the file no longer carries, and
reproducing them requires re-applying the tilt from the LandscapeNormalArray.

Measured over 1,855 single-instance leaf lanes (where a lane's sphere must be
exactly that one instance's sphere): flat explains 683, tilted explains 1,242,
and together they explain 1,855 - 100.0%. The choice is consistent per
collection type, with one anomalous lane in 1,855.

Our own writer's instances are never tilted, so its spheres should stay FLAT - a
tilted cull volume would not match what the runtime draws. The tilt belongs here,
in the oracle, so the gate measures the BUILDER rather than a transform
difference.

Output TSV (one row per retail type-1 record that has a subsection table):
    map, recordId, collectionType, objectCount, spheres(4*n floats), tableHex

Usage:
    subsection_spheres.py <oracle.jsonl> <mesh-spheres.tsv> <out.tsv>
where mesh-spheres.tsv comes from mesh_sphere_table.py.
"""

from __future__ import annotations

import json
import math
import struct
import sys
from pathlib import Path


def _normalise(v):
    length = math.sqrt(sum(c * c for c in v))
    return tuple(c / length for c in v) if length > 1e-12 else (0.0, 0.0, 1.0)


def _cross(a, b):
    return (a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0])


def transform_tilted(mesh_centre, c, s, scale, normal):
    """The scaled Z rotation post-multiplied by the ground basis, exactly as
    AddObjectsFromLayerElement composes it (0x02E3C9F9..0x02E3CAED)."""
    rot = [[c, s, 0.0], [-s, c, 0.0], [0.0, 0.0, scale]]
    n = _normalise(normal)
    x = _normalise(_cross((0.0, 1.0, 0.0), n))
    y = _cross(n, x)
    basis = [list(x), list(y), list(n)]
    combined = [[sum(rot[r][k] * basis[k][col] for k in range(3))
                 for col in range(3)] for r in range(3)]
    return tuple(sum(mesh_centre[k] * combined[k][col] for k in range(3))
                 for col in range(3))


def transform_flat(mesh_centre, c, s, scale):
    mx, my, mz = mesh_centre
    return (c * mx - s * my, s * mx + c * my, scale * mz)


def infer_tilt(records, mesh):
    """Decide TiltToSlope per (map, collectionType) from single-instance leaf
    lanes, where the lane's sphere must be exactly that instance's sphere.
    Lanes on flat ground cannot distinguish the two and are not counted."""
    votes: dict[tuple[str, int], list[int]] = {}
    for record in records:
        if not record.get("subsectionsPresent") or not record.get("tableHex"):
            continue
        key = (record["map"], record["collectionType"])
        if key not in mesh:
            continue
        mx, my, mz, _ = mesh[key]
        table = bytes.fromhex(record["tableHex"])
        normals = list(zip(record.get("normalX", []), record.get("normalY", []),
                           record.get("normalZ", [])))
        for element in range(len(table) // 0x50):
            base = element * 0x50
            lanes = [struct.unpack_from("<4f", table, base + off)
                     for off in (0x00, 0x10, 0x20)]
            count = table[base + 0x40:base + 0x44]
            start = table[base + 0x44:base + 0x48]
            for q in range(4):
                if count[q] != 1:
                    continue
                i = start[q]
                if i >= record["objectCount"] or i >= len(normals):
                    continue
                a, b = record["A"][i], record["B"][i]
                c, s, scale = a[0], a[1], b[3]
                target = (lanes[0][q], lanes[1][q], lanes[2][q])
                f = transform_flat((mx, my, mz), c, s, scale)
                t = transform_tilted((mx, my, mz), c, s, scale, normals[i])
                ok_flat = math.dist((f[0] + b[0], f[1] + b[1], f[2] + b[2]),
                                    target) < 1e-3
                ok_tilt = math.dist((t[0] + b[0], t[1] + b[1], t[2] + b[2]),
                                    target) < 1e-3
                if ok_flat == ok_tilt:
                    continue  # flat ground, or neither: no information
                tally = votes.setdefault(key, [0, 0])
                tally[1 if ok_tilt else 0] += 1
    return votes


def main() -> int:
    if len(sys.argv) != 4:
        raise SystemExit(__doc__)
    oracle_path, mesh_path, out_path = (Path(a) for a in sys.argv[1:])

    mesh: dict[tuple[str, int], tuple[float, float, float, float]] = {}
    with mesh_path.open(encoding="utf-8") as stream:
        stream.readline()
        for line in stream:
            if not line.strip():
                continue
            f = line.rstrip("\n").split("\t")
            mesh[(f[0], int(f[1]))] = (float(f[3]), float(f[4]), float(f[5]),
                                       float(f[6]))

    records = [json.loads(line) for line in oracle_path.open(encoding="utf-8")
               if line.strip()]
    votes = infer_tilt(records, mesh)
    tilted = {key: v[1] > v[0] for key, v in votes.items()}
    print("TiltToSlope inferred per collection (unambiguous lanes only):")
    for key in sorted(votes):
        flat_votes, tilt_votes = votes[key]
        print("  %-28s %-6s  %d flat / %d tilted"
              % ("%s/type%d" % key, "TILTED" if tilted[key] else "flat",
                 flat_votes, tilt_votes))
    mixed = [k for k, v in votes.items() if v[0] and v[1]]
    if mixed:
        print("  WARNING: mixed verdicts on %s - the flag should be per type"
              % ", ".join("%s/type%d" % k for k in mixed))

    written = skipped_no_table = skipped_no_mesh = 0
    missing: set[tuple[str, int]] = set()
    with out_path.open("w", encoding="utf-8", newline="\n") as out:
        out.write("map\trecordId\tcollectionType\tobjectCount\tspheres\ttableHex\n")
        for index, record in enumerate(records):
            if not record.get("subsectionsPresent") or not record.get("tableHex"):
                skipped_no_table += 1
                continue
            key = (record["map"], record["collectionType"])
            if key not in mesh:
                skipped_no_mesh += 1
                missing.add(key)
                continue
            mx, my, mz, mr = mesh[key]
            tilt = tilted.get(key, False)
            normals = list(zip(record.get("normalX", []),
                               record.get("normalY", []),
                               record.get("normalZ", [])))
            values: list[str] = []
            for i, (a, b) in enumerate(zip(record["A"], record["B"])):
                c, s = a[0], a[1]          # cos*scale, sin*scale
                px, py, pz, scale = b
                if tilt and i < len(normals):
                    cx, cy, cz = transform_tilted((mx, my, mz), c, s, scale,
                                                  normals[i])
                else:
                    cx, cy, cz = transform_flat((mx, my, mz), c, s, scale)
                values += [repr(cx + px), repr(cy + py), repr(cz + pz),
                           repr(mr * scale)]
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
