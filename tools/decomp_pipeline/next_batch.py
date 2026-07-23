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
              for p in glob.glob(str(ROOT / "rebuild/src/compiled/*.cpp"))}
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

    picked = []
    for r in man:
        if len(picked) >= count: break
        if not eligible(r): continue
        va = int(r["address"], 16); b = body(va)
        if not b or not (1 <= len(b) <= max_len) or not ends_clean(b): continue
        picked.append((r, b))
    picked.sort(key=lambda x: len(x[1]))

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
