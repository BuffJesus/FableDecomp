#!/usr/bin/env python3
"""Audit direct retail Guard repeat-lecture evidence against reconstructed Lua."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/guard_repeat_lecture_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Guard.lua").read_text(encoding="utf-8-sig")
    manifest = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Guard.json").read_text(encoding="utf-8-sig"))
    errors: list[str] = []
    regions = retail.get("regions", [])
    expected_regions = [("claimAndRecheck", 639, 205), ("lectureSelectionAndBodies", 3381, 1001)]
    if [(r.get("name"), r.get("rangeSize"), r.get("instructionCount")) for r in regions] != expected_regions:
        errors.append("retail region bounds or instruction counts differ")
    strings = regions[1].get("strings", []) if len(regions) > 1 else []
    expected_strings = [f"TEXT_QST_048_GUARD_CAUGHT_YOU_{n}" for n in (10, 20, 30, 40, 50, 60)]
    expected_strings += ["TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN", "TEXT_QST_048_GUARD_AFTER_READ_LIST"]
    if strings != expected_strings:
        errors.append("retail first/repeat speech sequence differs")
    lua_claims = (
        "return F.get(quest, F.BadDeedsPerformed) > F.get(quest, F.GuardsDealtWithBadDeeds)",
        "if not unpunished_bad_deeds(quest) then",
        "F.set(quest, F.GuardsDealtWithBadDeeds, F.get(quest, F.BadDeedsPerformed))",
    )
    if not all(fragment in lua for fragment in lua_claims):
        errors.append("Lua does not preserve retail recheck-and-claim flow")
    lua_lecture = ("if F.get(quest, F.GuardsSpokenOnce) then",
                   "TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN",
                   "TEXT_QST_048_GUARD_AFTER_READ_LIST",
                   "F.set(quest, F.GuardsSpokenOnce, true)")
    if not all(fragment in lua for fragment in lua_lecture):
        errors.append("Lua does not preserve retail first/repeat lecture selection")
    operations = manifest.get("operations", [])
    if not all(any(op.get("seq") == seq and "retail-byte-snapshot" in op.get("evidence", "")
                   for op in operations) for seq in (10, 24, 25, 32, 33, 35, 36, 38, 39)):
        errors.append("Guard inventory does not cite direct retail-byte evidence")
    return {"schema": "guard-repeat-lecture-byte-audit/0.1", "ok": not errors,
            "retailBytes": sum(r.get("rangeSize", 0) for r in regions), "errors": errors}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
