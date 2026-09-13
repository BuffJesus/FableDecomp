#!/usr/bin/env python3
"""Validate the complete retail/Forge position chain used by Affair Wife."""
from __future__ import annotations

import csv
import json
from pathlib import Path


def _manifest_rows(path: Path) -> dict[tuple[str, str], dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    wanted = {
        ("CScriptThing", "?GetPos@CScriptThing@@UBEABVC3DVector@@XZ"),
        ("CGameScriptThing", "CGameScriptThing::GetPos"),
        ("_global", "IsDistanceBetweenThingsUnder"),
    }
    return {(row["module"], row["name"]): row for row in rows
            if (row["module"], row["name"]) in wanted}


def audit(workspace: Path, forge_root: Path) -> dict:
    rows = _manifest_rows(workspace / "rebuild/manifest/functions.tsv")
    errors: list[str] = []
    if len(rows) != 3:
        errors.append(f"expected three unique position-chain manifest rows, found {len(rows)}")
        return {"schema": "affair-wife-position-chain-audit/0.1", "errors": errors, "ok": False}

    support = workspace / "ghidra_out/script_recovery/native_support"
    bodies: dict[str, str] = {}
    for key, row in rows.items():
        address = "0x" + row["address"].upper()
        path = support / f"{address}.c"
        if not path.exists():
            errors.append(f"missing native support export {path.name}")
            continue
        text = path.read_text(encoding="utf-8", errors="replace")
        if f"function entry {address}".lower() not in text.lower() or "DecompFuncsToDirectory.java" not in text:
            errors.append(f"{address}: invalid provenance header")
        bodies[key[0]] = text

    wrapper = bodies.get("CScriptThing", "")
    concrete = bodies.get("CGameScriptThing", "")
    distance = bodies.get("_global", "")
    for fragment in ("if (*(int **)(this + 4) == (int *)0x0)",
                     "return 0x143e8e0", "+ 0x18"):
        if fragment not in wrapper:
            errors.append(f"CScriptThing::GetPos missing {fragment}")
    if "return param_1 + 0x28" not in concrete:
        errors.append("CGameScriptThing::GetPos does not return its inline +0x28 position")
    for fragment in ("*(int *)param_1 + 300", "*(int *)param_2 + 300",
                     "*(int *)param_2 + 0x18", "*(int *)param_1 + 0x18",
                     "< param_3 * param_3"):
        if fragment not in distance:
            errors.append(f"retail distance predicate missing {fragment}")

    fse = forge_root / "FableScriptExtender"
    api_h = (fse / "FableAPI.h").read_text(encoding="utf-8-sig")
    api_cpp = (fse / "FableAPI.cpp").read_text(encoding="utf-8-sig")
    entity = (fse / "LuaEntityAPI.cpp").read_text(encoding="utf-8-sig")
    quest = (fse / "LuaQuestState.cpp").read_text(encoding="utf-8-sig")
    forge_fragments = {
        "fastcallSignature": "typedef bool(__fastcall* tIsDistanceBetweenThingsUnder)(const CScriptThing* thing1, const CScriptThing* thing2, float dist);" in api_h,
        "retailAddress": "ASLR<tIsDistanceBetweenThingsUnder>(0xCBE2FF)" in api_cpp,
        "normalizesFirst": "CScriptThing* pThing1 = GetScriptThingFromSolObject(thing1);" in quest,
        "normalizesSecond": "CScriptThing* pThing2 = GetScriptThingFromSolObject(thing2);" in quest,
        "callsRetailPredicate": "return IsDistanceBetweenThingsUnder_API(pThing1, pThing2, distance);" in quest,
        "entityUsesImplementation": "CGameScriptThing* pImp = reinterpret_cast<CGameScriptThing*>(pMe->pImp.Data);" in entity,
        "entityCallsGetPos": "const C3DVector* pPos = pVTable->GetPos(pImp);" in entity,
        "namedLookupLogsWorldUid": "pVTable->GetPThingUniqueID(pThing)" in api_cpp and "WorldUID = " in api_cpp,
        "namedLookupLogsWrapperPosition": "pVTable->GetPos(pThing)" in api_cpp and "WrapperPos = (" in api_cpp,
    }
    errors.extend(f"Forge position chain missing {name}" for name, ok in forge_fragments.items() if not ok)
    return {
        "schema": "affair-wife-position-chain-audit/0.1",
        "nativeFunctions": len(bodies),
        "forgeChecks": len(forge_fragments),
        "forgeChecksPassed": sum(forge_fragments.values()),
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(root, Path(r"D:\Code\ForgeFSE-retail-shadow")), indent=2))
