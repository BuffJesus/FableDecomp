#!/usr/bin/env python3
"""Trim manifest boundary OVER-CAPTURE where the oracle span swallows a trailing
NEXT function after this function's real terminator (ret / unconditional tail-jmp),
with NO int3 padding between (so trim_overcapture.py can't catch it).

Rule: cut the body at the end of the FIRST unconditional terminator instruction
(ret c3 / ret-N c2xx / jmp e9 rel32 / jmp eb rel8) that is NOT the final
instruction of the captured body. Code after an unconditional terminator with no
inbound label is unreachable => it belongs to the next function.

SAFE: a wrong cut only shortens the oracle -> the candidate DIFFERs and is not
landed (verify_and_land compares bytes). It can never cause a false landing.
Conditional jumps (7x / 0f8x) are NOT terminators, so forward-branch bodies
(if/else, loops) are left intact.

Usage:
  trim_tailjmp.py 0x<addr> <hexbytes>            # print trimmed len + bytes
  trim_tailjmp.py --oracle <oracle.tsv>         # rewrite over-captured rows in place
"""
import csv, sys
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

MD = Cs(CS_ARCH_X86, CS_MODE_32)

def trim(body: bytes, addr: int = 0):
    """Return (trimmed_bytes, was_trimmed).

    Cut at the first unconditional terminator (ret / jmp rel) whose end is not
    crossed by any earlier intra-body jump target. Tracking the max jump target
    keeps early-return bodies (a forward `je` over a `ret`) intact: their post-ret
    block is a live branch target, so the ret is not treated as the function end.
    """
    insns = list(MD.disasm(body, addr))
    if not insns:
        return body, False
    end = addr + len(body)
    max_target = addr  # highest address any seen jump can reach
    for ins in insns:
        m = ins.mnemonic
        if m.startswith("j"):  # jcc or jmp -> a rel target within/near the body
            try:
                tgt = int(ins.op_str, 16)
                if tgt > max_target:
                    max_target = tgt
            except ValueError:
                pass  # indirect jmp (e.g. jmp [eax]) -> no static target
        # any unconditional jmp (rel e9/eb OR indirect ff /4) ends a straight-line
        # path; the max_target guard above still protects a mid-body indirect tail
        # that a preceding branch jumps over.
        is_term = m == "ret" or m == "retn" or m == "jmp"
        if is_term:
            t_end = ins.address + len(ins.bytes)
            if t_end >= end:
                return body, False  # first real terminator is the final instr => clean
            if max_target < t_end:   # nothing branches at/after t_end => real fn end
                return body[: t_end - addr], True
            # else: reachable code past this terminator (early-return) -> keep scanning
    return body, False

def main():
    if len(sys.argv) >= 3 and sys.argv[1] == "--oracle":
        pass
    if sys.argv[1] == "--oracle":
        p = Path(sys.argv[2])
        rows = list(csv.DictReader(open(p, encoding="utf-8-sig"), delimiter="\t"))
        n = 0
        for r in rows:
            b = bytes.fromhex(r["bytes"]); a = int(r["address"], 16)
            tb, did = trim(b, a)
            if did:
                r["bytes"] = tb.hex(); r["length"] = str(len(tb)); n += 1
                print(f"TRIM {r['address']} -> {len(tb)} bytes")
        with open(p, "w", encoding="utf-8", newline="") as f:
            w = csv.writer(f, delimiter="\t", lineterminator="\n")
            w.writerow(["address", "name", "length", "bytes"])
            for r in rows:
                w.writerow([r["address"], r["name"], r["length"], r["bytes"]])
        print(f"trimmed {n} row(s) in {p}")
    else:
        addr = int(sys.argv[1], 16); body = bytes.fromhex(sys.argv[2])
        tb, did = trim(body, addr)
        print(f"{'TRIMMED' if did else 'CLEAN'} {len(body)} -> {len(tb)}")
        print(tb.hex())

if __name__ == "__main__":
    main()
