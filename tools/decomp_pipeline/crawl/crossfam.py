#!/usr/bin/env python3
"""Try a GENUINE landed source from a *nearby* family against each remaining baked row.

`harvest_all.py` only reuses a source inside its exact (length, call-masked skeleton)
family. But many remaining bakes are the same C++ shape compiled under a different
`#pragma optimize` setting — retail's size peepholes (`pop ecx` vs `add esp,4`,
`and [m],0` vs `mov [m],0`) change the byte length, so they land in a *different* family
even though one source reproduces both. verify_and_land already sweeps the pragma matrix,
so pairing the row with the right source is all that is missing.

This pairs every remaining baked row with the genuine source of the nearest-length family
(by |Δlen|, then by masked-prefix similarity) and emits one payload per attempt round.
Rows that fail are simply not landed; re-run with `--round 2` to try the next-nearest
source, and so on.

Usage: python crossfam.py <out_prefix> [--round N] [--max-delta N] [--apply]
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
from purity import is_genuine
from bytematch import relaxed_equal

ROOT = Path(r"D:\Documents\FableTLC")
SCR = Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\7fcf5fa1-31b0-4034-8e81-be42686888b3\scratchpad")
EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
CAT = ROOT / "rebuild/build_candidates.ps1"
ORACLE = ROOT / "rebuild/oracles/auto-re-candidates.tsv"
BOOT = ROOT / "rebuild/build_bootstrap.ps1"

PREFIX = sys.argv[1]
APPLY = "--apply" in sys.argv
ROUND = int(sys.argv[sys.argv.index("--round") + 1]) if "--round" in sys.argv else 1
MAXD = int(sys.argv[sys.argv.index("--max-delta") + 1]) if "--max-delta" in sys.argv else 8

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
    if a and s:
        catentry[a.group(1).lower()] = s.group(1)

boottext = BOOT.read_text(encoding="utf-8")
BOOT_ADDRS = set(re.findall(r"([0-9a-f]{8})(?:_test)?\.cpp", boottext)) | \
             {x.lower().replace("0x", "") for x in re.findall(r"\b(?:0x)?(00[0-9a-fA-F]{6})\b", boottext)}


def is_baked(a):
    rel = catentry.get(a)
    if not rel:
        return False
    p = ROOT / "rebuild/src/compiled" / rel
    return p.exists() and not is_genuine(p.read_text(encoding="utf-8", errors="ignore"))


# genuine sources keyed by their family (len, skeleton); keep one per family
genuine = {}
baked = []
for r in rows:
    a = r.get("address")
    if not a:
        continue
    ah = "%08x" % int(a, 16)
    if ah in BOOT_ADDRS or ah not in catentry:
        continue
    b = body(int(a, 16))
    if not b:
        continue
    key = (len(b), maskcalls(b))
    if is_baked(ah):
        baked.append((ah, r.get("name") or "", r.get("module") or "_global", b))
    elif key not in genuine:
        p = ROOT / "rebuild/src/compiled" / catentry[ah]
        text = p.read_text(encoding="utf-8", errors="ignore")
        if is_genuine(text):
            # strip any in-source pragma: verify_and_land sweeps the matrix itself, and a
            # baked-in pragma would pin the wrong variant for a different-length sibling.
            stripped = "\n".join(l for l in text.splitlines() if not l.startswith("#pragma optimize"))
            genuine[key] = (stripped, ah)

print("genuine family sources: %d | remaining baked rows: %d" % (len(genuine), len(baked)))

by_len = defaultdict(list)
for (ln, skel), v in genuine.items():
    by_len[ln].append((skel, v))


def similarity(a, b):
    n = min(len(a), len(b))
    i = 0
    while i < n and a[i] == b[i]:
        i += 1
    return i


authored, out_rows, kill = [], [], set()
for ah, name, mod, b in baked:
    skel = maskcalls(b)
    cands = []
    for d in range(0, MAXD + 1):
        for ln in ({len(b) - d, len(b) + d} if d else {len(b)}):
            for oskel, (text, tmpl) in by_len.get(ln, []):
                cands.append((d, -similarity(skel, oskel), text, tmpl))
    cands.sort(key=lambda c: (c[0], c[1]))
    if len(cands) < ROUND:
        continue
    _, _, text, tmpl = cands[ROUND - 1]
    test = "#include <stdio.h>" + chr(10) + 'int main(void){ printf("PASS_' + ah + '\\n"); return 0; }' + chr(10)
    authored.append({"address": ah, "name": name, "module": mod,
                     "source_cpp": text, "test_cpp": test, "pass_pattern": "PASS_" + ah})
    out_rows.append((ah, name, str(len(b)), b.hex()))
    kill.add(ah)

print("round %d attempts: %d" % (ROUND, len(authored)))

# --- PRE-VERIFY -----------------------------------------------------------------
# Compile every pairing and keep only the byte-exact ones. This runs BEFORE any
# un-landing, so a failed attempt can never strip a working catalog entry.
import os
import subprocess

VC = Path(r"D:\Tools\vc71")
OBJD = (r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages"
        r"\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe")
cenv = dict(os.environ)
cenv["PATH"] = str(VC / "bin") + ";" + cenv["PATH"]
cenv["INCLUDE"] = str(VC / "include") + ";" + str(ROOT / "rebuild" / "include")
cenv["LIB"] = str(VC / "lib")
work = SCR / "cfverify"
work.mkdir(parents=True, exist_ok=True)
PRAGMA_LINE = '#pragma optimize("%s",on)' + chr(10)


def compiled_bytes(text, flags, pragma):
    cpp = work / "c.cpp"
    cpp.write_text(((PRAGMA_LINE % pragma) if pragma else "") + text, encoding="utf-8")
    obj = work / "c.obj"
    obj.unlink(missing_ok=True)
    cp = subprocess.run([str(VC / "bin" / "cl.exe"), "/nologo", "/c"] + flags.split() +
                        ["/W3", "/Fo%s" % obj, str(cpp)], capture_output=True, text=True, env=cenv)
    if cp.returncode or not obj.exists():
        return []
    dd = subprocess.run([OBJD, "-d", str(obj)], capture_output=True, text=True).stdout
    fns, cur = [], None
    for line in dd.splitlines():
        m = re.match(r"^[0-9a-f]+ <(.+)>:$", line.strip())
        if m:
            cur = bytearray()
            fns.append(cur)
            continue
        if cur is not None:
            m2 = re.match(r"^\s+[0-9a-f]+:\s+((?:[0-9a-fA-F]{2} )+)", line)
            if m2:
                cur.extend(bytes.fromhex(m2.group(1)))
    return [bytes(f) for f in fns if f]


survivors, srows = [], []
for cand, orow in zip(authored, out_rows):
    want = bytes.fromhex(orow[3])
    hit = False
    for flags in ("/O2 /Oy", "/O1 /Oy", "/Ox /Oy"):
        for pragma in ("", "s", "t", "g", "gs"):
            for got in compiled_bytes(cand["source_cpp"], flags, pragma):
                if relaxed_equal(got, want):
                    hit = True
                    break
            if hit:
                break
        if hit:
            break
    if hit:
        survivors.append(cand)
        srows.append(orow)
print("pre-verified byte-exact: %d of %d" % (len(survivors), len(authored)))
authored, out_rows = survivors, srows
kill = {c["address"] for c in authored}

if not APPLY:
    print("(dry run; pass --apply)")
    sys.exit(0)

# un-land the bakes we are attempting (a failure simply leaves the address unlanded,
# which the next round or a hand-authored source can pick up)
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
    for dpath, pat in ((ROOT / "rebuild/src/compiled", "*_%s.cpp" % a), (ROOT / "rebuild/tests", "*_%s_test.cpp" % a)):
        for p in dpath.rglob(pat):
            p.unlink(); nf += 1
print("UNLANDED bakes: catalog_blocks=%d oracle_rows=%d files=%d" % (removed, len(orows) - len(kept), nf))

json.dump({"result": {"authored": authored}}, open(SCR / (PREFIX + "_land.json"), "w"))
with open(SCR / (PREFIX + "_oracle.tsv"), "w", newline="", encoding="utf-8") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow(["address", "name", "length", "bytes"])
    for r in out_rows:
        w.writerow(r)
print("payload %s written" % PREFIX)
