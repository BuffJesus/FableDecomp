#!/usr/bin/env python3
"""Trim an OVER-CAPTURED manifest row down to its first real function.

A manifest function's oracle span is [addr, next_manifest_addr). When the next real
function is missing from the manifest, the span swallows it whole (VC7.1 packs tiny
functions with no `0xCC` padding between them), so the row is longer than the body it
names and can never reach parity. `trim_overcapture.py` handles the padded case (cut at
a standalone int3 after a terminator); this handles the UNPADDED case by control-flow
analysis:

  decode the row; track the furthest forward branch target; the function ends at the
  first `ret` / `ret imm16` / unconditional `jmp` that no branch inside the row jumps
  past. Anything after that belongs to a different, unlisted function.

Only cuts when the decode is clean and something remains; otherwise returns the row
unchanged, so callers can use `true_body()` unconditionally.

CLI: python rowtrim.py <addr_hex> [...]   -> report the trim decision for each address
"""
import struct
import sys
from pathlib import Path

import capstone

EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")

_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
_MD.detail = True
_TERMINATORS = ("ret", "retf", "jmp")


def trim_body(body, va=0x400000):
    """-> (trimmed_bytes, cut_offset_or_None). cut_offset is where the extra fn starts."""
    if not body:
        return body, None
    reach = 0          # furthest byte offset any branch inside the row targets
    consumed = 0
    for insn in _MD.disasm(body, va):
        end = insn.address - va + insn.size
        consumed = end
        if insn.group(capstone.x86.X86_GRP_JUMP):
            op = insn.operands[0] if insn.operands else None
            if op is not None and op.type == capstone.x86.X86_OP_IMM:
                target = op.imm - va
                # only targets INSIDE the row keep the function alive past this point;
                # a branch out of the row is a tail call, not internal control flow.
                if 0 <= target < len(body) and target > reach:
                    reach = target
        if insn.mnemonic in _TERMINATORS and end > reach and end < len(body):
            return body[:end], end
    if consumed != len(body):
        return body, None      # decode did not cover the row -> do not touch it
    return body, None


def _image():
    data = EXE.read_bytes()
    e = struct.unpack_from("<I", data, 0x3C)[0]
    coff = e + 4
    nsec = struct.unpack_from("<H", data, coff + 2)[0]
    opt = struct.unpack_from("<H", data, coff + 16)[0]
    base = coff + 20 + opt
    secs = []
    for i in range(nsec):
        o = base + i * 40
        va, = struct.unpack_from("<I", data, o + 12)
        rp, = struct.unpack_from("<I", data, o + 20)
        rs, = struct.unpack_from("<I", data, o + 16)
        vs, = struct.unpack_from("<I", data, o + 8)
        secs.append((va, max(vs, rs), rp))
    return data, secs


if __name__ == "__main__":
    import csv
    data, secs = _image()

    def off(va):
        rva = va - 0x400000
        for va0, sz, rp in secs:
            if va0 <= rva < va0 + sz:
                return rp + (rva - va0)

    rows = list(csv.DictReader(open(Path(r"D:\Documents\FableTLC\rebuild\manifest\functions.tsv"),
                                    encoding="utf-8-sig"), delimiter="\t"))
    addrs = sorted(int(r["address"], 16) for r in rows if r.get("address"))
    nextof = {addrs[i]: addrs[i + 1] for i in range(len(addrs) - 1)}
    for a in sys.argv[1:]:
        va = int(a, 16)
        o = off(va)
        raw = data[o:o + (nextof[va] - va)]
        ee = len(raw)
        while ee > 0 and raw[ee - 1] in (0xCC, 0x90):
            ee -= 1
        body = raw[:ee]
        trimmed, cut = trim_body(body, va)
        if cut is None:
            print("%08x  len=%d  (no trim)  %s" % (va, len(body), body.hex()))
        else:
            print("%08x  len=%d -> %d  extra starts at +0x%x  %s | %s"
                  % (va, len(body), len(trimmed), cut, trimmed.hex(), body[cut:].hex()))
