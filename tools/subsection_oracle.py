#!/usr/bin/env python3
"""Retail ORACLE dumper for the local-detail subsection table.

Purpose
-------
The subsection-builder port (the code that, given a type-1 RepeatedMesh
primitive's instance array, emits the `elementCount * 0x50` subsection table)
must be proven by diffing its emitted BYTES against retail's.  This tool
produces the retail side of that diff: for every type-1 primitive in one or
more retail maps it dumps the complete builder INPUT (ObjectCount, maxScale,
the A and B instance arrays, the LandscapeNormal SoA arrays, the WindDelay
bytes) next to the builder OUTPUT (the raw subsection-table bytes, hex), one
JSON record per line.

Nothing here interprets the 0x50-byte element: it is captured verbatim so the
port can be byte-diffed without this tool's opinion entering the comparison.

Grammar
-------
Not re-derived.  The quadtree walk and the group/primitive grammar are the ones
in tools/localdetail_verify.py; that module is IMPORTED (Reader, walk,
parse_quad_header, parse_group_header) and its own decode_contents is run over
every payload as a cross-check, so this file cannot silently drift into being a
second, different parser.  Decompression is tools/lionhead_lz.py.

Type-2 (CLocalDetailPrimitiveZSpriteBatch) has an unrecovered on-disk layout,
so exactly as in localdetail_verify.py it STOPS the parse of its group.  Such
groups are counted and reported, never silently dropped: whatever type-1
primitives preceded the type-2 in that group are emitted, and each emitted
record carries groupTruncatedByZSprite.

Usage
-----
  subsection_oracle.py --stb <FinalAlbion_RT.stb> --out <oracle.jsonl> \
      Darkwood_3 StartOakValeWest
  subsection_oracle.py --pair <chunk.bin> <record.bin> --name Darkwood_3 \
      --out <oracle.jsonl>
"""

from __future__ import annotations

import argparse
import json
import struct
import subprocess
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import lionhead_lz
import localdetail_verify as ldv

Reader = ldv.Reader


def extract_primitives(payload: bytes):
    """Walk one decompressed group payload, capturing every type-1 primitive.

    Mirrors ldv.decode_contents step for step; the only difference is that the
    fields it reads and discards are retained here.
    """
    reader = Reader(payload)
    captured = []
    stats = {"collections": 0, "primitives": 0, "type0": 0, "type1": 0,
             "truncatedByZSprite": False, "unknownType": None,
             "trailingBytes": None}
    collection_count = reader.u32()
    for collection_index in range(collection_count):
        enabled = reader.u32()
        type_index = reader.u32()
        primitive_count = reader.u32()
        stats["collections"] += 1
        for primitive_index in range(primitive_count):
            primitive_type = reader.u32()
            stats["primitives"] += 1
            if primitive_type == 1:
                stats["type1"] += 1
                record = {
                    "collectionIndex": collection_index,
                    "collectionEnabled": enabled,
                    "collectionType": type_index,
                    "primitiveIndex": primitive_index,
                    "primitiveOffsetInPayload": reader.at - 4,
                }
                record["bbox"] = reader.f32(6)
                record["sphere"] = reader.f32(4)
                count = reader.u32()
                record["objectCount"] = count
                record["maxScale"] = reader.f32()
                if count == 0 or count > 255:
                    # ldv bails here too: the stream cannot be walked further.
                    record["unparsable"] = True
                    captured.append(record)
                    return captured, stats
                record["A"] = [reader.f32(4) for _ in range(count)]
                record["B"] = [reader.f32(4) for _ in range(count)]
                normals_present = reader.u8()
                record["normalsPresent"] = normals_present
                if normals_present:
                    padded = (count + 3) & ~3
                    record["normalPad"] = padded
                    record["normalX"] = reader.f32(padded)
                    record["normalY"] = reader.f32(padded)
                    record["normalZ"] = reader.f32(padded)
                else:
                    record["normalPad"] = 0
                    record["normalX"] = []
                    record["normalY"] = []
                    record["normalZ"] = []
                wind_present = reader.u8()
                record["windPresent"] = wind_present
                record["wind"] = list(reader.raw(count)) if wind_present else []
                subsections_present = reader.u8()
                record["subsectionsPresent"] = subsections_present
                if subsections_present:
                    element_count = reader.u32()
                    record["elementCount"] = element_count
                    record["tableOffsetInPayload"] = reader.at
                    record["tableHex"] = reader.raw(element_count * 0x50).hex()
                else:
                    record["elementCount"] = None
                    record["tableOffsetInPayload"] = None
                    record["tableHex"] = None  # ABSENT marker
                captured.append(record)
            elif primitive_type == 0:
                stats["type0"] += 1
                reader.f32(6)
                reader.f32(4)
                reader.f32(12)
                reader.f32()
            elif primitive_type == 2:
                stats["truncatedByZSprite"] = True
                return captured, stats
            else:
                stats["unknownType"] = primitive_type
                return captured, stats
    stats["trailingBytes"] = len(payload) - reader.at
    return captured, stats


