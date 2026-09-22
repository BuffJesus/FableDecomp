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
REGISTER_PATTERN = re.compile(r"\(\*\*\(code \*\*\)\((?:iVar\d+|piVar\d+|\*piVar\d+) \+ (" + OFFSET + r")\)\)")
PATTERNS = [
    re.compile(r"\(\*\*\(code \*\*\)\(\*\*\(int \*\*\)\((?:this|param_1|\*\(int \*\*\)\([^()]+\)|[a-zA-Z_0-9]+) \+ 0x40\) \+ (" + OFFSET + r")\)\)"),
    re.compile(r"\(\*\*\(code \*\*\)\(\*DAT_0143e8f8 \+ (" + OFFSET + r")\)\)"),
    REGISTER_PATTERN,
    # entity scripts reach the interface through their own +4 field
    re.compile(r"\(\*\*\(code \*\*\)\(\*\*\(int \*\*\)\((?:this|param_1) \+ 4\) \+ (" + OFFSET + r")\)\)"),
    # pre-normalized spelling of the same entity interface field
    re.compile(r"\(\*\*\(code \*\*\)\(GSI_VTBL \+ (0x[0-9a-f]+)\)\)"),
]


# ---- receiver aliases (CScriptThing) ---------------------------------------------------------------
# Interface-field aliases: `iVarN = **(int **)(this + 4);` (cached vtable) / `piVarN = *(int **)(this + 4);`
# (cached object); quest scripts spell the field `+ 0x40`.
RE_GSI_ALIAS = re.compile(
    r"^[ \t]*(?P<var>[A-Za-z_]\w*) = "
    r"\*{1,2}\(int \*\*\)\((?:this|param_\d+|\w+) \+ (?:4|0x40)\);", re.M)
# The entity's own CScriptThing lives at +8: `pCVarN = (CScriptThing *)(this + 8);` (cast optional).
RE_ME_ALIAS = re.compile(
    r"^[ \t]*(?P<var>[A-Za-z_]\w*) = (?:\(CScriptThing \*\))?\(?(?:this|param_\d+) \+ 8\)?;", re.M)
# `pCVarN = pCVarM;` copies whatever pCVarM aliases.
RE_COPY_ALIAS = re.compile(r"^[ \t]*(?P<var>[A-Za-z_]\w*) = (?P<src>[A-Za-z_]\w*);", re.M)
# A CScriptThing returned (by value, through the hidden slot Ghidra shows as `&stack...`) by an
# interface call: `piVarN = (int *)(**(code **)(**(int **)(this + 4) + 0x120))(` (GetThingWithScriptName),
# or by a thing slot that returns a CScriptThing (`*piVarM + 0x68` MsgWhoHitMe).
RE_THING_ALIAS = re.compile(
    r"^[ \t]*(?P<var>[A-Za-z_]\w*) = (?:\((?:int|CScriptThing) \*\))?\s*\(\*\*\(code \*\*\)\("
    r"(?:\*\*\(int \*\*\)\((?:this|param_\d+) \+ (?:4|0x40)\)|iVar\d+|\*DAT_0143e8f8"
    r"|\*(?:\(int \*\))?(?P<via>[A-Za-z_]\w*|\((?:this|param_\d+) \+ 8\))) \+ "
    r"(?P<off>" + OFFSET + r")\)\)\(", re.M)
# Any other assignment to a candidate receiver name ends its alias.
RE_ANY_DEF = re.compile(r"^[ \t]*(?P<var>[A-Za-z_]\w*) = ", re.M)

