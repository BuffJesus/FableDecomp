#!/usr/bin/env python3
"""Audit direct retail BarrelMan judgment evidence against Lua and entity inventory."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/barrel_man_judgment_retail_bytes.json").read_text(encoding="utf-8-sig"))
    manifest = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/entities/NOVI_BarrelMan.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BarrelMan.lua").read_text(encoding="utf-8")
    errors: list[str] = []
    regions = retail.get("regions", [])
    if [(r.get("name"), r.get("rangeSize"), r.get("instructionCount")) for r in regions] != [
            ("automaticJudgment", 934, 262), ("followupDialogue", 653, 187)]:
        errors.append("retail BarrelMan judgment region boundaries differ")
    judgment = retail.get("automaticJudgment", {})
    followup = retail.get("followupDialogue", {})
    if judgment.get("readsBarrelBrokenPersistent") is not False:
        errors.append("automatic judgment unexpectedly depends on broken barrels")
    if judgment.get("detected") != {"speech":"TEXT_QST_048_BARRELMAN_THANKS", "deedHelper":"0x00DB0660"}:
        errors.append("detected-hero retail outcome differs")
    if judgment.get("notDetected", {}).get("deedEnum") != 1 or judgment.get("notDetected", {}).get("deedHelper") != "0x00DAEA70":
        errors.append("absent-hero retail outcome differs")
    if list(followup.values())[-3:] != ["TEXT_QST_048_BARRELMAN_NO_TIME",
                                       "TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN",
                                       "TEXT_QST_048_BARRELMAN_LETDOWN_NOT_BROKE"]:
        errors.append("follow-up retail speech selection differs")
    fragments = ["F.set(quest, F.BarrelManSpokenToHeroOnReturn, true)",
                 "CanThingBe_Seen_ByOtherThing(hero, me)", "NOVI.hero_within(quest, me, SEE_RANGE)",
                 "if heroDetected then", "TEXT_THANKS", "Deeds.add_good(quest, me)",
                 "TEXT_WHERE_GONE", "HeroLetMeDown = true", "TEXT_LEFT_UNATTENDED",
                 "Deeds.add_bad(quest, me, 1)", "MyPhase = PHASE.DONE"]
    judge_body = lua[lua.index("local function judge_hero"):lua.index("local function ask_favour_and_leave")]
    positions, start = [], 0
    for fragment in fragments:
        position = judge_body.find(fragment, start); positions.append(position)
        if position >= 0: start = position + len(fragment)
    if any(position < 0 for position in positions): errors.append("Lua automatic judgment order differs")
    followup_fragments = ["if not HeroLetMeDown then", "TEXT_NO_TIME",
                          "elseif F.get(quest, F.BarrelBrokenPersistent) then", "TEXT_LETDOWN_BROKEN",
                          "TEXT_LETDOWN_NOT_BROKE"]
    if not all(fragment in lua for fragment in followup_fragments): errors.append("Lua follow-up selection differs")
    notes = "\n".join(manifest.get("coupling", []))
    if "does not affect the automatic return judgment" not in notes:
        errors.append("entity inventory does not preserve retail consequence separation")
    return {"schema":"barrel-man-judgment-byte-audit/0.1", "retailBytes":sum(r["rangeSize"] for r in regions),
            "luaJudgmentOperations":len(fragments), "errors":errors, "ok":not errors}


if __name__ == "__main__": print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
