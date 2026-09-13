#!/usr/bin/env python3
"""Decode New Oakvale RegisterMain ownership from retail x86 bytes."""
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


def export(exe: Path, cluster_path: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    va = next(int(row["address"], 0) for row in cluster["lifecycle"] if row["role"] == "RegisterMain")
    size = 0x8B
    code = raw[_offset(pe, va):_offset(pe, va) + size]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, va))
    pushes = [insn for insn in instructions if insn.mnemonic == "push" and len(insn.operands) == 1
              and insn.operands[0].type == X86_OP_IMM]
    calls = [insn.operands[0].imm for insn in instructions if insn.mnemonic == "call"
             and insn.operands[0].type == X86_OP_IMM]
    stores = {insn.operands[0].mem.disp: insn.operands[1].imm for insn in instructions
              if insn.mnemonic == "mov" and len(insn.operands) == 2
              and insn.operands[0].type == X86_OP_MEM and insn.operands[0].mem.base == X86_REG_ESI
              and insn.operands[1].type == X86_OP_IMM}
    string_pushes = [insn.operands[0].imm for insn in pushes
                     if insn.operands[0].imm >= pe.OPTIONAL_HEADER.ImageBase]
    if len(calls) < 5 or 0 not in stores or 0x34 not in stores or len(string_pushes) != 2:
        raise RuntimeError("retail RegisterMain instruction shape changed")
    vtable = stores[0]
    vtable_bytes = raw[_offset(pe, vtable):_offset(pe, vtable) + 8]
    slots = struct.unpack("<II", vtable_bytes)
    return {
        "schema": "new-oakvale-register-main-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "address": f"0x{va:08X}",
        "rangeSize": size,
        "functionBytesSha256": hashlib.sha256(code).hexdigest().upper(),
        "allocationSize": next(insn.operands[0].imm for insn in pushes if insn.operands[0].imm == 0x3C),
        "operatorNewTarget": f"0x{calls[0]:08X}",
        "threadName": _cstring(raw, pe, string_pushes[0]),
        "threadNameAddress": f"0x{string_pushes[0]:08X}",
        "spawnedConstructorTarget": f"0x{calls[2]:08X}",
        "spawnedVtable": f"0x{vtable:08X}",
        "spawnedVtableSha256": hashlib.sha256(vtable_bytes).hexdigest().upper(),
        "spawnedVtableSlots": [f"0x{slot:08X}" for slot in slots],
        "mainThunk": f"0x{stores[0x34]:08X}",
        "ownerFieldOffset": "0x38",
        "sectionName": _cstring(raw, pe, string_pushes[1]),
        "sectionNameAddress": f"0x{string_pushes[1]:08X}",
        "addSpawnedFunctionTarget": f"0x{calls[4]:08X}",
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
