#!/usr/bin/env python3
"""Give every landed candidate an oracle row, so every landed candidate is byte-checked.

The comparer reports `ORACLE_MISSING` for a catalog entry with no row in
`rebuild/oracles/auto-re-candidates.tsv`: it is compiled and behaviour-tested but its bytes
are never compared with retail. 3,392 of 18,177 entries (19%) were in that state -- landed
by lanes that predate the oracle plumbing -- so the headline parity numbers silently
excluded them.

3,267 of those ARE authoritative manifest function starts, so their retail bytes can simply
be extracted (same boundary + `rowtrim` control-flow trim as `pe_oracle.py`). The remaining
125 are NOT manifest starts and cannot be verified at all; they are listed, never invented.

Usage: python backfill_oracles.py [--write] [--limit N]
Without --write: report only.
"""
import csv
import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent / "crawl"))
from rowtrim import trim_body  # noqa: E402

ROOT = Path(r"D:\Documents\FableTLC")
EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
ORACLE = ROOT / "rebuild/oracles/auto-re-candidates.tsv"
CATALOG = ROOT / "rebuild/build_candidates.ps1"
MANIFEST = ROOT / "rebuild/manifest/functions.tsv"

WRITE = "--write" in sys.argv
LIMIT = int(sys.argv[sys.argv.index("--limit") + 1]) if "--limit" in sys.argv else 0

data = EXE.read_bytes()
e = struct.unpack_from("<I", data, 0x3C)[0]
coff = e + 4
nsec = struct.unpack_from("<H", data, coff + 2)[0]
opt = struct.unpack_from("<H", data, coff + 16)[0]
secbase = coff + 20 + opt
secs = []
for i in range(nsec):
    o = secbase + i * 40
    va, = struct.unpack_from("<I", data, o + 12)
    rp, = struct.unpack_from("<I", data, o + 20)
    rs, = struct.unpack_from("<I", data, o + 16)
    vs, = struct.unpack_from("<I", data, o + 8)
    secs.append((va, max(vs, rs), rp))


def off(va):
    rva = va - 0x400000
    for va0, sz, rp in secs:
        if va0 <= rva < va0 + sz:
            return rp + (rva - va0)


rows = list(csv.DictReader(open(MANIFEST, encoding="utf-8-sig"), delimiter="\t"))
byaddr = {}
for r in rows:
    a = r.get("address")
    if a:
        byaddr["%08x" % int(a, 16)] = r
addrs = sorted(int(a, 16) for a in byaddr)
nextof = {addrs[i]: addrs[i + 1] for i in range(len(addrs) - 1)}


def body(va):
    o = off(va)
    nx = nextof.get(va)
    if o is None or nx is None:
        return b""
    raw = data[o:o + (nx - va)]
    ee = len(raw)
    while ee > 0 and raw[ee - 1] in (0xCC, 0x90):
        ee -= 1
    return trim_body(raw[:ee], va)[0]


oracle_rows = list(csv.DictReader(open(ORACLE, encoding="utf-8-sig"), delimiter="\t"))
have = {r["address"].lower().replace("0x", "") for r in oracle_rows}
catalog = [m.group(1).lower()
           for m in re.finditer(r"Address\s*=\s*'([0-9a-fA-F]{8})'",
                                CATALOG.read_text(encoding="utf-8"))]

missing = [a for a in catalog if a not in have]
verifiable = [a for a in missing if a in byaddr]
unverifiable = [a for a in missing if a not in byaddr]

new, empty = [], []
for a in verifiable:
    if LIMIT and len(new) >= LIMIT:
        break
    b = body(int(a, 16))
    if not b:
        empty.append(a)
        continue
    name = byaddr[a].get("name") or ("sub_%s" % a)
    new.append({"address": a, "name": name, "length": str(len(b)), "bytes": b.hex()})

print("catalog=%d  oracle_missing=%d  verifiable=%d  extracted=%d  no_body=%d  "
      "NOT_A_MANIFEST_START=%d"
      % (len(catalog), len(missing), len(verifiable), len(new), len(empty),
         len(unverifiable)))
if unverifiable:
    print("  unverifiable (no authoritative function start -- never invent an oracle):")
    for a in unverifiable[:10]:
        print("    %s" % a)
    if len(unverifiable) > 10:
        print("    ... %d more" % (len(unverifiable) - 10))

if not WRITE:
    print("(report only; pass --write)")
    sys.exit(0)

fields = ["address", "name", "length", "bytes"]
with open(ORACLE, "w", encoding="utf-8", newline="") as f:
    w = csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
    w.writeheader()
    for r in oracle_rows:
        w.writerow({k: r.get(k, "") for k in fields})
    w.writerows(new)
print("APPENDED %d oracle rows -> %s" % (len(new), ORACLE))
