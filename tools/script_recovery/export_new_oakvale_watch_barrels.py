#!/usr/bin/env python3
"""Extract the complete WatchBarrels consequence thread from retail x86 bytes."""
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
    va = int(quest["functions"]["WatchBarrels"]["address"], 0)
    size = 0x287
    code = raw[_offset(pe, va):_offset(pe, va) + size]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, va))
    if not instructions or instructions[-1].address + instructions[-1].size != va + size:
        raise RuntimeError("retail WatchBarrels boundary no longer decodes exactly")
    string_addresses = {0x012D82AC, 0x012D9DB8, 0x012D8298, 0x012D9D98}
    strings = [_cstring(raw, pe, insn.operands[0].imm & 0xFFFFFFFF)
               for insn in instructions if insn.mnemonic == "push"
               and insn.operands[0].type == X86_OP_IMM
               and (insn.operands[0].imm & 0xFFFFFFFF) in string_addresses]
    indirect = [{"site":f"0x{insn.address:08X}", "slot":f"0x{insn.operands[0].mem.disp:X}"}
                for insn in instructions if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_MEM]
    direct = [{"site":f"0x{insn.address:08X}", "target":f"0x{insn.operands[0].imm:08X}"}
              for insn in instructions if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_IMM
              and insn.operands[0].imm in {0x00CB7940, 0x00DAEA70}]
    expected_strings = ["NOVI_Barrel", "NOVI_Barrel", "NOVI_Barrel", "OBJECT_GOLD_1",
                        "NOVI_CreatedBeetle", "CREATURE_OAKVALE_STAG_BEETLE"]
    expected_slots = ["0x12C", "0x1C", "0x12C", "0x0", "0x120", "0x924", "0x16C", "0x428", "0x1C", "0x0"]
    if strings != expected_strings or [row["slot"] for row in indirect] != expected_slots:
        raise RuntimeError("retail WatchBarrels instruction shape changed")
    expected_fields = {
        "attackOver":{"offset":"0x50", "readSites":["0x00DBE973"]},
        "barrelBrokenInstantaneous":{"offset":"0x74", "writeSites":["0x00DBE960", "0x00DBE98D"],
                                     "readSites":["0x00DBE97E"]},
        "barrelBrokenPos":{"offset":"0x76", "addressSite":"0x00DBEA77"},
    }
    for field in expected_fields.values():
        sites = field.get("readSites", []) + field.get("writeSites", []) + ([field["addressSite"]] if "addressSite" in field else [])
        if not all(any(f"0x{insn.address:08X}" == site and op.type == X86_OP_MEM
                       and f"0x{op.mem.disp:X}" == field["offset"]
                       for insn in instructions for op in insn.operands) for site in sites):
            raise RuntimeError(f"retail WatchBarrels field access changed at {field['offset']}")
    return {
        "schema":"new-oakvale-watch-barrels-retail-bytes/0.1",
        "source":"installed retail Fable.exe PE image",
        "sourceSha256":hashlib.sha256(raw).hexdigest().upper(),
        "address":f"0x{va:08X}", "rangeSize":size,
        "functionBytesSha256":hashlib.sha256(code).hexdigest().upper(),
        "instructionCount":len(instructions), "strings":strings,
        "indirectCalls":indirect, "keyDirectCalls":direct, "fields":expected_fields,
        "firstBreakBadDeed":{"counter":1, "enum":0, "callSite":"0x00DBE997", "target":"0x00DAEA70"},
        "goldBreakIndex":"total-1", "beetleThreshold":"broken>total-4",
        "createdBeetleMaxHealth":2.0, "createdBeetleHealthSetAbsolute":True,
        "terminationCalls":4,
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
