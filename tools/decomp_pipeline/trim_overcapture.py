"""trim_overcapture.py — trim manifest boundary OVER-CAPTURES in a crawl oracle.

A manifest function's byte span is taken as [addr, next_manifest_addr). When the
real next function ISN'T in the manifest, that span swallows the inter-function
`0xCC` (int3) alignment padding AND the head of the following function, so the
oracle row is longer than the real body and can never reach byte-parity
(e.g. 00a14e20 CGameEventPackage::Clear = real 8B `mov [ecx+0x20],0x320; ret`
vs a captured 19B). The harness's trailing-CC strip only removes padding at the
very END, not INTERIOR padding, so these are wrongly deferred.

The real boundary is the first standalone `int3` that follows the function's
terminating instruction (ret / ret imm / tail jmp). This tool disassembles the
captured bytes linearly, finds that first int3, verifies the byte before it ends
a control-flow (ret/jmp), and emits the trimmed length + bytes.

Usage:
  # print trimmed length + hex for addresses (read the retail exe):
  python tools/decomp_pipeline/trim_overcapture.py 0xa14e20 0x00c8c730
  # rewrite matching rows of an oracle TSV in place (address,name,length,bytes):
  python tools/decomp_pipeline/trim_overcapture.py --oracle path/to/oracle.tsv

Trim is applied ONLY when a real over-capture is detected (int3 run after a
terminating instruction with more bytes past it). Clean rows are left untouched,
so it is safe to run over a whole oracle.
"""
import sys
import struct
import subprocess
import tempfile
import os
import csv
import re

EXE = r"C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/Fable.exe"
OBJDUMP = (r"C:/Users/Cornelio/AppData/Local/Microsoft/WinGet/Packages/"
           r"BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe/"
           r"mingw64/bin/objdump.exe")

_data = None
_secs = None
_nxt = None
IMG = 0x400000


def _load():
    global _data, _secs, _nxt
    if _data is not None:
        return
    _data = open(EXE, "rb").read()
    e = struct.unpack_from("<I", _data, 0x3C)[0]
    coff = e + 4
    nsec = struct.unpack_from("<H", _data, coff + 2)[0]
    opt = struct.unpack_from("<H", _data, coff + 16)[0]
    sb = coff + 20 + opt
    _secs = []
    for i in range(nsec):
        o = sb + i * 40
        va = struct.unpack_from("<I", _data, o + 12)[0]
        rp = struct.unpack_from("<I", _data, o + 20)[0]
        rs = struct.unpack_from("<I", _data, o + 16)[0]
        vs = struct.unpack_from("<I", _data, o + 8)[0]
        _secs.append((va, max(vs, rs), rp))
    import csv as _csv
    rows = [int(r["address"], 16) for r in _csv.DictReader(
        open("rebuild/manifest/functions.tsv", encoding="utf-8-sig"), delimiter="\t")
        if r.get("address")]
    rows = sorted(set(rows))
    _nxt = {rows[i]: rows[i + 1] for i in range(len(rows) - 1)}


def _off(va):
    r = va - IMG
    for va0, sz, rp in _secs:
        if va0 <= r < va0 + sz:
            return rp + (r - va0)
    return None


# objdump line: "  <off>:\t<hex bytes>\t<mnemonic ...>"
DL = re.compile(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2}\s)+)\s*(\S.*)?$")


def _disasm(b, base):
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as f:
        f.write(b)
        tmp = f.name
    try:
        r = subprocess.run(
            [OBJDUMP, "-D", "-b", "binary", "-m", "i386", "-M", "intel",
             f"--adjust-vma=0x{base:x}", tmp],
            capture_output=True, text=True)
    finally:
        os.unlink(tmp)
    out = []
    for line in r.stdout.splitlines():
        m = DL.match(line)
        if not m:
            continue
        off = int(m.group(1), 16) - base
        size = len(m.group(2).split())
        mnem = (m.group(3) or "").strip()
        out.append((off, size, mnem))
    return out


def _is_terminator(mnem):
    return mnem.startswith("ret") or mnem.startswith("jmp")


def trim_length(addr, captured=None):
    """Return (trimmed_len, trimmed_bytes, was_overcapture) for a function VA."""
    _load()
    o = _off(addr)
    if o is None:
        return None, None, False
    if captured is None:
        end = _nxt.get(addr, addr + 400)
        captured = _data[o:o + (end - addr)]
    ins = _disasm(captured, addr)
    # find the first standalone int3 whose preceding instruction is a terminator
    for i, (off, size, mnem) in enumerate(ins):
        if mnem.startswith("int3"):
            if i == 0:
                continue
            prev = ins[i - 1]
            if _is_terminator(prev[2]):
                real = off
                if real < len(captured):
                    return real, bytes(captured[:real]), True
    # no interior int3 boundary: strip only trailing cc/90 (harness-compatible)
    b = bytearray(captured)
    while b and b[-1] in (0xCC, 0x90):
        b = b[:-1]
    return len(b), bytes(b), False


def main():
    if len(sys.argv) >= 3 and sys.argv[1] == "--oracle":
        path = sys.argv[2]
        rows = list(csv.DictReader(open(path, encoding="utf-8-sig"), delimiter="\t"))
        fields = rows[0].keys() if rows else ["address", "name", "length", "bytes"]
        changed = 0
        for row in rows:
            a = int(row["address"], 16)
            rl, rb, over = trim_length(a)
            if over and rb is not None and rb.hex() != row.get("bytes", ""):
                row["length"] = str(rl)
                row["bytes"] = rb.hex()
                changed += 1
                print(f"TRIM {a:08x} -> {rl} bytes")
        with open(path, "w", newline="", encoding="utf-8") as f:
            w = csv.DictWriter(f, fieldnames=list(fields), delimiter="\t",
                               lineterminator="\n")
            w.writeheader()
            w.writerows(rows)
        print(f"trimmed {changed} over-captured rows in {path}")
        return
    for arg in sys.argv[1:]:
        a = int(arg, 16)
        rl, rb, over = trim_length(a)
        tag = "OVER-CAPTURE" if over else "clean"
        print(f"{a:08x} {tag} len={rl}")
        if over:
            print(f"  bytes={rb.hex()}")


if __name__ == "__main__":
    main()
