#!/usr/bin/env python3
"""Match retail quest Init stores to reconstructed reset provenance."""
from __future__ import annotations

import json
import re
from pathlib import Path


def audit(workspace: Path) -> dict:
    manifest = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/persistence_manifest.json").read_text(encoding="utf-8-sig"))
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_init_retail_bytes.json").read_text(encoding="utf-8-sig"))
    native = (workspace / "ghidra_out/script_recovery/native_decompiles/0x00DAADD0.c").read_text(encoding="utf-8")
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/fields.lua").read_text(encoding="utf-8")
    fields = {row["name"]: row for row in manifest["fields"]}
    explicit: set[str] = set()
    for name, row in fields.items():
        start = int(row["retailOffset"].split("..")[0], 0)
        forms = (f"param_1 + 0x{start:x}", f"param_1 + {start}")
        if any(form in native for form in forms):
            explicit.add(name)
    special = {"TalkIntermittentTimer", "WhichBadDeedsPerformed"}
    expected_lua_resets = explicit - special
    reset_source = lua[lua.index("function F.reset_for_init("):lua.index("return F", lua.index("function F.reset_for_init("))]
    actual_lua_resets = set(re.findall(r"F\.([A-Za-z0-9_]+)", reset_source))
    actual_lua_resets -= {"reset_for_init", "reset_for_init_tail", "set", "WhichBadDeedsPerformed"}
    errors: list[str] = []
    writes = retail.get("scalarWrites", [])
    byte_widths: dict[int, int] = {}
    for write in writes:
        offset, width = int(write["fieldOffset"], 0), write["width"]
        for byte_offset in range(offset, offset + width):
            if byte_offset in byte_widths:
                errors.append(f"retail-byte Init writes overlap at {byte_offset:#x}")
            byte_widths[byte_offset] = width
    type_widths = {"bool": 1, "int": 4, "string": 12, "bool[5]": 5}
    for name in explicit - {"TalkIntermittentTimer"}:
        row = fields[name]
        offset = int(row["retailOffset"].split("..")[0], 0)
        width = type_widths[row["type"]]
        if not all(byte in byte_widths for byte in range(offset, offset + width)):
            errors.append(f"{name}: direct retail bytes do not cover the declared Init width")
        if row["type"] != "bool[5]" and byte_widths.get(offset) != width:
            errors.append(f"{name}: direct retail store width differs from {row['type']}")
    timer_reset = retail.get("timerReset", {})
    if (timer_reset.get("fieldOffset"), timer_reset.get("interfaceSlot"), timer_reset.get("value")) != ("0x104", "0x164", 0):
        errors.append("direct retail-byte talk timer reset is not SetTimer(+0x104, 0)")
    if actual_lua_resets != expected_lua_resets:
        errors.append(f"Lua reset set differs: missing={sorted(expected_lua_resets-actual_lua_resets)}, extra={sorted(actual_lua_resets-expected_lua_resets)}")
    non_explicit = set(fields) - explicit
    bad_defaults = sorted(name for name in non_explicit if fields[name]["initValue"] != "nil")
    if bad_defaults:
        errors.append(f"fields without native Init stores claim defaults: {bad_defaults}")
    if "param_1 + 0x104" not in native or "+ 0x164" not in native:
        errors.append("TalkIntermittentTimer native SetTimer reset is missing")
    if "param_1 + 0xfc" not in native or "param_1 + 0x100" not in native:
        errors.append("five-byte bad-deed array native reset is incomplete")
    return {
        "schema": "new-oakvale-quest-initialization-audit/0.2",
        "fieldFamilies": len(fields),
        "explicitInitStores": len(explicit),
        "luaScalarResets": len(actual_lua_resets),
        "producerInitializedFields": len(non_explicit),
        "directRetailScalarWrites": len(writes),
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
