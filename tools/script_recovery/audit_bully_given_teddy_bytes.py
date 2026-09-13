#!/usr/bin/env python3
"""Audit retail Bully GivenTeddy bytes against reconstructed Lua and inventory."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/bully_given_teddy_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Bully.lua").read_text(encoding="utf-8-sig")
    manifest = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Bully.json").read_text(encoding="utf-8-sig"))
    errors: list[str] = []
    region = retail.get("region", {})
    if (region.get("rangeSize"), region.get("instructionCount")) != (88, 28):
        errors.append("retail GivenTeddy region differs")
    expected = [
        ("GiveHeroGold", 1, "0x1F8"),
        ("TakeObjectFromHero", "OBJECT_TEDDY_BEAR_UNGIVEABLE", "0x1F4"),
        ("SpokeAboutFindingTeddy", True, "0x70"),
        ("TeddyRuined", True, "0x91"),
        ("AddBadDeed", 3, "0x00DAEA70"),
    ]
    actual = []
    for op in retail.get("operations", []):
        value = op.get("object", op.get("value"))
        destination = op.get("slot", op.get("parentOffset", op.get("target")))
        actual.append((op.get("name"), value, destination))
    if actual != expected:
        errors.append("retail GivenTeddy operation sequence differs")
    fragments = ("quest:GiveHeroGold(TEDDY_REWARD_GOLD)", "quest:TakeObjectFromHero(TEDDY_OBJECT)",
                 "F.set(quest, F.SpokeAboutFindingTeddy, true)", "F.set(quest, F.TeddyRuined, true)",
                 "Deeds.add_bad(quest, me, BAD_DEED_TEDDY_TO_BULLY)")
    positions = [lua.find(fragment) for fragment in fragments]
    if any(position < 0 for position in positions) or positions != sorted(positions):
        errors.append("Lua GivenTeddy operation sequence differs")
    operations = manifest.get("operations", [])
    if not all(any(op.get("seq") == seq and "retail-byte-snapshot" in op.get("evidence", "") for op in operations)
               for seq in range(8, 12)):
        errors.append("Bully inventory does not cite direct GivenTeddy bytes")
    constants = manifest.get("constants", [])
    if not all(any(row.get("name") == name and "retail-byte-snapshot" in row.get("evidence", "") for row in constants)
               for name in ("TEDDY_REWARD_GOLD", "BAD_DEED_TEDDY_TO_BULLY")):
        errors.append("Bully constants do not cite direct GivenTeddy bytes")
    return {"schema": "bully-given-teddy-byte-audit/0.1", "ok": not errors,
            "retailBytes": region.get("rangeSize", 0), "errors": errors}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
