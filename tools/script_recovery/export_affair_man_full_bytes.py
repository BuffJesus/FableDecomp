#!/usr/bin/env python3
"""Reproduce complete retail NOVI_AffairMan Init/Main byte snapshots."""
from __future__ import annotations

import argparse, hashlib, json
from pathlib import Path
import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs

REGIONS = (("Init", 0x00DB0950, 0x00DB09E0), ("Main", 0x00DB09E0, 0x00DB1DA2))

def export(exe: Path) -> dict:
    raw = exe.read_bytes(); pe = pefile.PE(data=raw, fast_load=True)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    rows = []
    for name, start, end in REGIONS:
        off = pe.get_offset_from_rva(start - pe.OPTIONAL_HEADER.ImageBase)
        code = raw[off:off + end - start]
        insns = list(decoder.disasm(code, start))
        if not insns or insns[-1].address + insns[-1].size != end:
            raise RuntimeError(f"{name} no longer decodes to its exact boundary")
        rows.append({"name": name, "address": f"0x{start:08X}", "endExclusive": f"0x{end:08X}",
                     "rangeSize": len(code), "bytesSha256": hashlib.sha256(code).hexdigest().upper(),
                     "instructionCount": len(insns), "terminal": insns[-1].mnemonic})
    return {"schema": "affair-man-full-retail-bytes/0.1", "source": "installed retail Fable.exe PE image",
            "sourceSha256": hashlib.sha256(raw).hexdigest().upper(), "nativeClass": "CNOVI_AffairMan",
            "regions": rows, "coverage": {"operations": "Init and complete Main dispatcher",
            "liveRelationships": ["NOVI_AffairWoman", "NOVI_AffairWife"],
            "terminalCleanup": "0x00DB1D7C-0x00DB1DA1"}}

def main() -> int:
    ap=argparse.ArgumentParser(); ap.add_argument("--exe",type=Path,required=True); ap.add_argument("--output",type=Path)
    a=ap.parse_args(); rendered=json.dumps(export(a.exe),indent=2)+"\n"
    if a.output: a.output.write_text(rendered,encoding="utf-8")
    else: print(rendered,end="")
    return 0
if __name__ == "__main__": raise SystemExit(main())
