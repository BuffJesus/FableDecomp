#!/usr/bin/env python3
"""Find retail .text twins of a reference body while ignoring selected byte ranges."""

import argparse
import csv
import re
import struct
from pathlib import Path


def text_section(data: bytes) -> tuple[int, int, bytes]:
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    count = struct.unpack_from("<H", data, pe + 6)[0]
    optional_size = struct.unpack_from("<H", data, pe + 20)[0]
    optional = pe + 24
    image_base = struct.unpack_from("<I", data, optional + 28)[0]
    table = optional + optional_size
    for index in range(count):
        entry = table + index * 40
        if data[entry : entry + 8].rstrip(b"\0") != b".text":
            continue
        virtual_address = struct.unpack_from("<I", data, entry + 12)[0]
        raw_size, raw_offset = struct.unpack_from("<II", data, entry + 16)
        return image_base, virtual_address, data[raw_offset : raw_offset + raw_size]
    raise ValueError("retail image has no .text section")


def parse_ranges(spec: str, length: int) -> set[int]:
    ignored: set[int] = set()
    for item in filter(None, spec.split(",")):
        bounds = item.split("-", 1)
        start = int(bounds[0], 0)
        end = int(bounds[-1], 0)
        if start < 0 or end < start or end >= length:
            raise ValueError(f"invalid ignored range: {item}")
        ignored.update(range(start, end + 1))
    return ignored


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("exe", type=Path)
    parser.add_argument("address", type=lambda value: int(value, 0))
    parser.add_argument("length", type=int)
    parser.add_argument("--ignore", default="", help="inclusive offsets/ranges, e.g. 7,9-12")
    parser.add_argument("--root", type=Path, default=Path.cwd())
    args = parser.parse_args()

    data = args.exe.read_bytes()
    image_base, text_rva, text = text_section(data)
    reference_offset = args.address - image_base - text_rva
    reference = text[reference_offset : reference_offset + args.length]
    if len(reference) != args.length:
        raise ValueError("reference body extends outside .text")
    ignored = parse_ranges(args.ignore, args.length)
    compared = tuple(index for index in range(args.length) if index not in ignored)
    hits = [
        image_base + text_rva + offset
        for offset in range(len(text) - args.length + 1)
        if all(text[offset + index] == reference[index] for index in compared)
    ]

    manifest_path = args.root / "rebuild" / "manifest" / "functions.tsv"
    with manifest_path.open(encoding="utf-8-sig", newline="") as stream:
        manifest = {
            row["address"].lower().replace("0x", ""): row
            for row in csv.DictReader(stream, delimiter="\t")
        }
    catalog = (args.root / "rebuild" / "build_candidates.ps1").read_text(encoding="utf-8")
    landed = {
        value.lower()
        for value in re.findall(r"Address\s*=\s*'([0-9a-fA-F]{8})'", catalog)
    }

    authoritative = sum(f"{address:08x}" in manifest for address in hits)
    landed_count = sum(f"{address:08x}" in landed for address in hits)
    print(f"hits={len(hits)} authoritative={authoritative} landed={landed_count}")
    for address in hits:
        key = f"{address:08x}"
        row = manifest.get(key, {})
        state = "LANDED" if key in landed else "UNLANDED"
        print(f"{key}\t{row.get('name', '<not-manifest>')}\t{row.get('module', '')}\t{state}")


if __name__ == "__main__":
    main()
