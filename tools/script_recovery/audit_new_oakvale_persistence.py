#!/usr/bin/env python3
"""Reconcile New Oakvale persistence across retail bytes, manifests, Lua, and Forge."""
from __future__ import annotations

import json
import re
from pathlib import Path


def audit(workspace: Path, forge_root: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_onpersist_retail_bytes.json").read_text(encoding="utf-8-sig"))
    manifest = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/persistence_manifest.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    fse_api = (forge_root / "FableScriptExtender/FableAPI.cpp").read_text(encoding="utf-8", errors="replace")
    fse_header = (forge_root / "FableScriptExtender/FableAPI.h").read_text(encoding="utf-8", errors="replace")
    fse_state = (forge_root / "FableScriptExtender/LuaQuestState.cpp").read_text(encoding="utf-8", errors="replace")
    fse_manager = (forge_root / "FableScriptExtender/LuaManager.cpp").read_text(encoding="utf-8", errors="replace")
    errors: list[str] = []
    transfers = retail.get("transfers", [])
    expected = {"key": "AttackOver", "fieldOffset": "0x50", "valueWidth": 1,
                "defaultWidth": 1, "defaultValue": 0, "transferTarget": "0x004045C0"}
    if len(transfers) != 1 or any(transfers[0].get(key) != value for key, value in expected.items()):
        errors.append("retail bytes do not contain the sole expected AttackOver signed-byte transfer")
    if manifest.get("persistedByRetail") != ["AttackOver"]:
        errors.append("persistence manifest does not identify exactly AttackOver")
    attack = next((row for row in manifest.get("fields", []) if row.get("name") == "AttackOver"), None)
    if not attack or attack.get("type") != "bool" or attack.get("retailOffset") != "0x50" or not attack.get("survivesSaveLoad"):
        errors.append("AttackOver manifest type/offset/save lifetime differs from retail")
    body = lua[lua.find("function OnPersist("):lua.find("function Main(")]
    calls = re.findall(r'PersistTransfer([A-Za-z]+)\(context,\s*"([^"]+)"', body)
    if calls != [("Bool", "AttackOver")]:
        errors.append(f"Lua OnPersist transfer set differs: {calls}")
    forge_fragments = [
        (fse_api, "CPersistContext_Transfer_bool_API = ASLR<tCPersistContext_Transfer_bool>(0x4045C0)"),
        (fse_header, "const char* name, bool* value, const bool* defaultValue"),
        (fse_state, "CPersistContext_Transfer_bool_API(pRealContext, name.c_str(), &tempValue, &transferDefault)"),
        (fse_manager, 'questState_type["PersistTransferBool"]'),
    ]
    for source, fragment in forge_fragments:
        if fragment not in source:
            errors.append(f"Forge persistence path missing {fragment}")
    return {
        "schema": "new-oakvale-persistence-audit/0.1",
        "retailTransfers": len(transfers),
        "persistedFields": len(manifest.get("persistedByRetail", [])),
        "retailTransferTarget": "0x004045C0",
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(root, Path(r"D:\Code\ForgeFSE-retail-shadow")), indent=2))
