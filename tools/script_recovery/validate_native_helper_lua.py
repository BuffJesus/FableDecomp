#!/usr/bin/env python3
"""Execute generated standalone helper Lua against its native-derived truth table."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
from typing import Any

from lupa import LuaRuntime


def validate(manifest_path: Path) -> dict[str, Any]:
    manifest = json.loads(manifest_path.read_text(encoding="utf-8-sig"))
    root = manifest_path.parent
    rows = []
    for entry in manifest["entries"]:
        path = root / entry["luaFile"]
        payload = path.read_bytes()
        actual_hash = hashlib.sha256(payload).hexdigest().upper()
        errors = []
        if actual_hash != entry["luaSha256"]:
            errors.append("sha256 mismatch")
        function = LuaRuntime(unpack_returned_tuples=True).execute(payload.decode("utf-8"))
        pattern = entry["semanticPattern"]
        for case in pattern["cases"]:
            actual = function(case["input"])
            if actual != case["return"]:
                errors.append(f"input {case['input']}: expected {case['return']}, got {actual}")
        used = {case["input"] for case in pattern["cases"]}
        defaults = [value for value in (0, -1, max(used, default=0) + 1) if value not in used]
        for value in defaults:
            actual = function(value)
            if actual != pattern["defaultReturn"]:
                errors.append(f"default {value}: expected {pattern['defaultReturn']}, got {actual}")
        rows.append({"targetAddress": entry["targetAddress"], "passed": not errors,
                     "cases": len(pattern["cases"]) + len(defaults), "errors": errors})
    return {"schema": "forgefse-native-helper-lua-validation/0.1",
            "summary": {"helpers": len(rows), "passed": sum(row["passed"] for row in rows),
                        "cases": sum(row["cases"] for row in rows),
                        "complete": all(row["passed"] for row in rows)},
            "helpers": rows}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = validate(args.manifest)
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result["summary"], sort_keys=True))
    return 0 if result["summary"]["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
