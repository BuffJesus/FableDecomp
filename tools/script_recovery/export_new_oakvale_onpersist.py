#!/usr/bin/env python3
"""Decode New Oakvale OnPersist directly from the installed retail PE."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_ECX, X86_REG_ESP


def _file_offset(pe: pefile.PE, va: int) -> int:
    return pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)


def _cstring(raw: bytes, pe: pefile.PE, va: int) -> str:
    start = _file_offset(pe, va)
    end = raw.index(b"\0", start)
    return raw[start:end].decode("ascii")


def export(exe: Path, cluster_path: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    va = next(int(row["address"], 0) for row in cluster["lifecycle"] if row["role"] == "OnPersist")
    code = raw[_file_offset(pe, va):_file_offset(pe, va) + 0x21]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, va))
    field_adds = [insn for insn in instructions if insn.mnemonic == "add" and len(insn.operands) == 2
                  and insn.operands[0].type == X86_OP_REG and insn.operands[0].reg == X86_REG_ECX
                  and insn.operands[1].type == X86_OP_IMM]
    key_pushes = [insn for insn in instructions if insn.mnemonic == "push" and len(insn.operands) == 1
                  and insn.operands[0].type == X86_OP_IMM and insn.operands[0].imm >= pe.OPTIONAL_HEADER.ImageBase]
    default_stores = [insn for insn in instructions if insn.mnemonic == "mov" and len(insn.operands) == 2
                      and insn.operands[0].type == X86_OP_MEM
                      and insn.operands[0].mem.base == X86_REG_ESP
                      and insn.operands[1].type == X86_OP_IMM]
    calls = [insn for insn in instructions if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_IMM]
    if not (len(field_adds) == len(key_pushes) == len(default_stores) == len(calls) == 1):
        raise RuntimeError("retail OnPersist instruction shape changed")
    field, key, default, call = field_adds[0], key_pushes[0], default_stores[0], calls[0]
    return {
        "schema": "new-oakvale-onpersist-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "address": f"0x{va:08X}",
        "rangeSize": len(code),
        "functionBytesSha256": hashlib.sha256(code).hexdigest().upper(),
        "transfers": [{
            "key": _cstring(raw, pe, key.operands[0].imm),
            "keyAddress": f"0x{key.operands[0].imm:08X}",
            "fieldOffset": f"0x{field.operands[1].imm:X}",
            "fieldAddressInstruction": f"0x{field.address:08X}",
            "valueWidth": 1,
            "defaultWidth": default.operands[0].size,
            "defaultValue": default.operands[1].imm,
            "transferTarget": f"0x{call.operands[0].imm:08X}",
        }],
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
