#!/usr/bin/env python3
"""Extract the retail Bully subdued/run-off transition from Fable.exe bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM


START = 0x00DBC86B
END = 0x00DBCCF4


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
        raise RuntimeError("retail Bully run-off boundary no longer decodes exactly")
    decoded = {insn.address: insn for insn in insns}

    expected_bytes = {
        0x00DBC87B: "c6416c01", 0x00DBC9F9: "6874512501",
        0x00DBCA2C: "68e8992d01", 0x00DBCA5F: "68fc782601",
        0x00DBCA9C: "8a486f", 0x00DBCAB4: "68dc992d01",
        0x00DBCAC2: "68b4992d01", 0x00DBCADB: "6aff",
        0x00DBCADd: "68dc992d01", 0x00DBCAEB: "6898992d01",
        0x00DBCB69: "687c992d01", 0x00DBCB9B: "8a416e",
        0x00DBCBAF: "6860992d01", 0x00DBCBEe: "c6406e01",
        0x00DBCC43: "6840992d01", 0x00DBCCBB: "c6406d01",
    }
    for address, expected in expected_bytes.items():
        actual = decoded.get(address)
        if actual is None or actual.bytes.hex() != expected.lower():
            raise RuntimeError(f"retail Bully run-off bytes changed at 0x{address:08X}")

    expected_slots = {
        0x00DBC88B: 0x548, 0x00DBC8B4: 0x20, 0x00DBC8E4: 0x20,
        0x00DBC92F: 0x20, 0x00DBC963: 0x20, 0x00DBC9A9: 0x20,
        0x00DBC9D8: 0x20, 0x00DBCB3C: 0x5C8, 0x00DBCB54: 0x5EC,
        0x00DBCB61: 0x5CC, 0x00DBCBE5: 0x5A4, 0x00DBCC76: 0x5CC,
        0x00DBCC82: 0x5EC, 0x00DBCCD1: 0x1B0,
    }
    for address, slot in expected_slots.items():
        insn = decoded[address]
        op = insn.operands[0]
        if insn.mnemonic != "call" or op.type != X86_OP_MEM or op.mem.disp != slot:
            raise RuntimeError(f"retail Bully run-off slot changed at 0x{address:08X}")

    expected_calls = {0x00DBCB8A: 0x00CBFB7D, 0x00DBCBCD: 0x00CBFB7D,
                      0x00DBCC61: 0x00CBFB7D, 0x00DBCCC2: 0x00DB0660}
    for address, target in expected_calls.items():
        insn = decoded[address]
        if insn.mnemonic != "call" or insn.operands[0].type != X86_OP_IMM or insn.operands[0].imm != target:
            raise RuntimeError(f"retail Bully direct call changed at 0x{address:08X}")

    return {
        "schema": "bully-run-off-retail-bytes/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "region": {"address": f"0x{START:08X}", "endExclusive": f"0x{END:08X}",
                   "rangeSize": len(code), "bytesSha256": hashlib.sha256(code).hexdigest().upper(),
                   "instructionCount": len(insns)},
        "subdued": {"stateStoreSite": "0x00DBC87B", "stateParentOffset": "0x6C",
                    "barRemovalSite": "0x00DBC88B", "barRemovalSlot": "0x548"},
        "acquisition": {"priority": 4, "order": ["BULLY", "HERO", "BRAT"],
                        "initialSites": ["0x00DBC8B4", "0x00DBC92F", "0x00DBC9A9"],
                        "retrySites": ["0x00DBC8E4", "0x00DBC963", "0x00DBC9D8"]},
        "actorMap": {"HERO": "hero resource", "BRAT": "victim resource", "BULLY": "bully resource",
                     "keyPushSites": ["0x00DBC9F9", "0x00DBCA2C", "0x00DBCA5F"]},
        "inputArgs": {"key": "$BRATLINE", "keyPushSites": ["0x00DBCAB4", "0x00DBCADd"],
                      "selector": "HeroAttackedVictim", "selectorParentOffset": "0x6F",
                      "trueValue": "TEXT_QST_048_VICTIM_THANKS_AFTER_HIT",
                      "falseValue": "TEXT_QST_048_VICTIM_THANKS"},
        "cutscenes": [
            {"name": "CS_OAKVALEINTRO_BULLYRUN1", "callSite": "0x00DBCB8A",
             "actorMap": True, "inputArgs": "$BRATLINE", "pause": True, "fixedCamera": True},
            {"name": "CS_OAKVALEINTRO_BULLYRUN2", "callSite": "0x00DBCBCD",
             "condition": "not GivenHeroTeddy", "actorMap": True, "inputArgs": None},
            {"name": "CS_OAKVALEINTRO_BULLYRUNDUMMY", "callSite": "0x00DBCC61",
             "condition": "GivenHeroTeddy", "actorMap": True, "inputArgs": None},
        ],
        "teddyBranch": {"stateLoadSite": "0x00DBCB9B", "stateParentOffset": "0x6E",
                        "clearVictimInformationSite": "0x00DBCBE5", "clearSlot": "0x5A4",
                        "stateStoreSite": "0x00DBCBEe"},
        "normalCleanup": ["FixMovieSequenceCamera(false)", "PauseAllNonScriptedEntities(false)",
                          "destroy movie resource", "destroy input-argument map", "destroy actor map",
                          "release BRAT", "release HERO"],
        "success": {"bullyRanOffStoreSite": "0x00DBCCBB", "bullyRanOffParentOffset": "0x6D",
                    "addGoodDeedSite": "0x00DBCCC2", "removeThingSite": "0x00DBCCD1",
                    "removeThingArgs": ["BULLY", False, True], "releaseBullySite": "0x00DBCCEF"},
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = export(args.exe)
    rendered = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(rendered, encoding="utf-8")
    else:
        print(rendered, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
