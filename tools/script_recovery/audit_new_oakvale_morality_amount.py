#!/usr/bin/env python3
"""Certify New Oakvale's per-deed morality amount from installed TLC data."""
from __future__ import annotations

import argparse
import hashlib
import json
import math
import sys
from pathlib import Path

try:
    from tools.parse_frontend import decode_entry, load_all
except ModuleNotFoundError:
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
    from parse_frontend import decode_entry, load_all


def audit(workspace: Path, forge_root: Path) -> dict:
    compiled_root = workspace / "work/ui_proto/base"
    script_bin = compiled_root / "data/CompiledDefs/script.bin"
    entries, schema, _ = load_all(str(compiled_root), str(workspace / "ghidra_out/def_schema.json"), "script.bin")
    entry = next(item for item in entries if item["index"] == 597)
    decoded, leftover = decode_entry(entry, schema)
    amount = decoded.get("OVI_MoralityChangePerDeed")

    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_quest_full_retail_bytes.json").read_text(encoding="utf-8-sig"))
    regions = {row["name"]: row for row in retail["regions"]}
    good = (workspace / "ghidra_out/script_recovery/native_decompiles/0x00DB0660.c").read_text(encoding="utf-8-sig")
    bad = (workspace / "ghidra_out/script_recovery/native_decompiles/0x00DAEA70.c").read_text(encoding="utf-8-sig")
    deeds = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/deeds.lua").read_text(encoding="utf-8-sig")
    forge = forge_root / "FableScriptExtender"
    api_h = (forge / "GameInterface.h").read_text(encoding="utf-8-sig")
    api_cpp = (forge / "GameInterface.cpp").read_text(encoding="utf-8-sig")
    quest_cpp = (forge / "LuaQuestState.cpp").read_text(encoding="utf-8-sig")

    checks = {
        "retailScriptBinHash": hashlib.sha256(script_bin.read_bytes()).hexdigest().upper() == "3D6D09939B612478F70C6AA1EB45A5BE0E850B27D534F78D996A00466501C1D9",
        "scriptDefEntryAndFieldDecode": (
            entry["definition"] == "CScriptDef" and entry["name"] == "SCRIPT_DEF"
            and leftover == 0 and math.isclose(amount, 0.001, rel_tol=0.0, abs_tol=1e-9)),
        "retailDeedFunctionHashes": (
            regions["AddGoodDeed"]["bytesSha256"] == "19C66A9CB848B1FD83155620CC241A10898A853BDDAC18163BF8D8C266105E04"
            and regions["AddBadDeed"]["bytesSha256"] == "753AF26B2782E0BE9FD4B147E452E8D1585F374DD1C3E8F78D3EB94722D7CC33"),
        "retailGoodLoadsPositiveAndBadNegatesSameField": (
            "DAT_0143e90c + 0xd64" in good
            and "-*(float *)(DAT_0143e90c + 0xd64)" in bad
            and "+ 0x270" in good and "+ 0x270" in bad),
        "forgeUsesExactFloatSlot": (
            "tGiveHeroMorality)(CGameScriptInterfaceBase* This, float amount)" in api_h
            and "GiveHeroMorality_API = (tGiveHeroMorality)pVTable[156]; // 0x270" in api_cpp
            and "GiveHeroMorality_API(m_pGameInterface, amount);" in quest_cpp),
        "luaUsesDecodedMagnitudeWithOppositeSigns": all(fragment in deeds for fragment in (
            "Deeds.MORALITY_CHANGE_PER_DEED = 0.001",
            "quest:GiveHeroMorality(Deeds.MORALITY_CHANGE_PER_DEED)",
            "quest:GiveHeroMorality(-Deeds.MORALITY_CHANGE_PER_DEED)")),
    }
    return {"schema": "new-oakvale-morality-amount-audit/0.1", "ok": all(checks.values()),
            "decodedAmount": amount, "checks": checks}


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
