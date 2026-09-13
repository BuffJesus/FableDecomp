#!/usr/bin/env python3
"""Extract NOVI_Bully::GivenTeddy from the installed retail executable."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM


START = 0x00DBCD00
END = 0x00DBCD58


def _offset(pe: pefile.PE, va: int) -> int:
    return pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)


def export(exe: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    code = raw[_offset(pe, START):_offset(pe, START) + END - START]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    insns = list(decoder.disasm(code, START))
    if not insns or insns[-1].address + insns[-1].size != END or insns[-1].mnemonic != "ret":
        raise RuntimeError("retail GivenTeddy boundary no longer decodes exactly")
    decoded = {insn.address: insn for insn in insns}
    expected_bytes = {
        0x00DBCD09: "6a01", 0x00DBCD13: "6858892d01",
        0x00DBCD3D: "c6417001", 0x00DBCD44: "c6829100000001",
        0x00DBCD4E: "6a03",
    }
    for address, expected in expected_bytes.items():
        if decoded[address].bytes.hex() != expected:
            raise RuntimeError(f"retail GivenTeddy bytes changed at 0x{address:08X}")
    for address, slot in ((0x00DBCD0B, 0x1F8), (0x00DBCD2B, 0x1F4)):
        insn = decoded[address]
        op = insn.operands[0]
        if insn.mnemonic != "call" or op.type != X86_OP_MEM or op.mem.disp != slot:
            raise RuntimeError(f"retail GivenTeddy interface slot changed at 0x{address:08X}")
    call = decoded[0x00DBCD50]
    if call.mnemonic != "call" or call.operands[0].type != X86_OP_IMM or call.operands[0].imm != 0x00DAEA70:
        raise RuntimeError("retail GivenTeddy AddBadDeed target changed")
    return {
        "schema": "bully-given-teddy-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "region": {"address": f"0x{START:08X}", "endExclusive": f"0x{END:08X}",
                   "rangeSize": len(code), "bytesSha256": hashlib.sha256(code).hexdigest().upper(),
                   "instructionCount": len(insns)},
        "operations": [
            {"order": 1, "name": "GiveHeroGold", "value": 1,
             "argumentSite": "0x00DBCD09", "callSite": "0x00DBCD0B", "slot": "0x1F8"},
            {"order": 2, "name": "TakeObjectFromHero", "object": "OBJECT_TEDDY_BEAR_UNGIVEABLE",
             "stringPushSite": "0x00DBCD13", "callSite": "0x00DBCD2B", "slot": "0x1F4"},
            {"order": 3, "name": "SpokeAboutFindingTeddy", "value": True,
             "storeSite": "0x00DBCD3D", "parentOffset": "0x70"},
            {"order": 4, "name": "TeddyRuined", "value": True,
             "storeSite": "0x00DBCD44", "parentOffset": "0x91"},
            {"order": 5, "name": "AddBadDeed", "value": 3,
             "argumentSite": "0x00DBCD4E", "callSite": "0x00DBCD50", "target": "0x00DAEA70"},
        ],
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    rendered = json.dumps(export(args.exe), indent=2) + "\n"
    if args.output:
        args.output.write_text(rendered, encoding="utf-8")
    else:
        print(rendered, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
