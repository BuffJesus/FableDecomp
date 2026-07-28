#!/usr/bin/env python3
"""Extract and disassemble original D3D9 shaders from Fable TLC shaders.big.

The bank payload starts with a little-endian byte count followed by the
original Direct3D shader token stream. On Windows, d3dcompiler_47.dll can
disassemble the shipped shader-model 1.x bytecode without translating it.

Example:
  python tools/dump_shader_asm.py SHADERS.BIG \
      --name SPRITE_GROUP_BILLBOARD --name PSHADER_SPRITE_GROUP \
      --output work/particle_shaders.asm
"""

import argparse
import ctypes
import hashlib
import os
import struct
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

import parse_bigb


SHADER_VERSIONS = {
    0xFFFE0101: "vs_1_1",
    0xFFFF0101: "ps_1_1",
}


def shader_bytecode_from_payload(payload):
    """Validate and return (declared size, version, token stream)."""
    if len(payload) < 8:
        raise ValueError("shader payload is shorter than its header")
    declared_size, version = struct.unpack_from("<II", payload)
    bytecode = payload[4:]
    if version not in SHADER_VERSIONS:
        raise ValueError(f"unsupported shader version token 0x{version:08x}")
    if declared_size > len(bytecode):
        raise ValueError(
            f"declared shader size {declared_size} exceeds "
            f"{len(bytecode)} payload bytes"
        )
    return declared_size, SHADER_VERSIONS[version], bytecode


def iter_shader_entries(path):
    with open(path, "rb") as bank:
        data = bank.read()
    magic, _version, footer_offset, _footer_size = (
        parse_bigb.parse_header(data)
    )
    if magic != b"BIGB":
        raise ValueError("not a BIGB bank")
    subbanks, _footer_end = parse_bigb.parse_footer(data, footer_offset)
    for subbank in subbanks:
        entries, _stats, _end, error = parse_bigb.parse_toc(
            data, subbank, footer_offset
        )
        if error:
            raise ValueError(
                f'{subbank["name"]} table-of-contents error: {error}'
            )
        for entry in entries:
            start = entry["offset"]
            end = start + entry["size"]
            yield subbank["name"], entry, data[start:end]


def disassemble_d3d9(bytecode):
    """Return D3DDisassemble's exact assembly text."""
    compiler = ctypes.WinDLL("d3dcompiler_47.dll")
    compiler.D3DDisassemble.argtypes = [
        ctypes.c_void_p,
        ctypes.c_size_t,
        ctypes.c_uint,
        ctypes.c_char_p,
        ctypes.POINTER(ctypes.c_void_p),
    ]
    compiler.D3DDisassemble.restype = ctypes.c_long

    source = ctypes.create_string_buffer(bytecode)
    blob = ctypes.c_void_p()
    result = compiler.D3DDisassemble(
        source, len(bytecode), 0, None, ctypes.byref(blob)
    )
    if result < 0:
        raise OSError(
            f"D3DDisassemble failed with HRESULT "
            f"0x{ctypes.c_uint32(result).value:08x}"
        )

    vtable = ctypes.cast(
        blob, ctypes.POINTER(ctypes.POINTER(ctypes.c_void_p))
    ).contents
    get_pointer = ctypes.WINFUNCTYPE(
        ctypes.c_void_p, ctypes.c_void_p
    )(vtable[3])
    get_size = ctypes.WINFUNCTYPE(
        ctypes.c_size_t, ctypes.c_void_p
    )(vtable[4])
    release = ctypes.WINFUNCTYPE(
        ctypes.c_uint, ctypes.c_void_p
    )(vtable[2])
    try:
        pointer = get_pointer(blob)
        size = get_size(blob)
        return ctypes.string_at(pointer, size).rstrip(b"\0").decode(
            "ascii", "replace"
        )
    finally:
        release(blob)


def build_dump(path, names):
    needles = tuple(name.casefold() for name in names)
    sections = []
    for subbank, entry, payload in iter_shader_entries(path):
        if not any(needle in entry["name"].casefold() for needle in needles):
            continue
        declared_size, version, bytecode = shader_bytecode_from_payload(
            payload
        )
        sections.append(
            "\n".join((
                f'; === {entry["name"]} ===',
                f'; subbank={subbank} id={entry["id"]} '
                f'type={entry["type"]}',
                f'; bank_payload_bytes={len(payload)} '
                f'declared_shader_bytes={declared_size} version={version}',
                f'; payload_sha256={hashlib.sha256(payload).hexdigest()}',
                disassemble_d3d9(bytecode),
            ))
        )
    if not sections:
        raise ValueError("no shaders matched the requested names")
    return "\n\n".join(sections) + "\n"


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("shader_bank", help="path to shaders.big")
    parser.add_argument(
        "--name", action="append", required=True,
        help="case-insensitive shader-name substring; may be repeated",
    )
    parser.add_argument("--output", help="write combined assembly to this path")
    args = parser.parse_args()

    assembly = build_dump(args.shader_bank, args.name)
    if args.output:
        with open(args.output, "w", encoding="utf-8", newline="\n") as output:
            output.write(assembly)
        count = assembly.count("; === ")
        print(f"wrote {count} retail shaders -> {args.output}")
    else:
        print(assembly, end="")


if __name__ == "__main__":
    main()
