#!/usr/bin/env python3
"""Lock the single caller-owned movie bracket used by the dead-father macro."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def audit(lua: Path, forge: Path, runtime_log: Path) -> dict:
    script = lua.read_text(encoding="utf-8-sig")
    native_host = forge.read_text(encoding="utf-8-sig")
    runtime = runtime_log.read_text(encoding="utf-8-sig", errors="replace")
    start = script.index("local function play_dead_father_scene")
    end = script.index("\nend", start) + 4
    scene = script[start:end]
    checks = {
        "luaOwnsOneExplicitBracket": all(x in scene for x in (
            "Quest:StartMovieSequence()", "Quest:RunCutsceneWithSetup(", "Quest:EndMovieSequence()")),
        "luaOrdersBracketAroundMacro": scene.index("Quest:StartMovieSequence()") <
            scene.index("Quest:RunCutsceneWithSetup(") < scene.index("Quest:EndMovieSequence()"),
        "forgeIndexesMovieByCallingLuaVm": all(x in native_host for x in (
            "m_movieHandlesByLuaState.find(luaState)",
            "callerMovie == m_movieHandlesByLuaState.end() || callerMovie->second == nullptr")),
        "forgeDoesNotOpenOrCloseBorrowedMovie": all(x in native_host for x in (
            "if (ownsMovieSequence) {", "Reusing caller-owned movie sequence.")),
        "runtimeActuallyBorrowedMovie": "[RunCutsceneWithSetup] Reusing caller-owned movie sequence." in runtime,
        "runtimeEnteredAndExitedFatherMacro": all(x in runtime for x in (
            "ENTERING BLOCKING RunCutsceneMacro_Func for 'CS_OAKVALE_INTRO_FATHER'",
            "EXITED BLOCKING RunCutsceneMacro_Func")),
        "runtimeSkipReachedNativeMacro": "skippable=1" in runtime,
    }
    return {"schema": "dead-father-movie-bracket-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--lua", type=Path, required=True)
    parser.add_argument("--forge", type=Path, required=True)
    parser.add_argument("--runtime-log", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.lua, args.forge, args.runtime_log)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
