#!/usr/bin/env python3
"""Report candidate rigid-buffer layouts in preserved Fable II MDL body blobs."""

from __future__ import annotations

import argparse
import json
import math
import struct
from pathlib import Path


def u32(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def finite_f32_ratio(data: bytes, offset: int, count: int, stride: int) -> float:
    good = 0
    for index in range(count):
        base = offset + index * stride
        if base + 12 > len(data):
            break
        values = struct.unpack_from(">fff", data, base)
        good += all(math.isfinite(value) and abs(value) < 100000 for value in values)
    return good / count if count else 0.0


def analyze(path: Path) -> dict:
    data = path.read_bytes()
    candidates = []
    # The body prefix is variable. Locate the characteristic u16 marker followed
    # by four bounded big-endian fields, then score common rigid vertex strides.
    for marker in range(len(data) - 18):
        if data[marker:marker + 2] != b"\x00\x01":
            continue
        fields = [u32(data, marker + 2 + i * 4) for i in range(4)]
        face_count, vertex_count = fields[2], fields[3]
        if not (2 < face_count < 65535 and 2 < vertex_count < 65535):
            continue
        vertex_offset = marker + 18
        layouts = []
        for stride in (20, 24, 28, 32, 36, 40, 44, 48):
            face_offset = vertex_offset + vertex_count * stride
            end = face_offset + face_count * 2
            if end > len(data):
                continue
            indices = struct.unpack_from(">%dH" % face_count, data, face_offset)
            layouts.append({
                "stride": stride,
                "face_offset": face_offset,
                "end": end,
                "trailing_bytes": len(data) - end,
                "finite_position_ratio": finite_f32_ratio(
                    data, vertex_offset, vertex_count, stride),
                "index_in_range_ratio": sum(i < vertex_count or i == 0xFFFF
                                            for i in indices) / face_count,
                "restart_count": indices.count(0xFFFF),
                "max_index": max((i for i in indices if i != 0xFFFF),
                                 default=None),
            })
        candidates.append({
            "marker_offset": marker,
            "fields": fields,
            "vertex_offset": vertex_offset,
            "layouts": layouts,
        })
    return {"file": str(path), "bytes": len(data), "candidates": candidates}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("paths", type=Path, nargs="+")
    parser.add_argument("--out", type=Path)
    args = parser.parse_args()
    result = {"schema": "fableforge.f2_lowpoly_layout_analysis.v1",
              "files": [analyze(path.resolve()) for path in args.paths]}
    encoded = json.dumps(result, indent=2) + "\n"
    if args.out:
        args.out.write_text(encoded, encoding="utf-8")
    print(encoded, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
