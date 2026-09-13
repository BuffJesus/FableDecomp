#!/usr/bin/env python3
"""Validate structural ownership and sequencing in New Oakvale evidence inventories."""
from __future__ import annotations

import json
from pathlib import Path

try:
    from tools.script_recovery.build_reconstruction_manifests import native_function_name
except ModuleNotFoundError:
    from build_reconstruction_manifests import native_function_name


def audit(entities_dir: Path) -> dict:
    errors: list[str] = []
    operations = 0
    nested_helpers: set[str] = set()
    for path in sorted(entities_dir.glob("*.json")):
        data = json.loads(path.read_text(encoding="utf-8-sig"))
        functions = set(data.get("functions", {}))
        rows = data.get("operations", [])
        operations += len(rows)
        sequences = [row.get("seq") for row in rows]
        if sequences != list(range(1, len(rows) + 1)):
            errors.append(f"{path.name}: operation sequence is not contiguous and ordered")
        for row in rows:
            label = row.get("function")
            owner = native_function_name(row)
            if owner not in functions:
                errors.append(f"{path.name}#{row.get('seq')}: undeclared native owner {owner}")
            if isinstance(label, str) and "/" in label:
                nested_helpers.add(f"{path.stem}.{label}")
    return {
        "schema": "new-oakvale-inventory-structure-audit/0.1",
        "inventoriedOperations": operations,
        "nestedLuaHelpers": sorted(nested_helpers),
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(root / "refs/script_recovery/new_oakvale_intro/entities"), indent=2))
