#!/usr/bin/env python3
"""Validate entity allocators and ensure native local fields fit retail objects."""
from __future__ import annotations

import json
import re
from pathlib import Path


def audit(layouts_path: Path, vtables_path: Path, translation_path: Path, inventories: Path) -> dict:
    layouts = json.loads(layouts_path.read_text(encoding="utf-8-sig"))["entities"]
    vtables = json.loads(vtables_path.read_text(encoding="utf-8-sig"))["entities"]
    functions = json.loads(translation_path.read_text(encoding="utf-8-sig"))["functions"]
    errors: list[str] = []
    fields = slack = 0
    expected_entities = set(vtables)
    if set(layouts) != expected_entities:
        errors.append("layout snapshot does not cover exactly the 16 entity classes")
    by_address = {row["address"].lower(): row for row in functions}
    for entity, layout in layouts.items():
        allocator = by_address.get(layout["allocator"].lower())
        if not allocator:
            errors.append(f"{entity}: allocator absent from translation unit")
            continue
        decompile = allocator.get("decompile", "")
        size_match = re.search(r"operator_new\((0x[0-9a-fA-F]+|\d+)\)", decompile)
        actual_size = int(size_match.group(1), 0) if size_match else None
        if actual_size != layout["objectSize"]:
            errors.append(f"{entity}: allocator size {actual_size} != snapshot {layout['objectSize']}")
        vtable_token = vtables[entity]["vtable"].lower().removeprefix("0x")
        if vtable_token not in decompile.lower():
            errors.append(f"{entity}: allocator does not install its retail vtable")
        inventory = json.loads((inventories / f"{entity}.json").read_text(encoding="utf-8-sig"))
        required = 0x1C
        for field in inventory.get("localFields", []):
            if field["offset"] == "stack":
                continue
            fields += 1
            width = 1 if field.get("type") == "bool" or isinstance(field.get("initValue"), bool) else 4
            required = max(required, int(field["offset"], 0) + width)
        if required > layout["objectSize"]:
            errors.append(f"{entity}: fields require {required} bytes, object has {layout['objectSize']}")
        else:
            slack += layout["objectSize"] - required
    return {
        "schema": "new-oakvale-entity-layout-audit/0.1",
        "entityAllocators": len(layouts),
        "classLocalFields": fields,
        "aggregateTailSlackBytes": slack,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(
        root / "ghidra_out/script_recovery/new_oakvale_entity_layouts.json",
        root / "ghidra_out/script_recovery/new_oakvale_entity_vtables.json",
        root / "refs/script_recovery/new_oakvale_intro/translation_unit.json",
        root / "refs/script_recovery/new_oakvale_intro/entities"), indent=2))
