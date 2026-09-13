#!/usr/bin/env python3
"""Audit retail ManageQuestCoreMarkers against Lua and interface evidence."""
from __future__ import annotations

import csv
import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_manage_core_markers_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    forge = (Path(r"D:\Code\ForgeFSE-retail-shadow") / "FableScriptExtender/GameInterface.cpp").read_text(encoding="utf-8-sig")
    with (workspace / "ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv").open(encoding="utf-8-sig", newline="") as handle:
        catalog = {row["offset"].lower():row for row in csv.DictReader(handle, delimiter="\t")}
    errors: list[str] = []
    if (retail.get("rangeSize"), retail.get("instructionCount"), len(retail.get("terminationCalls", []))) != (944, 287, 11):
        errors.append("retail ManageQuestCoreMarkers boundary or termination topology differs")
    if (retail.get("goldMinimum"), retail.get("tutorialId")) != (3, 19):
        errors.append("retail marker wait constants differ")
    if retail.get("markerSequence") != ["add:father", "remove:father", "add:bookTrader", "remove:bookTrader",
                                        "add:theresa", "remove:theresa", "add:father"]:
        errors.append("retail marker handoff order differs")
    slots = {row["slot"] for row in retail.get("indirectCalls", [])} - {"0x4"}
    fallback = {"0x1FC":"GetHeroGold_API = (tGetHeroGold)pVTable[127]; // 0x1FC"}
    for slot in slots - set(fallback):
        row = catalog.get(slot.lower())
        if row is None or row.get("executable", "").lower() != "true": errors.append(f"marker slot {slot} is not executable")
    if not all(line in forge for line in fallback.values()): errors.append("Forge marker-slot fallback differs")
    body = lua[lua.index("function ManageQuestCoreMarkers("):]
    fragments = ["NOVI_LiveFather", "NOVI_BookTrader", "NOVI_Theresa", "MiniMapAddMarker(father",
                 "GetHeroGold() >= GOLD_NEEDED_FOR_SWEETS", "IsHeroControlledByPlayer()",
                 "MiniMapRemoveMarker(father)", "MiniMapAddMarker(trader", "DisplayTutorial(TUTORIAL_QUEST_CORE_MARKER)",
                 "MsgIsTutorialClickedPast()", "F.get(quest, F.GivenSweets)", "MiniMapRemoveMarker(trader)",
                 "MiniMapAddMarker(theresa", "F.get(quest, F.GivenTheresaChocs)", "MiniMapRemoveMarker(theresa)",
                 "MiniMapAddMarker(father"]
    positions, start = [], 0
    for fragment in fragments:
        position = body.find(fragment, start); positions.append(position)
        if position >= 0: start = position + len(fragment)
    if any(position < 0 for position in positions): errors.append("Lua marker operations are missing or out of retail order")
    return {"schema":"new-oakvale-manage-core-markers-audit/0.1", "retailBytes":retail.get("rangeSize"),
            "luaOperations":len(fragments), "errors":errors, "ok":not errors}


if __name__ == "__main__": print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
