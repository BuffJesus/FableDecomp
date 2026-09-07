#!/usr/bin/env python3
"""Probe explicit Fable II MDL header/body sources for BWSMarket low-poly houses."""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
import tempfile
from pathlib import Path


MODELS = [
    r"Art\Environment\Regions\Bowerstone\Structures\dotXSI\BS_Market_LowPoly_House1\BS_Market_LowPoly_House1.mdl",
    r"Art\Environment\Regions\Bowerstone\Structures\dotXSI\BS_Market_LowPoly_House2\BS_Market_LowPoly_House2.mdl",
    r"Art\Environment\Regions\Bowerstone\Structures\dotXSI\BS_Market_LowPoly_House3\BS_Market_LowPoly_House3.mdl",
]


def norm(value: str) -> str:
    return value.lower().replace("\\", "/")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("fable2re", type=Path)
    parser.add_argument("level_body", type=Path)
    parser.add_argument("--out", type=Path)
    args = parser.parse_args()
    root = args.fable2re.resolve()
    f2tool = root / "Fable2AssetBrowser/source/build/f2tool.exe"
    header_bank = root / "Fable2Recomp/assets/game/data/Globals/globals_model_headers.bnk"
    bodies = {
        "level": args.level_body.resolve(),
        "globals": root / "Fable2Recomp/assets/game/data/Globals/globals_models.bnk",
    }
    sys.path.insert(0, str(root / "Fable2Native/tools"))
    sys.path.insert(0, str(root / "Fable2AssetBrowser/source/addons"))
    import cook_levels  # type: ignore
    import fable_mdl_format as mdl  # type: ignore

    header_index = cook_levels._bnk_name_index(header_bank)
    body_indices = {name: cook_levels._bnk_name_index(path)
                    for name, path in bodies.items()}
    rows = []
    with tempfile.TemporaryDirectory(prefix="f2_lowpoly_probe_") as td:
        temp = Path(td)

        def extract(bank: Path, entry: str, filename: str) -> bytes:
            destination = temp / filename
            subprocess.run([str(f2tool), "extract", str(bank), entry,
                            str(destination)], check=True, capture_output=True)
            return destination.read_bytes()

        for model in MODELS:
            header_entry = cook_levels._resolve(header_index, model)
            if not header_entry:
                rows.append({"model": model, "status": "missing_header"})
                continue
            header = extract(header_bank, header_entry, "header.bin")
            for source, body_bank in bodies.items():
                body_entry = cook_levels._resolve(body_indices[source], model)
                row = {"model": model, "source": source,
                       "header_entry": header_entry, "body_entry": body_entry,
                       "header_bytes": len(header)}
                if not body_entry:
                    row["status"] = "missing_body"
                    rows.append(row)
                    continue
                body = extract(body_bank, body_entry, f"{source}.bin")
                row["body_bytes"] = len(body)
                messages = []
                try:
                    info, geometries = mdl.parse(
                        header + body, log=messages.append, file_path=model)
                    row.update({
                        "status": "decoded" if geometries else "no_geometry",
                        "geometries": len(geometries),
                        "vertices": sum(len(g.positions) // 3 for g in geometries),
                        "triangles": sum(len(g.indices) // 3 for g in geometries),
                        "materials": [{
                            "diffuse": getattr(g, "diffuse", ""),
                            "normal": getattr(g, "normal_tex", ""),
                            "specular": getattr(g, "specular_tex", ""),
                        } for g in geometries],
                        "buffers": [{
                            "vertex_count": b.vertex_count,
                            "vertex_offset": b.vertex_offset,
                            "face_count": b.face_count,
                            "face_offset": b.face_offset,
                            "submesh_count": b.submesh_count,
                            "is_alt_path": b.is_alt_path,
                            "is_foliage": b.is_foliage,
                            "foliage_stride": b.foliage_stride,
                            "combined_bytes": len(header) + len(body),
                        } for b in info.mesh_buffers],
                    })
                except Exception as exc:  # diagnostic must preserve exact failure
                    row.update({"status": "error", "error_type": type(exc).__name__,
                                "error": str(exc)})
                row["log"] = messages
                rows.append(row)
    result = {"schema": "fableforge.f2_lowpoly_mdl_probe.v1", "rows": rows}
    encoded = json.dumps(result, indent=2) + "\n"
    if args.out:
        args.out.write_text(encoded, encoding="utf-8")
    print(encoded, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
