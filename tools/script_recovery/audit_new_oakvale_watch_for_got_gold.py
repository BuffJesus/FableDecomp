#!/usr/bin/env python3
"""Audit retail WatchForGotGold against Lua and the interface catalog."""
from __future__ import annotations

import csv
import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_watch_for_got_gold_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    forge_bindings = (Path(r"D:\Code\ForgeFSE-retail-shadow") / "FableScriptExtender/GameInterface.cpp").read_text(encoding="utf-8-sig")
    with (workspace / "ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv").open(encoding="utf-8-sig", newline="") as handle:
        catalog = {row["offset"].lower():row for row in csv.DictReader(handle, delimiter="\t")}
    errors: list[str] = []
    if (retail.get("rangeSize"), retail.get("instructionCount"), retail.get("terminationCalls")) != (
            212, 65, ["0x00DBE300", "0x00DBE31F"]):
        errors.append("retail WatchForGotGold boundary or termination topology differs")
    if retail.get("goldComparison") != {"sites":["0x00DBE2F1", "0x00DBE318"], "operation":">",
                                       "operand":2, "equivalentMinimumGold":3}:
        errors.append("retail gold threshold differs")
    if retail.get("objective") != "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03" or retail.get("objectiveTrailingStrings") != ["", ""]:
        errors.append("retail objective operands differ")
    interface_slots = {row["slot"] for row in retail.get("indirectCalls", [])}
    for slot in interface_slots - {"0x1FC"}:
        row = catalog.get(slot.lower())
        if row is None or row.get("executable", "").lower() != "true":
            errors.append(f"WatchForGotGold interface slot {slot} is not executable")
    if "GetHeroGold_API = (tGetHeroGold)pVTable[127]; // 0x1FC" not in forge_bindings:
        errors.append("Forge does not bind omitted retail slot 0x1FC as GetHeroGold")
    body = lua[lua.index("function WatchForGotGold("):lua.index("function ManageQuestCoreMarkers(")]
    fragments = ["quest:GetHeroGold() > GOLD_NEEDED_FOR_SWEETS - 1",
                 "quest:SetQuestCardObjective(quest:GetActiveQuestName(), TEXT_OBJECTIVE_GOT_GOLD, \"\", \"\")"]
    if not all(fragment in body for fragment in fragments):
        errors.append("Lua WatchForGotGold threshold or objective differs")
    return {"schema":"new-oakvale-watch-for-got-gold-audit/0.1", "retailBytes":retail.get("rangeSize"),
            "errors":errors, "ok":not errors}


if __name__ == "__main__": print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
