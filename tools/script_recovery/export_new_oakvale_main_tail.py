#!/usr/bin/env python3
"""Decode the post-binding tail of New Oakvale quest Main from retail bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_EDI, X86_REG_ESI


def _offset(pe: pefile.PE, va: int) -> int:
    return pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)


def _cstring(raw: bytes, pe: pefile.PE, va: int) -> str:
    start = _offset(pe, va)
    return raw[start:raw.index(b"\0", start)].decode("ascii")


def export(exe: Path, cluster_path: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    main_va = next(int(row["address"], 0) for row in cluster["lifecycle"] if row["role"] == "Main")
    va, end = main_va + 0x686, main_va + 0x7E2
    code = raw[_offset(pe, va):_offset(pe, end)]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, va))
    direct_calls = [(insn.address, insn.operands[0].imm) for insn in instructions
                    if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_IMM]
    indirect = [(insn.address, insn.operands[0].mem.disp) for insn in instructions
                if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_MEM]
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
            if value:
                strings.append({"value": value, "address": f"0x{pointer:08X}", "push": f"0x{insn.address:08X}"})
    field_reads = [insn for insn in instructions if insn.mnemonic == "mov" and len(insn.operands) == 2
                   and insn.operands[1].type == X86_OP_MEM and insn.operands[1].mem.base == X86_REG_ESI]
    stores = {(insn.operands[0].mem.disp, insn.operands[1].imm) for insn in instructions
              if insn.mnemonic == "mov" and len(insn.operands) == 2
              and insn.operands[0].type == X86_OP_MEM and insn.operands[0].mem.base == X86_REG_EDI
              and insn.operands[1].type == X86_OP_IMM}
    required_calls = {0xCB8930, 0xCB7940, 0xBFEA1A, 0xCDD450, 0xCB7E50, 0xDBDE40}
    if not required_calls.issubset({target for _, target in direct_calls}):
        raise RuntimeError("retail Main tail direct-call shape changed")
    return {
        "schema": "new-oakvale-main-tail-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "address": f"0x{va:08X}", "rangeSize": len(code),
        "functionEnd": f"0x{end:08X}", "rangeSha256": hashlib.sha256(code).hexdigest().upper(),
        "postAddBaseTarget": "0x00CB8930",
        "postAddInterfaceSlot": "0x100",
        "attackOverField": "0x50",
        "attackOverWidth": next(insn.operands[1].size for insn in field_reads if insn.operands[1].mem.disp == 0x50),
        "terminationTarget": "0x00CB7940",
        "deactivateSlot": "0x460",
        "reloadSection": next(row["value"] for row in strings if row["value"] == "Q__OakValeIntro_PostAttack"),
        "reloadDelay": 0,
        "activeQuestNameSlot": "0xA3C",
        "objectiveSlot": "0x4A0",
        "objective": next(row["value"] for row in strings if row["value"].startswith("TEXT_QUEST_")),
        "spawnedThread": next(row["value"] for row in strings if row["value"] == "StartBarrelTimer"),
        "spawnedObjectSize": 0x3C,
        "spawnedConstructorTarget": "0x00CDD450",
        "spawnedVtable": f"0x{next(value for disp, value in stores if disp == 0):08X}",
        "spawnedCallback": f"0x{next(value for disp, value in stores if disp == 0x34):08X}",
        "spawnedOwnerOffset": "0x38",
        "addSpawnedTarget": "0x00CB7E50",
        "doMissionTarget": "0x00DBDE40",
        "indirectCallSlots": [f"0x{slot:X}" for _, slot in indirect],
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--cluster", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = export(args.exe, args.cluster)
    text = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(text, encoding="utf-8")
    else:
        print(text, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
