#!/usr/bin/env python3
"""Extract New Oakvale Main's 16 entity bindings from retail x86 bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_EDI


def _offset(pe: pefile.PE, va: int) -> int:
    return pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)


def _cstring(raw: bytes, pe: pefile.PE, va: int) -> str:
    start = _offset(pe, va)
    return raw[start:raw.index(b"\0", start)].decode("ascii")


def export(exe: Path, cluster_path: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    va = next(int(row["address"], 0) for row in cluster["lifecycle"] if row["role"] == "Main")
    size = 0x686  # through the final temporary-name cleanup, before OnInitialActivate
    code = raw[_offset(pe, va):_offset(pe, va) + size]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, va))
    names = []
    callbacks = []
    vtables = []
    for insn in instructions:
        if insn.mnemonic == "push" and insn.operands[0].type == X86_OP_IMM:
            pointer = insn.operands[0].imm
            if pe.OPTIONAL_HEADER.ImageBase <= pointer < pe.OPTIONAL_HEADER.ImageBase + pe.OPTIONAL_HEADER.SizeOfImage:
                try:
                    value = _cstring(raw, pe, pointer)
                except (ValueError, UnicodeDecodeError):
                    continue
                if value.startswith(("NOVI_", "OVI_")):
                    names.append((insn.address, pointer, value))
        if (insn.mnemonic == "mov" and len(insn.operands) == 2
                and insn.operands[0].type == X86_OP_MEM and insn.operands[0].mem.base == X86_REG_EDI
                and insn.operands[1].type == X86_OP_IMM):
            if insn.operands[0].mem.disp == 0x10:
                callbacks.append((insn.address, insn.operands[1].imm))
            elif insn.operands[0].mem.disp == 0:
                vtables.append(insn.operands[1].imm)
    direct_calls = [insn.operands[0].imm for insn in instructions if insn.mnemonic == "call"
                    and insn.operands[0].type == X86_OP_IMM]
    if len(names) != 16 or len(callbacks) != 16 or len(vtables) != 16:
        raise RuntimeError("retail Main entity-binding instruction shape changed")
    bindings = [{
        "name": name[2], "nameAddress": f"0x{name[1]:08X}", "namePush": f"0x{name[0]:08X}",
        "allocatorCallback": f"0x{callback[1]:08X}", "callbackStore": f"0x{callback[0]:08X}",
    } for name, callback in zip(names, callbacks)]
    return {
        "schema": "new-oakvale-main-bindings-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "mainAddress": f"0x{va:08X}",
        "bindingBlockSize": size,
        "bindingBlockSha256": hashlib.sha256(code).hexdigest().upper(),
        "bindingObjectSize": 0x1C,
        "bindingVtable": f"0x{vtables[0]:08X}",
        "parentFieldOffset": "0x08",
        "allocatorFieldOffset": "0x10",
        "enabledFieldOffset": "0x14",
        "auxiliaryFieldOffset": "0x18",
        "operatorNewCalls": direct_calls.count(0xBFEA1A),
        "addBindingTarget": "0x00CB8230",
        "addBindingCalls": direct_calls.count(0xCB8230),
        "bindings": bindings,
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
