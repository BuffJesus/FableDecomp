#!/usr/bin/env python3
"""Audit the Forge entity-control bridge required by reconstructed New Oakvale scripts."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def _region(source: str, start_marker: str, end_marker: str) -> str:
    start = source.find(start_marker)
    if start < 0:
        return ""
    end = source.find(end_marker, start + len(start_marker))
    return source[start:end] if end >= 0 else ""


def _ordered(text: str, *needles: str) -> bool:
    """True when every needle occurs in `text`, each after the previous one."""
    position = 0
    for needle in needles:
        found = text.find(needle, position)
        if found < 0:
            return False
        position = found + len(needle)
    return True


def audit(forge_root: Path) -> dict:
    source = (forge_root / "FableScriptExtender" / "LuaEntityAPI.cpp").read_text(
        encoding="utf-8-sig")
    manager = (forge_root / "FableScriptExtender" / "LuaManager.cpp").read_text(
        encoding="utf-8-sig")

    acquire = _region(source, "bool LuaEntityAPI::AcquireControl(", "void LuaEntityAPI::ReleaseControl(")
    release = _region(source, "void LuaEntityAPI::ReleaseControl(", "void LuaEntityAPI::MakeBehavioral(")

    checks = {
        "luaMethodsRegistered": (
            'cscriptThing_type["AcquireControl"]' in manager
            and 'cscriptThing_type["ReleaseControl"]' in manager
        ),
        "callerPriorityForwarded": (
            "priority.value_or" in acquire
            and "StartScriptingEntity_API(m_pGameInterface, pMe, m_pControlHandle, scriptPriority)" in acquire
        ),
        "nativeRetryLoopYields": "while (true)" in acquire and "NewScriptFrame_API(m_pGameInterface);" in acquire,
        "terminationDestroysPendingHandle": (
            "if (IsThreadTerminating())" in acquire
            and "CSGROSTB_Destroy_API(m_pControlHandle);" in acquire
            and "Game_free(m_pControlHandle);" in acquire
            and "return false;" in acquire
        ),
        "liveCrossVmHandleBorrowed": (
            "g_controlHandlesByEntityData.find" in acquire
            and "m_borrowedControlHandles[pMe->pImp.Data]" in acquire
        ),
        "borrowReleasePreservesOwner": (
            "m_borrowedControlHandles.find" in release
            and "Borrowed control handle released without destroying its owner's resource." in release
            and release.find("return;", release.find("m_borrowedControlHandles.find"))
            < release.find("CSGROSTB_Destroy_API(m_pControlHandle);")
        ),
        # Retail nests a second StartScriptingEntity resource inside ONE script owner
        # (Affair Wife: priority-4 conversation over priority-3 Main; Affair Man: two
        # priority-4 resources). A Forge handle has no owner identity, so a second
        # scheduler resource for an actor this VM already controls is never granted
        # (2026-09-11 runtime evidence). The observable retail contract is that the
        # outer resource survives the nested release: nested acquires must reuse the
        # live handle with a depth count, and only the outermost release may destroy it.
        "nestedAcquireReusesOwnedHandle": (
            "int& depthSlot = m_ownedControlDepth[entityData];" in acquire
            and "++depthSlot" in acquire
            and "m_suspendedControlHandles" not in source
        ),
        # Retail Main loops call StartScriptingEntity again on the SAME resource every
        # iteration (Bully/Victim/Barrel Man) and the engine treats it as idempotent;
        # v11 counted each call and depth inflated past 400. A re-acquire at the held
        # priority saturates at one level (the outer handle must still survive the
        # following release: Affair Man's conversation precedes his walk-home loop).
        "samePriorityReacquireSaturates": _ordered(
            acquire, "heldPriority->second == requestedPriority && depthSlot > 0", "return true;",
            "++depthSlot"
        ),
        "nestedAcquireSkipsSelfBorrow": _ordered(
            acquire, "++depthSlot", "return true;", "else if (entityData)",
            "g_controlHandlesByEntityData.find"
        ),
        "freshAcquireResetsDepth": "m_ownedControlDepth[m_pControlledEntityData] = 0;" in acquire,
        "nestedReleaseRetainsOuterHandle": (
            _ordered(release, "m_ownedControlDepth.find", "--depth->second;", "return;",
                     "CSGROSTB_Destroy_API(m_pControlHandle);")
        ),
        "outermostReleaseClearsDepth": _ordered(
            release, "m_ownedControlDepth.erase(releasedEntityData);", "CSGROSTB_Destroy_API(m_pControlHandle);"
        ),
    }
    return {"schema": "forgefse-control-abi-audit/0.4", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--forge-root", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.forge_root)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