# Call heads through a CScriptThing vtable (`*(int *)recv + off` = recv->vtbl[off]).
THING_PATTERNS = [
    re.compile(r"\(\*\*\(code \*\*\)\(\*\(int \*\)\((?P<recv>(?:this|param_\d+) \+ 8)\) \+ (?P<off>" + OFFSET + r")\)\)\("),
    re.compile(r"\(\*\*\(code \*\*\)\(\*\(int \*\)(?P<recv>[A-Za-z_]\w*) \+ (?P<off>" + OFFSET + r")\)\)\("),
    re.compile(r"\(\*\*\(code \*\*\)\(\*(?P<recv>[A-Za-z_]\w*) \+ (?P<off>" + OFFSET + r")\)\)\("),
    # a thing handle whose Data field the lowering folded back onto the handle itself: `*(int *)(recv + 0x0)`
    # is the object (TraderConflictGood TraderToRescue 0x00DFE0F0: `GetHeroTargetedThing()->IsEqualTo(me)`)
    re.compile(r"\(\*\*\(code \*\*\)\(\*\(int \*\)\((?P<recv>[A-Za-z_]\w*) \+ 0x0\) \+ (?P<off>" + OFFSET + r")\)\)\("),
    # the same receiver spelled through the object's first dword, which is its vtable (WaspAttacker
    # 0x00E11480: `while (**(code **)(r1._0_4_ + 0x12c))()` = while the WaspVictim is alive)
    re.compile(r"\(\*\*\(code \*\*\)\((?P<recv>[A-Za-z_]\w*)\._0_4_ \+ (?P<off>" + OFFSET + r")\)\)\("),
]
ME_RECEIVER = "(CScriptThing *)(this + 8)"
# lowering pseudo-calls / interface calls whose value is a CScriptThing (a stack slot filled by one is a thing receiver)
PSEUDO_GSI_THING = ("GetHero", "GetThingWithScriptName", "GetNearestWithScriptName", "CreateCreature",
                    "GetRandomThingWithScriptName", "GetNearestWithDefName")


def _pseudo_thing_re(extra=()):
    """Interface calls whose value is a CScriptThing. The manifest already says which interface slots return
    one (`thing_returning`); naming them here keeps the alias table from depending on a hand-kept list
    (`GetHeroTargetedThing` was missing, so TraderToRescue's `IsEqualTo` had no receiver and stayed native)."""
    names = "|".join(sorted({*PSEUDO_GSI_THING, *extra}, key=len, reverse=True))
    return re.compile(r"^[ 	]*(?P<var>[A-Za-z_]\w*) = (?:\((?:int|CScriptThing) \*\))?\s*"
                      r"(?:QUESTTHING_Empty|ENTITYTHING_Empty|QUESTTHING_Get|ENTITYTHING_Get|LOCALLIST_At"
                      r"|(?:QUEST|ENTITY)LIST_At_\w+|RESOURCE_ScriptThing|GSI->(?:" + names + r"))\(", re.M)


RE_PSEUDO_THING = _pseudo_thing_re()


def _thing_out_param_re(extra=()):
    """`GSI->Name(&VAR, ..)` where Name returns a CScriptThing: VAR is the hidden return, so it is a thing."""
    names = "|".join(sorted({*PSEUDO_GSI_THING, *extra}, key=len, reverse=True))
    return re.compile(r"^[ \t]*GSI->(?:" + names + r")\(\s*&(?P<var>[A-Za-z_]\w*)\s*[,)]", re.M)


# the same hidden return before the slots are named: an interface call with no assignment whose first
# argument is the out slot (`(**(code **)(**(int **)(this + 4) + 0x120))(&xStack_2c,&xStack_30);`)
RE_THING_OUT_RAW = re.compile(
    r"^[ \t]*\(\*\*\(code \*\*\)\((?:\*\*\(int \*\*\)\((?:this|param_\d+) \+ (?:4|0x40)\)"
    r"|iVar\d+|\*DAT_0143e8f8) \+ (?P<off>" + OFFSET + r")\)\)\("
    r"(?:\s*\*\(int \*\*\)\((?:\(int\))?(?:this|param_\d+) \+ (?:4|0x40)\)\s*,)?"
    r"\s*&(?P<var>[A-Za-z_]\w*)\s*[,)]", re.M)

RE_THING_RETURN = re.compile(r"@CScriptThing@@[UM][AB]E\?AV1@")


