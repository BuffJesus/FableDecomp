#!/usr/bin/env python3
"""Extract the complete retail Bully teddy-dispatch region from Main."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM


START = 0x00DBB5F0
END = 0x00DBBD83


def _offset(pe: pefile.PE, va: int) -> int:
    return pe.get_offset_from_rva(va - pe.OPTIONAL_HEADER.ImageBase)


def export(exe: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    code = raw[_offset(pe, START):_offset(pe, START) + END - START]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    insns = list(decoder.disasm(code, START))
    if not insns or insns[-1].address + insns[-1].size != END:
        raise RuntimeError("retail Bully teddy-dispatch boundary no longer decodes exactly")
    decoded = {insn.address: insn for insn in insns}
    expected_bytes = {
        0x00DBB5F0: "8a4524", 0x00DBB615: "68c8d12501", 0x00DBB646: "6858892d01",
        0x00DBB773: "68609b2d01", 0x00DBB7DD: "686c212c01", 0x00DBB7F0: "6888212c01",
        0x00DBB803: "683c9b2d01", 0x00DBB8B9: "83fe01", 0x00DBB8D4: "c6819600000001",
        0x00DBB965: "68189b2d01", 0x00DBBA3C: "6858892d01", 0x00DBBBDA: "c6819600000001",
        0x00DBBC73: "68189b2d01", 0x00DBBD16: "68f89a2d01",
    }
    for address, expected in expected_bytes.items():
        if decoded[address].bytes.hex() != expected:
            raise RuntimeError(f"retail Bully teddy-dispatch bytes changed at 0x{address:08X}")
    expected_slots = {
        0x00DBB63D: 0x6C, 0x00DBB675: 0x2E0, 0x00DBB708: 0x5C8,
        0x00DBB723: 0x5EC, 0x00DBB83B: 0x1C8, 0x00DBB876: 0x9C,
        0x00DBB89E: 0x9C, 0x00DBB9CB: 0x5A4, 0x00DBB9D7: 0x5EC,
        0x00DBB9F2: 0x8C, 0x00DBBA2E: 0x8C, 0x00DBBA84: 0x20,
        0x00DBBAB4: 0x20, 0x00DBBAFE: 0x5C8, 0x00DBBB19: 0x5EC,
        0x00DBBCD9: 0x5A4, 0x00DBBCE5: 0x5EC, 0x00DBBD71: 0x5EC,
    }
    for address, slot in expected_slots.items():
        insn = decoded[address]
        op = insn.operands[0]
        if insn.mnemonic != "call" or op.type != X86_OP_MEM or op.mem.disp != slot:
            raise RuntimeError(f"retail Bully teddy-dispatch slot changed at 0x{address:08X}")
    for address in (0x00DBB9BC, 0x00DBBCCA):
        insn = decoded[address]
        if insn.mnemonic != "call" or insn.operands[0].type != X86_OP_IMM or insn.operands[0].imm != 0x00DBCD00:
            raise RuntimeError(f"retail Bully GivenTeddy call changed at 0x{address:08X}")
    return {
        "schema": "bully-teddy-dispatch-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "region": {"address": f"0x{START:08X}", "endExclusive": f"0x{END:08X}",
                   "rangeSize": len(code), "bytesSha256": hashlib.sha256(code).hexdigest().upper(),
                   "instructionCount": len(insns)},
        "gate": {"field": "DoneIntro", "localOffset": "0x24", "testSite": "0x00DBB5F0"},
        "dispatch": [
            {"order": 1, "condition": "talked to by HERO and hero possesses teddy",
             "talkSlot": "0x6C", "possessionSlot": "0x2E0", "path": "yes/no offer"},
            {"order": 2, "condition": "presented item equals teddy", "messageSlot": "0x8C",
             "path": "direct acceptance"},
            {"order": 3, "condition": "presented other item still possessed by hero",
             "messageSlot": "0x8C", "possessionSlot": "0x2E0", "path": "refusal"},
        ],
        "offer": {"firstLine": "TEXT_QST_048_BULLY_FOUND_TEDDY_ONE",
                  "question": "TEXT_QST_048_GIVE_TEDDY_TO_BULLY",
                  "yes": "TEXT_OBJECT_HERO_ANSWER_YES", "no": "TEXT_OBJECT_HERO_ANSWER_NO",
                  "questionSlot": "0x1C8", "answerSlot": "0x9C", "acceptedAnswer": 1,
                  "acceptedLine": "TEXT_QST_048_BULLY_FOUND_TEDDY_TWO",
                  "victimComplaintParentOffset": "0x96", "givenTeddyCallSite": "0x00DBB9BC",
                  "clearInformationSlot": "0x5A4"},
        "directAcceptance": {"line": "TEXT_QST_048_BULLY_FOUND_TEDDY_TWO",
                             "victimComplaintParentOffset": "0x96",
                             "givenTeddyCallSite": "0x00DBBCCA", "clearInformationSlot": "0x5A4"},
        "refusal": {"line": "TEXT_QST_048_BULLY_DONT_WANT", "acquirePriority": 4},
        "movieScope": {"startSlot": "0x5C8", "pauseSlot": "0x5EC",
                       "normalEnd": ["PauseAllNonScriptedEntities(false)", "destroy movie resource"]},
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    rendered = json.dumps(export(args.exe), indent=2) + "\n"
    if args.output:
        args.output.write_text(rendered, encoding="utf-8")
    else:
        print(rendered, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
