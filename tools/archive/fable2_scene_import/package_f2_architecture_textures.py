#!/usr/bin/env python3
"""Append cooked F2 architecture albedos to one TLC textures.big rebuild."""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path


def file_sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest().upper()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source_big", type=Path)
    parser.add_argument("dds_package", type=Path)
    parser.add_argument("output_big", type=Path)
    parser.add_argument("--bank", default="GBANK_MAIN_PC")
    args = parser.parse_args()
    if args.source_big.resolve() == args.output_big.resolve():
        raise SystemExit("refusing in-place textures.big mutation")
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import big_write  # type: ignore
    import texture_build  # type: ignore

    manifest_path = args.dds_package / "texture_manifest.json"
    cooked = json.loads(manifest_path.read_text(encoding="utf-8"))
    source = args.source_big.read_bytes()
    _buf, parsed = texture_build.load_big(str(args.source_big))
    bank_entries = next(entries for sub, entries in parsed
                        if sub["name"] == args.bank)
    existing_names = {entry["name"] for entry in bank_entries}
    next_id = max(entry["id"] for entry in bank_entries) + 1
    additions = []
    assigned = []
    for row in sorted(cooked["textures"], key=lambda item: item["texture"].lower()):
        if row["status"] != "cooked":
            raise ValueError(f"uncooked diffuse texture: {row['texture']}")
        image = texture_build.load_image_rgba(str(args.dds_package / row["dds"]))
        real_height, real_width = image.shape[:2]
        alloc_width = texture_build._pow2_up(real_width)
        alloc_height = texture_build._pow2_up(real_height)
        image = texture_build.fit_to_alloc(image, alloc_width, alloc_height)
        has_alpha = bool((image[..., 3] < 255).any())
        texture_format = (texture_build.FMT_DXT3 if has_alpha
                          else texture_build.FMT_DXT1)
        payload, info = texture_build.build_entry(
            image, texture_format, real_w=real_width, real_h=real_height)
        name_hash = hashlib.sha256(row["texture"].lower().encode("utf-8")).hexdigest()[:12]
        name = f"F2_BWS_{name_hash.upper()}"
        if name in existing_names:
            raise ValueError(f"texture entry already exists: {name}")
        additions.append({"sub": args.bank, "name": name, "payload": payload,
                          "type": 0, "info": info, "id": next_id})
        assigned.append({"texture": row["texture"], "entry": name,
                         "texture_id": next_id,
                         "format": "dxt3" if has_alpha else "dxt1",
                         "width": real_width, "height": real_height,
                         "payload_bytes": len(payload),
                         "source_dds": row["dds"],
                         "source_dds_sha256": row["dds_sha256"]})
        existing_names.add(name)
        next_id += 1
    rebuilt = big_write.rebuild(source, adds=additions)
    args.output_big.parent.mkdir(parents=True, exist_ok=True)
    args.output_big.write_bytes(rebuilt)
    result = {
        "schema": "fableforge.f2_architecture_tlc_texture_package.v1",
        "installed": False,
        "source_big": str(args.source_big.resolve()),
        "source_big_sha256": file_sha(args.source_big),
        "output_big": str(args.output_big.resolve()),
        "output_big_sha256": file_sha(args.output_big),
        "dds_manifest": str(manifest_path.resolve()),
        "bank": args.bank,
        "entries_added": len(assigned),
        "assigned_textures": assigned,
    }
    output_manifest = args.output_big.with_suffix(args.output_big.suffix + ".manifest.json")
    output_manifest.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"entries_added": len(assigned),
                      "first_id": assigned[0]["texture_id"],
                      "last_id": assigned[-1]["texture_id"]}, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
