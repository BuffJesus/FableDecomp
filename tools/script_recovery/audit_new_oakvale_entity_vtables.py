#!/usr/bin/env python3
"""Validate retail vtable slots against inventory ownership and native exports."""
from __future__ import annotations

import json
import re
from pathlib import Path


def audit(snapshot: Path, inventories: Path, root: Path, translation_unit: Path) -> dict:
    evidence = json.loads(snapshot.read_text(encoding="utf-8-sig"))
    translation = translation_unit.read_text(encoding="utf-8-sig").lower()
    errors: list[str] = []
    checked = decompiles = 0
    inventory_entities = set()
    for inventory_path in inventories.glob("*.json"):
        inventory = json.loads(inventory_path.read_text(encoding="utf-8-sig"))
        if not inventory_path.stem.startswith("Q_") and {"Main", "Init"} <= set(inventory.get("functions", {})):
            inventory_entities.add(inventory["entity"])
    if set(evidence.get("entities", {})) != inventory_entities:
        errors.append("vtable snapshot does not cover exactly the Main/Init inventories")
    if evidence.get("slots") != ["destructor", "Main", "Init"]:
        errors.append("vtable slot order is not destructor/Main/Init")
    if not re.fullmatch(r"[0-9A-F]{64}", evidence.get("sourceSha256", "")):
        errors.append("retail executable SHA-256 is missing or malformed")
    for entity, slots in evidence.get("entities", {}).items():
        checked += 1
        inventory_path = inventories / f"{entity}.json"
        if not inventory_path.is_file():
            errors.append(f"{entity}: inventory missing")
            continue
        data = json.loads(inventory_path.read_text(encoding="utf-8-sig"))
        if int(data.get("vtable", "0"), 0) != int(slots["vtable"], 0):
            errors.append(f"{entity}: vtable differs from retail snapshot")
        for function in ("Main", "Init"):
            record = data.get("functions", {}).get(function, {})
            if int(record.get("address", "0"), 0) != int(slots[function], 0):
                errors.append(f"{entity}: {function} differs from retail vtable slot")
            decompile = root / record.get("decompileFile", "")
            if not decompile.is_file():
                errors.append(f"{entity}: {function} native decompile missing")
            else:
                decompiles += 1
        if slots["vtable"].lower() not in translation:
            errors.append(f"{entity}: allocator/vtable evidence absent from translation unit")
    return {
        "schema": "new-oakvale-entity-vtable-audit/0.1",
        "entities": checked,
        "functionSlots": checked * 2,
        "nativeDecompiles": decompiles,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(
        root / "ghidra_out/script_recovery/new_oakvale_entity_vtables.json",
        root / "refs/script_recovery/new_oakvale_intro/entities", root,
        root / "refs/script_recovery/new_oakvale_intro/translation_unit.json"), indent=2))
