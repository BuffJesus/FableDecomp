#!/usr/bin/env python3
"""Audit retail Barrel Man judgment versus later broken-stock dialogue."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def _between(text: str, start: str, end: str) -> str:
    begin = text.index(start)
    finish = text.index(end, begin)
    return text[begin:finish]


def audit(disassembly: Path, lua_script: Path) -> dict:
    native = disassembly.read_text(encoding="utf-8-sig").lower()
    lua = lua_script.read_text(encoding="utf-8-sig")
    judge = _between(native, "00db5a48:", "00db5dee:")
    followup = _between(native, "00db65bf:", "00db684c:")
    checks = {
        "judgmentSetsReturnedFlag": "00db5a4b: c6417301" in judge,
        "judgmentShortCircuitsVisibilityThenTenMetres": all(x in judge for x in (
            "00db5a93: ff95e0090000", "00db5a9b: 0f8587010000", "00db5aa6: 6800002041",
            "00db5ab5:", "isdistancebetweenthingsunder", "00db5abc: 0f8566010000")),
        "judgmentDoesNotReadBrokenBarrelFlag": "[ecx + 0x75]" not in judge,
        "detectedHeroGetsThanksAndGoodDeed": all(x in judge for x in (
            "00db5c28:", "s_text_qst_048_barrelman_thanks", "00db5d93:", "addgooddeed")),
        "absentHeroGetsWhereGoneAndBadDeed": all(x in judge for x in (
            "s_text_qst_048_scrmsg_barrelman_wh", "00db5b3c: c6461d01", "00db5c08:", "addbaddeed")),
        "followupRequiresHeroLetMeDown": all(x in followup for x in (
            "00db65d7: 8a461d", "00db65de: 0f84b2010000")),
        "followupThenReadsBrokenBarrelFlag": all(x in followup for x in (
            "00db65f4: 8a4175", "00db65fb: 0f84c7000000")),
        "followupBrokenAndUnbrokenLinesSeparated": all(x in followup for x in (
            "s_text_qst_048_barrelman_letdown_b", "s_text_qst_048_barrelman_letdown_n")),
        "luaDocumentsRetailAndScopesBrokenStockCompatibility": all(x in lua for x in (
            "local heroDetected = quest:CanThingBe_Seen_ByOtherThing(hero, me)",
            "heroDetected = NOVI.hero_within(quest, me, SEE_RANGE)",
            "local stockBroken = F.get(quest, F.BarrelBrokenPersistent)",
            "if heroDetected and stockBroken then",
            "HeroLetMeDown = true",
            "speak_to_hero(quest, me, TEXT_LETDOWN_BROKEN)",
            "elseif heroDetected then",
            "if not HeroLetMeDown then",
            "elseif F.get(quest, F.BarrelBrokenPersistent) then",
            "TEXT_LETDOWN_BROKEN")),
    }
    return {"schema": "barrel-man-consequence-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--disassembly", type=Path, required=True)
    parser.add_argument("--lua-script", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.disassembly, args.lua_script)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
