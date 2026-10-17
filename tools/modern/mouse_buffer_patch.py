#!/usr/bin/env python3
"""Write a copy of retail Fable.exe with a larger DirectInput mouse buffer.

Retail sets DIPROP_BUFFERSIZE to 256 records in CMouseDX::SetExclusiveMode (0xAB5710, both the
exclusive and the non-exclusive branch) and drains the buffer once per 15 Hz world frame. A
4,000-8,000 Hz mouse produces about 530-1,070 records per world frame, so the buffer overflows and
look motion is lost (docs/modern/FRAME_TIMING.md, "mouse"). This writes the same executable with
a larger buffer. The read loop 0xAB4910 already refills until the device is empty.

    python tools/modern/mouse_buffer_patch.py --exe <Fable.exe> --check
    python tools/modern/mouse_buffer_patch.py --exe <Fable.exe> --out <patched.exe> [--records 8192]

The input is never modified; --out must be a different path. Only the Steam retail executable is
accepted (SHA-256 below), and both patch sites are checked byte for byte before writing. Untested
in-game: try it on a copy, with the game install not shared by another session.
"""
from __future__ import annotations

import argparse
import hashlib
import struct
import sys
from pathlib import Path

RETAIL_SHA256 = "41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10"
RETAIL_SIZE = 16_666_624
# File offsets of `mov dword ptr [esp+0x24], imm32` (C7 44 24 24): DIPROPDWORD.dwData.
SITES = {0x6B57B6: "exclusive (VA 0xAB57B6)", 0x6B586D: "non-exclusive (VA 0xAB586D)"}
OPCODE = bytes.fromhex("c7442424")
RETAIL_RECORDS = 256


def site_values(data: bytes) -> dict[int, int | None]:
    """The buffer size each site sets, or None if the instruction is not there."""
    out = {}
    for off in SITES:
        out[off] = struct.unpack_from("<I", data, off + 4)[0] if data[off:off + 4] == OPCODE else None
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--exe", required=True, type=Path)
    ap.add_argument("--out", type=Path)
    ap.add_argument("--records", type=int, default=8192, help="DIPROP_BUFFERSIZE to write (default 8192)")
    ap.add_argument("--check", action="store_true", help="report the current buffer size and exit")
    args = ap.parse_args()

    data = args.exe.read_bytes()
    values = site_values(data)
    if args.check:
        for off, name in SITES.items():
            print(f"{name}: {values[off] if values[off] is not None else 'instruction not found'}")
        retail = hashlib.sha256(data).hexdigest() == RETAIL_SHA256
        print("retail Steam executable" if retail else "not the unmodified retail executable")
        return 0

    if args.out is None:
        ap.error("--out is required unless --check")
    if args.out.resolve() == args.exe.resolve():
        ap.error("--out must differ from --exe; this tool never modifies the input")
    if not 1 <= args.records <= 0x10000:
        ap.error("--records must be 1..65536")
    if len(data) != RETAIL_SIZE or hashlib.sha256(data).hexdigest() != RETAIL_SHA256:
        print("refusing: not the unmodified Steam retail Fable.exe (size/SHA-256 differ)", file=sys.stderr)
        return 1
    if any(v != RETAIL_RECORDS for v in values.values()):
        print(f"refusing: unexpected bytes at a patch site: {values}", file=sys.stderr)
        return 1

    patched = bytearray(data)
    for off in SITES:
        struct.pack_into("<I", patched, off + 4, args.records)
    args.out.write_bytes(bytes(patched))
    after = site_values(bytes(patched))
    changed = sum(a != b for a, b in zip(data, patched))
    print(f"wrote {args.out} ({changed} bytes changed); buffer size {RETAIL_RECORDS} -> {args.records} at "
          + ", ".join(f"{SITES[o]}={after[o]}" for o in SITES))
    print("Note: the PE checksum is not used by Windows for user executables, so it is left as is.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