def cross_check(payload, captured, stats):
    """Run the reference parser over the same payload and compare counters."""
    problems = []
    reference = ldv.decode_contents(payload, problems)
    notes = []
    if reference["primitives"] != stats["primitives"]:
        notes.append("primitive count %d != reference %d"
                     % (stats["primitives"], reference["primitives"]))
    with_tables = sum(1 for r in captured if r.get("subsectionsPresent"))
    if reference["withSubsections"] != with_tables:
        notes.append("table count %d != reference %d"
                     % (with_tables, reference["withSubsections"]))
    if bool(reference["skippedZSprite"]) != bool(stats["truncatedByZSprite"]):
        notes.append("ZSprite truncation disagrees with reference")
    return notes


def dump_map(name, chunk, record, header_offset, out, totals):
    root = ldv.parse_quad_header(record, header_offset)
    if not (0 < root["fbPos"] + root["offIn"] < len(chunk)):
        raise SystemExit("%s: root quad header at record+0x%x does not point "
                         "into the chunk" % (name, header_offset))
    nodes = ldv.walk(chunk, root, [])
    for node in nodes:
        for group in node["groups"]:
            at = group["fbPos"] + group["offIn"]
            uncompressed, compressed = struct.unpack_from("<II", chunk, at)
            payload = lionhead_lz.decompress(chunk[at + 8:at + 8 + compressed],
                                             uncompressed)
            totals["groups"] += 1
            captured, stats = extract_primitives(payload)
            drift = cross_check(payload, captured, stats)
            if drift:
                raise SystemExit("%s group @0x%x: parser drift vs "
                                 "localdetail_verify: %s"
                                 % (name, at, "; ".join(drift)))
            if stats["truncatedByZSprite"]:
                totals["groupsTruncatedByZSprite"] += 1
            if stats["unknownType"] is not None:
                totals["groupsUnknownType"] += 1
            for row in captured:
                row.update({
                    "map": name,
                    "nodePath": node["path"],
                    "nodeDepth": node["depth"],
                    "groupOffset": at,
                    "groupPayloadLen": len(payload),
                    "groupFade": group["fade"],
                    "groupBand": group["band"],
                    "groupCacheGroup": group["cacheGroup"],
                    "groupTruncatedByZSprite": stats["truncatedByZSprite"],
                })
                out.write(json.dumps(row) + "\n")
                totals["type1"] += 1
                if row.get("unparsable"):
                    totals["unparsable"] += 1
                    continue
                if row["subsectionsPresent"]:
                    totals["withTable"] += 1
                    totals["pairs"][(row["objectCount"], row["elementCount"])] += 1
                else:
                    totals["withoutTable"] += 1
                    totals["noTableCounts"][row["objectCount"]] += 1
                totals["objectCounts"][row["objectCount"]] += 1


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("maps", nargs="*", help="map base names, e.g. Darkwood_3")
    parser.add_argument("--stb", type=Path)
    parser.add_argument("--forge", type=Path,
                        default=Path(r"D:\Code\FableForge\build\forge.exe"))
    parser.add_argument("--workdir", type=Path,
                        default=Path("tmp/subsection_oracle"))
    parser.add_argument("--pair", nargs=2, type=Path, metavar=("CHUNK", "RECORD"),
                        help="use an already-extracted chunk + common record")
    parser.add_argument("--name", default="unnamed", help="map name for --pair")
    parser.add_argument("--header-offset", type=lambda v: int(v, 0), default=0x7d)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    totals = {"groups": 0, "type1": 0, "withTable": 0, "withoutTable": 0,
              "unparsable": 0, "groupsTruncatedByZSprite": 0,
              "groupsUnknownType": 0, "pairs": Counter(),
              "objectCounts": Counter(), "noTableCounts": Counter()}

    args.out.parent.mkdir(parents=True, exist_ok=True)
    with args.out.open("w", encoding="utf-8") as out:
        if args.pair:
            dump_map(args.name, args.pair[0].read_bytes(),
                     args.pair[1].read_bytes(), args.header_offset, out, totals)
        else:
            if not args.stb or not args.maps:
                parser.error("either --pair or --stb plus map names is required")
            args.workdir.mkdir(parents=True, exist_ok=True)
            for name in args.maps:
                entry = "Data\\Levels\\FinalAlbion\\%s.lev" % name
                chunk_path = (args.workdir / "Data" / "Levels" / "FinalAlbion"
                              / ("%s.lev" % name))
                record_path = args.workdir / ("%s.record.bin" % name)
                subprocess.run([str(args.forge), "stb", "extract", str(args.stb),
                                str(args.workdir), "%s.lev" % name],
                               check=True, stdout=subprocess.DEVNULL)
                subprocess.run([str(args.forge), "stb", "record", str(args.stb),
                                entry, str(record_path)],
                               check=True, stdout=subprocess.DEVNULL)
                dump_map(name, chunk_path.read_bytes(), record_path.read_bytes(),
                         args.header_offset, out, totals)

    print("oracle: %s" % args.out)
    print("groups walked            : %d" % totals["groups"])
    print("  truncated by ZSprite   : %d (type-2 layout UNRECOVERED; type-1 "
          "primitives before it were captured)" % totals["groupsTruncatedByZSprite"])
    print("  stopped, unknown type  : %d" % totals["groupsUnknownType"])
    print("type-1 records captured  : %d" % totals["type1"])
    print("  with subsection table  : %d" % totals["withTable"])
    print("  table ABSENT           : %d" % totals["withoutTable"])
    print("  unparsable ObjectCount : %d" % totals["unparsable"])
    print("\nObjectCount distribution (all type-1):")
    for value, hits in sorted(totals["objectCounts"].items()):
        print("  %3d -> %d" % (value, hits))
    print("\nObjectCount distribution (table ABSENT):")
    for value, hits in sorted(totals["noTableCounts"].items()):
        print("  %3d -> %d" % (value, hits))
    print("\n(ObjectCount -> elementCount) pairs, table present:")
    print("  objCount  elemCount  records")
    for (obj, elem), hits in sorted(totals["pairs"].items()):
        print("  %8d  %9d  %7d" % (obj, elem, hits))
    ambiguous = Counter(obj for obj, _ in totals["pairs"])
    multi = sorted(o for o, n in ambiguous.items() if n > 1)
    if multi:
        print("\nObjectCounts mapping to MORE THAN ONE elementCount (so the "
              "split is NOT a pure function of ObjectCount): %s"
              % ", ".join(str(v) for v in multi))
    else:
        print("\nevery ObjectCount maps to a single elementCount in this sample")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
