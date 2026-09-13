#!/usr/bin/env python3
"""Lock Guard single-authority claiming and repeat-lecture state flow."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def audit(disassembly: Path, lua_script: Path, runtime_log: Path) -> dict:
    native = disassembly.read_text(encoding="utf-8-sig").lower()
    lua = lua_script.read_text(encoding="utf-8-sig")
    runtime = runtime_log.read_text(encoding="utf-8-sig", errors="replace")
    checks = {
        "nativeLoopComparesBadAgainstDealt": all(x in native for x in (
            "00dac7a3: 8b4858", "00dac7a6: 2b4868")),
        "nativeRechecksAfterChase": all(x in native for x in (
            "00dac9f8: 8b4858", "00dac9fb: 2b4868", "00daca00:")),
        "nativeClaimsCurrentCounterBeforeLecture": all(x in native for x in (
            "00daca1c: 8b5058", "00daca1f: 895068")),
        "nativeBranchesOnGuardsSpokenOnce": all(x in native for x in (
            "00dacb33: 8a8892000000", "00dacb3d: 0f85e4070000", "00dad327:")),
        "nativeFirstLectureSetsSpokenOnce": "00dad31b: c6819200000001" in native,
        "nativeRepeatLectureUsesAgainLine": "s_text_qst_048_guard_caught_you_ag" in native,
        "luaRechecksAndClaimsBeforeSpeaking": all(x in lua for x in (
            "if not unpunished_bad_deeds(quest) then",
            "F.set(quest, F.GuardsDealtWithBadDeeds, F.get(quest, F.BadDeedsPerformed))",
            "if F.get(quest, F.GuardsSpokenOnce) then",
            "TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN")),
        "runtimeObservedTwoDistinctClaims": all(x in runtime for x in (
            "NOVI_PROBE Guard claimed lecture bad=1 dealt=1",
            "NOVI_PROBE Guard claimed lecture bad=2 dealt=2")),
        "runtimeObservedFirstAndRepeatLines": all(x in runtime for x in (
            "Key: TEXT_QST_048_GUARD_CAUGHT_YOU_10",
            "Key: TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN")),
    }
    return {"schema": "guard-repeat-lecture-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--disassembly", type=Path, required=True)
    parser.add_argument("--lua-script", type=Path, required=True)
    parser.add_argument("--runtime-log", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.disassembly, args.lua_script, args.runtime_log)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
