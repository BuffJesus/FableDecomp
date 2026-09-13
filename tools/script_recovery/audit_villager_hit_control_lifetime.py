#!/usr/bin/env python3
"""Check that Villager hit control survives the bottom frame to the next loop head."""
from __future__ import annotations

import json
from pathlib import Path


def audit(lua_script: Path, entity_manifest: Path) -> dict:
    lua = lua_script.read_text(encoding="utf-8-sig")
    manifest = json.loads(entity_manifest.read_text(encoding="utf-8-sig"))
    main = lua[lua.index("function Main(quest, me)"):]
    loop_release = main.index("if hit_control_held then\n      NOVI.release(quest, me)")
    hit_test = main.index("if hero_hit_me(me) then")
    held_write = main.index("hit_control_held = true")
    bottom_frame = main.index("if not NOVI.frame(quest, me) then", held_write)
    checks = {
        "hitControlTrackedLocally": "local hit_control_held = false" in main,
        "normalReleaseAtLoopHeadBeforeHitTest": loop_release < hit_test,
        "hitResourceRetainedThroughBottomFrame": held_write < bottom_frame,
        "terminationReleasesRetainedControl": (
            "if hit_control_held then NOVI.release(quest, me) end\n      return" in main[bottom_frame:]
        ),
        "hitHelperDoesNotReleaseOnSuccess": (
            "quest:EndMovieSequence()\n  return true" in lua
        ),
        "manifestRecordsExactLoopHeadLifetime": any(
            op.get("seq") == 7 and "loop top" in op.get("lua", "")
            for op in manifest["operations"]
        ),
        "manifestHasNoRemainingVillagerUncertainty": not manifest.get("uncertainties"),
    }
    return {"schema": "villager-hit-control-lifetime-audit/0.1", "ok": all(checks.values()), "checks": checks}
