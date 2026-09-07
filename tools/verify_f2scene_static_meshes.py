#!/usr/bin/env python3
"""Independently decode and verify a convert_f2scene_static_meshes package."""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest().upper()


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("package", type=Path)
    args = ap.parse_args()
    manifest_path = args.package / "mesh_manifest.json"
    root = json.loads(manifest_path.read_text(encoding="utf-8"))
    if root.get("schema") != "fableforge.f2scene_tlc_static_meshes.v1":
        raise SystemExit("FAIL unsupported manifest schema")
    addon = Path(__file__).resolve().parent / "blender_addon" / "io_scene_fable"
    sys.path.insert(0, str(addon))
    import fable_core as fc  # noqa: E402
    import mesh_rw  # noqa: E402

    checks = failures = compiled = vertices = triangles = 0
    seen_blocks, seen_names = set(), set()
    for row in root.get("meshes", []):
        block = row["block"]
        checks += 1
        if block in seen_blocks:
            failures += 1; print(f"FAIL duplicate block {block}")
        seen_blocks.add(block)
        if row["status"] != "compiled":
            continue
        compiled += 1
        payload_path, info_path = args.package / row["payload"], args.package / row["info"]
        for path, key in ((payload_path, "payload_sha256"), (info_path, "info_sha256")):
            checks += 2
            if not path.is_file() or digest(path) != row[key]:
                failures += 1; print(f"FAIL file/hash {path}")
        payload, info = payload_path.read_bytes(), info_path.read_bytes()
        name, prims, bones = fc._parse_mesh_primitives(payload, 1)
        mats = mesh_rw.decode_to_compose_args(payload, 1)[2]
        checks += 6
        if name != row["mesh_name"] or name in seen_names:
            failures += 1; print(f"FAIL name {name}")
        seen_names.add(name)
        if bones is not None or len(prims) != row["submeshes"]:
            failures += 1; print(f"FAIL primitive grammar {name}")
        source_slots = row.get("source_material_slots", [])
        checks += 2
        if len(source_slots) != len(prims):
            failures += 1; print(f"FAIL source material slot count {name}")
        if (any(not isinstance(slot, int) for slot in source_slots) or
                len(set(source_slots)) != len(source_slots)):
            failures += 1; print(f"FAIL source material slot identities {name}")
        got_v = got_t = 0
        for prim in prims:
            verts, faces, _uv = fc._prim_geometry(prim, 1)
            got_v += len(verts); got_t += len(faces)
        if (got_v, got_t) != (row["vertices"], row["triangles"]):
            failures += 1; print(f"FAIL geometry counts {name}")
        material_status = row.get("material_status")
        assigned = row.get("assigned_diffuse_textures", [])
        checks += 2
        if material_status == "authoritative_f2_albedo_import":
            if len(mats) != len(prims) or len(assigned) != len(prims):
                failures += 1; print(f"FAIL textured material count {name}")
            elif any(not isinstance(item.get("texture_id"), int) or
                     (item.get("source_texture") and item["texture_id"] <= 0) or
                     (not item.get("source_texture") and item["texture_id"] != 0) or
                     mats[index].get("diffuse_id") != item["texture_id"]
                     for index, item in enumerate(assigned)):
                failures += 1; print(f"FAIL textured material identities {name}")
        elif (len(mats) != len(prims) or
              any(m.get("diffuse_id") != 0 for m in mats)):
            failures += 1; print(f"FAIL diagnostic materials {name}")
        if len(info) < 60 or struct.unpack_from("<I", info, 44)[0] != 1:
            failures += 1; print(f"FAIL Info LOD grammar {name}")
        vertices += got_v; triangles += got_t
    expected = root["counts"]
    checks += 4
    actual_missing = len(root["meshes"]) - compiled
    if compiled != expected["compiled_blocks"]: failures += 1
    if actual_missing != expected["missing_geometry_blocks"]: failures += 1
    if vertices != expected["vertices"]: failures += 1
    if triangles != expected["triangles"]: failures += 1
    print(json.dumps({"checks": checks, "failures": failures, "compiled_blocks": compiled,
                      "vertices": vertices, "triangles": triangles}, indent=2))
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
