#!/usr/bin/env python3
"""Reproduce the New Oakvale vtable snapshot directly from retail Fable.exe."""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path

import pefile


def export(exe: Path, inventories: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase
    entities = {}
    for path in sorted(inventories.glob("*.json")):
        data = json.loads(path.read_text(encoding="utf-8-sig"))
        if path.stem.startswith("Q_"):
            continue
        functions = data.get("functions", {})
        if "Main" not in functions or "Init" not in functions:
            continue
        main = int(functions["Main"]["address"], 0)
        init = int(functions["Init"]["address"], 0)
        pair = struct.pack("<II", main, init)
        matches = []
        cursor = 0
        while True:
            offset = raw.find(pair, cursor)
            if offset < 0:
                break
            matches.append(offset)
            cursor = offset + 1
        if len(matches) != 1:
            raise RuntimeError(f"{data['entity']}: expected one Main/Init vtable pair, found {len(matches)}")
        va = base + pe.get_rva_from_offset(matches[0]) - 4
        offset = pe.get_offset_from_rva(va - base)
        destructor, main, init = struct.unpack_from("<III", raw, offset)
        entities[data["entity"]] = {
            "vtable": f"0x{va:08X}", "destructor": f"0x{destructor:08X}",
            "Main": f"0x{main:08X}", "Init": f"0x{init:08X}"}
    # Every deleting destructor starts with a relative call to the same shared
    # cleanup routine.  Record the target as binary evidence, without adopting
    # Ghidra/BSim's unrelated donor class name for it.
    cleanup_targets = set()
    for row in entities.values():
        va = int(row["destructor"], 0)
        offset = pe.get_offset_from_rva(va - base)
        code = raw[offset:offset + 32]
        call = code.find(b"\xe8")
        if call < 0:
            raise RuntimeError(f"{row['destructor']}: no near call in deleting destructor")
        displacement = struct.unpack_from("<i", code, call + 1)[0]
        cleanup_targets.add(va + call + 5 + displacement)
    if len(cleanup_targets) != 1:
        raise RuntimeError(f"entity destructors have {len(cleanup_targets)} shared cleanup targets")
    cleanup = cleanup_targets.pop()

    return {
        "schema": "new-oakvale-entity-vtables/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "imageBase": f"0x{base:08X}",
        "slots": ["destructor", "Main", "Init"],
        "sharedEntityCleanup": {
            "address": f"0x{cleanup:08X}",
            "classification": "entity-binding base cleanup; exact class name unresolved",
            "ghidraDonorName": "NUISystem::CFrontEndScreen::~CFrontEndScreen",
            "baseVtable": "0x012C3224",
            "scriptThingVtable": "0x01238C8C",
            "tailTarget": "0x0099A2E0"
        },
        "entities": entities,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--inventories", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = export(args.exe, args.inventories)
    text = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(text, encoding="utf-8")
    else:
        print(text, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
