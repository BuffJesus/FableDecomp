#!/usr/bin/env python3
"""Extract DoMission phase anchors directly from retail x86 bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_ESI


def _offset(pe: pefile.PE, va: int) -> int:
    return pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)


def _cstring(raw: bytes, pe: pefile.PE, va: int) -> str:
    start = _offset(pe, va)
    return raw[start:raw.index(b"\0", start)].decode("ascii")


def export(exe: Path, cluster_path: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    quest_path = cluster_path.parents[1] / "new_oakvale_intro/entities/Q_NewOakValeIntro.json"
    quest = json.loads(quest_path.read_text(encoding="utf-8-sig"))
    va = int(quest["functions"]["DoMission"]["address"], 0)
    if cluster.get("script") != "Q_NewOakValeIntro":
        raise RuntimeError("cluster is not Q_NewOakValeIntro")
    size = 0x494
    code = raw[_offset(pe, va):_offset(pe, va) + size]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, va))
    direct = [insn.operands[0].imm for insn in instructions if insn.mnemonic == "call"
              and insn.operands[0].type == X86_OP_IMM]
    slots = [insn.operands[0].mem.disp for insn in instructions if insn.mnemonic == "call"
             and insn.operands[0].type == X86_OP_MEM]
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
    callback_stores = [insn.operands[1].imm for insn in instructions if insn.mnemonic == "mov"
                       and len(insn.operands) == 2 and insn.operands[0].type == X86_OP_MEM
                       and insn.operands[0].mem.disp == 0x34 and insn.operands[1].type == X86_OP_IMM]
    attack_reads = [insn.operands[1].size for insn in instructions if insn.mnemonic == "mov"
                    and len(insn.operands) == 2 and insn.operands[1].type == X86_OP_MEM
                    and insn.operands[1].mem.base == X86_REG_ESI and insn.operands[1].mem.disp == 0x50]
    expected_callbacks = [0xDBE890, 0xDBE2E0, 0xDBE4E0]
    if callback_stores != expected_callbacks or not attack_reads or set(attack_reads) != {1}:
        raise RuntimeError("retail DoMission phase-anchor shape changed")
    return {
        "schema": "new-oakvale-do-mission-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "address": f"0x{va:08X}", "rangeSize": size,
        "functionBytesSha256": hashlib.sha256(code).hexdigest().upper(),
        "strings": strings,
        "spawnedCallbacks": [f"0x{x:08X}" for x in callback_stores],
        "attackOverReadCount": len(attack_reads), "attackOverWidths": sorted(set(attack_reads)),
        "terminationTarget": "0x00CB7940", "terminationChecks": direct.count(0xCB7940),
        "attackStuffTarget": "0x00DBE3C0", "postAttackStuffTarget": "0x00DBEB20",
        "directPhaseCalls": [f"0x{x:08X}" for x in direct if x in {0xDBE3C0, 0xDBEB20}],
        "interfaceSlots": [f"0x{x:X}" for x in slots],
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
