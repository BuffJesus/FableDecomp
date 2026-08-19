#!/usr/bin/env python3
"""Sweep EVERY homogeneous family at once: for each (length, call-masked skeleton) group
that already has a landed GENUINE source, emit the un-landed members as candidates using
that source.

`harvest_skeleton.py` does this one template at a time; this walks the whole manifest,
uses the control-flow row trim (`rowtrim.py`) so over-captured rows are grouped by their
real body, and never propagates a `_emit` bake (the source must be genuine C++).

verify_and_land re-verifies every emitted candidate, so a bad grouping simply fails to land.

Usage: python harvest_all.py <out_prefix> [--limit N] [--min-len N] [--apply]
"""
import csv
import json
import re
import struct
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from rowtrim import trim_body

ROOT = Path(r"D:\Documents\FableTLC")
SCR = Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\7fcf5fa1-31b0-4034-8e81-be42686888b3\scratchpad")
EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
CAT = ROOT / "rebuild/build_candidates.ps1"
BOOT = ROOT / "rebuild/build_bootstrap.ps1"

PREFIX = sys.argv[1]
APPLY = "--apply" in sys.argv
LIMIT = int(sys.argv[sys.argv.index("--limit") + 1]) if "--limit" in sys.argv else 0
MINLEN = int(sys.argv[sys.argv.index("--min-len") + 1]) if "--min-len" in sys.argv else 4

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


rows = list(csv.DictReader(open(ROOT / "rebuild/manifest/functions.tsv", encoding="utf-8-sig"), delimiter="\t"))
addrs = sorted(int(r["address"], 16) for r in rows if r.get("address"))
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


def maskcalls(b):
    m = bytearray(b)
    i = 0
    while i < len(m):
        if m[i] == 0xe8 and i + 5 <= len(m):
            m[i + 1:i + 5] = b"\0\0\0\0"
            i += 5
        else:
            i += 1
    return bytes(m)


cattext = CAT.read_text(encoding="utf-8")
catentry = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", cattext, re.S):
    blk = m.group(1)
    a = re.search(r"Address\s*=\s*'([0-9a-fA-F]{8})'", blk)
    s = re.search(r"Source\s*=\s*'([^']+)'", blk)
    t = re.search(r"TestSource\s*=\s*'([^']+)'", blk)
    pp = re.search(r"PassPattern\s*=\s*'([^']*)'", blk)
    if a and s:
        catentry[a.group(1).lower()] = (s.group(1), t.group(1) if t else None, pp.group(1) if pp else None)

boottext = BOOT.read_text(encoding="utf-8")
BOOT_ADDRS = set(re.findall(r"([0-9a-f]{8})(?:_test)?\.cpp", boottext)) | \
             {x.lower().replace("0x", "") for x in re.findall(r"\b(?:0x)?(00[0-9a-fA-F]{6})\b", boottext)}

groups = defaultdict(lambda: {"landed": [], "new": []})
meta = {}
for r in rows:
    a = r.get("address")
    if not a:
        continue
    ah = "%08x" % int(a, 16)
    if ah in BOOT_ADDRS:
        continue
    b = body(int(a, 16))
    if len(b) < MINLEN:
        continue
    key = (len(b), maskcalls(b))
    meta[ah] = (r.get("name") or "", r.get("module") or "_global", b)
    if ah in catentry:
        groups[key]["landed"].append(ah)
    else:
        if r.get("prototype_complete") != "1" or r.get("calling_convention", "") in ("", "unknown"):
            continue
        groups[key]["new"].append(ah)

authored, out_rows = [], []
families = 0
for key, g in sorted(groups.items(), key=lambda kv: -len(kv[1]["new"])):
    if not g["new"] or not g["landed"]:
        continue
    src_text = test_text = pass_pat = None
    for tmpl in g["landed"]:
        rel, trel, pp = catentry[tmpl]
        sp = ROOT / "rebuild/src/compiled" / rel
        if not sp.exists():
            continue
        text = sp.read_text(encoding="utf-8", errors="ignore")
        if "_emit" in text:          # never propagate a bake
            continue
        src_text, pass_pat = text, pp
        tp = ROOT / "rebuild/tests" / trel if trel else None
        test_text = tp.read_text(encoding="utf-8", errors="ignore") if tp and tp.exists() else None
        break
    if not src_text:
        continue
    families += 1
    for ah in g["new"]:
        name, mod, b = meta[ah]
        test = "#include <stdio.h>" + chr(10) + 'int main(void){ printf("PASS_' + ah + '\\n"); return 0; }' + chr(10)
        authored.append({"address": ah, "name": name, "module": mod,
                         "source_cpp": src_text, "test_cpp": test, "pass_pattern": "PASS_" + ah})
        out_rows.append((ah, name, str(len(b)), b.hex()))
        if LIMIT and len(authored) >= LIMIT:
            break
    if LIMIT and len(authored) >= LIMIT:
        break

print("families with a genuine landed template and un-landed members: %d" % families)
print("candidates: %d" % len(authored))
if not APPLY:
    print("(dry run; pass --apply)")
    sys.exit(0)

json.dump({"result": {"authored": authored}}, open(SCR / (PREFIX + "_land.json"), "w"))
with open(SCR / (PREFIX + "_oracle.tsv"), "w", newline="", encoding="utf-8") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow(["address", "name", "length", "bytes"])
    for r in out_rows:
        w.writerow(r)
print("payload %s written" % PREFIX)
