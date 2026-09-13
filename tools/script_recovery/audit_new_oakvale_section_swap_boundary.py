#!/usr/bin/env python3
"""Lock New Oakvale's raid section swap across retail bytes, Forge, and Lua."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def _ordered(text: str, fragments: tuple[str, ...]) -> bool:
    cursor = 0
    for fragment in fragments:
        cursor = text.find(fragment, cursor)
        if cursor < 0:
            return False
        cursor += len(fragment)
    return True


def audit(workspace: Path, forge_root: Path) -> dict:
    evidence = workspace / "ghidra_out/script_recovery"
    retail = json.loads((evidence / "new_oakvale_quest_full_retail_bytes.json").read_text(encoding="utf-8-sig"))
    attack = next(region for region in retail["regions"] if region["name"] == "AttackStuff")
    decompile = (evidence / "native_decompiles/0x00DBE3C0.c").read_text(encoding="utf-8-sig")
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8-sig")
    common = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/common.lua").read_text(encoding="utf-8-sig")
    forge = forge_root / "FableScriptExtender"
    api_h = (forge / "GameInterface.h").read_text(encoding="utf-8-sig")
    api_cpp = (forge / "GameInterface.cpp").read_text(encoding="utf-8-sig")
    quest_cpp = (forge / "LuaQuestState.cpp").read_text(encoding="utf-8-sig")

    checks = {
        "retailFunctionBoundaryAndHash": attack == {
            "name": "AttackStuff", "address": "0x00DBE3C0", "endExclusive": "0x00DBE4DD",
            "rangeSize": 285, "bytesSha256": "E842CCECCF47BFB0BC2C829A199B0889340D6FD750ED429578FD489B732E80DC",
            "instructionCount": 83, "terminal": "ret"},
        "retailUsesActivateThenImmediateDeactivateSlots": _ordered(decompile, (
            '"Q__OakValeIntro_PostAttack"', "+ 0x450", '"Q_NewOakValeIntro_PreAttack"', "+ 0x460")),
        "forgeSignaturesPreserveNamePointerAndDelay": all(fragment in api_h for fragment in (
            "tActivateQuest)(CGameScriptInterfaceBase* This, const CCharString* pQuestName)",
            "tDeactivateQuest)(CGameScriptInterfaceBase* This, const CCharString* pQuestName, unsigned int unknown)")),
        "forgeResolvesExactRetailVtableSlots": all(fragment in api_cpp for fragment in (
            "ActivateQuest_API = (tActivateQuest)pVTable[276];   // 0x450",
            "DeactivateQuest_API = (tDeactivateQuest)pVTable[280]; // 0x460")),
        "forgeForwardsExactLuaArguments": all(fragment in quest_cpp for fragment in (
            "ActivateQuest_API(m_pGameInterface, fq);",
            "DeactivateQuest_API(m_pGameInterface, fq, delay);")),
        "luaPreservesRetailSectionSpellings": all(fragment in common for fragment in (
            'NOVI.SECTION_PRE_ATTACK       = "Q_NewOakValeIntro_PreAttack"',
            'NOVI.SECTION_POST_ATTACK      = "Q__OakValeIntro_PostAttack"')),
        "luaPreservesSwapAndPresentationOrder": _ordered(lua, (
            "function AttackStuff(questObject)",
            "Quest:ActivateQuest(NOVI.SECTION_POST_ATTACK)",
            "Quest:DeactivateQuest(NOVI.SECTION_PRE_ATTACK, DEACTIVATE_NOW)",
            "Quest:SetTimeOfDay(TIME_OF_DAY_RAID)",
            "Quest:TransitionToTheme(THEME_POST_ATTACK, THEME_TRANSITION_SECONDS)",
            "Quest:SetQuestCardObjective(active_name(), TEXT_OBJECTIVE_POST_ATTACK, \"\", \"\")")),
    }
    return {"schema": "new-oakvale-section-swap-boundary-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--workspace", type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument("--forge-root", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.workspace.resolve(), args.forge_root.resolve())
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
