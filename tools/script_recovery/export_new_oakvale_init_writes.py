#!/usr/bin/env python3
"""Extract New Oakvale scalar Init writes directly from retail x86 bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_MEM, X86_REG_ESI


def export(exe: Path, cluster_path: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    init_va = next(int(row["address"], 0) for row in cluster["lifecycle"] if row["role"] == "Init")
    main_va = next(int(row["address"], 0) for row in cluster["lifecycle"] if row["role"] == "Main")
    size = main_va - init_va
    start = pe.get_offset_from_rva(init_va - pe.OPTIONAL_HEADER.ImageBase)
    code = raw[start:start + size]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, init_va))
    writes = []
    for insn in instructions:
        if insn.mnemonic != "mov" or len(insn.operands) != 2 or insn.operands[0].type != X86_OP_MEM:
            continue
        target = insn.operands[0]
        if target.mem.base != X86_REG_ESI or not 0x48 <= target.mem.disp <= 0x100:
            continue
        # Embedded speech-vector construction starts at +0x9c; only the deed array at +fc/+100
        # belongs to scalar initialization in that portion of the object.
        if 0x9C <= target.mem.disp < 0xFC:
            continue
        writes.append({
            "address": f"0x{insn.address:08X}",
            "fieldOffset": f"0x{target.mem.disp:X}",
            "width": target.size,
            "source": insn.op_str.split(", ", 1)[1],
        })
    timer_loads = [insn for insn in instructions if insn.mnemonic == "mov" and len(insn.operands) == 2
                   and insn.operands[1].type == X86_OP_MEM
                   and insn.operands[1].mem.base == X86_REG_ESI
                   and insn.operands[1].mem.disp == 0x104]
    timer_calls = [insn for insn in instructions if insn.mnemonic == "call" and len(insn.operands) == 1
                   and insn.operands[0].type == X86_OP_MEM and insn.operands[0].mem.disp == 0x164]
    if len(writes) != 27 or len(timer_loads) != 1 or len(timer_calls) != 1:
        raise RuntimeError("retail Init scalar/timer instruction shape changed")
    return {
        "schema": "new-oakvale-init-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "initAddress": f"0x{init_va:08X}",
        "rangeSize": size,
        "functionBytesSha256": hashlib.sha256(code).hexdigest().upper(),
        "scalarWrites": writes,
        "timerReset": {
            "loadAddress": f"0x{timer_loads[0].address:08X}",
            "fieldOffset": "0x104",
            "callAddress": f"0x{timer_calls[0].address:08X}",
            "interfaceSlot": "0x164",
            "value": 0,
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
