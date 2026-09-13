#!/usr/bin/env python3
"""Annotate decompiled script text with retail CGameScriptInterface slot names.

Rewrites `(**(code **)(**(int **)(this + 0x40) + 0xNNN))(` style dispatch through the proven
script-interface field (or the DAT_0143e8f8 singleton) into `GSI->Name(`, using the authoritative
vtable dump in ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv. Offsets that dump
does not cover are resolved by reading the retail vtable from Fable.exe and naming the target through
the PDB-derived ghidra_out/engine_api.tsv. Calls through other vtables are left untouched. Output is
for human reading only; it is not evidence by itself.

With `thing_slots` (ghidra_out/script_recovery/cscriptthing_vtable_slots.tsv, the CScriptThing vtable
at 0x01238C8C) the pass is receiver-sensitive: calls through the entity's own thing (`this + 8`, or a
`pCVarN` aliased to it), or through a `pCVarN`/`piVarN` that holds the CScriptThing returned by a
thing-returning interface call (GetHero, GetThingWithScriptName, ...), become
`CScriptThing::Name(receiver, ...)`; a `*piVarN` receiver is only named as an interface slot when it
is not such a thing alias (previously every `*piVarN` was assumed to be the interface).
"""
from __future__ import annotations

import argparse
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SLOTS = ROOT / "ghidra_out" / "script_recovery" / "gamescriptinterface_vtable_slots.tsv"
THING_SLOTS = ROOT / "ghidra_out" / "script_recovery" / "cscriptthing_vtable_slots.tsv"
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


def load_thing_slots(path: Path = THING_SLOTS) -> dict[int, tuple[str, str]]:
    """CScriptThing vtable offset -> (method name, mangled name) from the retail vtable dump.

    The mangled name encodes the operand shape the lifter needs (hidden return slot for `?AV`
    returns, `ABVCCharString@@` string operands, `XZ` for no operands). Missing file -> {}.
    """
    table: dict[int, tuple[str, str]] = {}
    if not path.exists():
        return table
    lines = path.read_text(encoding="utf-8-sig").splitlines()
    if not lines:
        return table
    header = lines[0].split("\t")
    try:
        i_off, i_name, i_mangled = header.index("offset"), header.index("name"), header.index("mangled")
    except ValueError:
        return table
    for line in lines[1:]:
        cols = line.split("\t")
        if len(cols) <= max(i_off, i_name, i_mangled) or not cols[i_name]:
            continue
        table[int(cols[i_off], 16)] = (cols[i_name], cols[i_mangled])
    return table


OFFSET = r"(?:0x[0-9a-f]+|\d+)"  # Ghidra renders some offsets (notably 0x12c) as decimal.
PATTERNS = [
    re.compile(r"\(\*\*\(code \*\*\)\(\*\*\(int \*\*\)\((?:this|param_1|\*\(int \*\*\)\([^()]+\)|[a-zA-Z_0-9]+) \+ 0x40\) \+ (" + OFFSET + r")\)\)"),
    re.compile(r"\(\*\*\(code \*\*\)\(\*DAT_0143e8f8 \+ (" + OFFSET + r")\)\)"),
    re.compile(r"\(\*\*\(code \*\*\)\((?:iVar\d+|piVar\d+|\*piVar\d+) \+ (" + OFFSET + r")\)\)"),
    # entity scripts reach the interface through their own +4 field
    re.compile(r"\(\*\*\(code \*\*\)\(\*\*\(int \*\*\)\((?:this|param_1) \+ 4\) \+ (" + OFFSET + r")\)\)"),
    # pre-normalized spelling of the same entity interface field
    re.compile(r"\(\*\*\(code \*\*\)\(GSI_VTBL \+ (0x[0-9a-f]+)\)\)"),
]


# ---- receiver aliases (CScriptThing) ---------------------------------------------------------------
# Interface-field aliases: `iVarN = **(int **)(this + 4);` (cached vtable) / `piVarN = *(int **)(this + 4);`
# (cached object); quest scripts spell the field `+ 0x40`.
RE_GSI_ALIAS = re.compile(
    r"^[ \t]*(?P<var>iVar\d+|piVar\d+|iStack_[0-9a-f]+|ppuVar\d+) = "
    r"\*{1,2}\(int \*\*\)\((?:this|param_\d+|\w+) \+ (?:4|0x40)\);", re.M)
# The entity's own CScriptThing lives at +8: `pCVarN = (CScriptThing *)(this + 8);` (cast optional).
RE_ME_ALIAS = re.compile(
    r"^[ \t]*(?P<var>pCVar\d+) = (?:\(CScriptThing \*\))?\(?(?:this|param_\d+) \+ 8\)?;", re.M)
# `pCVarN = pCVarM;` copies whatever pCVarM aliases.
RE_COPY_ALIAS = re.compile(r"^[ \t]*(?P<var>pCVar\d+|piVar\d+) = (?P<src>pCVar\d+|piVar\d+);", re.M)
# A CScriptThing returned (by value, through the hidden slot Ghidra shows as `&stack...`) by an
# interface call: `piVarN = (int *)(**(code **)(**(int **)(this + 4) + 0x120))(` (GetThingWithScriptName),
# or by a thing slot that returns a CScriptThing (`*piVarM + 0x68` MsgWhoHitMe).
RE_THING_ALIAS = re.compile(
    r"^[ \t]*(?P<var>pCVar\d+|piVar\d+) = (?:\((?:int|CScriptThing) \*\))?\s*\(\*\*\(code \*\*\)\("
    r"(?:\*\*\(int \*\*\)\((?:this|param_\d+) \+ (?:4|0x40)\)|iVar\d+|\*DAT_0143e8f8"
    r"|\*(?:\(int \*\))?(?P<via>piVar\d+|pCVar\d+|\((?:this|param_\d+) \+ 8\))) \+ "
    r"(?P<off>" + OFFSET + r")\)\)\(", re.M)
