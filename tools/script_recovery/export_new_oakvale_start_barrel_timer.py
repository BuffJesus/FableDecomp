#!/usr/bin/env python3
"""Extract the complete StartBarrelTimer thread from retail x86 bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
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
    va = int(quest["functions"]["StartBarrelTimer"]["address"], 0)
    size = 562
    code = raw[_offset(pe, va):_offset(pe, va) + size]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, va))
    if not instructions or instructions[-1].address + instructions[-1].size != va + size:
        raise RuntimeError("retail StartBarrelTimer boundary no longer decodes exactly")
    indirect = [
        {"site": f"0x{insn.address:08X}", "slot": f"0x{insn.operands[0].mem.disp:X}"}
        for insn in instructions if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_MEM
    ]
    direct = [
        {"site": f"0x{insn.address:08X}", "target": f"0x{insn.operands[0].imm:08X}"}
        for insn in instructions if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_IMM
        and insn.operands[0].imm in {0x00CB7940, 0x00CBE2FF}
    ]
    pointers = {0x0122D70E, 0x012CA7C8, 0x012D9000}
    strings = [_cstring(raw, pe, insn.operands[0].imm & 0xFFFFFFFF)
               for insn in instructions if insn.mnemonic == "push"
               and insn.operands[0].type == X86_OP_IMM
               and (insn.operands[0].imm & 0xFFFFFFFF) in pointers]
    slots = [row["slot"] for row in indirect]
    expected_slots = ["0x168", "0x1C", "0x168", "0x510", "0x120", "0x1C", "0x118",
                      "0x534", "0x168", "0x530", "0x548"]
    if strings != ["", "HUD_CLOCK_ICON", "M_WHouse_GuardPoint"] or slots != expected_slots:
        raise RuntimeError("retail StartBarrelTimer instruction shape changed")
    fields = {
        "watchTimer": {"offset":"0x108", "readSites":["0x00DB4F7C", "0x00DB4FAD", "0x00DB513D"]},
        "guiBarrelCounter": {"offset":"0x60", "storeSite":"0x00DB5045",
                             "readSites":["0x00DB5128", "0x00DB515F", "0x00DB5186"]},
        "barrelManSpokenToHeroOnReturn": {"offset":"0x73", "readSites":["0x00DB5084", "0x00DB516F"]},
    }
    for field in fields.values():
        sites = field.get("readSites", []) + ([field["storeSite"]] if "storeSite" in field else [])
        if not all(any(f"0x{insn.address:08X}" == site and op.type == X86_OP_MEM
                       and f"0x{op.mem.disp:X}" == field["offset"]
                       for insn in instructions for op in insn.operands) for site in sites):
            raise RuntimeError(f"retail field access changed at {field['offset']}")
    return {
        "schema":"new-oakvale-start-barrel-timer-retail-bytes/0.1",
        "source":"installed retail Fable.exe PE image",
        "sourceSha256":hashlib.sha256(raw).hexdigest().upper(),
        "address":f"0x{va:08X}", "rangeSize":size,
        "functionBytesSha256":hashlib.sha256(code).hexdigest().upper(),
        "instructionCount":len(instructions), "strings":strings,
        "indirectCalls":indirect, "keyDirectCalls":direct, "fields":fields,
        "barArguments":{"current":45.0, "maximum":0.0, "colour1":"0xFF00FF00",
                        "colour2":"0xFF00FF00", "texture":"HUD_CLOCK_ICON", "text":"", "scale":1.0},
        "guardDistanceBits":"0x40000000",
        "guardDistance":struct.unpack("<f", struct.pack("<I", 0x40000000))[0],
        "insideColour":"0xFF00FF00", "outsideColour":"0xFFFF0000",
        "unchangedMaximum":-1.0, "unchangedScale":-1.0,
        "terminationCalls":4, "removeOnNormalExitOnly":True,
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
