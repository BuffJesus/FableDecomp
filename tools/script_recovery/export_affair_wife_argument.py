#!/usr/bin/env python3
"""Extract Affair Wife's post-arrival argument loop from retail executable bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_ESP


START, END = 0x00DB35C1, 0x00DB3CAB
STRING_ADDRESSES = {0x012D8D68, 0x012D8D54, 0x012D8E60, 0x012D8D28,
                    0x012D8D00, 0x012D8CD8, 0x012D8CB4}


def _offset(pe: pefile.PE, va: int) -> int:
    return pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)


def _cstring(raw: bytes, pe: pefile.PE, va: int) -> str:
    start = _offset(pe, va)
    return raw[start:raw.index(b"\0", start)].decode("ascii")


def export(exe: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    code = raw[_offset(pe, START):_offset(pe, START) + END - START]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    insns = list(decoder.disasm(code, START))
    if not insns or insns[-1].address + insns[-1].size != END:
        raise RuntimeError("retail Affair Wife argument boundary no longer decodes exactly")
    by_address = {insn.address: insn for insn in insns}

    def imm(address: int) -> int:
        op = by_address[address].operands[0]
        if op.type != X86_OP_IMM:
            raise RuntimeError(f"expected immediate at 0x{address:08X}")
        return op.imm & 0xFFFFFFFF

    def esp_local(address: int, displacement: int) -> bool:
        return any(op.type == X86_OP_MEM and op.mem.base == X86_REG_ESP and op.mem.disp == displacement
                   for op in by_address[address].operands)

    if struct.unpack("<f", struct.pack("<I", imm(0x00DB35D5)))[0] != 15.0:
        raise RuntimeError("retail Affair Wife argument visibility distance changed")
    if not (esp_local(0x00DB36D2, 0x34) and esp_local(0x00DB394E, 0x34) and esp_local(0x00DB3B1A, 0x34)
            and esp_local(0x00DB3B7D, 0x30) and esp_local(0x00DB3C16, 0x30)
            and esp_local(0x00DB3C62, 0x34)):
        raise RuntimeError("retail Affair Wife husband-wrapper operands changed")
    expected_slots = {0x00DB36D8: 0x76C, 0x00DB3954: 0x76C, 0x00DB3B20: 0x76C, 0x00DB3B70: 0x5B0,
                      0x00DB3B8A: 0x5B4, 0x00DB3C24: 0x5B8, 0x00DB3C6F: 0x5B8}
    for address, slot in expected_slots.items():
        op = by_address[address].operands[0]
        if by_address[address].mnemonic != "call" or op.type != X86_OP_MEM or op.mem.disp != slot:
            raise RuntimeError(f"retail Affair Wife call slot changed at 0x{address:08X}")
    strings = [_cstring(raw, pe, insn.operands[0].imm & 0xFFFFFFFF)
               for insn in insns if insn.mnemonic == "push"
               and insn.operands[0].type == X86_OP_IMM
               and (insn.operands[0].imm & 0xFFFFFFFF) in STRING_ADDRESSES]
    return {
        "schema": "affair-wife-argument-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "region": {"address": f"0x{START:08X}", "rangeSize": END - START,
                   "bytesSha256": hashlib.sha256(code).hexdigest().upper(),
                   "instructionCount": len(insns), "strings": strings},
        "argumentGate": {"heroDistance": 15.0, "distanceSite": "0x00DB35D5",
                         "skipTarget": "0x00DB3CAB"},
        "animations": ["ST_ARGUING_POINT_AWAY", "ST_ARGUING_POINT_AT"],
        "husbandFacing": [
            {"context": "during pointing animation", "localLoadSite": "0x00DB36D2", "local": "ESP+0x34",
             "callSite": "0x00DB36D8", "slot": "0x76C", "force": False},
            {"context": "after hit", "localLoadSite": "0x00DB394E", "local": "ESP+0x34",
             "callSite": "0x00DB3954", "slot": "0x76C", "force": True},
            {"context": "after hero talk", "localLoadSite": "0x00DB3B1A", "local": "ESP+0x34",
             "callSite": "0x00DB3B20", "slot": "0x76C", "force": True},
        ],
        "conversation": {
            "constructor": {"site": "0x00DB3B70", "slot": "0x5B0", "flags": [False, False]},
            "addHusband": {"localLoadSite": "0x00DB3B7D", "local": "ESP+0x30",
                           "callSite": "0x00DB3B8A", "slot": "0x5B4"},
            "wifeLine": {"listenerLocalLoadSite": "0x00DB3C16", "listenerLocal": "ESP+0x30",
                         "callSite": "0x00DB3C24", "slot": "0x5B8"},
            "husbandLine": {"speakerLocalLoadSite": "0x00DB3C62", "speakerLocal": "ESP+0x34",
                            "callSite": "0x00DB3C6F", "slot": "0x5B8",
                            "speech": "TEXT_QST_048_AFFAIRMAN_IN_TROUBLE"},
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
