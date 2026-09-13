#!/usr/bin/env python3
"""Audit thing lookup and position forwarding used by the Affair Wife route."""
from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


def audit(forge_root: Path, pdb_names: Path, retail_slots: Path,
          position_native: Path | None = None) -> dict:
    folder = forge_root / "FableScriptExtender"
    header = (folder / "GameInterface.h").read_text(encoding="utf-8-sig")
    entity_header = (folder / "EntityScriptingAPI.h").read_text(encoding="utf-8-sig")
    interface = (folder / "GameInterface.cpp").read_text(encoding="utf-8-sig")
    quest = (folder / "LuaQuestState.cpp").read_text(encoding="utf-8-sig")
    entity = (folder / "LuaEntityAPI.cpp").read_text(encoding="utf-8-sig")
    pdb = pdb_names.read_text(encoding="utf-8-sig")
    slots = retail_slots.read_text(encoding="utf-8-sig")
    native = position_native.read_text(encoding="utf-8-sig") if position_native else ""
    checks = {
        "pdbNameOnlyReturnsScriptThingByValue": (
            "?GetThingWithScriptName@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@@Z" in pdb),
        "pdbRegionalReturnsScriptThingByValue": (
            "?GetThingWithScriptName@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@0@Z" in pdb),
        "nameOnlyRetailSlot72": (
            "0x120\t0126102c\t008a7d60\t?GetThingWithScriptName@" in slots and
            "GetThingWithScriptName_ByName_API = (tGetThingWithScriptName1)pVTable[72];" in interface),
        "hiddenResultAbiDeclared": all(text in header for text in (
            "tGetThingWithScriptName1)(CGameScriptInterfaceBase* This, CScriptThing* pResult, const CCharString* pName)",
            "tGetThingWithScriptName2)(CGameScriptInterfaceBase* This, CScriptThing* pResult, const CCharString* pName, const CCharString* pRegionName)",
        )),
        "nameOnlyForwardsName": (
            "GetThingWithScriptName_ByName_API(m_pGameInterface, pHeapResult, fsScriptName);" in quest),
        "regionalForwardsNameThenRegion": (
            "GetThingWithScriptName_ByNameAndRegion_API(m_pGameInterface, pHeapResult, fsScript, fsRegion);" in quest),
        "vectorLayoutXYZ": bool(re.search(
            r"struct C3DVector\s*\{\s*float x, y, z;\s*\};", entity_header)),
        "pdbGetPosReturnsConstReference": (
            "?GetPos@CGameScriptThing@@UBEABVC3DVector@@XZ" in pdb),
        "forgeGetPosTypedefMatches": (
            "typedef const C3DVector* (__thiscall* tCGameScriptThing_GetPos)(CGameScriptThing* This);" in entity_header),
        "luaCopiesXYZUnchanged": all(text in entity for text in (
            "const C3DVector* pPos = pVTable->GetPos(pImp);",
            'pos_table["x"] = pPos->x;',
            'pos_table["y"] = pPos->y;',
            'pos_table["z"] = pPos->z;',
        )),
        "scriptThingGetPosDispatchesImplementationSlot18": (
            not native or all(text in native for text in (
                "?GetPos@CScriptThing", "(**(code **)(*piVar8 + 0x18))()"))),
        "gameScriptThingGetPosReturnsInlineVector30": (
            not native or all(text in native for text in (
                "?GetPos@CGameScriptThing", "return in_ECX + 0x30;"))),
    }
    return {"schema": "forgefse-lookup-position-abi-audit/0.1",
            "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--forge-root", type=Path, required=True)
    parser.add_argument("--pdb-names", type=Path, required=True)
    parser.add_argument("--retail-slots", type=Path, required=True)
    parser.add_argument("--position-native", type=Path)
    args = parser.parse_args()
    result = audit(args.forge_root, args.pdb_names, args.retail_slots, args.position_native)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
