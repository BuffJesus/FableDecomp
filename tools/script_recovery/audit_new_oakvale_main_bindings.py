#!/usr/bin/env python3
"""Reconcile Main's retail binding table with layouts, Lua, and Forge."""
from __future__ import annotations

import json
import re
from pathlib import Path


def audit(workspace: Path, forge_root: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_main_bindings_retail_bytes.json").read_text(encoding="utf-8-sig"))
    layouts = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_entity_layouts.json").read_text(encoding="utf-8-sig"))["entities"]
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    header = (forge_root / "FableScriptExtender/FableAPI.h").read_text(encoding="utf-8", errors="replace")
    api = (forge_root / "FableScriptExtender/FableAPI.cpp").read_text(encoding="utf-8", errors="replace")
    state = (forge_root / "FableScriptExtender/LuaQuestState.cpp").read_text(encoding="utf-8", errors="replace")
    errors: list[str] = []
    bindings = retail.get("bindings", [])
    retail_names = [row["name"] for row in bindings]
    lua_table = lua[lua.index("local ENTITY_BINDINGS"):lua.index("-- quest-card objectives")]
    lua_names = re.findall(r'\{\s*"((?:NOVI|OVI)_[^"]+)"\s*,', lua_table)
    if lua_names != retail_names:
        errors.append("Lua entity binding order differs from retail bytes")
    for row in bindings:
        layout = layouts.get(row["name"])
        if not layout or layout.get("allocator", "").upper() != row["allocatorCallback"].upper():
            errors.append(f"{row['name']}: layout allocator differs from Main callback")
    expected_header = [
        "sizeof(CEntityScriptBindingBase) == 0x1C",
        "offsetof(CEntityScriptBindingBase, pParentScript) == 0x08",
        "offsetof(CEntityScriptBindingBase, pAllocFunc) == 0x10",
        "offsetof(CEntityScriptBindingBase, bSomething) == 0x14",
        "offsetof(CEntityScriptBindingBase, unknown_zero) == 0x18",
    ]
    for fragment in expected_header:
        if fragment not in header:
            errors.append(f"Forge entity-binding ABI missing {fragment}")
    if "AddEntityScriptBinding_API = ASLR<tAddEntityScriptBinding>(0xCB8230)" not in api:
        errors.append("Forge AddEntityScriptBinding target differs from retail")
    for fragment in ("Game_malloc(sizeof(CEntityScriptBindingBase))", "pBinding->pAllocFunc = pAllocFunc",
                     "pBinding->bSomething = true", "AddEntityScriptBinding_API(&m_pParentHost->base, pBinding)"):
        if fragment not in state:
            errors.append(f"Forge FinalizeEntityBindings missing {fragment}")
    for key, value in (("bindingObjectSize", 0x1C), ("operatorNewCalls", 16), ("addBindingCalls", 16),
                       ("addBindingTarget", "0x00CB8230")):
        if retail.get(key) != value:
            errors.append(f"retail binding invariant {key} differs")
    return {
        "schema": "new-oakvale-main-bindings-audit/0.1",
        "retailBindings": len(bindings),
        "allocatorMatches": sum(1 for row in bindings if layouts.get(row["name"], {}).get("allocator", "").upper() == row["allocatorCallback"].upper()),
        "forgeAbiAssertions": len(expected_header),
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(root, Path(r"D:\Code\ForgeFSE-retail-shadow")), indent=2))
