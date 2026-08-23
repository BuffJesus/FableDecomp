#!/usr/bin/env python3
"""Structurally decode and check a baked local-detail (foliage) section.

Grammar recovered in work/no_donor_terrain_pack/RE_NOTES_20260822_subsections_manager_palette.md
and docs/FOLIAGE_LOCAL_DETAIL_RE.md:

  group block   u32 groupCount, then per group a 0x28-byte header + 4 child slots
  group header  +0x00 payloadOffset (absolute in chunk)   +0x04 payloadSize
                +0x0c..+0x18 world bounding sphere        +0x1c fade
                +0x20 distance-band index (INTEGER)       +0x24 cache group
  payload       LZO framed [u32 uncompressedLen][u32 compressedLen][body]
  contents      u32 collectionCount
                per collection: u32 enabled, u32 type, u32 primitiveCount
                per primitive:  u32 primitiveType
                  type 1 (RepeatedMesh): 6f bbox, 4f sphere, u32 objectCount,
                    f32 maxScale, A[count][4]f, B[count][4]f,
                    u8 normalsPresent -> 3 * P floats, P = (count+3)&~3
                    u8 windPresent    -> count bytes
                    u8 subsectionsPresent -> u32 elemCount, elemCount * 0x50
                  type 0 (Mesh): 6f bbox, 4f sphere, 12f matrix, 1f

Checks applied to every type-1 primitive (the constraints the engine enforces):
  - 0 < objectCount <= 32   (MAX_BATCH_SIZE; the format's <256 assert is looser,
    but the renderer's cull-compaction lanes are strided 32 floats apart)
  - the normal array is present and every normal is unit length
  - A[i] xy magnitude == B[i].w (the rotation/scale pairing retail proves)
  - the primitive bounding sphere actually contains its instances

Usage:
  localdetail_verify.py <chunk.bin> --section <offset>   # offset = info localDetailMapPtr
  localdetail_verify.py <chunk.bin> --scan               # find the section by scanning
"""

from __future__ import annotations

import argparse
import math
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import lionhead_lz

MAX_BATCH = 32


class Reader:
    def __init__(self, data: bytes, offset: int = 0) -> None:
        self.data = data
        self.at = offset

    def u8(self) -> int:
        value = self.data[self.at]
        self.at += 1
        return value

    def u32(self) -> int:
        value = struct.unpack_from("<I", self.data, self.at)[0]
        self.at += 4
        return value

    def f32(self, count: int = 1):
        values = struct.unpack_from("<%df" % count, self.data, self.at)
        self.at += 4 * count
        return values[0] if count == 1 else list(values)

    def raw(self, count: int) -> bytes:
        value = self.data[self.at:self.at + count]
        self.at += count
        return value