# Any other assignment to a candidate receiver name ends its alias.
RE_ANY_DEF = re.compile(r"^[ \t]*(?P<var>pCVar\d+|piVar\d+|iVar\d+) = ", re.M)

# Call heads through a CScriptThing vtable (`*(int *)recv + off` = recv->vtbl[off]).
THING_PATTERNS = [
    re.compile(r"\(\*\*\(code \*\*\)\(\*\(int \*\)\((?P<recv>(?:this|param_\d+) \+ 8)\) \+ (?P<off>" + OFFSET + r")\)\)\("),
    re.compile(r"\(\*\*\(code \*\*\)\(\*\(int \*\)(?P<recv>pCVar\d+|pCStack_[0-9a-f]+) \+ (?P<off>" + OFFSET + r")\)\)\("),
    re.compile(r"\(\*\*\(code \*\*\)\(\*(?P<recv>piVar\d+) \+ (?P<off>" + OFFSET + r")\)\)\("),
]
ME_RECEIVER = "(CScriptThing *)(this + 8)"
RE_THING_RETURN = re.compile(r"@CScriptThing@@[UM][AB]E\?AV1@")


def _statement_end(text: str, start: int) -> int:
    end = text.find(";", start)
    return len(text) if end < 0 else end


def _annotate_things(text: str, thing_slots: dict[int, tuple[str, str]],
                     thing_returning: frozenset[int] | set[int], entity: bool) -> str:
    """One streaming pass: alias definitions and call heads in text order; a call's receiver is
    classified by the aliases in force at that point (a redefinition ends the previous alias)."""
    events: list[tuple[int, int, str, re.Match[str]]] = []
    for kind, pattern in (("gsi", RE_GSI_ALIAS), ("me", RE_ME_ALIAS), ("copy", RE_COPY_ALIAS),
                          ("thing", RE_THING_ALIAS), ("any", RE_ANY_DEF)):
        for m in pattern.finditer(text):
            # a definition takes effect after its statement (its own rhs sees the old aliases);
            # the generic reset runs before the specific classification of the same statement
            events.append((_statement_end(text, m.start()), 1 if kind == "any" else 2, kind, m))
    for pattern in THING_PATTERNS:
        for m in pattern.finditer(text):
            events.append((m.start(), 0, "call", m))
    events.sort(key=lambda e: (e[0], e[1]))
    me: set[str] = set()
    things: set[str] = set()
    gsi: set[str] = set()
    edits: list[tuple[int, int, str]] = []
    for _pos, _rank, kind, m in events:
        if kind == "any":
            var = m.group("var")
            me.discard(var); things.discard(var); gsi.discard(var)
            continue
        if kind == "gsi":
            gsi.add(m.group("var"))
            continue
        if kind == "me":
            if entity:
                me.add(m.group("var"))
            continue
        if kind == "copy":
            var, src = m.group("var"), m.group("src")
            if src in me:
                me.add(var)
            elif src in things:
                things.add(var)
            elif src in gsi:
                gsi.add(var)
            continue
        if kind == "thing":
            var, off, via = m.group("var"), int(m.group("off"), 0), m.group("via")
            if via:
                via_is_thing = via in things or via in me or (via.endswith("+ 8)") and entity)
                if via_is_thing and RE_THING_RETURN.search(thing_slots.get(off, ("", ""))[1]):
                    things.add(var)
            elif off in thing_returning:
                things.add(var)
            continue
        # call head
        recv, off = m.group("recv"), int(m.group("off"), 0)
        if recv.endswith("+ 8"):
            if not entity:
                continue
            receiver = ME_RECEIVER
        elif recv in me:
            receiver = ME_RECEIVER
        elif recv in things:
            receiver = recv
        else:
            continue          # unknown receiver: leave the dispatch as it is (the GSI pass may name it)
        slot = thing_slots.get(off)
        if not slot:
            continue
        tail = ", " if m.end() < len(text) and text[m.end()] != ")" else ""
        edits.append((m.start(), m.end(), f"CScriptThing::{slot[0]}({receiver}{tail}"))
    for start, end, replacement in sorted(edits, reverse=True):
        text = text[:start] + replacement + text[end:]
    return text


def annotate(text: str, slots: dict[int, str], thing_slots: dict[int, tuple[str, str]] | None = None,
             thing_returning: frozenset[int] | set[int] = frozenset(), entity: bool = True) -> str:
    """Name interface slots (`GSI->Name(`) and, when `thing_slots` is given, CScriptThing slots
    (`CScriptThing::Name(receiver, `). `thing_returning` lists the interface offsets whose result is a
    CScriptThing (so `piVarN = (int *)GSI->GetHero()` makes piVarN a thing receiver); `entity` enables
    the `this + 8` receiver (entity scripts keep their CScriptThing at +8)."""
    if thing_slots:
        text = _annotate_things(text, thing_slots, thing_returning, entity)

    def sub(match: re.Match[str]) -> str:
        offset = int(match.group(1), 0)
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
