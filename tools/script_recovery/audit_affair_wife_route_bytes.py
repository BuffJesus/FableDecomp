#!/usr/bin/env python3
"""Audit direct retail Affair Wife route bytes against Lua and entity inventory."""
from __future__ import annotations

import json
from pathlib import Path


def _ordered(text: str, needles: tuple[str, ...]) -> bool:
    cursor = 0
    for needle in needles:
        cursor = text.find(needle, cursor)
        if cursor < 0:
            return False
        cursor += len(needle)
    return True


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/affair_wife_route_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_AffairWife.lua").read_text(encoding="utf-8-sig")
    manifest = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/entities/NOVI_AffairWife.json").read_text(encoding="utf-8-sig"))
    errors: list[str] = []
    region = retail.get("region", {})
    if (region.get("rangeSize"), region.get("instructionCount"), region.get("strings")) != (
            411, 118, ["NOVI_AffairMan", "TEXT_QST_048_AFFAIR_WIFE_RUNNING_TO_HUBBY"]):
        errors.append("retail route region structure differs")
    if retail.get("movement") != {
            "positionReadSite": "0x00DB3473", "positionVtableSlot": "0x18",
            "nullPosition": "0x0143E8E0", "callSite": "0x00DB3486",
            "helper": "0x007E72F0", "radius": 2.0, "moveType": 1,
            "trailingFlags": [False, True], "issuedOnce": True}:
        errors.append("retail movement snapshot differs")
    arrival = retail.get("arrival", {})
    if (arrival.get("distance"), arrival.get("initialTestSite"), arrival.get("loopTestSite"),
            arrival.get("thingOperand"), arrival.get("falseLoopTarget"), arrival.get("reachedTarget")) != (
            3.0, "0x00DB34A2", "0x00DB3586", "same live husband local ESP+0x34",
            "0x00DB34B0", "0x00DB3593"):
        errors.append("retail live-husband arrival loop differs")
    if not _ordered(lua, (
            "local man = quest:GetThingWithScriptName(SCRIPT_NAME_MAN)",
            "if man then target = man:GetPos() end",
            "me:MoveToPosition(target, RUN_TO_HUSBAND_RADIUS, MOVE_RUN, false, true)",
            "quest:ClearThingHasInformation(me)",
            "while not quest:IsDistanceBetweenThingsUnder(me, man, HUSBAND_REACHED_DISTANCE) do",
            "me:ClearCommands()",
            "quest:EntitySetAsUseMovementInActions(me, false)")):
        errors.append("Lua does not preserve the retail route transition")
    operations = manifest.get("operations", [])
    if not all(any(op.get("seq") == seq and "retail-byte-snapshot" in op.get("evidence", "")
                   for op in operations) for seq in (14, 15, 16, 17, 18, 44, 45, 46, 47)):
        errors.append("Affair Wife inventory does not cite direct route-byte evidence")
    return {"schema": "affair-wife-route-byte-audit/0.1", "ok": not errors,
            "retailBytes": region.get("rangeSize", 0), "errors": errors}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