def decode_contents(payload: bytes, problems: list[str]) -> dict:
    reader = Reader(payload)
    summary = {"collections": [], "primitives": 0, "instances": 0,
               "batchSizes": [], "withSubsections": 0, "skippedZSprite": 0,
               "zsprite": 0}
    collection_count = reader.u32()
    for _ in range(collection_count):
        enabled = reader.u32()
        type_index = reader.u32()
        primitive_count = reader.u32()
        collection = {"enabled": enabled, "type": type_index,
                      "primitives": primitive_count, "repeated": 0, "mesh": 0}
        for _ in range(primitive_count):
            primitive_type = reader.u32()
            summary["primitives"] += 1
            if primitive_type == 1:
                collection["repeated"] += 1
                bbox = reader.f32(6)
                sphere = reader.f32(4)
                count = reader.u32()
                reader.f32()  # maxScale
                if not 0 < count <= MAX_BATCH:
                    problems.append(
                        "ObjectCount %d violates 0 < count <= %d (MAX_BATCH_SIZE)"
                        % (count, MAX_BATCH))
                if count == 0 or count > 255:
                    return summary  # cannot keep parsing safely
                a = [reader.f32(4) for _ in range(count)]
                b = [reader.f32(4) for _ in range(count)]
                summary["instances"] += count
                summary["batchSizes"].append(count)
                normals_present = reader.u8()
                if not normals_present:
                    problems.append(
                        "LandscapeNormalArray absent: RenderSubPrimitive "
                        "dereferences its base unconditionally (AV)")
                    normals = []
                else:
                    padded = (count + 3) & ~3
                    xs = reader.f32(padded)
                    ys = reader.f32(padded)
                    zs = reader.f32(padded)
                    normals = list(zip(xs[:count], ys[:count], zs[:count]))
                    for i, (nx, ny, nz) in enumerate(normals):
                        length = math.sqrt(nx * nx + ny * ny + nz * nz)
                        if abs(length - 1.0) > 1e-3:
                            problems.append(
                                "normal %d is not unit length (|n|=%.5f)" % (i, length))
                            break
                wind_present = reader.u8()
                if wind_present:
                    reader.raw(count)
                subsections_present = reader.u8()
                if subsections_present:
                    summary["withSubsections"] += 1
                    element_count = reader.u32()
                    reader.raw(element_count * 0x50)
                for i in range(count):
                    pairing = math.hypot(a[i][0], a[i][1])
                    if abs(pairing - b[i][3]) > 1e-4 * max(1.0, abs(b[i][3])):
                        problems.append(
                            "instance %d: |A.xy|=%.6f != B.w=%.6f" % (i, pairing, b[i][3]))
                        break
                for i in range(count):
                    distance = math.sqrt(sum((b[i][axis] - sphere[axis]) ** 2
                                             for axis in range(3)))
                    if distance > sphere[3] + 1e-3:
                        problems.append(
                            "instance %d lies %.3f outside its primitive sphere (r=%.3f)"
                            % (i, distance - sphere[3], sphere[3]))
                        break
                for i in range(count):
                    if not (bbox[0] - 1e-3 <= b[i][0] <= bbox[3] + 1e-3 and
                            bbox[1] - 1e-3 <= b[i][1] <= bbox[4] + 1e-3):
                        problems.append("instance %d lies outside the primitive bbox" % i)
                        break
            elif primitive_type == 0:
                collection["mesh"] += 1
                summary["instances"] += 1
                reader.f32(6)
                reader.f32(4)
                reader.f32(12)
                reader.f32()
            elif primitive_type == 2:
                # CLocalDetailPrimitiveMeshZSpriteBatch::Save, 0x02EE2420:
                # bbox[6], sphere[4], u32 count, then count records made from
                # source[0x00..0x2f], source+0x30 float, source[0x34..0x43]
                # (0x44 bytes total), followed by count auxiliary float4s.
                collection.setdefault("zsprite", 0)
                collection["zsprite"] += 1
                summary["zsprite"] += 1
                reader.f32(6)
                reader.f32(4)
                count = reader.u32()
                summary["instances"] += count
                for _ in range(count):
                    reader.f32(12)
                    reader.f32()
                    reader.f32(4)
                reader.f32(count * 4)
            else:
                problems.append("unknown primitive type %d" % primitive_type)
                return summary
        summary["collections"].append(collection)
    if reader.at != len(payload) and not summary["skippedZSprite"]:
        problems.append("contents did not consume exactly: %d of %d bytes"
                        % (reader.at, len(payload)))
    return summary


def parse_quad_header(chunk: bytes, at: int) -> dict:
    """44-byte CQuadTreeElement<CLocalDetailCacheMap> header."""
    return {
        "sphere": struct.unpack_from("<4f", chunk, at),
        "maxFade": struct.unpack_from("<f", chunk, at + 16)[0],
        "mask": struct.unpack_from("<I", chunk, at + 20)[0],
        "fbPos": struct.unpack_from("<I", chunk, at + 24)[0],
        "fbSize": struct.unpack_from("<I", chunk, at + 28)[0],
        "offIn": struct.unpack_from("<I", chunk, at + 32)[0],
        "cell": struct.unpack_from("<4H", chunk, at + 36),
    }


def parse_group_header(chunk: bytes, at: int) -> dict:
    """0x28-byte group header. The payload lives at fbPos + offIn."""
    return {
        "fbPos": struct.unpack_from("<I", chunk, at)[0],
        "fbSize": struct.unpack_from("<I", chunk, at + 4)[0],
        "offIn": struct.unpack_from("<I", chunk, at + 8)[0],
        "sphere": struct.unpack_from("<4f", chunk, at + 0x0c),
        "fade": struct.unpack_from("<f", chunk, at + 0x1c)[0],
        "band": struct.unpack_from("<I", chunk, at + 0x20)[0],
        "cacheGroup": struct.unpack_from("<H", chunk, at + 0x24)[0],
        "b26": chunk[at + 0x26],
    }


