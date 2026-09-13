#!/usr/bin/env python3
"""Extract WatchForGotGold directly from the installed retail executable."""
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
    va = int(quest["functions"]["WatchForGotGold"]["address"], 0)
    size = 0xD4
    code = raw[_offset(pe, va):_offset(pe, va) + size]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, va))
    if not instructions or instructions[-1].address + instructions[-1].size != va + size:
        raise RuntimeError("retail WatchForGotGold boundary no longer decodes exactly")
    indirect = [{"site":f"0x{insn.address:08X}", "slot":f"0x{insn.operands[0].mem.disp:X}"}
                for insn in instructions if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_MEM]
    terminations = [insn.address for insn in instructions if insn.mnemonic == "call"
                    and insn.operands[0].type == X86_OP_IMM and insn.operands[0].imm == 0x00CB7940]
    strings = [_cstring(raw, pe, insn.operands[0].imm & 0xFFFFFFFF)
               for insn in instructions if insn.mnemonic == "push"
               and insn.operands[0].type == X86_OP_IMM
               and (insn.operands[0].imm & 0xFFFFFFFF) in {0x0122D70E, 0x012D9D2C}]
    expected_calls = [{"site":"0x00DBE2EB", "slot":"0x1FC"}, {"site":"0x00DBE2FB", "slot":"0x1C"},
                      {"site":"0x00DBE312", "slot":"0x1FC"}, {"site":"0x00DBE37A", "slot":"0xA3C"},
                      {"site":"0x00DBE384", "slot":"0x4A0"}]
    if indirect != expected_calls or strings != ["", "", "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03"]:
        raise RuntimeError("retail WatchForGotGold instruction shape changed")
    return {
        "schema":"new-oakvale-watch-for-got-gold-retail-bytes/0.1",
        "source":"installed retail Fable.exe PE image",
        "sourceSha256":hashlib.sha256(raw).hexdigest().upper(),
        "address":f"0x{va:08X}", "rangeSize":size,
        "functionBytesSha256":hashlib.sha256(code).hexdigest().upper(),
        "instructionCount":len(instructions), "strings":strings, "indirectCalls":indirect,
        "goldComparison":{"sites":["0x00DBE2F1", "0x00DBE318"], "operation":">", "operand":2,
                          "equivalentMinimumGold":3},
        "terminationCalls":[f"0x{x:08X}" for x in terminations],
        "objective":"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03",
        "objectiveTrailingStrings":["", ""],
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
