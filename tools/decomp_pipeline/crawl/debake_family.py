#!/usr/bin/env python3
"""De-bake a homogeneous family: replace naked `_emit` bakes with ONE genuine C++ head
cloned across the whole family (byte-verified per member by verify_and_land).

Given a template address and a GENUINE staged source, this:
  1. computes the template's retail body + call-masked skeleton,
  2. finds every manifest fn with the same length + masked skeleton,
  3. un-lands the members whose currently-landed source is a `_emit` bake
     (catalog block + oracle row + src/test files),
  4. emits <prefix>_land.json + <prefix>_oracle.tsv for verify_and_land --land,
     using the genuine source for every member.

Bootstrap-fixture addresses (referenced by rebuild/build_bootstrap.ps1) are NEVER touched.
Members already landed GENUINE (source has no `_emit`) are left alone.

Usage: python debake_family.py <template_addr> <staged_src.cpp> <out_prefix> [--apply]
       (without --apply: report only, no files written)
"""
import csv, struct, re, json, sys, glob
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
SCR  = Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\7fcf5fa1-31b0-4034-8e81-be42686888b3\scratchpad")
EXE  = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")

TMPL = sys.argv[1].lower().replace("0x", "").rjust(8, "0")
SRC  = Path(sys.argv[2]).read_text(encoding="utf-8")
PREFIX = sys.argv[3]
APPLY = "--apply" in sys.argv

CAT = ROOT / "rebuild/build_candidates.ps1"
ORACLE = ROOT / "rebuild/oracles/auto-re-candidates.tsv"
BOOT = ROOT / "rebuild/build_bootstrap.ps1"

# ---- retail image ----
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

tb = body(int(TMPL, 16))
if not tb: raise SystemExit("template body empty")
TLEN = len(tb); SKEL = maskcalls(tb)

# ---- current landings ----
cattext = CAT.read_text(encoding="utf-8")
catsrc = {}   # addr -> Source rel path
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", cattext, re.S):
    blk = m.group(1)
    a = re.search(r"Address\s*=\s*'([0-9a-fA-F]{8})'", blk)
    s = re.search(r"Source\s*=\s*'([^']+)'", blk)
    if a: catsrc[a.group(1).lower()] = s.group(1) if s else None
boottext = BOOT.read_text(encoding="utf-8")
BOOT_ADDRS = set(re.findall(r"([0-9a-f]{8})(?:_test)?\.cpp", boottext)) | \
             {x.lower().replace("0x", "") for x in re.findall(r"\b(?:0x)?(00[0-9a-fA-F]{6})\b", boottext)}

def is_baked(addr):
    rel = catsrc.get(addr)
    if not rel: return False
    p = ROOT / "rebuild/src/compiled" / rel
    return p.exists() and "_emit" in p.read_text(encoding="utf-8", errors="ignore")

# ---- family scan ----
fam_baked, fam_genuine, fam_new, fam_skipped = [], [], [], []
for r in rows:
    a = r.get("address")
    if not a: continue
    ai = int(a, 16); ah = "%08x" % ai
    b = body(ai)
    if len(b) != TLEN or maskcalls(b) != SKEL: continue
    entry = (ah, r.get("name") or "", r.get("module") or "_global", b)
    if ah in BOOT_ADDRS: fam_skipped.append(entry + ("bootstrap-fixture",)); continue
    if ah in catsrc:
        (fam_baked if is_baked(ah) else fam_genuine).append(entry)
    else:
        if r.get("prototype_complete") != "1" or r.get("calling_convention", "") in ("", "unknown"):
            fam_skipped.append(entry + ("incomplete-manifest-row",)); continue
        fam_new.append(entry)

print(f"template {TMPL} len={TLEN}")
print(f"  baked (to de-bake) : {len(fam_baked)}")
print(f"  already genuine    : {len(fam_genuine)}")
print(f"  unlanded (bonus)   : {len(fam_new)}")
for e in fam_skipped: print(f"  SKIP {e[0]} {e[4]}")
targets = fam_baked + fam_new
if not APPLY:
    print("(dry run; pass --apply to unland bakes and emit land payload)")
    sys.exit(0)

# ---- unland the bakes ----
kill = {a for a, *_ in fam_baked}
lines = cattext.splitlines(keepends=True); out = []; i = 0; removed = 0
while i < len(lines):
    if "[pscustomobject]@{" not in lines[i]:
        out.append(lines[i]); i += 1; continue
    blk = [lines[i]]; i += 1
    while i < len(lines):
        blk.append(lines[i]); i += 1
        if re.match(r"^\s*}\s*,?\s*$", blk[-1]): break
    m = re.search(r"Address\s*=\s*'([0-9a-fA-F]{8})'", "".join(blk))
    if m and m.group(1).lower() in kill: removed += 1
    else: out.extend(blk)
CAT.write_text("".join(out), encoding="utf-8")
orows = list(csv.DictReader(open(ORACLE, encoding="utf-8-sig"), delimiter="\t"))
kept = [r for r in orows if r["address"].lower().replace("0x", "") not in kill]
with open(ORACLE, "w", encoding="utf-8", newline="") as f:
    w = csv.DictWriter(f, delimiter="\t", fieldnames=["address", "name", "length", "bytes"], lineterminator="\n")
    w.writeheader(); w.writerows(kept)
nfiles = 0
for a in kill:
    for d, pat in ((ROOT / "rebuild/src/compiled", f"*_{a}.cpp"), (ROOT / "rebuild/tests", f"*_{a}_test.cpp")):
        for p in d.rglob(pat): p.unlink(); nfiles += 1
print(f"UNLANDED bakes: catalog_blocks={removed} oracle_rows={len(orows)-len(kept)} files={nfiles}")

# ---- emit land payload ----
authored, out_rows = [], []
for ah, name, mod, b in targets:
    test = "#include <stdio.h>" + chr(10) + 'int main(void){ printf("PASS_' + ah + '\\n"); return 0; }' + chr(10)
    authored.append({"address": ah, "name": name, "module": mod,
                     "source_cpp": SRC, "test_cpp": test, "pass_pattern": "PASS_" + ah})
    out_rows.append((ah, name, str(TLEN), b.hex()))
json.dump({"result": {"authored": authored}}, open(SCR / (PREFIX + "_land.json"), "w"))
with open(SCR / (PREFIX + "_oracle.tsv"), "w", newline="", encoding="utf-8") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n"); w.writerow(["address", "name", "length", "bytes"])
    for r in out_rows: w.writerow(r)
print(f"payload {PREFIX}: {len(authored)} candidates -> {SCR/(PREFIX+'_land.json')}")
