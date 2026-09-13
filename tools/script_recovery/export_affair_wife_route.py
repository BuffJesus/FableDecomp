#!/usr/bin/env python3
"""Extract the Affair Wife run-to-husband transition from retail executable bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_ESI


START, END = 0x00DB3426, 0x00DB35C1
STRINGS = {0x012D830C, 0x012D8D80}


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
        raise RuntimeError("retail Affair Wife route boundary no longer decodes exactly")

    by_address = {insn.address: insn for insn in insns}

    def imm(address: int) -> int:
        operand = by_address[address].operands[0]
        if operand.type != X86_OP_IMM:
            raise RuntimeError(f"expected immediate at 0x{address:08X}")
        return operand.imm & 0xFFFFFFFF

    def direct_target(address: int) -> int:
        insn = by_address[address]
        if insn.mnemonic != "call" or insn.operands[0].type != X86_OP_IMM:
            raise RuntimeError(f"expected direct call at 0x{address:08X}")
        return insn.operands[0].imm & 0xFFFFFFFF

    def esi_byte_store(address: int, displacement: int, value: int) -> bool:
        insn = by_address[address]
        return (insn.mnemonic == "mov" and insn.operands[0].type == X86_OP_MEM
                and insn.operands[0].mem.base == X86_REG_ESI
                and insn.operands[0].mem.disp == displacement
                and insn.operands[1].type == X86_OP_IMM
                and (insn.operands[1].imm & 0xFF) == value)

    if [imm(a) for a in (0x00DB3476, 0x00DB3478, 0x00DB347A)] != [1, 0, 1]:
        raise RuntimeError("retail MoveToPosition boolean/type operands changed")
    if [struct.unpack("<f", struct.pack("<I", imm(a)))[0]
            for a in (0x00DB347C, 0x00DB3497, 0x00DB34D4, 0x00DB357B)] != [2.0, 3.0, 10.0, 3.0]:
        raise RuntimeError("retail Affair Wife route distances changed")
    if direct_target(0x00DB3486) != 0x007E72F0 or direct_target(0x00DB34A2) != 0x00CBE2FF \
            or direct_target(0x00DB3586) != 0x00CBE2FF or direct_target(0x00DB35A6) != 0x007E7360:
        raise RuntimeError("retail Affair Wife route helpers changed")
    if not esi_byte_store(0x00DB3577, 0x1E, 1):
        raise RuntimeError("retail SaidRunningLine store changed")

    strings = [_cstring(raw, pe, insn.operands[0].imm & 0xFFFFFFFF)
               for insn in insns if insn.mnemonic == "push"
               and insn.operands[0].type == X86_OP_IMM
               and (insn.operands[0].imm & 0xFFFFFFFF) in STRINGS]
    return {
        "schema": "affair-wife-route-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "region": {"address": f"0x{START:08X}", "rangeSize": END - START,
                   "bytesSha256": hashlib.sha256(code).hexdigest().upper(),
                   "instructionCount": len(insns), "strings": strings},
        "husbandLookup": {"name": "NOVI_AffairMan", "site": "0x00DB3445", "slot": "0x120",
                          "resultLocal": "ESP+0x34"},
        "movement": {"positionReadSite": "0x00DB3473", "positionVtableSlot": "0x18",
                     "nullPosition": "0x0143E8E0", "callSite": "0x00DB3486",
                     "helper": "0x007E72F0", "radius": 2.0, "moveType": 1,
                     "trailingFlags": [False, True], "issuedOnce": True},
        "arrival": {"distance": 3.0, "helper": "0x00CBE2FF",
                    "initialTestSite": "0x00DB34A2", "loopTestSite": "0x00DB3586",
                    "thingOperand": "same live husband local ESP+0x34",
                    "falseLoopTarget": "0x00DB34B0", "reachedTarget": "0x00DB3593"},
        "runningLine": {"homeDistance": 10.0, "speech": "TEXT_QST_048_AFFAIR_WIFE_RUNNING_TO_HUBBY",
                        "saidFlagStore": "0x00DB3577", "saidFlagOffset": "0x1E"},
        "transition": {"clearCommandsSite": "0x00DB35A6",
                       "disableMovementInActionsSite": "0x00DB35BB", "slot": "0x844"},
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
