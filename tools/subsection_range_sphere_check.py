#!/usr/bin/env python3
"""Validate retail subsection spheres from serialized member ranges.

The baked arrays are already remapped. Replaying the builder would apply a
second permutation, so this check uses each lane's stored start/count range.
"""
import argparse, csv, math, struct

def main():
    p = argparse.ArgumentParser()
    p.add_argument("input", help="TSV from subsection_spheres.py")
    p.add_argument("--tolerance", type=float, default=3.5e-4)
    a = p.parse_args(); lanes = within = 0; worst = [0.0] * 4
    with open(a.input, encoding="utf-8") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            v = list(map(float, row["spheres"].split()))
            spheres = [v[i:i+4] for i in range(0, len(v), 4)]
            table = bytes.fromhex(row["tableHex"])
            for element in range(len(table) // 0x50):
                base = element * 0x50
                stored = [struct.unpack_from("<4f", table, base + off)
                          for off in (0, 0x10, 0x20, 0x30)]
                for lane, count in enumerate(table[base+0x40:base+0x44]):
                    if not count: continue
                    start = table[base+0x44+lane]
                    members = spheres[start:start+count]
                    lo = [min(s[i]-s[3] for s in members) for i in range(3)]
                    hi = [max(s[i]+s[3] for s in members) for i in range(3)]
                    centre = [(lo[i]+hi[i])/2.0 for i in range(3)]
                    radius = max(math.dist(s[:3], centre)+s[3] for s in members)
                    want = [stored[i][lane] for i in range(4)]
                    errors = [abs(x-y) for x, y in zip(centre+[radius], want)]
                    worst = [max(x, y) for x, y in zip(worst, errors)]
                    lanes += 1; within += max(errors) <= a.tolerance
    print(f"populated lanes: {lanes}")
    print(f"within {a.tolerance:.7g}: {within}/{lanes} ({100*within/lanes:.2f}%)")
    print("worst absolute error x/y/z/r: " + " / ".join(f"{x:.9g}" for x in worst))
    return 0 if within == lanes else 1

if __name__ == "__main__": raise SystemExit(main())
