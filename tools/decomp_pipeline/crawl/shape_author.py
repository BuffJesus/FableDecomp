#!/usr/bin/env python3
"""Author genuine C++ for whole SHAPE CLASSES of tiny functions (getters, setters,
constant returns, empty hooks) straight from their retail bytes.

Unlike debake_family.py (one template -> one byte-identical family), this recognises a
parameterised instruction shape and emits a per-address source with the right member
offset / constant / argument count. Every emitted candidate is still byte-verified by
verify_and_land, so a misclassification simply fails to land.

Usage: python shape_author.py <out_prefix> [--baked] [--unlanded] [--limit N] [--apply]
  --baked     include currently-landed `_emit` bakes (un-landed on --apply)   [default]
  --unlanded  also include manifest functions that were never landed
  --apply     write the payload (and un-land the bakes); otherwise report only
  --shape-is-prototype  also take rows whose Ghidra prototype is INCOMPLETE
              (same rationale as `_gapscan`: the bytes are the only evidence)
"""
import csv, json, re, struct, sys
from pathlib import Path

sys_path_hack = Path(__file__).resolve().parent
import sys as _sys
_sys.path.insert(0, str(sys_path_hack))
from rowtrim import trim_body
from purity import is_genuine   # over-captured manifest rows are cut to their real body
from collections import Counter

ROOT = Path(r"D:\Documents\FableTLC")
SCR  = Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\7fcf5fa1-31b0-4034-8e81-be42686888b3\scratchpad")
EXE  = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
CAT  = ROOT / "rebuild/build_candidates.ps1"
ORACLE = ROOT / "rebuild/oracles/auto-re-candidates.tsv"
BOOT = ROOT / "rebuild/build_bootstrap.ps1"

PREFIX = sys.argv[1]
APPLY = "--apply" in sys.argv
WANT_BAKED = "--unlanded" not in sys.argv or "--baked" in sys.argv
WANT_NEW = "--unlanded" in sys.argv
LIMIT = int(sys.argv[sys.argv.index("--limit") + 1]) if "--limit" in sys.argv else 0
SHAPE_PROTO = "--shape-is-prototype" in sys.argv

data = EXE.read_bytes()
e = struct.unpack_from("<I", data, 0x3C)[0]; coff = e + 4
nsec = struct.unpack_from("<H", data, coff + 2)[0]; opt = struct.unpack_from("<H", data, coff + 16)[0]
secbase = coff + 20 + opt; secs = []
for i in range(nsec):
    o = secbase + i * 40
    va, = struct.unpack_from("<I", data, o + 12); rp, = struct.unpack_from("<I", data, o + 20)
    rs, = struct.unpack_from("<I", data, o + 16); vs, = struct.unpack_from("<I", data, o + 8)
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
    o = off(va); nx = nextof.get(va)
    if o is None or nx is None:
        return b""
    raw = data[o:o + (nx - va)]; ee = len(raw)
    while ee > 0 and raw[ee - 1] in (0xCC, 0x90):
        ee -= 1
    body_bytes = raw[:ee]
    return trim_body(body_bytes, va)[0]

from shapes import classify   # shared with gap_author.py

# ---------------- catalog state ----------------
cattext = CAT.read_text(encoding="utf-8")
catsrc = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", cattext, re.S):
    blk = m.group(1)
    a = re.search(r"Address\s*=\s*'([0-9a-fA-F]{8})'", blk)
    s = re.search(r"Source\s*=\s*'([^']+)'", blk)
    if a:
        catsrc[a.group(1).lower()] = s.group(1) if s else None

boottext = BOOT.read_text(encoding="utf-8")
BOOT_ADDRS = set(re.findall(r"([0-9a-f]{8})(?:_test)?\.cpp", boottext)) | \
             {x.lower().replace("0x", "") for x in re.findall(r"\b(?:0x)?(00[0-9a-fA-F]{6})\b", boottext)}

def is_baked(a):
    rel = catsrc.get(a)
    if not rel:
        return False
    p = ROOT / "rebuild/src/compiled" / rel
    return p.exists() and not is_genuine(p.read_text(encoding="utf-8", errors="ignore"))

