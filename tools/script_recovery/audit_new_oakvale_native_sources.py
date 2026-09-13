#!/usr/bin/env python3
"""Verify every inventoried New Oakvale function has a reproducible local decompile."""
from __future__ import annotations

import json
from pathlib import Path


def normalized_address(value: str) -> str:
    return f"0x{int(value, 16):08X}"


def audit(workspace: Path, entities_dir: Path) -> dict:
    errors: list[str] = []
    functions = 0
    expected_files: set[Path] = set()
    inventories = {
        inventory.stem: json.loads(inventory.read_text(encoding="utf-8-sig"))
        for inventory in sorted(entities_dir.glob("*.json"))
    }
    function_index = {
        f"{script}.{name}": normalized_address(function["address"])
        for script, data in inventories.items()
        for name, function in data.get("functions", {}).items()
    }
    coverage_aliases = 0
    for script, data in inventories.items():
        inventory_name = f"{script}.json"
        for name, function in data.get("functions", {}).items():
            functions += 1
            address = normalized_address(function["address"])
            relative = function.get("decompileFile")
            if not relative:
                errors.append(f"{inventory_name}:{name}: missing decompileFile")
                continue
            expected = Path("ghidra_out/script_recovery/native_decompiles") / f"{address}.c"
            if Path(relative).as_posix() != expected.as_posix():
                errors.append(f"{inventory_name}:{name}: stale locator {relative}; expected {expected.as_posix()}")
                continue
            source = workspace / expected
            expected_files.add(source.resolve())
            if not source.is_file():
                errors.append(f"{inventory_name}:{name}: missing {expected.as_posix()}")
                continue
            header = source.read_text(encoding="utf-8", errors="replace").splitlines()[0]
            if f"function entry {address}" not in header or "DecompFuncsToDirectory.java" not in header:
                errors.append(f"{inventory_name}:{name}: invalid provenance header")
            alias = function.get("coverageAlias")
            if alias:
                coverage_aliases += 1
                if function_index.get(alias) != address:
                    errors.append(
                        f"{inventory_name}:{name}: coverage alias {alias} does not resolve to {address}"
                    )

    export_dir = workspace / "ghidra_out/script_recovery/native_decompiles"
    actual_files = {path.resolve() for path in export_dir.glob("0x*.c")}
    for extra in sorted(actual_files - expected_files):
        errors.append(f"unreferenced native decompile: {extra.relative_to(workspace).as_posix()}")
    return {
        "schema": "new-oakvale-native-source-audit/0.1",
        "inventoryFunctions": functions,
        "uniqueNativeEntries": len(expected_files),
        "exportedDecompiles": len(actual_files),
        "validatedCoverageAliases": coverage_aliases,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(root, root / "refs/script_recovery/new_oakvale_intro/entities"), indent=2))
