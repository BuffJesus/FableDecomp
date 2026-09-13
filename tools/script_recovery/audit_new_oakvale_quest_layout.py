#!/usr/bin/env python3
"""Validate named quest fields against the recovered native object layout."""
from __future__ import annotations

import json
from pathlib import Path


WIDTHS = {"bool": 1, "int": 4, "string": 12, "bool[5]": 5}


def _range(text: str) -> tuple[int, int]:
    if ".." in text:
        first, last = text.split("..", 1)
        return int(first, 0), int(last, 0) + 1
    start = int(text, 0)
    return start, start


def audit(workspace: Path) -> dict:
    persistence = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/persistence_manifest.json").read_text(encoding="utf-8-sig"))
    layout = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_quest_layout.json").read_text(encoding="utf-8-sig"))
    quest = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/entities/Q_NewOakValeIntro.json").read_text(encoding="utf-8-sig"))
    constructor = (workspace / "ghidra_out/script_recovery/native_quest_construction/0x00DAAC00.c").read_text(encoding="utf-8")
    base = (workspace / "ghidra_out/script_recovery/native_quest_construction/0x00CB8110.c").read_text(encoding="utf-8")
    errors: list[str] = []
    if quest.get("objectSize") != layout["objectSize"]:
        errors.append("quest inventory and layout object sizes differ")

    occupied: dict[int, str] = {}
    for field in persistence["fields"]:
        start, explicit_end = _range(field["retailOffset"])
        width = WIDTHS.get(field["type"])
        if width is None:
            errors.append(f"{field['name']}: unknown width for {field['type']}")
            continue
        end = start + width
        if explicit_end != start and explicit_end != end:
            errors.append(f"{field['name']}: range width differs from type")
        if end > layout["objectSize"]:
            errors.append(f"{field['name']}: extends beyond object")
        for offset in range(start, end):
            if offset in occupied:
                errors.append(f"{field['name']}: overlaps {occupied[offset]} at {offset:#x}")
            occupied[offset] = field["name"]

    regions = [(int(row["offset"], 0), int(row["offset"], 0) + row["size"], row["name"])
               for row in layout["regions"]]
    if regions[0][0] != 0 or regions[-1][1] != layout["objectSize"] or any(
            left[1] != right[0] for left, right in zip(regions, regions[1:])):
        errors.append("layout regions are not contiguous across the complete object")
    for fragment in ("this + 0x9c", "this + 0xf8", "this + 0x104", "this + 0x108"):
        if fragment not in constructor:
            errors.append(f"derived constructor missing layout anchor {fragment}")
    for fragment in ("this + 4", "this + 0x38", "this[0x3d]"):
        if fragment not in base:
            errors.append(f"base constructor missing layout anchor {fragment}")
    return {
        "schema": "new-oakvale-quest-layout-audit/0.1",
        "objectSize": layout["objectSize"],
        "namedFields": len(persistence["fields"]),
        "namedBytes": len(occupied),
        "layoutRegions": len(regions),
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
