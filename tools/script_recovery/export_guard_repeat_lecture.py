#!/usr/bin/env python3
"""Extract Guard bad-deed claiming and first/repeat lecture regions from retail bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM


REGIONS = ((0x00DAC7A3, 0x00DACA22, "claimAndRecheck"),
           (0x00DACB33, 0x00DAD868, "lectureSelectionAndBodies"))
TEXT_ADDRESSES = {0x012D862C, 0x012D8608, 0x012D8510, 0x012D84EC,
                  0x012D84C8, 0x012D84A4, 0x012D8480, 0x012D845C}


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
    decoded: dict[str, list] = {}
    regions = []
    for start, end, name in REGIONS:
        code = raw[_offset(pe, start):_offset(pe, start) + end - start]
        insns = list(decoder.disasm(code, start))
        if not insns or insns[-1].address + insns[-1].size != end:
            raise RuntimeError(f"retail Guard {name} boundary no longer decodes exactly")
        decoded[name] = insns
        regions.append({
            "name": name, "address": f"0x{start:08X}", "rangeSize": end - start,
            "bytesSha256": hashlib.sha256(code).hexdigest().upper(),
            "instructionCount": len(insns),
            "strings": [_cstring(raw, pe, insn.operands[0].imm & 0xFFFFFFFF)
                        for insn in insns if insn.mnemonic == "push"
                        and insn.operands[0].type == X86_OP_IMM
                        and (insn.operands[0].imm & 0xFFFFFFFF) in TEXT_ADDRESSES],
        })

    claim, lecture = decoded["claimAndRecheck"], decoded["lectureSelectionAndBodies"]

    def mem_at(insns: list, address: int, displacement: int) -> bool:
        return any(insn.address == address and any(
            op.type == X86_OP_MEM and op.mem.disp == displacement for op in insn.operands)
            for insn in insns)

    required = ((claim, 0x00DAC7A3, 0x58), (claim, 0x00DAC7A6, 0x68),
                (claim, 0x00DAC9F8, 0x58), (claim, 0x00DAC9FB, 0x68),
                (claim, 0x00DACA1C, 0x58), (claim, 0x00DACA1F, 0x68),
                (lecture, 0x00DACB33, 0x92), (lecture, 0x00DAD31B, 0x92))
    if not all(mem_at(*item) for item in required):
        raise RuntimeError("retail Guard lecture field accesses changed")
    return {
        "schema": "guard-repeat-lecture-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "regions": regions,
        "claiming": {
            "loopComparison": {"badDeedsSite": "0x00DAC7A3", "badDeedsOffset": "0x58",
                               "dealtSubtractSite": "0x00DAC7A6", "dealtOffset": "0x68",
                               "condition": "BadDeedsPerformed - GuardsDealtWithBadDeeds > 0"},
            "postChaseRecheck": {"badDeedsSite": "0x00DAC9F8", "dealtSubtractSite": "0x00DAC9FB",
                                 "noWorkBranch": "0x00DAD87E"},
            "claimCurrentCount": {"loadSite": "0x00DACA1C", "storeSite": "0x00DACA1F",
                                  "assignment": "GuardsDealtWithBadDeeds = BadDeedsPerformed"},
        },
        "lecture": {
            "spokenOnceRead": {"site": "0x00DACB33", "parentOffset": "0x92"},
            "repeatBranch": {"site": "0x00DACB3D", "target": "0x00DAD327",
                             "speech": "TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN"},
            "firstSpeech": [f"TEXT_QST_048_GUARD_CAUGHT_YOU_{n}" for n in (10, 20, 30, 40, 50, 60)],
            "firstSetsSpokenOnce": {"site": "0x00DAD31B", "value": 1},
            "repeatTailSpeech": "TEXT_QST_048_GUARD_AFTER_READ_LIST",
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = export(args.exe)
    text = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(text, encoding="utf-8")
    else:
        print(text, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