def _statement_end(text: str, start: int) -> int:
    end = text.find(";", start)
    return len(text) if end < 0 else end


def _annotate_things(text: str, thing_slots: dict[int, tuple[str, str]],
                     thing_returning: frozenset[int] | set[int], entity: bool,
                     pseudo_names: tuple[str, ...] = ()) -> str:
    """One streaming pass: alias definitions and call heads in text order; a call's receiver is
    classified by the aliases in force at that point (a redefinition ends the previous alias)."""
    events: list[tuple[int, int, str, re.Match[str]]] = []
    pseudo = _pseudo_thing_re(pseudo_names)
    for kind, pattern in (("gsi", RE_GSI_ALIAS), ("me", RE_ME_ALIAS), ("copy", RE_COPY_ALIAS),
                          ("thing", RE_THING_ALIAS), ("pseudo", pseudo),
                          ("pseudo", _thing_out_param_re(pseudo_names)),
                          ("outraw", RE_THING_OUT_RAW), ("any", RE_ANY_DEF)):
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
    # a slot filled by a thing-valued pseudo-call anywhere in the function is a thing receiver even before
    # that statement in text order (loop-carried values: the store sits at the loop tail)
    pseudo_things = {m.group("var") for m in pseudo.finditer(text)}
    # a stack slot that keeps such a value across loop iterations (`piStack_14 = pCVar8;` after
    # `pCVar8 = GSI->CreateCreature(..)`, read at the loop head: ScorpionHome 0x00D643A0's spawn position)
    # is a thing receiver before its store in text order too; registers are left to the streaming aliases
    thing_defs: dict[str, int] = {}
    for m in pseudo.finditer(text):
        thing_defs[m.group("var")] = thing_defs.get(m.group("var"), 0) + 1
    for m in RE_THING_ALIAS.finditer(text):
        if not m.group("via") and int(m.group("off"), 0) in thing_returning:
            thing_defs[m.group("var")] = thing_defs.get(m.group("var"), 0) + 1
    # only a source whose EVERY definition is thing-valued (a register Ghidra also reuses for `me` or a
    # scalar -- NOVI_Bully's pCVar6 -- proves nothing about a slot copied from it)
    sources = {var for var, n in thing_defs.items()
               if n == len(re.findall(r'^[ \t]*' + re.escape(var) + r' = ', text, re.M))}
    while True:
        carried = {m.group("var") for m in RE_COPY_ALIAS.finditer(text)
                   if m.group("src") in sources and "Stack_" in m.group("var") and m.group("var") not in sources
                   and len(re.findall(r'^[ \t]*' + re.escape(m.group("var")) + r' = ', text, re.M)) == 1}
        sources |= carried
        if not carried:
            break
        pseudo_things |= carried
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
        if kind == "pseudo":
            things.add(m.group("var"))
            continue
        if kind == "outraw":
            if int(m.group("off"), 0) in thing_returning:
                things.add(m.group("var"))
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
        elif recv in things or recv in pseudo_things:
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


RE_GSI_VALUE = re.compile(r"(?:\(int \*+\))?\*{1,2}\(int \*\*\)\((?:this|param_\d+|\w+) \+ (?:4|0x40)\)$|\*?DAT_0143e8f8$")


def _is_gsi_alias_at(text: str, var: str, pos: int, depth: int = 0) -> bool:
    """Whether `var`'s latest definition before `pos` (text order) is an interface pointer / vtable
    load, or a copy of a register that is one there. With no definition in sight the register is
    taken as the interface (the decompiler hoists some loads out of the printed body)."""
    prev = None
    for m in re.finditer(r"^[ \t]*" + re.escape(var) + r" = ([^;\n]*);", text[:pos], re.M):
        prev = m
    if prev is None:
        return True
    value = prev.group(1).strip()
    if RE_GSI_VALUE.match(value):
        return True
    if depth < 3 and re.fullmatch(r"(?:\(int \*+\))?\*?(\w+)", value):
        src = re.fullmatch(r"(?:\(int \*+\))?\*?(\w+)", value).group(1)
        return _is_gsi_alias_at(text, src, prev.start(), depth + 1)
    return False


