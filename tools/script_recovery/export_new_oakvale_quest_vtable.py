#!/usr/bin/env python3
"""Export the New Oakvale quest lifecycle vtable directly from retail Fable.exe."""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path

import pefile

ROLES = ["destructor", "RegisterMain", "Main", "Init", "OnPersist"]


def export(exe: Path, cluster_path: Path) -> dict:
    raw = exe.read_bytes()
    pe = pefile.PE(data=raw, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    vtable = int(cluster["vtableAddress"], 0)
    offset = pe.get_offset_from_rva(vtable - base)
    roles = ROLES
    values = struct.unpack_from(f"<{len(roles)}I", raw, offset)
    slots = [{"slot": index, "role": role, "address": f"0x{address:08X}"}
             for index, (role, address) in enumerate(zip(roles, values))]

    deleting_va = values[0]
    deleting_offset = pe.get_offset_from_rva(deleting_va - base)
    code = raw[deleting_offset:deleting_offset + 32]
    call_index = code.find(b"\xe8")
    if call_index < 0:
        raise RuntimeError(f"{deleting_va:#x}: no near call in deleting destructor")
    displacement = struct.unpack_from("<i", code, call_index + 1)[0]
    implementation = deleting_va + call_index + 5 + displacement
    table_bytes = raw[offset:offset + len(roles) * 4]
    return {
        "schema": "new-oakvale-quest-vtable/0.1",
        "source": "installed retail Fable.exe PE image",
        "sourceSha256": hashlib.sha256(raw).hexdigest().upper(),
        "imageBase": f"0x{base:08X}",
        "vtable": f"0x{vtable:08X}",
        "vtableBytesSha256": hashlib.sha256(table_bytes).hexdigest().upper(),
        "slots": slots,
        "deletingDestructorImplementation": f"0x{implementation:08X}",
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--cluster", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = export(args.exe, args.cluster)
    text = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(text, encoding="utf-8")
    else:
        print(text, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
