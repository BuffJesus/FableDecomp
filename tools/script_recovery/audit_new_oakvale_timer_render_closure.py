#!/usr/bin/env python3
"""Join static bar/render proofs with the archived New Oakvale timer observation."""
from __future__ import annotations

import json
import re
import sys
from pathlib import Path

try:
    from tools.script_recovery.audit_new_oakvale_start_barrel_timer import audit as audit_timer
    from tools.script_recovery.audit_quest_info_bar_render_chain import audit as audit_render
except ModuleNotFoundError:
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
    from script_recovery.audit_new_oakvale_start_barrel_timer import audit as audit_timer
    from script_recovery.audit_quest_info_bar_render_chain import audit as audit_render


def audit(workspace: Path, forge_root: Path = Path(r"D:\Code\ForgeFSE-retail-shadow")) -> dict:
    timer = audit_timer(workspace, forge_root)
    render = audit_render(workspace, forge_root)
    observation = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/runtime_evidence/timer-render-observation-20260910.json").read_text(encoding="utf-8-sig"))
    runtime = (workspace / "refs/script_recovery/new_oakvale_intro/runtime_evidence/interactive-20260910-redtimer-bullyrun-barrels.log").read_text(encoding="utf-8-sig")
    values = [int(value) for value in re.findall(r"NOVI_PROBE StartBarrelTimer update bar=\d+ value=(-?\d+)", runtime)]
    observed = observation["observations"]
    checks = {
        "retailForgeLuaTimerBoundary": timer["ok"],
        "nativeForgeRenderChain": render["ok"],
        "visualCorrectionObservedRedNotBlue": (
            observed["warehouseTimerOutOfRangeColour"] == "red"
            and observed["formerIncorrectColour"] == "blue"),
        "runtimeCountdownIsCompleteAndNonnegative": (
            values[:46] == list(range(45, -1, -1)) and all(value >= 0 for value in values)),
        "observationScopeIsNewOakvaleSpecific": "unrelated HUD bars" in observation["scope"],
    }
    return {"schema": "new-oakvale-timer-render-closure-audit/0.1", "ok": all(checks.values()),
            "observedCountdown": values[:46], "checks": checks}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
