#!/usr/bin/env python3
"""Audit movement operand forwarding used by reconstructed New Oakvale entities."""
from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


def audit(forge_root: Path, pdb_names: Path) -> dict:
    folder = forge_root / "FableScriptExtender"
    header = (folder / "EntityScriptingAPI.h").read_text(encoding="utf-8-sig")
    source = (folder / "LuaEntityAPI.cpp").read_text(encoding="utf-8-sig")
    manager = (folder / "LuaManager.cpp").read_text(encoding="utf-8-sig")
    pdb = pdb_names.read_text(encoding="utf-8-sig")
    checks = {
        "pdbFollowOrderTargetDistanceBool": bool(re.search(
            r"FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z", pdb)),
        "pdbMoveOrderVectorRadiusTypeBoolBool": bool(re.search(
            r"MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z", pdb)),
        "forgeFollowTypedefMatches": bool(re.search(
            r"FollowThing\)\([^;]+const CScriptThing\* pThing, float distance, bool b1\)", header)),
        "forgeMoveTypedefMatches": bool(re.search(
            r"MoveToPosition\)\([^;]+const C3DVector\* pPos, float radius, EScriptEntityMoveType moveType, bool b1, bool b2\)", header)),
        "followForwardsUnchanged": "pVTable->FollowThing(pExpert, pConstTargetThing, distance, avoidObstacles);" in source,
        "moveForwardsUnchanged": (
            "pVTable->MoveToPosition(pExpert, &pos, radius, eMoveType, bAvoidObstacles, bIgnorePathPref);" in source),
        "luaMoveBindsOperandAwareNonBlockingPath": all(fragment in manager for fragment in (
            'cscriptThing_type["MoveToPosition"]',
            "sol::optional<int> moveType, sol::optional<bool> b1, sol::optional<bool> b2",
            "entityAPI->MoveToPosition_NonBlocking(pMe, pos, radius, moveType.value_or(ENTITY_MOVE_WALK), b1, b2)")),
        "nonBlockingPathUsesCallerOperands": all(fragment in source for fragment in (
            "void LuaEntityAPI::MoveToPosition_NonBlocking",
            "bool bAvoidObstacles = b1.value_or(true);",
            "bool bIgnorePathPref = b2.value_or(false);")),
    }
    return {"schema": "forgefse-movement-abi-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--forge-root", type=Path, required=True)
    parser.add_argument("--pdb-names", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.forge_root, args.pdb_names)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
