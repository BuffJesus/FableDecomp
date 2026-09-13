#!/usr/bin/env python3
"""Audit direct retail Affair Wife argument bytes against Lua and inventory."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/affair_wife_argument_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_AffairWife.lua").read_text(encoding="utf-8-sig")
    manifest = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/entities/NOVI_AffairWife.json").read_text(encoding="utf-8-sig"))
    errors: list[str] = []
    region = retail.get("region", {})
    expected_strings = ["ST_ARGUING_POINT_AWAY", "ST_ARGUING_POINT_AT",
                        "TEXT_QST_048_AFFAIR_WIFE_ON_HIT", "TEXT_QST_048_AFFAIR_WIFE_THANKYOU_SINGLE",
                        "TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_", "TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_10",
                        "TEXT_QST_048_AFFAIRMAN_IN_TROUBLE"]
    if (region.get("rangeSize"), region.get("instructionCount"), region.get("strings")) != (1770, 536, expected_strings):
        errors.append("retail argument region structure differs")
    facing = retail.get("husbandFacing", [])
    if [(row.get("context"), row.get("local"), row.get("slot"), row.get("force")) for row in facing] != [
            ("during pointing animation", "ESP+0x34", "0x76C", False),
            ("after hit", "ESP+0x34", "0x76C", True),
            ("after hero talk", "ESP+0x34", "0x76C", True)]:
        errors.append("retail husband-facing operands differ")
    conversation = retail.get("conversation", {})
    if (conversation.get("addHusband", {}).get("slot"),
            conversation.get("wifeLine", {}).get("listenerLocal"),
            conversation.get("husbandLine", {}).get("speakerLocal"),
            conversation.get("husbandLine", {}).get("speech")) != (
            "0x5B4", "ESP+0x30", "ESP+0x34", "TEXT_QST_048_AFFAIRMAN_IN_TROUBLE"):
        errors.append("retail argument conversation roles differ")
    lua_fragments = ("NOVI.hero_within(quest, me, ARGUE_HERO_DISTANCE)",
                     "quest:EntitySetFacingAngleTowardsThing(me, man, false)",
                     "quest:EntitySetFacingAngleTowardsThing(me, man, true)",
                     "quest:AddPersonToConversation(conv, man)",
                     "quest:AddLineToConversation(conv, key, me, man)",
                     "quest:AddLineToConversation(conv, TEXT_MAN_IN_TROUBLE, man, me)")
    if not all(fragment in lua for fragment in lua_fragments):
        errors.append("Lua does not preserve retail husband targeting and dialogue roles")
    operations = manifest.get("operations", [])
    if not all(any(op.get("seq") == seq and "retail-byte-snapshot" in op.get("evidence", "")
                   for op in operations) for seq in (48, 49, 50, 52, 54, 55, 56, 57, 58, 59, 60)):
        errors.append("Affair Wife inventory does not cite direct argument-byte evidence")
    return {"schema": "affair-wife-argument-byte-audit/0.1", "ok": not errors,
            "retailBytes": region.get("rangeSize", 0), "errors": errors}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
