#!/usr/bin/env python3
"""Extract ManageQuestCoreMarkers directly from retail x86 bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM


def _offset(pe: pefile.PE, va: int) -> int:
    return pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)


def _cstring(raw: bytes, pe: pefile.PE, va: int) -> str:
    start = _offset(pe, va)
    return raw[start:raw.index(b"\0", start)].decode("ascii")


def export(exe: Path, quest_path: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    quest = json.loads(quest_path.read_text(encoding="utf-8-sig"))
    va = int(quest["functions"]["ManageQuestCoreMarkers"]["address"], 0)
    size = 0x3B0
    code = raw[_offset(pe, va):_offset(pe, va) + size]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, va))
    if not instructions or instructions[-1].address + instructions[-1].size != va + size:
        raise RuntimeError("retail ManageQuestCoreMarkers boundary no longer decodes exactly")
    string_addresses = {0x012D8378, 0x012D82D8, 0x012D8360, 0x01258AB0}
    strings = [_cstring(raw, pe, insn.operands[0].imm & 0xFFFFFFFF)
               for insn in instructions if insn.mnemonic == "push" and insn.operands[0].type == X86_OP_IMM
               and (insn.operands[0].imm & 0xFFFFFFFF) in string_addresses]
    indirect = [{"site":f"0x{insn.address:08X}", "slot":f"0x{insn.operands[0].mem.disp:X}"}
                for insn in instructions if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_MEM]
    terminations = [f"0x{insn.address:08X}" for insn in instructions if insn.mnemonic == "call"
                    and insn.operands[0].type == X86_OP_IMM and insn.operands[0].imm == 0x00CB7940]
    expected_strings = ["NOVI_LiveFather", "NOVI_BookTrader", "NOVI_Theresa", "HUD_ORB_QUEST_CORE",
                        "HUD_ORB_QUEST_CORE", "HUD_ORB_QUEST_CORE", "HUD_ORB_QUEST_CORE"]
    expected_slots = ["0x120", "0x120", "0x120", "0x578", "0x1FC", "0x1C", "0x1FC", "0x628",
                      "0x1C", "0x628", "0x580", "0x578", "0x1D8", "0xA4", "0x1C", "0xA4", "0x1C",
                      "0x580", "0x578", "0x1C", "0x580", "0x578", "0x4", "0x4", "0x4"]
    if strings != expected_strings or [row["slot"] for row in indirect] != expected_slots:
        raise RuntimeError("retail ManageQuestCoreMarkers instruction shape changed")
    field_reads = {"GivenSweets":{"offset":"0x94", "sites":["0x00DBE6CC", "0x00DBE6EF"]},
                   "GivenTheresaChocs":{"offset":"0x95", "sites":["0x00DBE744", "0x00DBE763"]}}
    for field in field_reads.values():
        if not all(any(insn.address == int(site, 0) and any(op.type == X86_OP_MEM and op.mem.disp == int(field["offset"], 0)
                                                        for op in insn.operands) for insn in instructions)
                   for site in field["sites"]):
            raise RuntimeError(f"retail marker field access changed at {field['offset']}")
    return {
        "schema":"new-oakvale-manage-core-markers-retail-bytes/0.1",
        "source":"installed retail Fable.exe PE image",
        "sourceSha256":hashlib.sha256(raw).hexdigest().upper(), "address":f"0x{va:08X}",
        "rangeSize":size, "functionBytesSha256":hashlib.sha256(code).hexdigest().upper(),
        "instructionCount":len(instructions), "strings":strings, "indirectCalls":indirect,
        "terminationCalls":terminations, "goldMinimum":3, "tutorialId":19,
        "fieldReads":field_reads,
        "markerSequence":["add:father", "remove:father", "add:bookTrader", "remove:bookTrader",
                          "add:theresa", "remove:theresa", "add:father"],
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--quest", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = export(args.exe, args.quest)
    text = json.dumps(result, indent=2) + "\n"
    if args.output: args.output.write_text(text, encoding="utf-8")
    else: print(text, end="")
    return 0


if __name__ == "__main__": raise SystemExit(main())
