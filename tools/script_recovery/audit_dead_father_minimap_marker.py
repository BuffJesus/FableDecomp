#!/usr/bin/env python3
"""Lock OVI Dead Father's minimap marker operands and lifecycle."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def audit(translation_unit: Path, pdb_names: Path, forge_source: Path,
          lua_script: Path, entity_evidence: Path) -> dict:
    archive = translation_unit.read_text(encoding="utf-8-sig")
    init = next((line for line in archive.splitlines()
                 if line.lower().startswith('{"address":"0x00db8290"')), "")
    pdb = pdb_names.read_text(encoding="utf-8-sig")
    forge = forge_source.read_text(encoding="utf-8-sig")
    lua = lua_script.read_text(encoding="utf-8-sig")
    evidence = json.loads(entity_evidence.read_text(encoding="utf-8-sig"))
    add_body = forge.split("void LuaQuestState::MiniMapAddMarker", 1)[-1].split(
        "\n}\n", 1)[0]
    checks = {
        "pdbProvesThingThenStringSignature":
            "?MiniMapAddMarker@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z" in pdb,
        "nativeConstructsQuestCoreOrbLiteral": all(x in init for x in (
            "HUD_ORB_QUEST_CORE", "CCharString::CCharString")),
        "nativeSelectsMeThingForMarkerCall":
            "(param_1 + 8)" in init and "+ 0x578" in init,
        "forgeWrapperAcceptsThingThenMarker":
            "(CScriptThing* pThing, const std::string& markerName)" in forge,
        "forgeForwardsThingThenConvertedString": all(x in add_body for x in (
            "FableString fsMarker(markerName.c_str())",
            "MiniMapAddMarker_API(m_pGameInterface, pThing, fsMarker.get())")),
        "luaPassesMeThenQuestCoreOrb":
            "quest:MiniMapAddMarker(me, MINIMAP_MARKER)" in lua,
        "evidenceRecordsRetailMarkerLifecycle":
            evidence["operations"][0]["argsKnown"] is True
            and evidence["operations"][8]["native"] == "GSI->MiniMapRemoveMarker(me)"
            and "terminate before DadFound (retail leaves the marker)" in evidence["cleanupPaths"][1]["missingOn"],
    }
    return {"schema": "dead-father-minimap-marker-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--translation-unit", type=Path, required=True)
    parser.add_argument("--pdb-names", type=Path, required=True)
    parser.add_argument("--forge-source", type=Path, required=True)
    parser.add_argument("--lua-script", type=Path, required=True)
    parser.add_argument("--entity-evidence", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.translation_unit, args.pdb_names, args.forge_source,
                   args.lua_script, args.entity_evidence)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
