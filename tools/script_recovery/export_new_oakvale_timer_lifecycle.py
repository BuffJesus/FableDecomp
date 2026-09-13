#!/usr/bin/env python3
"""Decode New Oakvale quest timer ownership directly from retail x86 bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_MEM, X86_OP_REG, X86_REG_EAX, X86_REG_EDX, X86_REG_ESI


def _code(raw: bytes, pe: pefile.PE, va: int, size: int) -> bytes:
    base = pe.OPTIONAL_HEADER.ImageBase
    start = pe.get_offset_from_rva(va - base)
    return raw[start:start + size]


def _hex(value: int) -> str:
    return f"0x{value:03X}" if value < 0x1000 else f"0x{value:08X}"


def export(exe: Path, cluster_path: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    constructor_va = int(cluster["constructorAddress"], 0)
    destructor_va = int(cluster["destructorImplementationAddress"], 0)
    constructor_bytes = _code(raw, pe, constructor_va, 0xDA)
    destructor_bytes = _code(raw, pe, destructor_va, 0x1C2)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    constructor = list(decoder.disasm(constructor_bytes, constructor_va))
    destructor = list(decoder.disasm(destructor_bytes, destructor_va))

    registrations: list[dict] = []
    for index, insn in enumerate(constructor):
        if (insn.mnemonic != "call" or len(insn.operands) != 1
                or insn.operands[0].type != X86_OP_MEM
                or insn.operands[0].mem.disp != 0x15C):
            continue
        if index + 1 >= len(constructor):
            raise RuntimeError("timer registration call has no result store")
        store = constructor[index + 1]
        if (store.mnemonic != "mov" or len(store.operands) != 2
                or store.operands[0].type != X86_OP_MEM
                or store.operands[0].mem.base != X86_REG_ESI
                or store.operands[1].type != X86_OP_REG
                or store.operands[1].reg != X86_REG_EAX):
            raise RuntimeError("timer registration result is not stored from eax into this")
        registrations.append({
            "callAddress": _hex(insn.address),
            "interfaceSlot": _hex(insn.operands[0].mem.disp),
            "storeAddress": _hex(store.address),
            "fieldOffset": _hex(store.operands[0].mem.disp),
        })

    deregistrations: list[dict] = []
    for index, insn in enumerate(destructor):
        if (insn.mnemonic != "call" or len(insn.operands) != 1
                or insn.operands[0].type != X86_OP_MEM
                or insn.operands[0].mem.disp != 0x160):
            continue
        source = next((candidate for candidate in reversed(destructor[max(0, index - 5):index])
                       if candidate.mnemonic == "mov" and len(candidate.operands) == 2
                       and candidate.operands[0].type == X86_OP_REG
                       and candidate.operands[0].reg == X86_REG_EDX
                       and candidate.operands[1].type == X86_OP_MEM
                       and candidate.operands[1].mem.base == X86_REG_ESI), None)
        if source is None:
            raise RuntimeError("timer deregistration call has no nearby this-field load")
        deregistrations.append({
            "loadAddress": _hex(source.address),
            "fieldOffset": _hex(source.operands[1].mem.disp),
            "callAddress": _hex(insn.address),
            "interfaceSlot": _hex(insn.operands[0].mem.disp),
        })

    if len(registrations) != 2 or len(deregistrations) != 2:
        raise RuntimeError("expected exactly two timer registrations and deregistrations")
    return {
        "schema": "new-oakvale-timer-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "constructor": {
            "address": _hex(constructor_va),
            "rangeSize": len(constructor_bytes),
            "functionBytesSha256": hashlib.sha256(constructor_bytes).hexdigest().upper(),
            "registrations": registrations,
        },
        "destructor": {
            "address": _hex(destructor_va),
            "rangeSize": len(destructor_bytes),
            "functionBytesSha256": hashlib.sha256(destructor_bytes).hexdigest().upper(),
            "deregistrations": deregistrations,
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
