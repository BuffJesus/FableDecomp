#!/usr/bin/env python3
"""Audit retail Bully teddy dispatch against Lua and entity inventory."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/bully_teddy_dispatch_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Bully.lua").read_text(encoding="utf-8-sig")
    manifest = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Bully.json").read_text(encoding="utf-8-sig"))
    errors: list[str] = []
    region = retail.get("region", {})
    if region.get("rangeSize") != 1939:
        errors.append("retail Bully teddy-dispatch byte range differs")
    dispatch = retail.get("dispatch", [])
    if [row.get("path") for row in dispatch] != ["yes/no offer", "direct acceptance", "refusal"]:
        errors.append("retail Bully teddy dispatch order differs")
    offer = retail.get("offer", {})
    if (offer.get("acceptedAnswer"), offer.get("victimComplaintParentOffset"), offer.get("givenTeddyCallSite")) != (1, "0x96", "0x00DBB9BC"):
        errors.append("retail Bully offered-teddy branch differs")
    direct = retail.get("directAcceptance", {})
    if (direct.get("victimComplaintParentOffset"), direct.get("givenTeddyCallSite")) != ("0x96", "0x00DBBCCA"):
        errors.append("retail Bully direct-teddy branch differs")
    fragments = ("local talked_with_teddy = me:IsTalkedToByHero()", "quest:IsObjectInThingsPossession(TEDDY_OBJECT, quest:GetHero())",
                 "return offer_teddy_dialogue(quest, me)", "g_PresentedItemName == TEDDY_OBJECT",
                 "return accept_presented_teddy(quest, me)", "quest:IsObjectInThingsPossession(presented, quest:GetHero())",
                 "return refuse_item(quest, me)", "quest:GiveHeroYesNoQuestion(TEXT.GIVE_TEDDY_QUESTION",
                 "if answer == ANSWER_YES then", "F.set(quest, F.VictimComplainsAboutLosingTeddy, true)",
                 "if not speak_if_alive(quest, me, TEXT.FOUND_TEDDY_TWO)", "given_teddy(quest, me)",
                 "quest:ClearThingHasInformation(me)", "TEXT.DONT_WANT")
    if not all(fragment in lua for fragment in fragments):
        errors.append("Lua Bully teddy dispatch differs")
    operations = manifest.get("operations", [])
    if not all(any(op.get("seq") == seq for op in operations) for seq in range(20, 40)):
        errors.append("Bully inventory lacks teddy-dispatch operations")
    snapshots = manifest.get("retailByteSnapshots", [])
    if not any(row.get("name") == "teddyDispatch" and row.get("range") == "0x00DBB5F0-0x00DBBD83"
               and row.get("file") == "ghidra_out/script_recovery/bully_teddy_dispatch_retail_bytes.json"
               for row in snapshots):
        errors.append("Bully inventory does not cite the teddy-dispatch byte snapshot")
    return {"schema": "bully-teddy-dispatch-byte-audit/0.1", "ok": not errors,
            "retailBytes": region.get("rangeSize", 0), "instructionCount": region.get("instructionCount", 0),
            "errors": errors}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
