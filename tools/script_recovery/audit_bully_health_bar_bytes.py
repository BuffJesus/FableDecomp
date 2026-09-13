#!/usr/bin/env python3
"""Audit direct retail Bully health-bar evidence against Lua and inventory."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/bully_health_bar_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Bully.lua").read_text(encoding="utf-8-sig")
    manifest = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Bully.json").read_text(encoding="utf-8-sig"))
    errors: list[str] = []
    regions = retail.get("regions", [])
    if [(r.get("name"), r.get("rangeSize"), r.get("instructionCount")) for r in regions] != [
            ("createAndUpdate", 503, 134), ("subduedRemoval", 38, 11)]:
        errors.append("retail Bully health-bar regions differ")
    creation = retail.get("creation", {})
    if (creation.get("unsetSentinel"), creation.get("primaryColourBytes"), creation.get("secondaryColourBytes"),
            creation.get("texture"), creation.get("slot"), creation.get("handleParentOffset")) != (
            -999, "00 FF 00 FF", "00 00 FF FF", "HUD_QUEST_ICON_GRANDSON", "0x510", "0x64"):
        errors.append("retail Bully health-bar creation operands differ")
    if retail.get("update", {}) != {"current": "float(InitialHealth - HitsTaken)", "maximum": -1.0,
            "scale": -1.0, "callSite": "0x00DBC582", "slot": "0x530"}:
        errors.append("retail Bully health-bar update differs")
    removal = retail.get("removal", {})
    if (removal.get("bullySubduedParentOffset"), removal.get("handleParentOffset"), removal.get("slot")) != (
            "0x6C", "0x64", "0x548"):
        errors.append("retail Bully health-bar removal differs")
    fragments = ("local HEALTH_COUNTER_UNSET = -999", "local INFO_BAR_MAX = 0",
                 "local INFO_BAR_UNCHANGED = -1.0", "local INFO_BAR_SCALE = 1.0",
                 "local INFO_BAR_FILLED_COLOUR = { r = 0, g = 255, b = 0, a = 255 }",
                 "local INFO_BAR_EMPTY_COLOUR = { r = 255, g = 0, b = 0, a = 255 }",
                 "quest:AddQuestInfoBar(InitialHealth, INFO_BAR_MAX",
                 "HitsTaken = HitsTaken + 1", "if InitialHealth <= HitsTaken then",
                 "quest:RemoveQuestInfoElement(F.get(quest, F.GUIBullyHealthCounter))",
                 "quest:UpdateQuestInfoBar(F.get(quest, F.GUIBullyHealthCounter), InitialHealth - HitsTaken")
    if not all(fragment in lua for fragment in fragments):
        errors.append("Lua Bully health-bar lifecycle differs")
    operations = manifest.get("operations", [])
    if not all(any(op.get("seq") == seq and "retail-byte-snapshot" in op.get("evidence", "")
                   for op in operations) for seq in (51, 52, 54, 55, 56, 64)):
        errors.append("Bully inventory does not cite direct health-bar byte evidence")
    return {"schema": "bully-health-bar-byte-audit/0.1", "ok": not errors,
            "retailBytes": sum(r.get("rangeSize", 0) for r in regions), "errors": errors}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
