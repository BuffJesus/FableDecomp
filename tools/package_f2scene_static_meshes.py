#!/usr/bin/env python3
"""Append a verified static-mesh intermediate package to a graphics.big copy."""
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import big_write  # noqa: E402
import parse_bigb  # noqa: E402


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("source_big", type=Path)
    ap.add_argument("mesh_package", type=Path)
    ap.add_argument("output_big", type=Path)
    args = ap.parse_args()
    if args.source_big.resolve() == args.output_big.resolve():
        raise SystemExit("refusing in-place graphics.big mutation")
    manifest_path = args.mesh_package / "mesh_manifest.json"
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    if manifest.get("schema") != "fableforge.f2scene_tlc_static_meshes.v1":
        raise SystemExit("unsupported mesh package")
    source = args.source_big.read_bytes()
    _magic, _version, footer, _size = parse_bigb.parse_header(source)
    subs, _ = parse_bigb.parse_footer(source, footer)
    mesh_sub = next(s for s in subs if s["name"] == "MBANK_ALLMESHES")
    entries, _stats, _end, error = parse_bigb.parse_toc(source, mesh_sub, footer)
    if error:
        raise SystemExit(f"source TOC parse failed: {error}")
    next_id = max(e["id"] for e in entries) + 1
    adds, assigned = [], []
    for row in manifest["meshes"]:
        if row["status"] != "compiled":
            continue
        payload = (args.mesh_package / row["payload"]).read_bytes()
        info = (args.mesh_package / row["info"]).read_bytes()
        adds.append({"sub": "MBANK_ALLMESHES", "name": row["mesh_name"],
                     "payload": payload, "type": 1, "info": info, "id": next_id})
        assigned.append({"block": row["block"], "mesh_name": row["mesh_name"],
                         "model_id": next_id})
        next_id += 1
    rebuilt = big_write.rebuild(source, adds=adds)
    args.output_big.parent.mkdir(parents=True, exist_ok=True)
    args.output_big.write_bytes(rebuilt)
    out = {"schema": "fableforge.f2scene_tlc_graphics_package.v1",
           "source_big": str(args.source_big.resolve()),
           "output_big": str(args.output_big.resolve()),
           "mesh_manifest": str(manifest_path.resolve()),
           "entries_added": len(assigned), "assigned_models": assigned,
           "runtime_installed": False}
    out_path = args.output_big.with_suffix(args.output_big.suffix + ".manifest.json")
    out_path.write_text(json.dumps(out, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(out, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
