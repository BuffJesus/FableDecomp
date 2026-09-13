#!/usr/bin/env python3
"""Extract AttackStuff transition operands directly from retail x86 bytes."""
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
    va = int(quest["functions"]["AttackStuff"]["address"], 0)
    size = 0x11D
    code = raw[_offset(pe, va):_offset(pe, va) + size]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, va))
    strings = []
    for insn in instructions:
        if insn.mnemonic != "push" or insn.operands[0].type != X86_OP_IMM:
            continue
        pointer = insn.operands[0].imm
        if pe.OPTIONAL_HEADER.ImageBase <= pointer < pe.OPTIONAL_HEADER.ImageBase + pe.OPTIONAL_HEADER.SizeOfImage:
            try:
                value = _cstring(raw, pe, pointer)
            except (ValueError, UnicodeDecodeError):
                continue
            if value and value not in strings:
                strings.append(value)
    slots = [insn.operands[0].mem.disp for insn in instructions if insn.mnemonic == "call"
             and insn.operands[0].type == X86_OP_MEM]
    immediates = [insn.operands[0].imm & 0xFFFFFFFF for insn in instructions if insn.mnemonic == "push"
                  and insn.operands[0].type == X86_OP_IMM]
    expected_slots = [0x450, 0x460, 0xA18, 0xA40, 0xA3C, 0x4A0]
    if slots != expected_slots or 0x41B80000 not in immediates or len(strings) != 4:
        raise RuntimeError("retail AttackStuff instruction shape changed")
    return {
        "schema": "new-oakvale-attack-stuff-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "address": f"0x{va:08X}", "rangeSize": size,
        "functionBytesSha256": hashlib.sha256(code).hexdigest().upper(),
        "strings": strings,
        "interfaceSlots": [f"0x{x:X}" for x in slots],
        "timeOfDayBits": "0x41B80000",
        "timeOfDay": struct.unpack("<f", struct.pack("<I", 0x41B80000))[0],
        "deactivateDelay": 0, "themeTransitionSeconds": 0.0,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--quest", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = export(args.exe, args.quest)
    text = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(text, encoding="utf-8")
    else:
        print(text, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
