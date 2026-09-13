#!/usr/bin/env python3
"""Audit quest Main's post-binding retail tail against Lua and Forge."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path, forge_root: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_main_tail_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    api = (forge_root / "FableScriptExtender/FableAPI.cpp").read_text(encoding="utf-8", errors="replace")
    game = (forge_root / "FableScriptExtender/GameInterface.cpp").read_text(encoding="utf-8", errors="replace")
    state = (forge_root / "FableScriptExtender/LuaQuestState.cpp").read_text(encoding="utf-8", errors="replace")
    errors: list[str] = []
    expected = {
        "postAddBaseTarget": "0x00CB8930", "postAddInterfaceSlot": "0x100",
        "attackOverField": "0x50", "attackOverWidth": 1, "terminationTarget": "0x00CB7940",
        "deactivateSlot": "0x460", "reloadSection": "Q__OakValeIntro_PostAttack", "reloadDelay": 0,
        "activeQuestNameSlot": "0xA3C", "objectiveSlot": "0x4A0",
        "objective": "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01", "spawnedThread": "StartBarrelTimer",
        "spawnedObjectSize": 0x3C, "spawnedConstructorTarget": "0x00CDD450",
        "spawnedCallback": "0x00DB4F70", "spawnedOwnerOffset": "0x38",
        "addSpawnedTarget": "0x00CB7E50", "doMissionTarget": "0x00DBDE40",
    }
    for key, value in expected.items():
        if retail.get(key) != value:
            errors.append(f"retail Main tail {key} differs")
    main = lua[lua.index("function Main("):lua.index("function DoMission(")]
    lua_fragments = [
        "Quest:FinalizeEntityBindings()", "if F.get(Quest, F.AttackOver) then",
        "if Quest:IsActiveThreadTerminating() then return end",
        "Quest:DeactivateQuest(NOVI.SECTION_POST_ATTACK, DEACTIVATE_NOW)",
        "Quest:SetQuestCardObjective(active_name(), TEXT_OBJECTIVE_START",
        'Quest:CreateThread("StartBarrelTimer"', "DoMission(Quest)",
    ]
    positions = [main.find(fragment) for fragment in lua_fragments]
    if any(position < 0 for position in positions) or positions != sorted(positions):
        errors.append("Lua Main tail operations are missing or out of retail order")
    forge_fragments = [
        (api, "PostAddScriptedEntities_CScriptBase_API = ASLR<tPostAddScriptedEntities_CScriptBase>(0xCB8930)"),
        (game, "PostAddScriptedEntities_API = (tPostAddScriptedEntities)pVTable[64]"),
        (game, "DeactivateQuest_API = (tDeactivateQuest)pVTable[280]"),
        (game, "SetQuestCardObjective_API = (tSetQuestCardObjective)pVTable[296]"),
        (state, "PostAddScriptedEntities_CScriptBase_API(&m_pParentHost->base)"),
        (state, "PostAddScriptedEntities_API(m_pGameInterface)"),
    ]
    for source, fragment in forge_fragments:
        if fragment not in source:
            errors.append(f"Forge Main-tail path missing {fragment}")
    return {
        "schema": "new-oakvale-main-tail-audit/0.1",
        "retailTailBytes": retail.get("rangeSize"),
        "indirectSlots": len(retail.get("indirectCallSlots", [])),
        "luaOperations": len(lua_fragments),
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(root, Path(r"D:\Code\ForgeFSE-retail-shadow")), indent=2))
