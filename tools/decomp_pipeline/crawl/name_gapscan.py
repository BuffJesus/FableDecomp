#!/usr/bin/env python3
"""Name the discovered `_gapscan` functions from their RTTI vtable membership.

`manifest_add_gaps.py` adds recovered function starts as `sub_<addr>` with module
`_gapscan`, because nothing about the bytes tells you what the function IS. RTTI does:
if an address sits in a class vtable, it is that class's virtual function at that slot.

Only `_gapscan` rows with a placeholder `sub_*` name are touched, so Ghidra-derived names
are never overwritten. Provenance is preserved (`agent_source` stays `gapscan`) and the
naming source is recorded in `agent_source_path` = "rtti-vtables.tsv".

Usage: python name_gapscan.py [--write]
"""
import csv
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from rtti import build, demangle_class

ROOT = Path(r"D:\Documents\FableTLC")
MANIFEST = ROOT / "rebuild/manifest/functions.tsv"
csv.field_size_limit(10 ** 9)

slots, _, _, _ = build()
with open(MANIFEST, encoding="utf-8-sig", newline="") as f:
    reader = csv.DictReader(f, delimiter="\t")
    fields = reader.fieldnames
    rows = list(reader)

named = 0
for r in rows:
    # key off the PROVENANCE field, not module: the first naming pass rewrites
    # module to the class, so a module check would skip its own output.
    if r.get("agent_source") != "gapscan":
        continue
    # re-nameable: a placeholder, or a name this tool itself assigned
    if not ((r.get("name") or "").startswith("sub_")
            or r.get("agent_source_path") == "rtti-vtables.tsv"):
        continue
    va = int(r["address"], 16)
    hit = slots.get(va)
    if not hit:
        continue
    cls, vt, idx = hit
    cname = demangle_class(cls)
    r["name"] = "%s::vfunc_%d" % (cname, idx)
    r["module"] = cname
    r["agent_source_path"] = "rtti-vtables.tsv"
    named += 1

print("gapscan rows named from RTTI vtables: %d" % named)
if "--write" not in sys.argv:
    print("(dry run; pass --write)")
    sys.exit(0)
tmp = MANIFEST.with_suffix(".tsv.new")
with open(tmp, "w", encoding="utf-8", newline="") as f:
    w = csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
    w.writeheader()
    w.writerows(rows)
tmp.replace(MANIFEST)
print("manifest updated")
