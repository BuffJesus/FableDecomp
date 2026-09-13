#!/usr/bin/env python3
"""Lock the native Affair Wife route transition and its Forge distance ABI."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def _ordered(text: str, needles: tuple[str, ...]) -> bool:
    cursor = 0
    for needle in needles:
        cursor = text.find(needle, cursor)
        if cursor < 0:
            return False
        cursor += len(needle)
    return True


def audit(disassembly: Path, forge_root: Path, lua_script: Path, pdb_names: Path | None = None) -> dict:
    native = disassembly.read_text(encoding="utf-8-sig").lower()
    folder = forge_root / "FableScriptExtender"
    api = (folder / "FableAPI.h").read_text(encoding="utf-8-sig")
    quest = (folder / "LuaQuestState.cpp").read_text(encoding="utf-8-sig")
    lua = lua_script.read_text(encoding="utf-8-sig")
    pdb = pdb_names.read_text(encoding="utf-8-sig") if pdb_names else ""

    checks = {
        "nativeLooksUpAffairManByName": _ordered(native, (
            "00db3426:", "s_novi_affairman", "00db3445:", "call dword ptr [edx + 0x120]")),
        "nativeSnapshotsLiveHusbandPosition": _ordered(native, (
            "00db3462:", "mov ecx,dword ptr [esp + 0x34]", "00db3471:",
            "call dword ptr [eax + 0x18]", "00db3476:")),
        "nativeMoveOperandsAreTwoRunFalseTrue": _ordered(native, (
            "00db3476: 6a01", "00db3478: 6a00", "00db347a: 6a01",
            "00db347c: 6800000040", "00db3481: 50", "00db3486:")),
        "nativeFirstDistanceUsesSameHusbandLocal": _ordered(native, (
            "00db3497: 6800004040", "00db349c: 8d542434", "00db34a0: 8bcf",
            "00db34a2:", "isdistancebetweenthingsunder")),
        "nativeLoopRetestsLiveHusband": _ordered(native, (
            "00db357b: 6800004040", "00db3580: 8d542434", "00db3584: 8bcf",
            "00db3586:", "isdistancebetweenthingsunder", "00db358d:", "jz 0x00db34b0")),
        "nativeClearsCommandsOnlyAfterReachedBranch": _ordered(native, (
            "00db34a9:", "jnz 0x00db3593", "00db358d:", "jz 0x00db34b0",
            "00db3593:", "00db35a6:", "clearcommands")),
        "pdbConversationConstructorHasTwoBools": (
            not pdb or "?AddNewConversation@CGameScriptInterface@@UBEJABVCScriptThing@@_N1@Z" in pdb),
        "nativeConversationConstructorsPassFalseFalse": all(_ordered(native, sequence) for sequence in (
            ("00db3334: 6a00", "00db3336: 6a00", "00db333e: 57", "00db333f:"),
            ("00db350b: 6a00", "00db350d: 6a00", "00db350f: 57", "00db3510:"),
            ("00db3b6b: 6a00", "00db3b6d: 6a00", "00db3b6f: 57", "00db3b70:"),
        )),
        "forgeForwardsBothConversationBools": all(fragment in quest for fragment in (
            "int LuaQuestState::AddNewConversation(CScriptThing* pThing, bool b1, bool b2)",
            "AddNewConversation_API(m_pGameInterface, pThing, b1, b2)")),
        "forgeDistanceHelperIsFastcall": (
            "typedef bool(__fastcall* tIsDistanceBetweenThingsUnder)(const CScriptThing* thing1, const CScriptThing* thing2, float dist);"
            in api),
        "forgeNormalizesBothLuaRepresentations": all(fragment in quest for fragment in (
            "CScriptThing* pThing1 = GetScriptThingFromSolObject(thing1);",
            "CScriptThing* pThing2 = GetScriptThingFromSolObject(thing2);",
            "return IsDistanceBetweenThingsUnder_API(pThing1, pThing2, distance);")),
        "luaRetailMoveThenWaitsOnLiveThing": _ordered(lua, (
            "local man = quest:GetThingWithScriptName(SCRIPT_NAME_MAN)",
            "if man then target = man:GetPos() end",
            "me:MoveToPosition(target, RUN_TO_HUSBAND_RADIUS, MOVE_RUN, false, true)",
            "while not quest:IsDistanceBetweenThingsUnder(me, man, HUSBAND_REACHED_DISTANCE) do",
            "me:ClearCommands()")),
        "luaOnlyRetriesEndedShortTask": _ordered(lua, (
            "if retryCount < ROUTE_RETRY_LIMIT and not me:IsPerformingScriptTask() then",
            "local target = man and man:GetPos() or ZERO_POSITION",
            '"NOVI_PROBE AffairWife ROUTE_RETRY count=%d',
            "me:MoveToPosition(target, RUN_TO_HUSBAND_RADIUS, MOVE_RUN, false, true)")),
    }
    return {"schema": "affair-wife-route-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--disassembly", type=Path, required=True)
    parser.add_argument("--forge-root", type=Path, required=True)
    parser.add_argument("--lua-script", type=Path, required=True)
    parser.add_argument("--pdb-names", type=Path)
    args = parser.parse_args()
    result = audit(args.disassembly, args.forge_root, args.lua_script, args.pdb_names)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
