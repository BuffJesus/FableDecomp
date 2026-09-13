#!/usr/bin/env python3
"""Audit retail WatchBarrels consequence production against Lua and interface evidence."""
from __future__ import annotations

import csv
import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_watch_barrels_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    deeds = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/deeds.lua").read_text(encoding="utf-8")
    forge_bindings = (Path(r"D:\Code\ForgeFSE-retail-shadow") / "FableScriptExtender/GameInterface.cpp").read_text(encoding="utf-8-sig")
    with (workspace / "ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv").open(encoding="utf-8-sig", newline="") as handle:
        catalog = {row["offset"].lower():row for row in csv.DictReader(handle, delimiter="\t")}
    errors: list[str] = []
    if (retail.get("rangeSize"), retail.get("instructionCount"), retail.get("terminationCalls")) != (647, 208, 4):
        errors.append("retail WatchBarrels boundary or termination topology differs")
    if retail.get("firstBreakBadDeed") != {"counter":1, "enum":0, "callSite":"0x00DBE997", "target":"0x00DAEA70"}:
        errors.append("retail first-break consequence differs")
    if (retail.get("goldBreakIndex"), retail.get("beetleThreshold"), retail.get("createdBeetleMaxHealth"),
            retail.get("createdBeetleHealthSetAbsolute")) != ("total-1", "broken>total-4", 2.0, True):
        errors.append("retail reward branches differ")
    interface_slots = {row["slot"] for row in retail.get("indirectCalls", [])} - {"0x0"}
    for slot in interface_slots - {"0x428", "0x924"}:
        row = catalog.get(slot.lower())
        if row is None or row.get("executable", "").lower() != "true":
            errors.append(f"WatchBarrels interface slot {slot} is not executable")
    forge_fallbacks = [
        "EntitySetMaxHealth_API = (tEntitySetMaxHealth)pVTable[266]; // 0x428",
        "AddItemToContainer_API = (tAddItemToContainer)pVTable[585]; // 0x924",
    ]
    if not all(line in forge_bindings for line in forge_fallbacks):
        errors.append("Forge does not bind omitted retail slots 0x428/0x924 exactly")
    if 'BAD_DEED_BARREL_BROKEN = 0' not in deeds:
        errors.append("Lua bad-deed enum does not preserve retail zero")
    body = lua[lua.index("function WatchBarrels("):lua.index("function WatchForGotGold(")]
    fragments = ["GetAllThingsWithScriptName(NOVI.SCRIPT_BARREL)",
                 "F.set(quest, F.BarrelBrokenInstantaneous, false)", "F.get(quest, F.AttackOver)",
                 "F.get(quest, F.BarrelBrokenInstantaneous)", "broken = broken + 1",
                 "F.set(quest, F.BarrelBrokenInstantaneous, false)", "if broken == 1",
                 "Deeds.add_bad(quest, nil, Deeds.BAD_DEED_BARREL_BROKEN)", "broken == total - 1",
                 "AddItemToContainer", "broken > total - (BEETLES_FROM_LAST_N_BARRELS + 1)",
                 "CreateCreature", "F.get(quest, F.BarrelBrokenPos)",
                 "EntitySetMaxHealth(beetle, BEETLE_MAX_HEALTH, true)", "NewScriptFrame()"]
    positions, start = [], 0
    for fragment in fragments:
        position = body.find(fragment, start)
        positions.append(position)
        if position >= 0: start = position + len(fragment)
    if any(position < 0 for position in positions):
        errors.append("Lua WatchBarrels operations are missing or out of retail order")
    for diagnostic in ("NOVI_PROBE WatchBarrels GOLD_ARM", "NOVI_PROBE WatchBarrels GOLD_INSERT_RETURNED"):
        if diagnostic not in body:
            errors.append(f"missing final-gold diagnostic: {diagnostic}")
    return {"schema":"new-oakvale-watch-barrels-audit/0.1", "retailBytes":retail.get("rangeSize"),
            "luaOperations":len(fragments), "errors":errors, "ok":not errors}


if __name__ == "__main__": print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
