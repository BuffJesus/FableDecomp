#!/usr/bin/env python3
"""Annotate decompiled script text with retail CGameScriptInterface slot names.

Rewrites `(**(code **)(**(int **)(this + 0x40) + 0xNNN))(` style dispatch through the proven
script-interface field (or the DAT_0143e8f8 singleton) into `GSI->Name(`, using the authoritative
vtable dump in ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv. Offsets that dump
does not cover are resolved by reading the retail vtable from Fable.exe and naming the target through
the PDB-derived ghidra_out/engine_api.tsv. Calls through other vtables are left untouched. Output is
for human reading only; it is not evidence by itself.
"""
from __future__ import annotations

import argparse
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SLOTS = ROOT / "ghidra_out" / "script_recovery" / "gamescriptinterface_vtable_slots.tsv"
ENGINE_API = ROOT / "ghidra_out" / "engine_api.tsv"
VTABLE_BASE = 0x01260F0C
RETAIL_EXE = Path(r"C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/Fable.exe")

METHOD_RE = re.compile(r"\?(\w+)@CGameScriptInterface@")
SLOT_RE = re.compile(r"\?(\w+)@")


def _pe_reader(exe: Path):
    """Return a callable mapping a virtual address to the u32 stored there (retail PE32)."""
    data = exe.read_bytes()
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    nsec = struct.unpack_from("<H", data, pe + 6)[0]
    opt = struct.unpack_from("<H", data, pe + 20)[0]
    base = struct.unpack_from("<I", data, pe + 24 + 28)[0]
    sections = []
    for i in range(nsec):
        o = pe + 24 + opt + i * 40
        _vs, va, rs, ro = struct.unpack_from("<IIII", data, o + 8)
        sections.append((va, rs, ro))

    def read(va: int):
        rva = va - base
        for sva, rs, ro in sections:
            if sva <= rva < sva + rs:
                return struct.unpack_from("<I", data, ro + rva - sva)[0]
        return None
    return read


def load_slots(path: Path = SLOTS, engine_api: Path = ENGINE_API, exe: Path = RETAIL_EXE,
               max_offset: int = 0x1000) -> dict[int, str]:
    """Slot offset -> method name (dump first, retail vtable + engine_api names as fallback)."""
    table: dict[int, str] = {}
    for line in path.read_text(encoding="utf-8-sig").splitlines()[1:]:
        cols = line.split("\t")
        if len(cols) < 5:
            continue
        m = SLOT_RE.match(cols[4])
        table[int(cols[1], 16)] = m.group(1) if m else cols[4]
    if engine_api.exists() and exe.exists():
        names: dict[int, str] = {}
        for line in engine_api.read_text(encoding="utf-8", errors="replace").splitlines():
            cols = line.rstrip("\n").split("\t")
            if len(cols) < 7:
                continue
            try:
                address = int(cols[0], 16)
            except ValueError:
                continue
            m = METHOD_RE.match(cols[6])
            if m:
                names.setdefault(address, m.group(1))
        read = _pe_reader(exe)
        for offset in range(0, max_offset, 4):
            if offset in table:
                continue
            target = read(VTABLE_BASE + offset)
            if target in names:
                table[offset] = names[target]
    return table


PATTERNS = [
    re.compile(r"\(\*\*\(code \*\*\)\(\*\*\(int \*\*\)\((?:this|param_1|\*\(int \*\*\)\([^()]+\)|[a-zA-Z_0-9]+) \+ 0x40\) \+ (0x[0-9a-f]+)\)\)"),
    re.compile(r"\(\*\*\(code \*\*\)\(\*DAT_0143e8f8 \+ (0x[0-9a-f]+)\)\)"),
    re.compile(r"\(\*\*\(code \*\*\)\((?:iVar\d+|piVar\d+|\*piVar\d+) \+ (0x[0-9a-f]+)\)\)"),
    # entity scripts reach the interface through their own +4 field
    re.compile(r"\(\*\*\(code \*\*\)\(\*\*\(int \*\*\)\((?:this|param_1) \+ 4\) \+ (0x[0-9a-f]+)\)\)"),
    # pre-normalized spelling of the same entity interface field
    re.compile(r"\(\*\*\(code \*\*\)\(GSI_VTBL \+ (0x[0-9a-f]+)\)\)"),
]


def annotate(text: str, slots: dict[int, str]) -> str:
    def sub(match: re.Match[str]) -> str:
        offset = int(match.group(1), 16)
        name = slots.get(offset)
        return f"GSI->{name}" if name else match.group(0)
    for pattern in PATTERNS:
        text = pattern.sub(sub, text)
    return text


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("files", nargs="+", type=Path)
    parser.add_argument("--in-place", action="store_true")
    args = parser.parse_args()
    slots = load_slots()
    for file in args.files:
        result = annotate(file.read_text(encoding="utf-8"), slots)
        if args.in_place:
            file.write_text(result, encoding="utf-8")
        else:
            sys.stdout.write(result)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
