#!/usr/bin/env python3
"""Prove destroyed NOVI barrels dispatch the bound-thing alive predicate failure."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def _record(archive: str, address: str) -> str:
    needle = '{"address":"' + address.lower() + '"'
    return next((line for line in archive.splitlines() if line.lower().startswith(needle)), "")


def audit(translation_unit: Path, donor_pdb: Path, forge_host: Path,
          barrel_lua: Path, runtime_log: Path) -> dict:
    archive = translation_unit.read_text(encoding="utf-8-sig")
    allocator = _record(archive, "0x00DB7D00")
    callback = _record(archive, "0x00DB7DB0")
    pdb = donor_pdb.read_text(encoding="utf-8-sig")
    forge = forge_host.read_text(encoding="utf-8-sig")
    lua = barrel_lua.read_text(encoding="utf-8-sig")
    runtime = runtime_log.read_text(encoding="utf-8-sig", errors="replace")
    checks = {
        "allocatorCreatesBoundActiveEntityScript": all(x in allocator for x in (
            "operator_new(0x1c)", "CScriptThing::CScriptThing", "0x012D94F0")),
        "derivedVtablePointsAtBarrelCallback": '"pointedFrom":[{"site":"0x012D9504"' in callback,
        "donorPdbIdentifiesAlivePredicateInfrastructure": all(x in pdb for x in (
            "?OnPredicateFail@CNOVI_Barrel@CQ_NewOakValeIntroScript@NScript@@EAEXXZ",
            "??0CActiveEntityScriptBase@@QAE@ABVCScriptThing@@_KK@Z",
            "??RCIsThingAlive@NScript@@UBE_NXZ")),
        "retailCallbackSetsBothBreakFlagsAndPosition": all(x in callback for x in (
            "this + 0x14) + 0x74", "this + 0x14) + 0x75",
            "this + 8) + 0x18", "iVar1 + 0x76", "iVar1 + 0x7a", "iVar1 + 0x7e")),
        "forgeTestsNativeBoundThingAlive": all(x in forge for x in (
            "if (!IsBoundThingAlive())", "pVTable->IsAlive(pImp)")),
        "forgeDispatchesCallbackOnce": all(x in forge for x in (
            "if (m_predicateFailDispatched) return", "m_predicateFailDispatched = true",
            'script->env["OnPredicateFail"]', "callback(script->pQuestState, &m_Me)")),
        "luaCallbackPreservesRetailWrites": all(x in lua for x in (
            "function OnPredicateFail(quest, me)", "F.BarrelBrokenInstantaneous",
            "F.BarrelBrokenPersistent", "F.BarrelBrokenPos", "me:GetPos()")),
        "runtimeObservedDestructionConsequence": "NOVI_PROBE BarrelMan JUDGE bad=1" in runtime,
    }
    return {"schema": "barrel-predicate-lifecycle-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--translation-unit", type=Path, required=True)
    parser.add_argument("--donor-pdb", type=Path, required=True)
    parser.add_argument("--forge-host", type=Path, required=True)
    parser.add_argument("--barrel-lua", type=Path, required=True)
    parser.add_argument("--runtime-log", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.translation_unit, args.donor_pdb, args.forge_host,
                   args.barrel_lua, args.runtime_log)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
