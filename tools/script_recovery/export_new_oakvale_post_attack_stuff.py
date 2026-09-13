#!/usr/bin/env python3
"""Extract PostAttackStuff state-transition evidence from retail x86 bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_ESI


def _offset(pe: pefile.PE, va: int) -> int:
    return pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)


def _cstring(raw: bytes, pe: pefile.PE, va: int) -> str:
    start = _offset(pe, va)
    return raw[start:raw.index(b"\0", start)].decode("ascii")


def export(exe: Path, quest_path: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    quest = json.loads(quest_path.read_text(encoding="utf-8-sig"))
    va = int(quest["functions"]["PostAttackStuff"]["address"], 0)
    size = 0x447
    code = raw[_offset(pe, va):_offset(pe, va) + size]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, va))
    if not instructions or instructions[-1].address + instructions[-1].size != va + size:
        raise RuntimeError("retail PostAttackStuff boundary no longer decodes exactly")

    string_pointers = [
        insn.operands[0].imm & 0xFFFFFFFF
        for insn in instructions
        if insn.mnemonic == "push" and insn.operands[0].type == X86_OP_IMM
        and (insn.operands[0].imm & 0xFFFFFFFF) in {
            0x012D9E04, 0x012D9DF8, 0x012D8958, 0x012D9DE4,
            0x01255174, 0x012D9DC8, 0x012D826C,
        }
    ]
    strings = [_cstring(raw, pe, pointer) for pointer in string_pointers]
    indirect = [
        {"site": f"0x{insn.address:08X}", "slot": f"0x{insn.operands[0].mem.disp:X}"}
        for insn in instructions
        if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_MEM
    ]
    direct = [
        {"site": f"0x{insn.address:08X}", "target": f"0x{insn.operands[0].imm:08X}"}
        for insn in instructions
        if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_IMM
        and insn.operands[0].imm in {0x00CB7940, 0x00CBE87F, 0x00CBE2FF, 0x00CBFB7D}
    ]
    dad_found = [
        insn for insn in instructions
        if insn.mnemonic == "mov" and len(insn.operands) == 2
        and insn.operands[0].type == X86_OP_MEM
        and insn.operands[0].mem.base == X86_REG_ESI
        and insn.operands[0].mem.disp == 0x51
        and insn.operands[1].type == X86_OP_IMM and insn.operands[1].imm == 1
    ]
    expected_strings = ["M_PostAttackStart", "M_PostAttackStart", "V_OakVale",
                        "OBJECT_TEDDY_BEAR_UNGIVEABLE", "MK_OVI_DADTRIGGER", "HERO",
                        "CS_OAKVALEINTRO_HESDEADJIM", "V_OakVale", "Q__OakValeIntro_PostAttack"]
    key_slots = [entry["slot"] for entry in indirect]
    expected_slots = ["0x120", "0x12C", "0x4", "0x1C", "0xAE8", "0x120", "0x118", "0x760",
                      "0x120", "0x6E0", "0x554", "0x1F4", "0x1C", "0x684", "0x680", "0xAE8",
                      "0x5D8", "0x120", "0x118", "0x1C", "0x118", "0x118", "0x20", "0x5C8",
                      "0x5EC", "0x5CC", "0x5CC", "0x5EC", "0x120", "0x6E0", "0xA20", "0x460",
                      "0xA44", "0xAE4"]
    if strings != expected_strings or key_slots != expected_slots or len(dad_found) != 1:
        raise RuntimeError("retail PostAttackStuff instruction shape changed")
    return {
        "schema": "new-oakvale-post-attack-stuff-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "address": f"0x{va:08X}", "rangeSize": size,
        "functionBytesSha256": hashlib.sha256(code).hexdigest().upper(),
        "instructionCount": len(instructions), "strings": strings,
        "indirectCalls": indirect, "keyDirectCalls": direct,
        "musicSets": [45, 57], "dadTriggerDistanceBits": "0x40A00000",
        "dadTriggerDistance": struct.unpack("<f", struct.pack("<I", 0x40A00000))[0],
        "dadFoundStore": {"site": f"0x{dad_found[0].address:08X}", "offset": "0x51", "width": 1, "value": 1},
        "heroScriptPriority": 4, "logbookStoryEntry": 20,
        "cutsceneTarget": "0x00CBFB7D", "distanceHelperTarget": "0x00CBE2FF",
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
