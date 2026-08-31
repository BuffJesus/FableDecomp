#!/usr/bin/env python3
"""Verify every file and safety invariant in a retail-shadow smoke package."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path


def sha256(path: Path) -> str:
    digest = hashlib.sha256(path.read_bytes())
    return digest.hexdigest().upper()


def verify(root: Path) -> dict[str, object]:
    manifest_path = root / "shadow_smoke_manifest.json"
    manifest = json.loads(manifest_path.read_text(encoding="utf-8-sig"))
    failures: list[str] = []
    safety = manifest.get("safety", {})
    if safety != {"mode": "shadow", "mutations": False, "saveWrites": False}:
        failures.append("unsafe manifest policy")
    for row in manifest.get("files", []):
        path = root / Path(row["path"])
        try:
            resolved = path.resolve()
            resolved.relative_to(root.resolve())
        except ValueError:
            failures.append(f"path escapes package: {row['path']}")
            continue
        if not path.is_file():
            failures.append(f"missing: {row['path']}")
        elif path.stat().st_size != row["size"] or sha256(path) != row["sha256"]:
            failures.append(f"hash/size mismatch: {row['path']}")
    config = (root / "FSE" / "retail_shadow.lua").read_text(encoding="utf-8-sig")
    expected = int(manifest.get("expectedScripts", 0))
    if "enabled = true" not in config:
        failures.append("shadow configuration is not enabled")
    if config.count("mutatingCallsAllowed = false") != expected:
        failures.append("mutation gates do not cover every script")
    if config.count("saveWritesAllowed = false") != expected:
        failures.append("save-write gates do not cover every script")
    return {"schema": "forgefse-retail-shadow-package-verification/0.1",
            "complete": not failures, "expectedScripts": expected,
            "verifiedFiles": len(manifest.get("files", [])), "failures": failures}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("package", type=Path)
    args = parser.parse_args()
    result = verify(args.package.resolve())
    print(json.dumps(result, sort_keys=True))
    return 0 if result["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
