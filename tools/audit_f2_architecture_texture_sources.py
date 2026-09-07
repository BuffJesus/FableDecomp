#!/usr/bin/env python3
"""Resolve traced Fable II architecture texture paths to authoritative BNKs."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("fable2re", type=Path)
    parser.add_argument("trace", type=Path)
    parser.add_argument("out", type=Path)
    parser.add_argument("banks", type=Path, nargs="+")
    args = parser.parse_args()
    root = args.fable2re.resolve()
    sys.path.insert(0, str(root / "Fable2Native/tools"))
    import cook_levels  # type: ignore

    trace = json.loads(args.trace.read_text(encoding="utf-8"))
    paths = sorted({path for model in trace["models"]
                    for path in model.get("unique_texture_paths", [])})
    usages: dict[str, list[dict]] = {path: [] for path in paths}
    for model in trace["models"]:
        for material in model.get("materials", []):
            for role in ("diffuse", "specular", "normal", "metallic", "extra"):
                path = material.get(role)
                if path:
                    usages.setdefault(path, []).append({
                        "block": model["block"], "role": role,
                        "mesh_index": material["mesh_index"],
                        "material_index": material["material_index"],
                    })
    banks = [bank.resolve() for bank in args.banks]
    indices = [(bank, cook_levels._bnk_name_index(bank)) for bank in banks]
    rows = []
    for path in paths:
        matches = []
        for bank, index in indices:
            entry = cook_levels._resolve(index, path)
            if entry:
                kind = ("header" if "header" in bank.name.lower()
                        else "payload")
                matches.append({"bank": str(bank), "entry": entry,
                                "kind": kind})
        payload_matches = [match for match in matches
                           if match["kind"] == "payload"]
        header_matches = [match for match in matches
                          if match["kind"] == "header"]
        status = ("payload_resolved" if payload_matches else
                  "header_only" if header_matches else "missing")
        rows.append({"texture": path, "status": status,
                     "usages": usages.get(path, []),
                     "payload_matches": payload_matches,
                     "header_matches": header_matches})
    result = {
        "schema": "fableforge.f2_architecture_texture_source_audit.v2",
        "source_trace": str(args.trace.resolve()),
        "banks": [str(bank) for bank in banks],
        "counts": {"textures": len(rows),
                   "payload_resolved": sum(row["status"] == "payload_resolved"
                                           for row in rows),
                   "header_only": sum(row["status"] == "header_only" for row in rows),
                   "missing": sum(row["status"] == "missing" for row in rows),
                   "multiple_payload_matches": sum(len(row["payload_matches"]) > 1
                                                   for row in rows),
                   "diffuse_textures": sum(any(use["role"] == "diffuse"
                                                for use in row["usages"])
                                           for row in rows),
                   "diffuse_payload_resolved": sum(
                       row["status"] == "payload_resolved" and
                       any(use["role"] == "diffuse" for use in row["usages"])
                       for row in rows)},
        "textures": rows,
    }
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result["counts"], indent=2))
    return 0 if result["counts"]["missing"] == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
