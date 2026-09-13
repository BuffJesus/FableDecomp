#!/usr/bin/env python3
"""Audit the retail childhood-to-raid transition against Lua and interface metadata."""
from __future__ import annotations

import csv
import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_attack_stuff_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    with (workspace / "ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv").open(encoding="utf-8-sig", newline="") as handle:
        catalog = {row["offset"].lower(): row for row in csv.DictReader(handle, delimiter="\t")}
    errors: list[str] = []
    expected_strings = ["Q__OakValeIntro_PostAttack", "Q_NewOakValeIntro_PreAttack",
                        "ENVIRONMENT_OV_POSTATTACK", "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_06"]
    expected_slots = ["0x450", "0x460", "0xA18", "0xA40", "0xA3C", "0x4A0"]
    if retail.get("strings") != expected_strings or retail.get("interfaceSlots") != expected_slots:
        errors.append("retail AttackStuff resource or interface order differs")
    if (retail.get("timeOfDay"), retail.get("deactivateDelay"), retail.get("themeTransitionSeconds")) != (23.0, 0, 0.0):
        errors.append("retail AttackStuff constants differ")
    for slot in expected_slots:
        row = catalog.get(slot.lower())
        if row is None or row.get("executable", "").lower() != "true":
            errors.append(f"AttackStuff slot {slot} is not executable")
    body = lua[lua.index("function AttackStuff("):lua.index("local function wait_for_post_attack_section")]
    fragments = ["Quest:ActivateQuest(NOVI.SECTION_POST_ATTACK)",
                 "Quest:DeactivateQuest(NOVI.SECTION_PRE_ATTACK, DEACTIVATE_NOW)",
                 "Quest:SetTimeOfDay(TIME_OF_DAY_RAID)",
                 "Quest:TransitionToTheme(THEME_POST_ATTACK, THEME_TRANSITION_SECONDS)",
                 "Quest:SetQuestCardObjective(active_name(), TEXT_OBJECTIVE_POST_ATTACK"]
    positions = [body.find(fragment) for fragment in fragments]
    if any(position < 0 for position in positions) or positions != sorted(positions):
        errors.append("Lua AttackStuff calls are missing or out of retail order")
    return {"schema":"new-oakvale-attack-stuff-audit/0.1", "retailBytes":retail.get("rangeSize"),
            "interfaceSlots":len(expected_slots), "luaOperations":len(fragments), "errors":errors, "ok":not errors}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
