#!/usr/bin/env python3
"""Convert selected Fable2RE F2SCENE blocks to standalone TLC type-1 meshes.

This deliberately does not edit graphics.big, game.bin, or TNG files.  The
result is an auditable intermediate package: one compiled payload and Info blob
per selected engine-level resource block plus a manifest tying it back to the
authoritative instances.json input.  Materials are diagnostic/untextured until
an explicit Fable II -> TLC texture mapping is supplied.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import math
import re
import sys
from collections import defaultdict
from pathlib import Path


MESH_RE = re.compile(r"^([A-Za-z]+)(\d+)_(\d+)$")
DEFAULT_INCLUDE = (r"\\buildings\\", r"\\structures\\")


def digest(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest().upper()


def f3(values: list[str], what: str) -> tuple[float, float, float]:
    if len(values) != 3:
        raise ValueError(f"{what}: expected three values")
    out = tuple(float(v) for v in values)
    if not all(math.isfinite(v) for v in out):
        raise ValueError(f"{what}: non-finite value")
    return out


def load_selected_blocks(instances_path: Path, patterns: list[str]) -> dict[int, dict]:
    root = json.loads(instances_path.read_text(encoding="utf-8"))
    if root.get("schema") != "fableforge.scene.instances.v1":
        raise ValueError("unsupported instances schema")
    regexes = [re.compile(p, re.IGNORECASE) for p in patterns]
    blocks: dict[int, dict] = {}
    counts: defaultdict[int, int] = defaultdict(int)
    engine_blocks: dict[int, str] = {}
    for row in root.get("instances", []):
        src = row.get("source_record") or {}
        if src.get("container") != "raw/defaultscenario.engine_level":
            continue
        resource = str(row.get("resource_id") or "")
        block = int(src["block"])
        prior_resource = engine_blocks.get(block)
        if prior_resource and prior_resource != resource:
            raise ValueError(f"block {block} has multiple resource identities")
        engine_blocks[block] = resource
        if not any(p.search(resource) for p in regexes):
            continue
        previous = blocks.get(block)
        if previous and previous["resource_id"] != resource:
            raise ValueError(f"block {block} has multiple resource identities")
        blocks[block] = {"resource_id": resource, "category": row.get("category")}
        counts[block] += 1
    for block, row in blocks.items():
        row["source_instance_count"] = counts[block]
    # cook_levels assigns m<N> by first appearance of each distinct model in
    # engine-block order.  N is not generally the source block number because
    # later blocks can reuse an earlier model (BWSMarket low-poly houses are the
    # first selected examples where the old coincidence breaks).
    resource_slots: dict[str, int] = {}
    for block in sorted(engine_blocks):
        resource = engine_blocks[block]
        resource_slots.setdefault(resource, len(resource_slots))
    for row in blocks.values():
        row["scene_model_slot"] = resource_slots[row["resource_id"]]
    return blocks


def parse_selected_scene(path: Path, wanted_slots: dict[int, int]) -> dict[int, list[dict]]:
    result: defaultdict[int, list[dict]] = defaultdict(list)
    current = None
    remaining_vertices = remaining_indices = 0
    with path.open("r", encoding="utf-8-sig") as stream:
        for line_no, raw in enumerate(stream, 1):
            parts = raw.strip().split()
            if not parts or parts[0] == "#":
                continue
            if parts[0] == "mesh":
                if current is not None and (remaining_vertices or remaining_indices):
                    raise ValueError(f"line {line_no}: previous mesh was truncated")
                match = MESH_RE.match(parts[1])
                if not match or len(parts) != 5:
                    raise ValueError(f"line {line_no}: invalid mesh header")
                family, block_text, submesh_text = match.groups()
                scene_slot, submesh = int(block_text), int(submesh_text)
                vcount, icount, material = map(int, parts[2:])
                if vcount < 0 or icount < 0 or icount % 3:
                    raise ValueError(f"line {line_no}: invalid mesh counts")
                current = None
                remaining_vertices, remaining_indices = vcount, icount
                if family.lower() == "m" and scene_slot in wanted_slots:
                    block = wanted_slots[scene_slot]
                    current = {"block": block, "submesh": submesh, "material": material,
                               "verts": [], "normals": [], "uvs": [], "indices": []}
                    result[block].append(current)
                continue
            if remaining_vertices:
                if parts[0] != "vertex" or len(parts) != 9:
                    raise ValueError(f"line {line_no}: expected vertex")
                if current is not None:
                    pos = f3(parts[1:4], f"line {line_no} position")
                    normal = f3(parts[4:7], f"line {line_no} normal")
                    # F2SCENE is render X/Y-up/Z; TLC authoring is game X/Y/Z-up.
                    current["verts"].append((pos[0], pos[2], pos[1]))
                    current["normals"].append((normal[0], normal[2], normal[1]))
                    uv = tuple(float(v) for v in parts[7:9])
                    if not all(math.isfinite(v) for v in uv):
                        raise ValueError(f"line {line_no}: non-finite UV")
                    current["uvs"].append(uv)
                remaining_vertices -= 1
                continue
            if remaining_indices:
                if parts[0] != "index" or len(parts) != 2:
                    raise ValueError(f"line {line_no}: expected index")
                if current is not None:
                    current["indices"].append(int(parts[1]))
                remaining_indices -= 1
                continue
    if remaining_vertices or remaining_indices:
        raise ValueError("scene ended inside a mesh")
    return dict(result)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("scene", type=Path)
    ap.add_argument("instances", type=Path)
    ap.add_argument("out", type=Path)
    ap.add_argument("--include", action="append", default=[])
    ap.add_argument("--material-trace", type=Path)
    ap.add_argument("--texture-manifest", type=Path)
    args = ap.parse_args()

    addon = Path(__file__).resolve().parent / "blender_addon" / "io_scene_fable"
    sys.path.insert(0, str(addon))
    import mesh_rw  # noqa: E402

    patterns = args.include or list(DEFAULT_INCLUDE)
    selected = load_selected_blocks(args.instances, patterns)
    wanted_slots = {row["scene_model_slot"]: block
                    for block, row in selected.items()}
    if len(wanted_slots) != len(selected):
        raise ValueError("selected blocks unexpectedly share a scene model slot")
    parsed = parse_selected_scene(args.scene, wanted_slots)
    traced_materials = {}
    texture_ids = {}
    if bool(args.material_trace) != bool(args.texture_manifest):
        raise ValueError("--material-trace and --texture-manifest must be supplied together")
    if args.material_trace:
        trace = json.loads(args.material_trace.read_text(encoding="utf-8"))
        traced_materials = {int(row["block"]): row.get("geometry_materials", [])
                            for row in trace["models"]}
        texture_package = json.loads(args.texture_manifest.read_text(encoding="utf-8"))
        texture_ids = {row["texture"]: int(row["texture_id"])
                       for row in texture_package["assigned_textures"]}
    args.out.mkdir(parents=True, exist_ok=True)
    records = []
    for block in sorted(selected):
        submeshes = parsed.get(block, [])
        if not submeshes:
            records.append({"block": block, **selected[block], "status": "missing_geometry"})
            continue
        prims = []
        ordered_submeshes = sorted(submeshes, key=lambda x: x["submesh"])
        geometry_materials = traced_materials.get(block, [])
        if args.material_trace and len(geometry_materials) != len(ordered_submeshes):
            raise ValueError(f"block {block}: geometry/material trace count mismatch")
        assigned_diffuse = []
        for geometry_index, sub in enumerate(ordered_submeshes):
            nverts = len(sub["verts"])
            if any(i < 0 or i >= nverts for i in sub["indices"]):
                raise ValueError(f"block {block} submesh {sub['submesh']}: index out of range")
            prims.append({"verts": sub["verts"], "normals": sub["normals"],
                          "uvs": sub["uvs"],
                          "faces": [tuple(sub["indices"][i:i + 3])
                                    for i in range(0, len(sub["indices"]), 3)],
                          "mat": len(prims), "layout": "float"})
            diffuse = (geometry_materials[geometry_index].get("diffuse", "")
                       if geometry_materials else "")
            if diffuse and diffuse not in texture_ids:
                raise ValueError(f"block {block}: no TLC texture for {diffuse}")
            assigned_diffuse.append({"source_texture": diffuse,
                                     "texture_id": texture_ids.get(diffuse, 0)})
        name = f"MESH_F2_BWS_BLOCK_{block:04d}"
        materials = [{"name": f"F2_{block:04d}_{i:02d}",
                      "diffuse_id": assigned_diffuse[i]["texture_id"],
                      "two_sided": False}
                     for i in range(len(prims))]
        # Stored LZO chunks keep the intermediate independently decodable even
        # on machines without the optional native Lionhead-LZO extension.
        payload, info = mesh_rw.compose_mesh(name, prims, materials, compress=False)
        stem = name.lower()
        payload_path, info_path = args.out / f"{stem}.bin", args.out / f"{stem}.info"
        payload_path.write_bytes(payload)
        info_path.write_bytes(info)
        records.append({"block": block, **selected[block], "status": "compiled",
                        "mesh_name": name, "submeshes": len(prims),
                        "source_material_slots": [sub["material"] for sub in
                                                  sorted(submeshes, key=lambda x: x["submesh"])],
                        "assigned_diffuse_textures": assigned_diffuse,
                        "vertices": sum(len(p["verts"]) for p in prims),
                        "triangles": sum(len(p["faces"]) for p in prims),
                        "payload": payload_path.name, "payload_sha256": digest(payload_path),
                        "info": info_path.name, "info_sha256": digest(info_path),
                        "material_status": ("authoritative_f2_albedo_import"
                                            if args.material_trace else
                                            "diagnostic_untextured_texture_id_0")})
    manifest = {
        "schema": "fableforge.f2scene_tlc_static_meshes.v1",
        "dry_run": True,
        "source": {"scene": str(args.scene.resolve()), "scene_sha256": digest(args.scene),
                   "instances": str(args.instances.resolve()),
                   "instances_sha256": digest(args.instances)},
        "selection_regex": patterns,
        "coordinate_transform": "F2SCENE render (x,y,z) -> TLC game (x,z,y)",
        "container_mutation": "none",
        "material_trace": str(args.material_trace.resolve()) if args.material_trace else None,
        "texture_manifest": str(args.texture_manifest.resolve()) if args.texture_manifest else None,
        "counts": {"selected_blocks": len(selected),
                   "compiled_blocks": sum(r["status"] == "compiled" for r in records),
                   "missing_geometry_blocks": sum(r["status"] != "compiled" for r in records),
                   "vertices": sum(r.get("vertices", 0) for r in records),
                   "triangles": sum(r.get("triangles", 0) for r in records)},
        "meshes": records,
    }
    manifest_path = args.out / "mesh_manifest.json"
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(manifest["counts"], indent=2))
    print(manifest_path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
