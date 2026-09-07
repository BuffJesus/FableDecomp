#!/usr/bin/env python3
"""Independently verify an appended F2 architecture TLC texture package."""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest().upper()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("archive", type=Path)
    args = parser.parse_args()
    manifest_path = args.archive.with_suffix(args.archive.suffix + ".manifest.json")
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    tools = Path(__file__).resolve().parent
    sys.path.insert(0, str(tools))
    import parse_bigb  # type: ignore
    import texture_build  # type: ignore

    data = args.archive.read_bytes()
    _magic, _version, footer, _size = parse_bigb.parse_header(data)
    subbanks, _ = parse_bigb.parse_footer(data, footer)
    bank = next(sub for sub in subbanks if sub["name"] == manifest["bank"])
    entries, _stats, _end, error = parse_bigb.parse_toc(data, bank, footer)
    if error:
        raise SystemExit(f"FAIL texture TOC: {error}")
    by_name = {entry["name"]: entry for entry in entries}
    failures = 0
    checks = 2
    if digest(args.archive) != manifest["output_big_sha256"]:
        failures += 1; print("FAIL archive hash")
    if len(manifest["assigned_textures"]) != manifest["entries_added"]:
        failures += 1; print("FAIL assigned count")
    dds_root = Path(manifest["dds_manifest"]).parent
    seen_ids = set()
    for assigned in manifest["assigned_textures"]:
        checks += 6
        entry = by_name.get(assigned["entry"])
        if not entry or entry["id"] != assigned["texture_id"]:
            failures += 1; print(f"FAIL entry identity {assigned['entry']}")
            continue
        if entry["id"] in seen_ids:
            failures += 1; print(f"FAIL duplicate id {entry['id']}")
        seen_ids.add(entry["id"])
        dds_path = dds_root / assigned["source_dds"]
        if not dds_path.is_file() or digest(dds_path) != assigned["source_dds_sha256"]:
            failures += 1; print(f"FAIL source DDS {dds_path}")
        payload = data[entry["offset"]:entry["offset"] + entry["size"]]
        try:
            decoded, mips = texture_build.decode_entry(entry["info"], payload,
                                                        max_mips=1)
        except Exception as exc:
            failures += 1; print(f"FAIL decode {assigned['entry']}: {exc}")
            continue
        if (not mips or decoded["width"] != assigned["width"] or
                decoded["height"] != assigned["height"] or
                decoded["fmt_name"].lower() != assigned["format"]):
            failures += 1; print(f"FAIL texture contract {assigned['entry']}")
    print(json.dumps({"checks": checks, "failures": failures,
                      "entries": len(manifest["assigned_textures"])}, indent=2))
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
