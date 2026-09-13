#!/usr/bin/env python3
"""Lock retail/Forge/OVI Dead Father acquisition termination equivalence."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def audit(translation_unit: Path, forge_source: Path, common_lua: Path,
          dead_father_lua: Path, fixture: Path) -> dict:
    archive = translation_unit.read_text(encoding="utf-8-sig")
    native = next((line for line in archive.splitlines()
                   if line.lower().startswith('{"address":"0x00db8300"')), "")
    forge = forge_source.read_text(encoding="utf-8-sig")
    acquire = forge.split("bool LuaEntityAPI::AcquireControl", 1)[-1].split("\n}\n\nvoid LuaEntityAPI::ReleaseControl", 1)[0]
    common = common_lua.read_text(encoding="utf-8-sig")
    dead_father = dead_father_lua.read_text(encoding="utf-8-sig")
    case = json.loads(fixture.read_text(encoding="utf-8-sig"))
    checks = {
        "nativeWaitsForStartScriptingEntity": all(x in native for x in (
            "while (cVar4 == '\\\\0')", "+ 0x20", "+ 0x1c")),
        "nativeTerminationBranchesToResourceCleanup": all(x in native for x in (
            "if (extraout_AL_00 != '\\\\0') goto LAB_00db8507", "LAB_00db8507:",
            "CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase")),
        "forgeRequiresEntityOrQuestTerminationAPI": "const bool hasTerminationAPI" in acquire
            and "IsActiveThreadTerminating_Entity_API" in acquire,
        "forgeChecksTerminationAfterEveryFailedAcquire": all(x in acquire for x in (
            "StartScriptingEntity_API", "NewScriptFrame_API(m_pGameInterface)", "if (IsThreadTerminating())")),
        "forgeDestroysFailedHandleAndReturnsFalse": all(x in acquire for x in (
            "CSGROSTB_Destroy_API(m_pControlHandle)", "Game_free(m_pControlHandle)",
            "m_pControlHandle = nullptr", "return false")),
        "commonPropagatesExplicitFalse": all(x in common for x in (
            "local result = me:AcquireControl(priority)", "if result == false then", "return false")),
        "deadFatherUnwindsBeforeSetup": "if not NOVI.acquire(quest, me, SCRIPT_PRIORITY) then NOVI.release(quest, me); return end" in dead_father,
        "fixtureForcesAcquireTermination": case.get("Entity.AcquireControl") == [False]
            and "ReleaseControl" in case["__meta__"]["expect"]
            and "EntityTeleportToThing" in case["__meta__"]["forbid"],
    }
    return {"schema": "dead-father-acquire-termination-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--translation-unit", type=Path, required=True)
    parser.add_argument("--forge-source", type=Path, required=True)
    parser.add_argument("--common-lua", type=Path, required=True)
    parser.add_argument("--dead-father-lua", type=Path, required=True)
    parser.add_argument("--fixture", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.translation_unit, args.forge_source, args.common_lua, args.dead_father_lua, args.fixture)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
