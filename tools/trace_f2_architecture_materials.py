#!/usr/bin/env python3
"""Trace authoritative MDL material texture paths for selected F2 architecture."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path


DEFAULT_INCLUDE = (r"\\buildings\\", r"\\structures\\")
RESOURCE_PREFIX = "mdl-fnv1lower-unknown:"


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest().upper()


def model_path(resource_id: str) -> str:
    if resource_id.lower().startswith(RESOURCE_PREFIX):
        return resource_id[len(RESOURCE_PREFIX):]
    raise ValueError(f"unsupported model resource identity: {resource_id}")


def selected_models(instances: Path, patterns: list[str]) -> list[dict]:
    root = json.loads(instances.read_text(encoding="utf-8"))
    regexes = [re.compile(pattern, re.IGNORECASE) for pattern in patterns]
    blocks: dict[int, str] = {}
    counts: dict[int, int] = {}
    for row in root["instances"]:
        source = row.get("source_record") or {}
        if source.get("container") != "raw/defaultscenario.engine_level":
            continue
        block = int(source["block"])
        resource = str(row.get("resource_id") or "")
        prior = blocks.get(block)
        if prior and prior != resource:
            raise ValueError(f"block {block} has multiple resources")
        blocks[block] = resource
        counts[block] = counts.get(block, 0) + 1
    slots: dict[str, int] = {}
    for block in sorted(blocks):
        slots.setdefault(blocks[block], len(slots))
    return [{"block": block, "scene_model_slot": slots[resource],
             "resource_id": resource, "model": model_path(resource),
             "source_instance_count": counts[block]}
            for block, resource in sorted(blocks.items())
            if any(regex.search(resource) for regex in regexes)]


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("fable2re", type=Path)
    parser.add_argument("instances", type=Path)
    parser.add_argument("level_body", type=Path)
    parser.add_argument("out", type=Path)
    parser.add_argument("--include", action="append", default=[])
    args = parser.parse_args()
    root = args.fable2re.resolve()
    sys.path.insert(0, str(root / "Fable2Native/tools"))
    sys.path.insert(0, str(root / "Fable2AssetBrowser/source/addons"))
    import cook_levels  # type: ignore
    import fable_mdl_format as mdl  # type: ignore

    f2tool = root / "Fable2AssetBrowser/source/build/f2tool.exe"
    header_bank = root / "Fable2Recomp/assets/game/data/Globals/globals_model_headers.bnk"
    body_bank = args.level_body.resolve()
    header_index = cook_levels._bnk_name_index(header_bank)
    body_index = cook_levels._bnk_name_index(body_bank)
    rows = []
    with tempfile.TemporaryDirectory(prefix="f2_arch_materials_") as temp_name:
        temp = Path(temp_name)

        def extract(bank: Path, entry: str, destination: Path) -> bytes:
            subprocess.run([str(f2tool), "extract", str(bank), entry,
                            str(destination)], check=True, capture_output=True)
            return destination.read_bytes()

        for source in selected_models(args.instances.resolve(),
                                      args.include or list(DEFAULT_INCLUDE)):
            header_entry = cook_levels._resolve(header_index, source["model"])
            body_entry = cook_levels._resolve(body_index, source["model"])
            row = dict(source)
            if not header_entry or not body_entry:
                row.update({"status": "missing_bank_entry",
                            "header_entry": header_entry, "body_entry": body_entry})
                rows.append(row)
                continue
            header = extract(header_bank, header_entry, temp / "header.bin")
            body = extract(body_bank, body_entry, temp / "body.bin")
            logs = []
            try:
                info, geometries = mdl.parse(header + body, log=logs.append,
                                             file_path=source["model"])
                materials = []
                for mesh_index, mesh in enumerate(info.meshes):
                    for material_index, material in enumerate(mesh.materials):
                        materials.append({
                            "mesh_index": mesh_index,
                            "material_index": material_index,
                            "diffuse": material.diffuse,
                            "specular": material.specular,
                            "normal": material.normal,
                            "metallic": material.metallic,
                            "extra": material.extra,
                        })
                row.update({
                    "status": "decoded" if geometries else "no_geometry",
                    "header_entry": header_entry, "body_entry": body_entry,
                    "header_bytes": len(header), "header_sha256": digest(header),
                    "body_bytes": len(body), "body_sha256": digest(body),
                    "geometries": len(geometries),
                    "vertices": sum(len(g.positions) // 3 for g in geometries),
                    "triangles": sum(len(g.indices) // 3 for g in geometries),
                    "materials": materials,
                    "geometry_materials": [{
                        "geometry_index": geometry_index,
                        "mesh_index": geometry.mesh_index,
                        "diffuse": geometry.diffuse,
                        "normal": geometry.normal_tex,
                        "specular": geometry.specular_tex,
                    } for geometry_index, geometry in enumerate(geometries)],
                    "unique_texture_paths": sorted({path for material in materials
                                                    for path in material.values()
                                                    if isinstance(path, str) and path}),
                    "log": logs,
                })
            except Exception as exc:
                row.update({"status": "error", "error_type": type(exc).__name__,
                            "error": str(exc), "log": logs})
            rows.append(row)
    result = {
        "schema": "fableforge.f2_architecture_material_trace.v1",
        "sources": {"instances": str(args.instances.resolve()),
                    "header_bank": str(header_bank), "body_bank": str(body_bank)},
        "selection_regex": args.include or list(DEFAULT_INCLUDE),
        "counts": {
            "models": len(rows),
            "decoded": sum(row["status"] == "decoded" for row in rows),
            "materials": sum(len(row.get("materials", [])) for row in rows),
            "unique_texture_paths": len({path for row in rows
                                         for path in row.get("unique_texture_paths", [])}),
        },
        "models": rows,
    }
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result["counts"], indent=2))
    return 0 if result["counts"]["decoded"] == result["counts"]["models"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
