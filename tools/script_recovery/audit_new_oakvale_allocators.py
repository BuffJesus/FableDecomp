#!/usr/bin/env python3
"""Validate standalone native exports for all New Oakvale entity allocators."""
from __future__ import annotations

import json
import re
from pathlib import Path


def audit(workspace: Path, layouts_path: Path, vtables_path: Path) -> dict:
    layouts = json.loads(layouts_path.read_text(encoding="utf-8-sig"))["entities"]
    vtables = json.loads(vtables_path.read_text(encoding="utf-8-sig"))["entities"]
    export_dir = workspace / "ghidra_out/script_recovery/native_allocators"
    expected = {row["allocator"].upper(): entity for entity, row in layouts.items()}
    actual = {path.stem.upper(): path for path in export_dir.glob("0x*.c")}
    errors: list[str] = []
    if set(actual) != set(expected):
        errors.append(
            f"allocator exports differ: missing={sorted(set(expected)-set(actual))}, "
            f"extra={sorted(set(actual)-set(expected))}")
    wired = 0
    for address, entity in sorted(expected.items()):
        path = actual.get(address)
        if not path:
            continue
        text = path.read_text(encoding="utf-8", errors="replace")
        lines = text.splitlines()
        if not lines or f"function entry {address.lower()}" not in lines[0].lower() or "DecompFuncsToDirectory.java" not in lines[0]:
            errors.append(f"{entity}: invalid allocator provenance header")
        size_match = re.search(r"puVar2\s*=\s*::operator_new\((0x[0-9a-fA-F]+|\d+)\)", text)
        size = int(size_match.group(1), 0) if size_match else None
        if size != layouts[entity]["objectSize"]:
            errors.append(f"{entity}: exported allocation size {size} differs from layout")
        vtable = vtables[entity]["vtable"].lower().removeprefix("0x")
        fragments = (
            "CScriptThing::CScriptThing((CScriptThing *)(puVar2 + 2),in_stack_00000008)",
            f"PTR__vector_deleting_destructor__{vtable}",
            "puVar2[5] = in_EDX", "puVar2[6] = in_stack_00000004",
            "puVar2 = ::operator_new(0xc)", "puVar2[1] = &LAB_00cdee00",
        )
        missing = [fragment for fragment in fragments if fragment not in text]
        if missing:
            errors.append(f"{entity}: missing allocator wiring {missing}")
        else:
            wired += 1
    return {
        "schema": "new-oakvale-allocator-export-audit/0.1",
        "entityAllocators": len(expected),
        "exportedDecompiles": len(actual),
        "fullyWiredAllocators": wired,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(
        root, root / "ghidra_out/script_recovery/new_oakvale_entity_layouts.json",
        root / "ghidra_out/script_recovery/new_oakvale_entity_vtables.json"), indent=2))
