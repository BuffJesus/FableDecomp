#!/usr/bin/env python3
"""Generalized homogeneous-family harvester for the parity crawl.

Many binary regions carry N byte-identical small functions (??_E vector-deleting
destructors, `_Dest_val` STL destroy helpers, `OnKill`/`OnInitialActivate` vtable
predicates, ...) whose ONLY per-fn delta is the `call rel32` (and other reloc)
dwords that verify_and_land masks. So one PROVEN-landed source lands the whole
family as RELOCATION_MATCH.

Given a template address that is ALREADY landed byte-exact, this finds its authored
source+test, computes its call-masked skeleton, then emits a chunk of every eligible
un-landed manifest fn with the same length + masked skeleton as a land.json+oracle
for `verify_and_land --land`. The harness re-verifies each clone, so a bad match just
fails to land (never a false land).

Usage: python harvest_skeleton.py <template_addr_hex8> <chunk_size> <out_prefix>
"""
import csv, struct, re, json, sys, glob
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
SCR  = Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\d9037860-7095-40c8-8a12-a018c1d9a369\scratchpad")
EXE  = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")

TMPL_ADDR = sys.argv[1].lower().replace("0x", "").rjust(8, "0")
N         = int(sys.argv[2])
PREFIX    = sys.argv[3]

def find_one(pat):
    hits = glob.glob(str(ROOT / pat), recursive=True)
    if not hits: raise SystemExit(f"no file for {pat}")
    return Path(hits[0])
src_path  = find_one(f"rebuild/src/compiled/**/*_{TMPL_ADDR}.cpp")
test_path = find_one(f"rebuild/tests/**/*_{TMPL_ADDR}_test.cpp")
src  = src_path.read_text(encoding="utf-8")
test = test_path.read_text(encoding="utf-8")
# the success print: a quoted string containing PASS but not FAIL; strip trailing \n
cands = [s for s in re.findall(r'"((?:[^"\\]|\\.)*)"', test)
         if "PASS" in s and "FAIL" not in s]
if not cands: raise SystemExit("cannot find PASS literal in template test")
PASS = cands[-1]
for suf in ("\\n", "\\r", "\n", "\r"):
    while PASS.endswith(suf): PASS = PASS[:-len(suf)]

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
def maskcalls(b):
    m = bytearray(b); i = 0
    while i < len(m):
        if m[i] == 0xe8 and i + 5 <= len(m): m[i + 1:i + 5] = b'\0\0\0\0'; i += 5
        else: i += 1
    return bytes(m)

tb = body(int(TMPL_ADDR, 16))
if not tb: raise SystemExit("template body empty")
TLEN = len(tb); SKEL = maskcalls(tb)

# module name from the template manifest row
meta = {int(r["address"], 16): r for r in rows if r.get("address")}
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
    if len(b) == TLEN and maskcalls(b) == SKEL:
        match.append((ai, r.get("name") or "", r.get("module") or "_global", b))
match.sort()
chunk = match[:N]
authored, orows = [], []
for ai, name, mod, b in chunk:
    ah = "%08x" % ai
    authored.append({"address": ah, "name": name, "module": mod,
                     "source_cpp": src, "test_cpp": test, "pass_pattern": PASS})
    orows.append((ah, name, str(TLEN), b.hex()))
json.dump({"result": {"authored": authored}}, open(SCR / (PREFIX + "_land.json"), "w"))
with open(SCR / (PREFIX + "_oracle.tsv"), "w", newline="", encoding="utf-8") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n"); w.writerow(["address", "name", "length", "bytes"])
    for row in orows: w.writerow(row)
print(f"template {TMPL_ADDR} len={TLEN} vein_remaining={len(match)} | {PREFIX}: {len(chunk)}")