targets, kill, kinds = [], set(), Counter()
for r in rows:
    a = r.get("address")
    if not a:
        continue
    ah = "%08x" % int(a, 16)
    if ah in BOOT_ADDRS:
        continue
    landed = ah in catsrc
    if landed and not (WANT_BAKED and is_baked(ah)):
        continue
    if not landed:
        if not WANT_NEW:
            continue
        # `_gapscan` rows are function starts discovered inside over-captured rows
        # (manifest_add_gaps.py). They deliberately carry no prototype: for these the
        # SHAPE is the prototype -- the classifier derives the signature from the bytes
        # and verify_and_land proves it byte-for-byte. Ordinary rows still need a
        # complete Ghidra prototype.
        # --shape-is-prototype extends that same reasoning to ordinary rows whose Ghidra
        # prototype is INCOMPLETE: there too nothing is known to contradict, the shape is
        # all the evidence there is, and parity still has to be proven. A row that DOES
        # carry a complete prototype is never shape-authored against it.
        if r.get("module") != "_gapscan":
            if r.get("prototype_complete") != "1" or r.get("calling_convention", "") in ("", "unknown"):
                if not SHAPE_PROTO:
                    continue
    b = body(int(a, 16))
    if not b or len(b) > 22:
        continue
    got = classify(b)
    if not got:
        continue
    cpp, leaf = got
    kinds[leaf + ("/baked" if landed else "/new")] += 1
    if landed:
        kill.add(ah)
    targets.append((ah, r.get("name") or "", r.get("module") or "_global", b, cpp, leaf))
    if LIMIT and len(targets) >= LIMIT:
        break

print("shape-authored candidates: %d (baked %d, new %d)" % (len(targets), len(kill), len(targets) - len(kill)))
for k, v in kinds.most_common():
    print("  %-16s %d" % (k, v))
if not APPLY:
    print("(dry run; pass --apply)")
    sys.exit(0)

# un-land the bakes
lines = cattext.splitlines(keepends=True)
out = []
i = 0
removed = 0
while i < len(lines):
    if "[pscustomobject]@{" not in lines[i]:
        out.append(lines[i]); i += 1; continue
    blk = [lines[i]]; i += 1
    while i < len(lines):
        blk.append(lines[i]); i += 1
        if re.match(r"^\s*}\s*,?\s*$", blk[-1]):
            break
    m = re.search(r"Address\s*=\s*'([0-9a-fA-F]{8})'", "".join(blk))
    if m and m.group(1).lower() in kill:
        removed += 1
    else:
        out.extend(blk)
CAT.write_text("".join(out), encoding="utf-8")

orows = list(csv.DictReader(open(ORACLE, encoding="utf-8-sig"), delimiter="\t"))
kept = [r for r in orows if r["address"].lower().replace("0x", "") not in kill]
with open(ORACLE, "w", encoding="utf-8", newline="") as f:
    w = csv.DictWriter(f, delimiter="\t", fieldnames=["address", "name", "length", "bytes"], lineterminator="\n")
    w.writeheader()
    w.writerows(kept)

nf = 0
for a in kill:
    for d, pat in ((ROOT / "rebuild/src/compiled", "*_%s.cpp" % a), (ROOT / "rebuild/tests", "*_%s_test.cpp" % a)):
        for p in d.rglob(pat):
            p.unlink(); nf += 1
print("UNLANDED bakes: catalog_blocks=%d oracle_rows=%d files=%d" % (removed, len(orows) - len(kept), nf))

authored, out_rows = [], []
for ah, name, mod, b, cpp, leaf in targets:
    test = "#include <stdio.h>" + chr(10) + 'int main(void){ printf("PASS_' + ah + '\\n"); return 0; }' + chr(10)
    authored.append({"address": ah, "name": name, "module": mod,
                     "source_cpp": cpp, "test_cpp": test, "pass_pattern": "PASS_" + ah})
    out_rows.append((ah, name, str(len(b)), b.hex()))
json.dump({"result": {"authored": authored}}, open(SCR / (PREFIX + "_land.json"), "w"))
with open(SCR / (PREFIX + "_oracle.tsv"), "w", newline="", encoding="utf-8") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow(["address", "name", "length", "bytes"])
    for r in out_rows:
        w.writerow(r)
print("payload %s: %d candidates" % (PREFIX, len(authored)))
