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
    if "naked" in text or "__asm" in text:
        return "naked_asm"
    return "genuine"
