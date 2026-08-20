#!/usr/bin/env python3
"""Relocation-tolerant byte comparison for the de-bake pre-verify guards.

`verify_and_land` does the authoritative check: it reads the real .text relocation
offsets out of the .obj and masks exactly those dwords (RELOCATION_MATCH). The
pre-verify guards inside the de-bake tools cannot easily do that, but they must not
FALSELY REFUSE a correct source either -- a guard that only masks `call rel32` rejects
every source whose difference is a *data* relocation (`mov [esi], offset g_vtable` ->
`c7 06 <imm32>`), which is extremely common in destructors and vptr re-seats.

`relaxed_equal` accepts a candidate when every differing dword in the freshly compiled
object is all-zero, i.e. an unresolved relocation slot. Retail has the linked address
there; a fresh .obj has zeros. That is precisely the relocation signature, so this is a
sound relaxation for a *guard*: anything it lets through is still proven or rejected by
verify_and_land's exact reloc-masked compare.
"""


def mask_calls(b):
    m = bytearray(b)
    i = 0
    while i < len(m):
        if m[i] == 0xE8 and i + 5 <= len(m):
            m[i + 1:i + 5] = b"\0\0\0\0"
            i += 5
        else:
            i += 1
    return bytes(m)


def relaxed_equal(built, retail):
    """True when `built` equals `retail` up to call/data relocation slots."""
    if len(built) != len(retail):
        return False
    b = mask_calls(built)
    r = mask_calls(retail)
    i = 0
    n = len(b)
    while i < n:
        if b[i] == r[i]:
            i += 1
            continue
        # a difference must be covered by a 4-byte all-zero run in the fresh object
        start = i
        end = min(i + 4, n)
        if any(b[j] != 0 for j in range(start, end)) or end - start < 4:
            return False
        i = end
    return True
