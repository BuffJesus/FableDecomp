#!/usr/bin/env python3
"""Audit retail DoMission phase anchors against Lua and Forge interfaces."""
from __future__ import annotations

import csv
import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_do_mission_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    with (workspace / "ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv").open(encoding="utf-8-sig", newline="") as handle:
        catalog = {row["offset"].lower(): row for row in csv.DictReader(handle, delimiter="\t")}
    errors: list[str] = []
    expected_strings = ["StartOakVale", "CREATURE_HERO_CHILD", "WatchBarrels", "WatchForGotGold",
                        "ManageQuestCoreMarkers", "Q_NewOakValeIntro_PreAttack", "HerosOldHouse"]
    expected_callbacks = ["0x00DBE890", "0x00DBE2E0", "0x00DBE4E0"]
    if retail.get("strings") != expected_strings:
        errors.append("DoMission retail string order differs")
    if retail.get("spawnedCallbacks") != expected_callbacks:
        errors.append("DoMission watcher callback order differs")
    if retail.get("attackOverReadCount") != 3 or retail.get("attackOverWidths") != [1]:
        errors.append("DoMission AttackOver accesses are not three byte reads")
    if retail.get("terminationChecks") != 6 or retail.get("terminationTarget") != "0x00CB7940":
        errors.append("DoMission termination checks differ")
    if retail.get("directPhaseCalls") != ["0x00DBE3C0", "0x00DBEB20"]:
        errors.append("DoMission attack/post-attack call order differs")
    unique_slots = {slot.lower() for slot in retail.get("interfaceSlots", [])}
    for slot in unique_slots:
        row = catalog.get(slot)
        if row is None or row.get("executable", "").lower() != "true":
            errors.append(f"DoMission interface slot {slot} is not executable in the catalog")
    if "return Quest:IsRegionLoaded(NOVI.REGION_START_OAKVALE)" not in lua:
        errors.append("Lua start-region waiter differs from retail StartOakVale")
    do_mission = lua[lua.index("function DoMission("):lua.index("-- spawned threads")]
    lua_fragments = [
        "if not wait_for_start_region() then return end", "if not F.get(Quest, F.AttackOver) then", "if not begin_childhood() then return end",
        "if not wait_for_attack() then return end", "if Quest:IsActiveThreadTerminating() then return end",
        "AttackStuff()", "if not PostAttackStuff() then return end", "complete_quest()",
    ]
    positions = [do_mission.find(fragment) for fragment in lua_fragments]
    if any(position < 0 for position in positions) or positions != sorted(positions):
        errors.append("Lua DoMission phase anchors are missing or out of retail order")
    for name, callback in zip(expected_strings[2:5], expected_callbacks):
        if f'Quest:CreateThread("{name}"' not in lua:
            errors.append(f"Lua does not spawn retail watcher {name} ({callback})")
    return {
        "schema": "new-oakvale-do-mission-audit/0.1",
        "retailBytes": retail.get("rangeSize"),
        "watcherThreads": len(expected_callbacks),
        "uniqueInterfaceSlots": len(unique_slots),
        "terminationChecks": retail.get("terminationChecks"),
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
