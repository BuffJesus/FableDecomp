#!/usr/bin/env python3
"""Decode New Oakvale quest allocation and construction facts from retail x86 bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_ESI


def _code(raw: bytes, pe: pefile.PE, va: int, size: int) -> bytes:
    base = pe.OPTIONAL_HEADER.ImageBase
    start = pe.get_offset_from_rva(va - base)
    return raw[start:start + size]


def export(exe: Path, cluster_path: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    allocator_va = int(cluster["allocatorAddress"], 0)
    constructor_va = int(cluster["constructorAddress"], 0)
    allocator_bytes = _code(raw, pe, allocator_va, 0x28)
    constructor_bytes = _code(raw, pe, constructor_va, 0xDC)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    allocator = list(decoder.disasm(allocator_bytes, allocator_va))
    constructor = list(decoder.disasm(constructor_bytes, constructor_va))

    pushes = [insn.operands[0].imm for insn in allocator
              if insn.mnemonic == "push" and len(insn.operands) == 1 and insn.operands[0].type == X86_OP_IMM]
    calls = [insn.operands[0].imm for insn in allocator
             if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_IMM]
    constructor_calls = [insn.operands[0].imm for insn in constructor
                         if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_IMM]
    vtable_stores = [insn.operands[1].imm for insn in constructor
                     if insn.mnemonic == "mov" and len(insn.operands) == 2
                     and insn.operands[0].type == X86_OP_MEM
                     and insn.operands[0].mem.base == X86_REG_ESI
                     and insn.operands[0].mem.disp == 0
                     and insn.operands[1].type == X86_OP_IMM]
    if not pushes or len(calls) < 2 or not constructor_calls or not vtable_stores:
        raise RuntimeError("retail quest ownership instruction shape is incomplete")
    return {
        "schema": "new-oakvale-quest-ownership/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "imageBase": f"0x{pe.OPTIONAL_HEADER.ImageBase:08X}",
        "allocator": {
            "address": f"0x{allocator_va:08X}",
            "functionBytesSha256": hashlib.sha256(allocator_bytes).hexdigest().upper(),
            "objectSize": pushes[0],
            "operatorNewTarget": f"0x{calls[0]:08X}",
            "constructorTarget": f"0x{calls[1]:08X}",
        },
        "constructor": {
            "address": f"0x{constructor_va:08X}",
            "functionBytesSha256": hashlib.sha256(constructor_bytes).hexdigest().upper(),
            "baseConstructorTarget": f"0x{constructor_calls[0]:08X}",
            "installedVtable": f"0x{vtable_stores[0]:08X}",
        },
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
