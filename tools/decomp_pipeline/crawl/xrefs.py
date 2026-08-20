#!/usr/bin/env python3
"""Cross-reference index over the retail image: which addresses are actually *entered*?

A function start recovered from an over-captured manifest row (see manifest_gaps.py) is a
hypothesis until something references it. Byte-parity alone cannot confirm it -- a 1-byte
`c3` "empty function" matches trivially whether or not that byte is a real function start.

This builds the set of addresses that are entered from somewhere:
  * direct `call rel32` (0xE8) and `jmp rel32` (0xE9) targets anywhere in .text
  * every dword stored anywhere in the image that lands on a .text address
    (vtable slots, function-pointer tables, thunk tables)

`is_entered(va)` is therefore evidence the address is a real callable entry point.
This is deliberately permissive on the reference side and strict about what it proves:
a hit means "something enters here", not "this is a complete function".
"""
import struct
from pathlib import Path

EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
IMAGE_BASE = 0x400000

_cache = None


def _sections(data):
    e = struct.unpack_from("<I", data, 0x3C)[0]
    coff = e + 4
    nsec = struct.unpack_from("<H", data, coff + 2)[0]
    opt = struct.unpack_from("<H", data, coff + 16)[0]
    base = coff + 20 + opt
    out = []
    for i in range(nsec):
        o = base + i * 40
        name = data[o:o + 8].rstrip(b"\0").decode("latin1")
        vs, = struct.unpack_from("<I", data, o + 8)
        va, = struct.unpack_from("<I", data, o + 12)
        rs, = struct.unpack_from("<I", data, o + 16)
        rp, = struct.unpack_from("<I", data, o + 20)
        out.append((name, va, max(vs, rs), rp, rs))
    return out


def build():
    global _cache
    if _cache is not None:
        return _cache
    data = EXE.read_bytes()
    secs = _sections(data)
    text = [s for s in secs if s[0] == ".text"]
    if not text:
        text = [secs[0]]
    tname, tva, tsz, trp, trs = text[0]
    lo, hi = IMAGE_BASE + tva, IMAGE_BASE + tva + tsz
    entered = set()

    # 1) direct rel32 call/jmp targets inside .text
    blob = data[trp:trp + trs]
    n = len(blob)
    i = 0
    while i < n - 5:
        op = blob[i]
        if op == 0xE8 or op == 0xE9:
            rel, = struct.unpack_from("<i", blob, i + 1)
            tgt = IMAGE_BASE + tva + i + 5 + rel
            if lo <= tgt < hi:
                entered.add(tgt)
            i += 5
            continue
        i += 1

    # 2) every stored dword anywhere in the file that points into .text
    for name, va, vsz, rp, rs in secs:
        if rs == 0:
            continue
        blob = data[rp:rp + rs]
        for o in range(0, len(blob) - 4, 4):
            v, = struct.unpack_from("<I", blob, o)
            if lo <= v < hi:
                entered.add(v)

    _cache = (entered, lo, hi)
    return _cache


def is_entered(va):
    entered, lo, hi = build()
    return va in entered


if __name__ == "__main__":
    entered, lo, hi = build()
    print("addresses entered from somewhere: %d" % len(entered))
    print(".text range: %08x-%08x" % (lo, hi))
