#!/usr/bin/env python3
"""Extract BarrelMan automatic judgment and follow-up dialogue regions from retail bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM


REGIONS = ((0x00DB5A48, 0x00DB5DEE, "automaticJudgment"),
           (0x00DB65BF, 0x00DB684C, "followupDialogue"))


def _offset(pe: pefile.PE, va: int) -> int:
    return pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)


def _cstring(raw: bytes, pe: pefile.PE, va: int) -> str:
    start = _offset(pe, va)
    return raw[start:raw.index(b"\0", start)].decode("ascii")


def export(exe: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    string_addresses = {0x012D91B0, 0x012D917C, 0x012D915C, 0x012D9140,
                        0x012D909C, 0x012D9070, 0x012D9050}
    regions = []
    all_insns = {}
    for start, end, name in REGIONS:
        code = raw[_offset(pe, start):_offset(pe, start) + end - start]
        insns = list(decoder.disasm(code, start))
        if not insns or insns[-1].address + insns[-1].size != end:
            raise RuntimeError(f"retail BarrelMan {name} boundary no longer decodes exactly")
        all_insns[name] = insns
        regions.append({
            "name":name, "address":f"0x{start:08X}", "rangeSize":end-start,
            "bytesSha256":hashlib.sha256(code).hexdigest().upper(), "instructionCount":len(insns),
            "strings":[_cstring(raw, pe, insn.operands[0].imm & 0xFFFFFFFF)
                       for insn in insns if insn.mnemonic == "push"
                       and insn.operands[0].type == X86_OP_IMM
                       and (insn.operands[0].imm & 0xFFFFFFFF) in string_addresses],
            "indirectCalls":[{"site":f"0x{insn.address:08X}", "slot":f"0x{insn.operands[0].mem.disp:X}"}
                             for insn in insns if insn.mnemonic == "call" and insn.operands[0].type == X86_OP_MEM],
        })
    judgment, followup = all_insns["automaticJudgment"], all_insns["followupDialogue"]
    def has_mem(insns, address: int, displacement: int) -> bool:
        return any(insn.address == address and any(op.type == X86_OP_MEM and op.mem.disp == displacement
                                                   for op in insn.operands) for insn in insns)
    if not (has_mem(judgment, 0x00DB5A4B, 0x73) and has_mem(judgment, 0x00DB5B3C, 0x1D)
            and has_mem(followup, 0x00DB65D7, 0x1D) and has_mem(followup, 0x00DB65F4, 0x75)):
        raise RuntimeError("retail BarrelMan consequence field accesses changed")
    if any(any(op.type == X86_OP_MEM and op.mem.disp == 0x75 for op in insn.operands) for insn in judgment):
        raise RuntimeError("automatic judgment unexpectedly reads BarrelBrokenPersistent")
    return {
        "schema":"barrel-man-judgment-retail-bytes/0.1",
        "source":"installed retail Fable.exe PE image",
        "sourceSha256":hashlib.sha256(raw).hexdigest().upper(), "regions":regions,
        "automaticJudgment":{
            "returnedFlagStore":{"site":"0x00DB5A4B", "parentOffset":"0x73", "width":1, "value":1},
            "detection":{"visibilitySlot":"0x9E0", "visibilitySite":"0x00DB5A93",
                         "fallbackDistance":10.0, "distanceSite":"0x00DB5AB5", "shortCircuitOr":True},
            "readsBarrelBrokenPersistent":False,
            "detected":{"speech":"TEXT_QST_048_BARRELMAN_THANKS", "deedHelper":"0x00DB0660"},
            "notDetected":{"conversation":"TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE",
                           "heroLetMeDownStore":"0x00DB5B3C", "instruction":"TEXT_QST_048_INSTRUCTION_LEFT_WAREHOUSE_UNATTENDED",
                           "deedHelper":"0x00DAEA70", "deedEnum":1},
            "finalBrain":"BRAIN_GOOD_VILLAGER_BASE", "finalPhase":5,
        },
        "followupDialogue":{
            "heroLetMeDownRead":{"site":"0x00DB65D7", "offset":"0x1D"},
            "barrelBrokenPersistentRead":{"site":"0x00DB65F4", "parentOffset":"0x75"},
            "heroStayedSpeech":"TEXT_QST_048_BARRELMAN_NO_TIME",
            "heroAbsentBrokenSpeech":"TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN",
            "heroAbsentUnbrokenSpeech":"TEXT_QST_048_BARRELMAN_LETDOWN_NOT_BROKE",
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = export(args.exe)
    text = json.dumps(result, indent=2) + "\n"
    if args.output: args.output.write_text(text, encoding="utf-8")
    else: print(text, end="")
    return 0


if __name__ == "__main__": raise SystemExit(main())
