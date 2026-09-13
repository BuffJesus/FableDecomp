#!/usr/bin/env python3
"""Audit direct retail Bully run-off evidence against Lua and inventory."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/bully_run_off_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Bully.lua").read_text(encoding="utf-8-sig")
    manifest = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Bully.json").read_text(encoding="utf-8-sig"))
    errors: list[str] = []
    region = retail.get("region", {})
    if (region.get("rangeSize"), region.get("instructionCount")) != (1161, 339):
        errors.append("retail Bully run-off region differs")
    if retail.get("acquisition", {}).get("order") != ["BULLY", "HERO", "BRAT"] or retail.get("acquisition", {}).get("priority") != 4:
        errors.append("retail Bully acquisition order/priority differs")
    input_args = retail.get("inputArgs", {})
    if (input_args.get("key"), input_args.get("selectorParentOffset"), input_args.get("trueValue"), input_args.get("falseValue")) != (
            "$BRATLINE", "0x6F", "TEXT_QST_048_VICTIM_THANKS_AFTER_HIT", "TEXT_QST_048_VICTIM_THANKS"):
        errors.append("retail Bully input substitution differs")
    if [x.get("name") for x in retail.get("cutscenes", [])] != [
            "CS_OAKVALEINTRO_BULLYRUN1", "CS_OAKVALEINTRO_BULLYRUN2", "CS_OAKVALEINTRO_BULLYRUNDUMMY"]:
        errors.append("retail Bully run-off cutscene sequence differs")
    if retail.get("normalCleanup") != ["FixMovieSequenceCamera(false)", "PauseAllNonScriptedEntities(false)",
            "destroy movie resource", "destroy input-argument map", "destroy actor map", "release BRAT", "release HERO"]:
        errors.append("retail Bully normal cleanup differs")
    success = retail.get("success", {})
    if (success.get("bullyRanOffParentOffset"), success.get("removeThingArgs"), success.get("releaseBullySite")) != (
            "0x6D", ["BULLY", False, True], "0x00DBCCEF"):
        errors.append("retail Bully success teardown differs")
    fragments = ("NOVI.acquire(quest, me, ACQUIRE_PRIORITY)", "NOVI.acquire(quest, hero, ACQUIRE_PRIORITY)",
                 "NOVI.acquire(quest, victim, ACQUIRE_PRIORITY)", "local actors = { HERO = hero, BRAT = victim, BULLY = me }",
                 'local input_args = { ["$BRATLINE"] = brat_line }', "quest:RunCutsceneWithSetup(CS_BULLYRUN1, actors, input_args)",
                 "quest:RunCutsceneWithSetup(CS_BULLYRUN2, actors, {})", "quest:ClearThingHasInformation(victim)",
                 "F.set(quest, F.GivenHeroTeddy, true)", "quest:RunCutsceneWithSetup(CS_BULLYRUN_DUMMY, actors, {})",
                 "NOVI.release(quest, victim)", "NOVI.release(quest, hero)", "F.set(quest, F.BullyRanOff, true)",
                 "Deeds.add_good(quest, me)", "quest:RemoveThing(me, false, true)", "NOVI.release(quest, me)")
    if not all(fragment in lua for fragment in fragments):
        errors.append("Lua Bully run-off lifecycle differs")
    operations = manifest.get("operations", [])
    if not all(any(op.get("seq") == seq and "retail-byte-snapshot" in op.get("evidence", "") for op in operations)
               for seq in range(57, 63)):
        errors.append("Bully inventory does not cite direct run-off byte evidence")
    return {"schema": "bully-run-off-byte-audit/0.1", "ok": not errors,
            "retailBytes": region.get("rangeSize", 0), "errors": errors}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