def annotate(text: str, slots: dict[int, str], thing_slots: dict[int, tuple[str, str]] | None = None,
             thing_returning: frozenset[int] | set[int] = frozenset(), entity: bool = True) -> str:
    """Name interface slots (`GSI->Name(`) and, when `thing_slots` is given, CScriptThing slots
    (`CScriptThing::Name(receiver, `). `thing_returning` lists the interface offsets whose result is a
    CScriptThing (so `piVarN = (int *)GSI->GetHero()` makes piVarN a thing receiver); `entity` enables
    the `this + 8` receiver (entity scripts keep their CScriptThing at +8)."""
    if thing_slots:
        # the interface slots the manifest marks as returning a CScriptThing, by name: their results are
        # thing receivers exactly like GetHero's
        text = _annotate_things(text, thing_slots, thing_returning, entity,
                                tuple(slots[off] for off in thing_returning if off in slots))

    def sub(match: re.Match[str]) -> str:
        offset = int(match.group(1), 0)
        name = slots.get(offset)
        return f"GSI->{name}" if name else match.group(0)
    def sub_register(match: re.Match[str]) -> str:
        # a register temporary (`piVar6`) is the interface only while its latest definition is an
        # interface load (or a copy of one); the same register later walks a vector of things
        var = re.match(r"\(\*\*\(code \*\*\)\(\*?(\w+)", match.group(0)).group(1)
        if _is_gsi_alias_at(text, var, match.start()):
            return sub(match)
        return match.group(0)
    for pattern in PATTERNS:
        text = pattern.sub(sub_register if pattern is REGISTER_PATTERN else sub, text)
    # interface-pointer / vtable aliases under any local name (typed exports name them `this_00`, ...)
    aliases = {m.group("var") for m in RE_GSI_ALIAS.finditer(text)
               if not re.match(r"(?:[a-z]{1,3}Var\d+|\w*Stack_[0-9a-f]+|this)$", m.group("var"))}
    # a stack slot holding the interface pointer itself (`xStack_7c = *(int **)(this + 4);` then
    # `(**(code **)(*xStack_7c + 0x5ec))(xStack_7c, true)`: FinalMaze 0x00D6xxxx's PauseAllNonScriptedEntities,
    # the export mistyped the slot as a by-value string) is the receiver when it is only ever assigned that way
    # (the object pointer only -- `= *(int **)`; a cached VTABLE in a stack slot, `= **(int **)`, is left alone:
    # NOVI_Guard's `(**(code **)(iStack_12c + 0x76c))(uVar11)` prints without its receiver and would mis-place)
    object_alias = re.compile(r"^[ \t]*(?P<var>\w*Stack_[0-9a-f]+) = \*\(int \*\*\)\((?:this|param_\d+|\w+) \+ (?:4|0x40)\);", re.M)
    for var in {m.group("var") for m in object_alias.finditer(text)}:
        if all(object_alias.match(text, m.start()) for m in RE_ANY_DEF.finditer(text) if m.group("var") == var):
            aliases.add(var)
    # a copy of an interface alias into a stack slot (`piStack_218 = piVar12;`, the pointer kept across a
    # loop) is the same receiver
    sources = aliases | {m.group("var") for m in RE_GSI_ALIAS.finditer(text)}
    aliases |= {m.group("var") for m in RE_COPY_ALIAS.finditer(text)
                if m.group("src") in sources and re.match(r"\w*Stack_[0-9a-f]+$", m.group("var"))}
    if aliases:
        names = "|".join(sorted(re.escape(a) for a in aliases))
        text = re.sub(r"\(\*\*\(code \*\*\)\(\*?(?:" + names + r") \+ (" + OFFSET + r")\)\)", sub, text)
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
