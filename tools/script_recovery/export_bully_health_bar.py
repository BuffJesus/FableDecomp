#!/usr/bin/env python3
"""Extract Bully quest-info-bar creation, update, and removal regions from retail bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_MEM


REGIONS = ((0x00DBC391, 0x00DBC588, "createAndUpdate"),
           (0x00DBC86B, 0x00DBC891, "subduedRemoval"))
EXPECTED_BYTES = {
    0x00DBC40C: "c684248a000000ff", 0x00DBC414: "c684248900000000",
    0x00DBC41C: "c684248800000000", 0x00DBC424: "c684248b000000ff",
    0x00DBC42C: "c644246600", 0x00DBC431: "c6442465ff",
    0x00DBC436: "c644246400", 0x00DBC43B: "c6442467ff",
}


def _offset(pe: pefile.PE, va: int) -> int:
    return pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)


def export(exe: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    decoded = {}
    regions = []
    for start, end, name in REGIONS:
        code = raw[_offset(pe, start):_offset(pe, start) + end - start]
        insns = list(decoder.disasm(code, start))
        if not insns or insns[-1].address + insns[-1].size != end:
            raise RuntimeError(f"retail Bully {name} boundary no longer decodes exactly")
        decoded.update({insn.address: insn for insn in insns})
        regions.append({"name": name, "address": f"0x{start:08X}", "rangeSize": end-start,
                        "bytesSha256": hashlib.sha256(code).hexdigest().upper(),
                        "instructionCount": len(insns)})
    for address, expected in EXPECTED_BYTES.items():
        if decoded[address].bytes.hex() != expected:
            raise RuntimeError(f"retail Bully color construction changed at 0x{address:08X}")
    expected_slots = {0x00DBC446: 0x510, 0x00DBC582: 0x530, 0x00DBC88B: 0x548}
    for address, slot in expected_slots.items():
        op = decoded[address].operands[0]
        if decoded[address].mnemonic != "call" or op.type != X86_OP_MEM or op.mem.disp != slot:
            raise RuntimeError(f"retail Bully HUD slot changed at 0x{address:08X}")
    return {
        "schema": "bully-health-bar-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(), "regions": regions,
        "creation": {"unsetSentinel": -999, "unsetCompareSite": "0x00DBC3A3",
                     "current": "float(InitialHealth)", "maximum": 0.0,
                     "primaryColourBytes": "00 FF 00 FF", "primaryColour": "opaque green",
                     "secondaryColourBytes": "00 00 FF FF", "secondaryColour": "opaque red (BGRA memory order; disasm 0x00DBC40C-0x00DBC424 writes [+0x88]=00 [+0x89]=00 [+0x8a]=FF [+0x8b]=FF)",
                     "texture": "HUD_QUEST_ICON_GRANDSON", "scale": 1.0,
                     "callSite": "0x00DBC446", "slot": "0x510",
                     "handleStoreSite": "0x00DBC44F", "handleParentOffset": "0x64"},
        "hitState": {"hitsIncrementSite": "0x00DBC49C", "hitsStoreSite": "0x00DBC4A1",
                     "subduedBranchSite": "0x00DBC4A6", "subduedTarget": "0x00DBC86B"},
        "update": {"current": "float(InitialHealth - HitsTaken)", "maximum": -1.0,
                   "scale": -1.0, "callSite": "0x00DBC582", "slot": "0x530"},
        "removal": {"bullySubduedStoreSite": "0x00DBC87B", "bullySubduedParentOffset": "0x6C",
                    "handleLoadSite": "0x00DBC885", "handleParentOffset": "0x64",
                    "callSite": "0x00DBC88B", "slot": "0x548"},
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = export(args.exe)
    text = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(text, encoding="utf-8")
    else:
        print(text, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
