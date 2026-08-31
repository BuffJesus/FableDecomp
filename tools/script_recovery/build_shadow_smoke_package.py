#!/usr/bin/env python3
"""Build a deployable, mutation-free ForgeFSE retail shadow smoke package."""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
import sys
from pathlib import Path

if __package__ in {None, ""}:
    sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from tools.script_recovery.build_override_manifest import build


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest().upper()


def package(catalog: Path, corpus_index: Path, corpus_sources: Path,
            dll: Path, output: Path) -> dict[str, object]:
    if not dll.is_file():
        raise FileNotFoundError(f"ForgeFSE DLL not found: {dll}")
    index = json.loads(corpus_index.read_text(encoding="utf-8-sig"))
    output.mkdir(parents=True, exist_ok=True)
    fse = output / "FSE"
    fse.mkdir(parents=True, exist_ok=True)

    copied: list[Path] = []
    target_dll = output / "FableScriptExtender.dll"
    shutil.copy2(dll, target_dll)
    copied.append(target_dll)

    for package_row in index["packages"]:
        package_root = corpus_sources / package_row["name"]
        for file_row in package_row["files"]:
            relative = Path(file_row["path"])
            if relative.name.lower() == "quests.lua" or not relative.name.lower().endswith(".lua"):
                continue
            source = package_root / relative
            destination = output / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, destination)
            if sha256(destination) != file_row["sha256"].upper():
                raise ValueError(f"copied source hash mismatch: {relative}")
            copied.append(destination)

    shadow_json = fse / "retail_shadow.json"
    shadow_lua = fse / "retail_shadow.lua"
    counts = build(catalog, corpus_index, shadow_json, "shadow", shadow_lua, True)
    copied.extend([shadow_json, shadow_lua])

    manifest = {
        "schema": "forgefse-retail-shadow-smoke-package/0.1",
        "safety": {"mode": "shadow", "mutations": False, "saveWrites": False},
        "expectedScripts": counts["shadow"],
        "files": [
            {"path": path.relative_to(output).as_posix(), "sha256": sha256(path), "size": path.stat().st_size}
            for path in sorted(copied)
        ],
        "log": "FSE/FableScriptExtender.log",
    }
    manifest_path = output / "shadow_smoke_manifest.json"
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    return {"scripts": counts["shadow"], "files": len(copied), "output": str(output)}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--catalog", type=Path, required=True)
    parser.add_argument("--corpus", type=Path, required=True)
    parser.add_argument("--sources", type=Path, required=True)
    parser.add_argument("--dll", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(package(args.catalog.resolve(), args.corpus.resolve(), args.sources.resolve(),
                             args.dll.resolve(), args.output.resolve()), sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
