#!/usr/bin/env python3
"""Self-sustaining candidate selector for the byte-match promotion loop.
Picks the next N untried high-yield functions from the manifest, mints their
retail oracle bytes straight from Fable.exe (Ghidra-free, via the pe_oracle
next-address length heuristic), applies a confidence filter, and writes a
staged oracle + targets pair under rebuild/oracles/pending/.

Usage: python next_batch.py <batch_name> <count> [max_len=96]
Prints the selected address list (JSON) to stdout for direct Workflow args.

Excludes anything already landed (src/compiled), already oracled
(auto-re-candidates.tsv), or already staged in a pending batch. Confidence
filter: keep only bodies that end in a real return/tail idiom (ret / ret n /
jmp), which rejects most pe_oracle length over-estimations up front. The
authoring self-verify still gates every row, so a stray over-long row can only
waste effort, never cause a bad land.
"""
import csv, json, struct, sys, glob, os
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
IMAGE_BASE = 0x400000
ACCESSOR_RET = ("bool", "char", "int", "long", "unsigned", "short", "float", "void", "*")

def tsv(p):
    with open(p, encoding="utf-8-sig", newline="") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def pe_sections(data):
    e = struct.unpack_from("<I", data, 0x3C)[0]
    coff = e + 4
    nsec = struct.unpack_from("<H", data, coff + 2)[0]
    opt = struct.unpack_from("<H", data, coff + 16)[0]
    sec = coff + 20 + opt
    out = []
    for i in range(nsec):
        off = sec + i * 40
        vsize, va, rawsize, rawptr = struct.unpack_from("<IIII", data, off + 8)
        out.append((va, vsize, rawptr, rawsize))
    return out

def va_to_off(secs, va):
    rva = va - IMAGE_BASE
    for sva, vsize, rawptr, rawsize in secs:
        if sva <= rva < sva + max(vsize, rawsize):
            return rawptr + (rva - sva)
    return None

def ends_clean(b):
    if not b: return False
    if b[-1] == 0xC3: return True                       # ret
    if len(b) >= 3 and b[-3] == 0xC2: return True       # ret imm16
    if b[-1] in (0xE9, 0xEB): return True               # tail jmp (best-effort)
    if len(b) >= 5 and b[-5] == 0xE9: return True        # jmp rel32
    return False

import subprocess, tempfile
OBJDUMP = r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe"
import re as _re
_ILINE = _re.compile(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2} )+)\s*(\S+)")
try:
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    _CAPSTONE = Cs(CS_ARCH_X86, CS_MODE_32)
except ImportError:
    _CAPSTONE = None
# byte patterns that begin a NEW function (MSVC prologues / thunks)
def _is_prologue(b, p):
    if p >= len(b): return False
    x = b[p]
    if x in (0x55, 0x56, 0x57, 0x53): return True          # push ebp/esi/edi/ebx
    if x in (0xC3, 0xC2): return True                      # adjacent empty/retN tiny function
    if x == 0x6A: return True                               # push imm8 (e.g. push -1 SEH, or push size)
    if x == 0xB8: return True                               # mov eax,imm32 (const-return fn)
    if p + 1 < len(b):
        pair = b[p:p+2]
        if pair in (b"\x8b\xff", b"\x83\xec", b"\x81\xec", b"\xc7\x01", b"\x8b\xf1", b"\x8b\xc1"): return True
    return False

def _disasm(seg):
    # Capstone is already part of the RE environment and avoids launching one
    # objdump process per candidate (roughly 90 seconds for a 500-row batch).
    # Keep objdump as a portability fallback for minimal installations.
    if _CAPSTONE is not None:
        return [(ins.address, ins.size, ins.mnemonic) for ins in _CAPSTONE.disasm(seg, 0)]
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as t:
        t.write(seg); n = t.name
    try:
        out = subprocess.run([OBJDUMP, "-D", "-b", "binary", "-m", "i386", "-M", "intel", n],
                             capture_output=True, text=True).stdout
    finally:
        os.unlink(n)
    ins = []
    for l in out.splitlines():
        m = _ILINE.match(l)
        if m:
            off = int(m.group(1), 16); nb = len(m.group(2).split()); ins.append((off, nb, m.group(3)))
    return ins

