#!/usr/bin/env python3
"""Cook authoritative Fable II architecture albedos to audited DDS files."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
import subprocess
import tempfile
from pathlib import Path


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest().upper()


def safe_stem(path: str) -> str:
    leaf = path.replace("\\", "/").rsplit("/", 1)[-1].rsplit(".", 1)[0]
    leaf = re.sub(r"[^A-Za-z0-9_.-]+", "_", leaf).strip("_") or "texture"
    suffix = hashlib.sha256(path.lower().encode("utf-8")).hexdigest()[:8]
    return f"{leaf}_{suffix}"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("f2tool", type=Path)
    parser.add_argument("tex_cook", type=Path)
    parser.add_argument("audit", type=Path)
    parser.add_argument("out", type=Path)
    args = parser.parse_args()
    audit = json.loads(args.audit.read_text(encoding="utf-8"))
    args.out.mkdir(parents=True, exist_ok=True)
    rows = []
    with tempfile.TemporaryDirectory(prefix="f2_arch_tex_") as temp_name:
        temp = Path(temp_name)
        for source in audit["textures"]:
            if not any(use["role"] == "diffuse" for use in source["usages"]):
                continue
            row = {"texture": source["texture"], "usages": source["usages"]}
            if not source["payload_matches"]:
                row["status"] = "missing_payload"
                rows.append(row)
                continue
            payload_match = source["payload_matches"][0]
            stem = safe_stem(source["texture"])
            raw_path = temp / f"{stem}.tex"
            dds_path = args.out / f"{stem}.dds"
            subprocess.run([str(args.f2tool), "extract", payload_match["bank"],
                            payload_match["entry"], str(raw_path)], check=True,
                           capture_output=True)
            raw = raw_path.read_bytes()
            command = [str(args.tex_cook), str(raw_path), str(dds_path)]
            header_record = None
            if source["header_matches"]:
                header_match = source["header_matches"][0]
                header_path = temp / f"{stem}.header"
                subprocess.run([str(args.f2tool), "extract", header_match["bank"],
                                header_match["entry"], str(header_path)], check=True,
                               capture_output=True)
                header = header_path.read_bytes()
                if len(header) >= 0x1C:
                    width, height, pixel_format = struct.unpack_from(">III", header, 0x10)
                    if width and height and pixel_format:
                        command += ["--pf", str(pixel_format), "--width", str(width),
                                    "--height", str(height)]
                    header_record = {"bank": header_match["bank"],
                                     "entry": header_match["entry"],
                                     "bytes": len(header), "sha256": sha(header),
                                     "width": width, "height": height,
                                     "pixel_format": pixel_format}
            result = subprocess.run(command, capture_output=True, text=True)
            if result.returncode != 0 or not dds_path.is_file():
                row.update({"status": "cook_error", "stdout": result.stdout.strip(),
                            "stderr": result.stderr.strip()})
                rows.append(row)
                continue
            dds = dds_path.read_bytes()
            row.update({
                "status": "cooked", "payload_source": payload_match,
                "source_bytes": len(raw), "source_sha256": sha(raw),
                "header_source": header_record,
                "dds": dds_path.name, "dds_bytes": len(dds), "dds_sha256": sha(dds),
            })
            rows.append(row)
    manifest = {
        "schema": "fableforge.f2_architecture_dds_package.v1",
        "source_audit": str(args.audit.resolve()),
        "counts": {"diffuse_textures": len(rows),
                   "cooked": sum(row["status"] == "cooked" for row in rows),
                   "failed": sum(row["status"] != "cooked" for row in rows)},
        "textures": rows,
    }
    (args.out / "texture_manifest.json").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(manifest["counts"], indent=2))
    return 0 if manifest["counts"]["failed"] == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
