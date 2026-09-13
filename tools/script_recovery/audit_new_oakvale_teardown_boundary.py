#!/usr/bin/env python3
"""Join Forge lifecycle guards to the archived New Oakvale completion teardown."""
from __future__ import annotations

import json
import re
from pathlib import Path


def _ordered(text: str, markers: list[str]) -> bool:
    cursor = -1
    for marker in markers:
        cursor = text.find(marker, cursor + 1)
        if cursor < 0:
            return False
    return True


def audit(workspace: Path, forge_root: Path = Path(r"D:\Code\ForgeFSE-retail-shadow")) -> dict:
    quest_cpp = (forge_root / "FableScriptExtender/LuaQuestHost.cpp").read_text(encoding="utf-8-sig")
    entity_cpp = (forge_root / "FableScriptExtender/LuaEntityHost.cpp").read_text(encoding="utf-8-sig")
    log = (workspace / "refs/script_recovery/new_oakvale_intro/runtime_evidence/interactive-20260910-redtimer-bullyrun-barrels.log").read_text(encoding="utf-8-sig")
    teardown_start = log.rfind("--- Entity 'NewOakValeIntro/Entities/NOVI_TeddyGirl': C++ OnPersist triggered. ---")
    tail = log[teardown_start:]
    exited = sorted(set(re.findall(r"EXITED LUA CALL for 'NewOakValeIntro/Entities/([^']+)'", tail)))
    sequence = [
        "EXITED LUA CALL for 'NewOakValeIntro/Entities/OVI_DeadFather'",
        "[LuaEntityHost::OnPredicateFail] Dispatching Lua callback for 'NewOakValeIntro/Entities/OVI_DeadFather'",
        "--- Quest 'NewOakValeIntro/NewOakValeIntro': C++ OnPersist() triggered. ---",
        "[C++] SetQuestAsCompleted: EXIT.",
        "[LuaQuestHost::Destructor] Tearing down quest 'NewOakValeIntro/NewOakValeIntro' (bDelete=1)",
        "===== shared_ptr Deleter Called =====",
    ]
    checks = {
        "questMainGuardsQuestTermination": "IsActiveThreadTerminating_Quest_API(&this->base)" in quest_cpp,
        "questMainGuardsInterfaceTermination": "IsActiveThreadTerminating_API(this->pInterface)" in quest_cpp,
        "spawnedThreadRunnerHasBothGuards": quest_cpp.count("IsActiveThreadTerminating_Quest_API(&this->base)") >= 2 and quest_cpp.count("IsActiveThreadTerminating_API(this->pInterface)") >= 2,
        "deletingDestructorCleansThreadsBeforeFree": _ordered(quest_cpp, ["if (bDelete)", "this->~LuaQuestHost();", "operator delete(this);"]),
        "nonDeletingDestructorStillCleansThreads": _ordered(quest_cpp, ["else {", "CleanupThreads();"]),
        "threadCleanupUnrefsSpawnedAndMainCoroutines": quest_cpp.count("luaL_unref") >= 2 and "m_threads.clear();" in quest_cpp,
        "entityEntryGuardsTermination": "IsActiveThreadTerminating_Entity_API(this)" in entity_cpp,
        "entityTeardownIsUnconditionalAndIdempotent": entity_cpp.count("UnregisterEntityScriptData(this);") >= 2 and "cannot be conditional on this flag" in entity_cpp,
        "runtimeCompletionTeardownOrder": _ordered(tail, sequence),
        "runtimeMultipleEntityClassesUnwound": len(exited) >= 10 and "NOVI_AffairWife" in exited and "OVI_DeadFather" in exited,
    }
    return {
        "schema": "new-oakvale-teardown-boundary-audit/0.1",
        "ok": all(checks.values()),
        "runtimeExitedEntityClasses": exited,
        "remainingRuntimeBoundary": "repeated mid-childhood region unload/reload re-entry timing",
        "checks": checks,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