def walk(chunk: bytes, node: dict, nodes: list[dict], depth: int = 0,
         path: str = "R") -> list[dict]:
    """Recursive quadtree walk. A node's block holds u32 groupCount, that many
    0x28-byte group headers, then four (u32 present, 44-byte header if present)
    child slots."""
    at = node["fbPos"] + node["offIn"]
    group_count = struct.unpack_from("<I", chunk, at)[0]
    entry = {"header": node, "depth": depth, "path": path, "groups": []}
    at += 4
    for _ in range(group_count):
        entry["groups"].append(parse_group_header(chunk, at))
        at += 0x28
    children = []
    for quadrant in range(4):
        present = struct.unpack_from("<I", chunk, at)[0]
        at += 4
        if present:
            children.append(parse_quad_header(chunk, at))
            at += 44
    entry["children"] = len(children)
    nodes.append(entry)
    for index, child in enumerate(children):
        walk(chunk, child, nodes, depth + 1, path + str(index))
    return nodes


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("chunk", type=Path)
    parser.add_argument("--record", type=Path, required=True,
                        help="the map's STB common record (holds the root quad header)")
    parser.add_argument("--header-offset", type=lambda v: int(v, 0), default=0x7d,
                        help="offset of the root quad header inside the record")
    args = parser.parse_args()

    chunk = args.chunk.read_bytes()
    record = args.record.read_bytes()
    root = parse_quad_header(record, args.header_offset)
    if not (0 < root["fbPos"] + root["offIn"] < len(chunk)):
        print("root quad header at record+0x%x does not point into the chunk"
              % args.header_offset)
        return 1

    problems: list[str] = []
    nodes = walk(chunk, root, [])
    print("root: sphere=(%.1f, %.1f, %.1f) r=%.1f maxFade=%.1f mask=%d cell=%s"
          % (root["sphere"] + (root["maxFade"], root["mask"], root["cell"])))
    print("%d quadtree node(s)" % len(nodes))

    total = {"groups": 0, "primitives": 0, "instances": 0, "batches": [],
             "withSubsections": 0, "skipped": 0}
    for node in nodes:
        for group in node["groups"]:
            total["groups"] += 1
            at = group["fbPos"] + group["offIn"]
            uncompressed, compressed = struct.unpack_from("<II", chunk, at)
            payload = lionhead_lz.decompress(chunk[at + 8:at + 8 + compressed],
                                             uncompressed)
            if len(payload) != uncompressed:
                problems.append("group payload decompressed to %d, header says %d"
                                % (len(payload), uncompressed))
            summary = decode_contents(payload, problems)
            total["primitives"] += summary["primitives"]
            total["instances"] += summary["instances"]
            total["batches"] += summary["batchSizes"]
            total["withSubsections"] += summary["withSubsections"]
            total["skipped"] += summary["skippedZSprite"]
            print("  node %-6s group @0x%x -> %d B: %d collection(s), %d primitive(s), "
                  "%d instance(s), %d subsection table(s), fade=%.1f band=%d cacheGroup=%d"
                  % (node["path"], at, len(payload), len(summary["collections"]),
                     summary["primitives"], summary["instances"],
                     summary["withSubsections"], group["fade"], group["band"],
                     group["cacheGroup"]))
    if total["batches"]:
        print("type-1 batch sizes: n=%d min=%d max=%d mean=%.1f"
              % (len(total["batches"]), min(total["batches"]), max(total["batches"]),
                 sum(total["batches"]) / len(total["batches"])))
    print("totals: %d group(s), %d primitive(s), %d instance(s), %d with a subsection table"
          % (total["groups"], total["primitives"], total["instances"],
             total["withSubsections"]))
    if total["skipped"]:
        print("%d group(s) stopped early at a type-2 ZSpriteBatch (layout not "
              "recovered); everything before it in those groups was checked"
              % total["skipped"])

    if problems:
        print("\nPROBLEMS (%d):" % len(problems))
        for problem in problems[:40]:
            print("  " + problem)
        return 1
    print("\nOK: structure and engine constraints hold")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
