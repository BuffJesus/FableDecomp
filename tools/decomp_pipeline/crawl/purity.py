#!/usr/bin/env python3
"""Single source of truth for "is this landed source a genuine reconstruction?".

A source is NOT genuine if it is hand-written machine code in any form:
  * `__declspec(naked)` + `_emit <byte>` byte bakes, and
  * `__declspec(naked)` / `__asm` blocks written with real mnemonics.

The second form was invisible to the original `_emit`-only check, which under-counted
the remaining work by ~585 functions. Both are byte-exact oracles at best, not faithful
compiler-emitted reconstructions -- see the byte-purity policy.

A bare `__asm` inside otherwise-real C++ (e.g. a single `__asm int 3`) still counts as
non-genuine here: any hand-written instruction stream is a purity debt.
"""

MARKERS = ("_emit", "__declspec(naked)", "declspec(naked)", "__asm")


def is_genuine(text):
    return not any(m in text for m in MARKERS)


def classify(text):
    if "_emit" in text:
        return "emit_bake"
    # The same markers as is_genuine: a bare "naked" (e.g. a comment saying
    # "no naked") is not a bake.
    if "declspec(naked)" in text or "__asm" in text:
        return "naked_asm"
    return "genuine"


# Headers whose inline asm is a proven-irreducible part of the original source, not a
# bake. Each entry needs its evidence in the header itself. fable_fpu.h: Lionhead's x87
# control-word helpers (fnstcw/fldcw have no VC7.1 intrinsic; _control87 is a CRT call).
SANCTIONED_ASM_HEADERS = {"fable_fpu.h"}

import re as _re
from pathlib import Path as _Path

_INCLUDE = _re.compile(r'^\s*#\s*include\s*"([^"]+)"', _re.M)
_INCLUDE_ROOT = _Path(__file__).resolve().parents[3] / "rebuild" / "include"


def included_asm_headers(text, include_root=_INCLUDE_ROOT):
    """Headers reachable through `#include "..."` that contain hand-written asm.

    The landed .cpp can be free of asm while an inline helper it pulls in is not;
    is_genuine() alone cannot see that. Returns sorted header names (relative to
    rebuild/include), sanctioned ones included -- callers decide.
    """
    seen, found, todo = set(), set(), list(_INCLUDE.findall(text))
    while todo:
        name = todo.pop()
        if name in seen:
            continue
        seen.add(name)
        path = include_root / name
        if not path.is_file():
            continue
        body = path.read_text(encoding="utf-8", errors="replace")
        if any(m in body for m in MARKERS):
            found.add(name)
        todo.extend(_INCLUDE.findall(body))
    return sorted(found)


def unsanctioned_header_asm(text, include_root=_INCLUDE_ROOT):
    return [h for h in included_asm_headers(text, include_root)
            if h.replace("\\", "/").rsplit("/", 1)[-1] not in SANCTIONED_ASM_HEADERS]
