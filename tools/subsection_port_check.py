#!/usr/bin/env python3
"""Flatten tools/subsection_oracle.py's JSONL into the TSV that
tools/subsection_port_check.cpp replays through FableForge's ported
CLocalDetailPrimitiveRepeatedMesh subsection builder.

One row per retail type-1 record that carries a subsection table:

    <objectCount> \t <B as x y z scale, count*4 floats> \t <tableHex>

B is the baked per-instance position/scale array; the engine's builder is fed
one C3DBoundingSphere per instance whose centre is
objectMatrix.TransformPoint(mesh.boundingSphere.centre) and whose radius is
mesh.boundingSphere.radius * scale.  The mesh sphere is NOT in the chunk, so the
harness substitutes the placement itself for the centre; that is exact only if
the mesh sphere is centred on the mesh origin, which is why the harness scores
the integer lanes (count/startIndex/childOffset, which the substitution does not
disturb in any sampled record) separately from the float lanes.

Usage:
    python tools/subsection_port_check.py tmp/subsection_oracle/oracle.jsonl out.tsv
"""
import json
import sys


def main() -> int:
    if len(sys.argv) != 3:
        sys.stderr.write(__doc__)
        return 2
    rows = 0
    with open(sys.argv[1], encoding="utf-8") as src, \
            open(sys.argv[2], "w", encoding="utf-8") as dst:
        for line in src:
            rec = json.loads(line)
            if not rec.get("subsectionsPresent"):
                continue
            flat = []
            for v in rec["B"]:
                flat.extend(v[:4])
            if len(flat) != 4 * rec["objectCount"]:
                raise SystemExit("B array length disagrees with objectCount")
            dst.write("%d\t%s\t%s\n" % (
                rec["objectCount"],
                " ".join("%r" % x for x in flat),
                rec["tableHex"]))
            rows += 1
    print("%d rows" % rows)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
