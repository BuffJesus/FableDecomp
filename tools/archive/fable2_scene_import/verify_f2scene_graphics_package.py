#!/usr/bin/env python3
"""Verify added F2 architecture entries in a disposable graphics.big."""
from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import parse_bigb  # noqa: E402


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("archive", type=Path)
    args = ap.parse_args()
    package = json.loads(args.archive.with_suffix(args.archive.suffix + ".manifest.json")
                         .read_text(encoding="utf-8"))
    mesh_manifest_path = Path(package["mesh_manifest"])
    mesh_manifest = json.loads(mesh_manifest_path.read_text(encoding="utf-8"))
    source_rows = {r.get("mesh_name"): r for r in mesh_manifest["meshes"]
                   if r["status"] == "compiled"}
    data = args.archive.read_bytes()
    magic, _version, footer, _size = parse_bigb.parse_header(data)
    failures = 0
    checks = 1
    if magic != b"BIGB": failures += 1
    subs, _ = parse_bigb.parse_footer(data, footer)
    all_meshes = {}
    for sub in subs:
        entries, _stats, _end, error = parse_bigb.parse_toc(data, sub, footer)
        checks += 2
        if error or len(entries) != sub["entry_count"]: failures += 1
        if sub["name"] == "MBANK_ALLMESHES":
            all_meshes = {e["name"]: e for e in entries}
    for assigned in package["assigned_models"]:
        checks += 4
        row = source_rows.get(assigned["mesh_name"])
        entry = all_meshes.get(assigned["mesh_name"])
        if row is None or entry is None:
            failures += 1; continue
        if entry["id"] != assigned["model_id"] or entry["type"] != 1:
            failures += 1
        payload = data[entry["offset"]:entry["offset"] + entry["size"]]
        if hashlib.sha256(payload).hexdigest().upper() != row["payload_sha256"]:
            failures += 1
    result = {"checks": checks, "failures": failures, "subbanks": len(subs),
              "mesh_entries": len(all_meshes),
              "added_entries": len(package["assigned_models"])}
    print(json.dumps(result, indent=2))
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