def first_function_len(seg):
    """Length of the FIRST function in seg. Splits merged pe_oracle rows at a
    ret -> (padding) -> prologue boundary. Returns len(seg) if no split found."""
    ins = _disasm(seg)
    for off, nb, mn in ins:
        if mn.startswith("ret"):
            end = off + nb
            p = end
            while p < len(seg) and seg[p] in (0xCC, 0x90): p += 1
            if p >= len(seg): return end          # only trailing padding -> this ret ends it
            if _is_prologue(seg, p): return end    # a new function starts -> split here
            # else: early return inside one function; keep scanning
    return len(seg)

def split_all(body):
    pieces = []; i = 0
    guard = 0
    while i < len(body) and guard < 32:
        guard += 1
        seg = body[i:]
        L = first_function_len(seg)
        if L <= 0 or L >= len(seg):
            pieces.append((i, body[i:])); break
        pieces.append((i, body[i:i + L])); i += L
        while i < len(body) and body[i] in (0xCC, 0x90): i += 1
    return pieces

def main():
    batch = sys.argv[1]; count = int(sys.argv[2])
    max_len = int(sys.argv[3]) if len(sys.argv) > 3 else 96
    data = EXE.read_bytes(); secs = pe_sections(data)
    man = tsv(ROOT / "rebuild/manifest/functions.tsv")
    addrs = sorted(int(r["address"], 16) for r in man if r.get("address"))
    nextof = {addrs[i]: addrs[i + 1] for i in range(len(addrs) - 1)}

    def body(va):
        off = va_to_off(secs, va); nxt = nextof.get(va)
        if off is None or nxt is None: return None
        raw = data[off:off + (nxt - va)]
        end = len(raw)
        while end > 0 and raw[end - 1] in (0xCC, 0x90): end -= 1
        return raw[:end]

    landed = {os.path.basename(p).split("_")[-1].replace(".cpp", "").lower()
              for p in glob.glob(str(ROOT / "rebuild/src/compiled/**/*.cpp"), recursive=True)}
    have = {r["address"].lower() for r in tsv(ROOT / "rebuild/oracles/auto-re-candidates.tsv")}
    pend = set()
    for p in glob.glob(str(ROOT / "rebuild/oracles/pending/*_oracle.tsv")):
        pend |= {r["address"].lower() for r in tsv(p)}
    excl = landed | have | pend

    def eligible(r):
        a = r["address"].lower()
        if a in excl: return False
        pc = r.get("prototype_complete", "").strip().lower()
        if pc in ("", "0", "no"): return False
        if r.get("calling_convention", "").strip() in ("", "unknown"): return False
        rt = r.get("return_type", "").strip().lower()
        if rt in ("", "undefined"): return False
        return any(k in rt for k in ACCESSOR_RET)

    picked = []; trimmed = 0
    seen_addr = set(excl)
    for r in man:
        if len(picked) >= count: break
        if not eligible(r): continue
        va = int(r["address"], 16); b = body(va)
        if not b: continue
        parts = split_all(b)
        first = parts[0][1]
        if len(parts) > 1: trimmed += 1
        if not (1 <= len(first) <= max_len) or not ends_clean(first): continue
        picked.append((r, first)); seen_addr.add(r["address"].lower())
    picked.sort(key=lambda x: len(x[1]))
    if trimmed:
        print(
            f"[next_batch] trimmed {trimmed} merged (over-length) manifest rows; "
            "non-manifest tails were not promoted",
            file=sys.stderr,
        )

    pending = ROOT / "rebuild/oracles/pending"
    with open(pending / f"{batch}_oracle.tsv", "w", encoding="utf-8", newline="") as f:
        w = csv.DictWriter(f, delimiter="\t", fieldnames=["address", "name", "length", "bytes"], lineterminator="\n")
        w.writeheader()
        for r, b in picked:
            w.writerow({"address": r["address"].lower(), "name": r["name"], "length": len(b), "bytes": b.hex()})
    tgt = [{"address": r["address"].lower(), "name": r["name"], "module": r.get("module", "_global"),
            "cc": r.get("calling_convention", "__fastcall"), "ret": r.get("return_type", "?"),
            "pc": r.get("parameter_count", "0")} for r, _ in picked]
    json.dump(tgt, open(pending / f"{batch}_targets.json", "w"))
    print(f"[next_batch] {batch}: selected {len(picked)} (max_len={max_len}) -> rebuild/oracles/pending/{batch}_oracle.tsv", file=sys.stderr)
    print(json.dumps([r["address"].lower() for r, _ in picked]))

if __name__ == "__main__":
    main()
