#!/usr/bin/env python3
"""Reconcile retail RegisterMain with the Forge spawned-function ABI."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path, forge_root: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_register_main_retail_bytes.json").read_text(encoding="utf-8-sig"))
    cluster = json.loads((workspace / "refs/script_recovery/native_clusters/Q_NewOakValeIntro.json").read_text(encoding="utf-8-sig"))
    header = (forge_root / "FableScriptExtender/FableAPI.h").read_text(encoding="utf-8", errors="replace")
    api = (forge_root / "FableScriptExtender/FableAPI.cpp").read_text(encoding="utf-8", errors="replace")
    host = (forge_root / "FableScriptExtender/LuaQuestHost.cpp").read_text(encoding="utf-8", errors="replace")
    errors: list[str] = []
    expected = {
        "address": "0x00DAACE0", "allocationSize": 0x3C, "operatorNewTarget": "0x00BFEA1A",
        "threadName": "Main", "spawnedConstructorTarget": "0x00CDD450",
        "spawnedVtable": "0x012D7A3C", "mainThunk": "0x00CDD440",
        "ownerFieldOffset": "0x38", "sectionName": "", "addSpawnedFunctionTarget": "0x00CB7E50",
    }
    for key, value in expected.items():
        if retail.get(key) != value:
            errors.append(f"retail RegisterMain {key} differs: {retail.get(key)!r}")
    lifecycle = next(row for row in cluster["lifecycle"] if row["role"] == "RegisterMain")
    if lifecycle["address"].upper() != retail["address"].upper():
        errors.append("cluster RegisterMain address differs from retail-byte snapshot")
    forge_fragments = [
        (header, "static_assert(sizeof(CSpawnedFunc) == 0x3C"),
        (header, "offsetof(CSpawnedFunc, pThunkToMain) == 0x34"),
        (header, "offsetof(CSpawnedFunc, pOwnerScript) == 0x38"),
        (header, 'FableString mainStr("Main")'),
        (header, "CSpawnedFunc_Construct(pSpawnedFunc, mainStr, 0)"),
        (header, "AddSpawnedFunction_func(pScriptBase, pSpawnedFunc, classStr)"),
        (api, "CSpawnedFunc_Construct = ASLR<tCSpawnedFunc_Constructor>(0xCDD450)"),
        (api, "AddSpawnedFunction_func = ASLR<tAddSpawnedFunction>(0xCB7E50)"),
        (host, "void LuaQuestHost::RegisterMain() { AutoRegisterMain"),
    ]
    for source, fragment in forge_fragments:
        if fragment not in source:
            errors.append(f"Forge RegisterMain path missing {fragment}")
    return {
        "schema": "new-oakvale-register-main-audit/0.1",
        "retailAllocationSize": retail.get("allocationSize"),
        "retailVtableSlots": len(retail.get("spawnedVtableSlots", [])),
        "forgeAbiAssertions": 3,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(root, Path(r"D:\Code\ForgeFSE-retail-shadow")), indent=2))
