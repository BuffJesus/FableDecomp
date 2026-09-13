#!/usr/bin/env python3
"""Validate both New Oakvale timer handles across retail, ForgeFSE, and Lua."""
from __future__ import annotations

import csv
import json
import re
from pathlib import Path


def audit(workspace: Path, forge_root: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_timer_retail_bytes.json").read_text(encoding="utf-8-sig"))
    constructor = (workspace / "ghidra_out/script_recovery/native_quest_construction/0x00DAAC00.c").read_text(encoding="utf-8")
    destructor = (workspace / "ghidra_out/script_recovery/native_quest_construction/0x00DBEFC0.c").read_text(encoding="utf-8")
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    fields = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/fields.lua").read_text(encoding="utf-8")
    forge_interface = (forge_root / "FableScriptExtender/GameInterface.cpp").read_text(encoding="utf-8", errors="replace")
    forge_lua = (forge_root / "FableScriptExtender/LuaQuestState.cpp").read_text(encoding="utf-8", errors="replace")
    forge_registration = (forge_root / "FableScriptExtender/LuaManager.cpp").read_text(encoding="utf-8", errors="replace")
    errors: list[str] = []

    registrations = retail.get("constructor", {}).get("registrations", [])
    deregistrations = retail.get("destructor", {}).get("deregistrations", [])
    if [(item.get("interfaceSlot"), item.get("fieldOffset")) for item in registrations] != [
            ("0x15C", "0x104"), ("0x15C", "0x108")]:
        errors.append("retail-byte snapshot does not register +0x104 then +0x108 through slot 0x15c")
    if [(item.get("interfaceSlot"), item.get("fieldOffset")) for item in deregistrations] != [
            ("0x160", "0x108"), ("0x160", "0x104")]:
        errors.append("retail-byte snapshot does not deregister +0x108 then +0x104 through slot 0x160")

    constructor_pattern = re.compile(
        r"\+ 0x15c\)\)\(\);\s*\n\s*\*\(undefined4 \*\)\(this \+ 0x104\) = uVar1;\s*\n"
        r"\s*uVar1 = \(\*\*\(code \*\*\)\(\*DAT_0143e8f8 \+ 0x15c\)\)\(\);\s*\n"
        r"\s*\*\(undefined4 \*\)\(this \+ 0x108\) = uVar1;"
    )
    if not constructor_pattern.search(constructor):
        errors.append("retail constructor does not register +0x104 then +0x108 through slot 0x15c")
    destructor_pattern = re.compile(
        r"\+ 0x160\)\)\(\*\(undefined4 \*\)\(param_1 \+ 0x108\)\);[\s\S]{0,200}"
        r"param_1 \+ 0x104\);\s*\n\s*\(\*\*\(code \*\*\)\(\*DAT_0143e8f8 \+ 0x160\)\)\(\);"
    )
    if not destructor_pattern.search(destructor):
        errors.append("retail destructor does not deregister +0x108 then +0x104 through slot 0x160")

    with (workspace / "ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv").open(
            encoding="utf-8-sig", newline="") as handle:
        rows = list(csv.DictReader(handle, delimiter="\t"))
    timer_slots = {row["offset"].lower(): row for row in rows if row["offset"].lower() in {"0x15c", "0x160", "0x164", "0x168"}}
    expected_names = {
        "0x15c": "RegisterTimer", "0x160": "DeregisterTimer",
        "0x164": "SetTimer", "0x168": "GetTimer",
    }
    for offset, name in expected_names.items():
        row = timer_slots.get(offset)
        if row is None or name not in row["current_name"] or row["executable"].lower() != "true":
            errors.append(f"retail interface slot {offset} is not executable {name}")

    forge_fragments = {
        "GameInterface.cpp": [
            "RegisterTimer_API = (tRegisterTimer)pVTable[87]",
            "DeregisterTimer_API = (tDeregisterTimer)pVTable[88]",
            "SetTimer_API = (tSetTimer)pVTable[89]",
            "GetTimer_API = (tGetTimer)pVTable[90]",
        ],
        "LuaQuestState.cpp": [
            "RegisterTimer_API(m_pGameInterface)",
            "DeregisterTimer_API(m_pGameInterface, timerId)",
        ],
        "LuaManager.cpp": [
            'questState_type["RegisterTimer"]', 'questState_type["DeregisterTimer"]',
            'questState_type["SetTimer"]', 'questState_type["GetTimer"]',
        ],
    }
    sources = {"GameInterface.cpp": forge_interface, "LuaQuestState.cpp": forge_lua,
               "LuaManager.cpp": forge_registration}
    for filename, fragments in forge_fragments.items():
        for fragment in fragments:
            if fragment not in sources[filename]:
                errors.append(f"Forge {filename} missing {fragment}")

    lua_sequence = [
        "F.set(Quest, F.TalkIntermittentTimer, Quest:RegisterTimer())",
        "F.set(Quest, F.WatchTimer, Quest:RegisterTimer())",
        "Quest:SetTimer(F.get_int(Quest, F.TalkIntermittentTimer), 0)",
    ]
    positions = [lua.find(fragment) for fragment in lua_sequence]
    if any(position < 0 for position in positions) or positions != sorted(positions):
        errors.append("Lua does not register talk/watch timers and zero the talk timer in retail order")
    init_body = lua[lua.find("function Init("):lua.find("function OnPersist(")]
    if re.search(r"if\s+(?:not\s+)?F\.(?:get|get_int).*Timer", init_body):
        errors.append("Lua timer registration is incorrectly guarded by a truthiness test")
    for name, offset in (("TalkIntermittentTimer", "0x104"), ("WatchTimer", "0x108")):
        if not re.search(rf"F\.{name}\s*=\s*key\(\"{name}\",\s*INT,\s*{offset},\s*nil\)", fields):
            errors.append(f"fields.lua missing {name} at {offset} with nil initial sentinel")

    return {
        "schema": "new-oakvale-timer-lifecycle-audit/0.2",
        "timerHandles": 2,
        "directRetailByteEvents": len(registrations) + len(deregistrations),
        "retailRegisterSlot": "0x15C",
        "retailDeregisterSlot": "0x160",
        "forgeTimerSlots": len(timer_slots),
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(root, Path(r"D:\Code\ForgeFSE-retail-shadow")), indent=2))
