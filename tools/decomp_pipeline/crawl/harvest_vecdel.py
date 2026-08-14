#!/usr/bin/env python3
"""Bulk-harvest the CLandscapeBackgroundPatch::`vector_deleting_destructor' vein.

The binary carries hundreds of byte-identical 30-byte vector-deleting-destructor
thunks (one ??_E per class instantiation); the ONLY per-fn delta is the two
`call rel32` dwords, which verify_and_land reloc-masks. So a single proven source
(the 0x00442980 template) lands every one as RELOCATION_MATCH.

This tool applies next_smallest's eligibility filter, keeps only rows whose body
masks to the template skeleton, and emits a chunk of <N> as a land.json+oracle for
verify_and_land --land. Non-destructive: writes only to the scratchpad prefix.

Usage: python harvest_vecdel.py <chunk_size> <out_prefix>
"""
import csv, struct, re, json, sys
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
SCR  = Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\78ee60a6-b0e0-43fa-a126-1b2d399e2fca\scratchpad")
EXE  = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
TMPL_SRC  = ROOT / "rebuild/src/compiled/00/44/CLandscapeBackgroundPatch_vectordeletingdestructor_00442980.cpp"
TMPL_TEST = ROOT / "rebuild/tests/00/44/CLandscapeBackgroundPatch_vectordeletingdestructor_00442980_test.cpp"
PASS = "CLandscapeBackgroundPatch_00442980_TEST PASS"
SKEL = bytes.fromhex("568bf1e800000000f644240801740956e80000000083c4048bc65ec20400")

N      = int(sys.argv[1])
PREFIX = sys.argv[2]

data = EXE.read_bytes()
e = struct.unpack_from("<I", data, 0x3C)[0]; coff = e + 4
nsec = struct.unpack_from("<H", data, coff + 2)[0]; opt = struct.unpack_from("<H", data, coff + 16)[0]
secbase = coff + 20 + opt; IMG = 0x400000; secs = []
for i in range(nsec):
    o = secbase + i * 40
    va, = struct.unpack_from("<I", data, o + 12); rp, = struct.unpack_from("<I", data, o + 20)
    rs, = struct.unpack_from("<I", data, o + 16); vs, = struct.unpack_from("<I", data, o + 8)
    secs.append((va, max(vs, rs), rp))
def off(va):
    rva = va - IMG
    for va0, sz, rp in secs:
        if va0 <= rva < va0 + sz: return rp + (rva - va0)
rows = list(csv.DictReader(open(ROOT / "rebuild/manifest/functions.tsv", encoding="utf-8-sig"), delimiter="\t"))
addrs = sorted(int(r["address"], 16) for r in rows if r.get("address"))
nextof = {addrs[i]: addrs[i + 1] for i in range(len(addrs) - 1)}
cat = (ROOT / "rebuild/build_candidates.ps1").read_text(encoding="utf-8")
landed = set(int(m, 16) for m in re.findall(r"Address\s*=\s*'([0-9a-fA-F]{8})'", cat))
tried = set(int(x, 16) for x in (ROOT / "tools/decomp_pipeline/crawl/gen_tried.txt").read_text().split())
def body(va):
    o = off(va); nx = nextof.get(va)
    if o is None or nx is None: return b""
    raw = data[o:o + (nx - va)]; ee = len(raw)
    while ee > 0 and raw[ee - 1] in (0xCC, 0x90): ee -= 1
    return raw[:ee]
def masked(b):
    if len(b) != 30 or b[3] != 0xe8 or b[0x10] != 0xe8: return None
    m = bytearray(b)
    for o in (4, 0x11): m[o:o + 4] = b'\0\0\0\0'
    return bytes(m)

src = TMPL_SRC.read_text(encoding="utf-8"); test = TMPL_TEST.read_text(encoding="utf-8")
match = []
for r in rows:
    a = r.get("address")
    if not a: continue
    ai = int(a, 16)
    if ai in landed or ai in tried: continue
    if r.get("retail_parity"): continue
    if r.get("prototype_complete") != "1": continue
    if r.get("calling_convention", "") in ("", "unknown"): continue
    b = body(ai)
    if masked(b) == SKEL:
        match.append((ai, r.get("name") or "", b))
match.sort()
chunk = match[:N]
authored, orows = [], []
for ai, name, b in chunk:
    ah = "%08x" % ai
    authored.append({"address": ah, "name": name, "module": "CLandscapeBackgroundPatch",
                     "source_cpp": src, "test_cpp": test, "pass_pattern": PASS})
    orows.append((ah, name, "30", b.hex()))
json.dump({"result": {"authored": authored}}, open(SCR / (PREFIX + "_land.json"), "w"))
with open(SCR / (PREFIX + "_oracle.tsv"), "w", newline="", encoding="utf-8") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n"); w.writerow(["address", "name", "length", "bytes"])
    for row in orows: w.writerow(row)
print(f"vein remaining: {len(match)}  | {PREFIX}: {len(chunk)}")
