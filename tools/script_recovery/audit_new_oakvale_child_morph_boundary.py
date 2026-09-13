#!/usr/bin/env python3
"""Lock the childhood Hero morph boundary and post-morph handle reacquisition."""
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
    evidence = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_do_mission_retail_bytes.json").read_text(encoding="utf-8-sig"))
    decompile = (workspace / "ghidra_out/script_recovery/native_decompiles/0x00DBDE40.c").read_text(encoding="utf-8-sig")
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8-sig")
    forge = forge_root / "FableScriptExtender"
    api_h = (forge / "GameInterface.h").read_text(encoding="utf-8-sig")
    api_cpp = (forge / "GameInterface.cpp").read_text(encoding="utf-8-sig")
    quest_cpp = (forge / "LuaQuestState.cpp").read_text(encoding="utf-8-sig")

    begin = lua[lua.index("local function begin_childhood()") : lua.index("local function wait_for_attack()")]
    checks = {
        "retailDoMissionBoundaryAndChildLiteral": (
            evidence.get("address") == "0x00DBDE40"
            and evidence.get("rangeSize") == 1172
            and evidence.get("functionBytesSha256") == "E3785B3E55A0B1F5556C67316632FAEEC99B891B1BCE0743BD1B762153A91542"
            and "CREATURE_HERO_CHILD" in evidence.get("strings", [])),
        "retailUsesGetHeroThenTurnCreatureSlot": _ordered(decompile, (
            '"CREATURE_HERO_CHILD"', "+ 0x118", "+ 0x178")),
        "retailReacquiresHeroAfterSectionFrame": _ordered(decompile, (
            "+ 0x450", "+ 0x1c", "+ 0x118", "+ 0x814")),
        "forgeTurnSignatureHasCallerOwnedOutBuffer": (
            "tTurnCreatureInto)(CGameScriptInterfaceBase* This, CScriptThing* pResult, const CScriptThing* pTarget, const CCharString* pNewDefName)"
            in api_h),
        "forgeResolvesExactTurnSlot": "TurnCreatureInto_API = (tTurnCreatureInto)pVTable[94];" in api_cpp,
        "forgeZeroesAndWrapsNativeResult": _ordered(quest_cpp, (
            "CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)))",
            "memset(pResult, 0, sizeof(CScriptThing));",
            "TurnCreatureInto_API(m_pGameInterface, pResult, pTarget, fsNewDefName.get());",
            "return WrapScriptThingOutput(pResult);")),
        "luaMorphsThenReacquiresAfterFrame": _ordered(begin, (
            "Quest:TurnCreatureInto(Quest:GetHero(), NOVI.CREATURE_HERO_CHILD)",
            "Quest:ActivateQuest(NOVI.SECTION_PRE_ATTACK)",
            "if not Quest:NewScriptFrame() then return false end",
            "local hero = Quest:GetHero()",
            "Quest:EntitySetAsKillable(hero, false, false)")),
        "luaDisablesChildSleepAndEnablesMoneyBag": _ordered(begin, (
            "Quest:SetHeroSleepingAsEnabled(false)",
            "Quest:DisplayMoneyBag(true)")),
    }
    return {"schema": "new-oakvale-child-morph-boundary-audit/0.1", "ok": all(checks.values()), "checks": checks}


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
