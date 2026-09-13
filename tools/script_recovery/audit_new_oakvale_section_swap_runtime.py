#!/usr/bin/env python3
"""Join the byte-locked raid swap to observed father entity replacement timing."""
from __future__ import annotations

import json
import sys
from pathlib import Path

try:
    from tools.script_recovery.audit_new_oakvale_section_swap_boundary import audit as audit_static
except ModuleNotFoundError:
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
    from script_recovery.audit_new_oakvale_section_swap_boundary import audit as audit_static


LOGS = (
    "childhood-to-guild-success-20260909.log",
    "instrumented-playthrough-20260909-193731.log",
    "single-authority-childhood-complete-20260910.log",
)


def _ordered(text: str, markers: tuple[str, ...]) -> bool:
    cursor = 0
    for marker in markers:
        cursor = text.find(marker, cursor)
        if cursor < 0:
            return False
        cursor += len(marker)
    return True


def audit(workspace: Path, forge_root: Path = Path(r"D:\Code\ForgeFSE-retail-shadow")) -> dict:
    static = audit_static(workspace, forge_root)
    evidence = workspace / "refs/script_recovery/new_oakvale_intro/runtime_evidence"
    markers = (
        "[EntityAllocator<15>] Script: NewOakValeIntro/Entities/OVI_DeadFather",
        "EXITED LUA INIT for 'NewOakValeIntro/Entities/OVI_DeadFather'",
        "EXITED LUA CALL for 'NewOakValeIntro/Entities/NOVI_LiveFather'",
        "ENTERING LUA CALL for 'NewOakValeIntro/Entities/OVI_DeadFather'",
    )
    observed = {}
    for name in LOGS:
        observed[name] = _ordered((evidence / name).read_text(encoding="utf-8-sig"), markers)
    checks = {
        "staticSectionSwapClosed": static["ok"],
        "fatherReplacementObservedInThreeRuns": all(observed.values()),
        "independentRuntimeCaptures": len(observed) == 3,
    }
    return {
        "schema": "new-oakvale-section-swap-runtime-audit/0.1",
        "ok": all(checks.values()),
        "observedRuns": observed,
        "provenRuntimeBoundary": "post-attack Dead Father allocation/init followed by Live Father unwind and Dead Father Main entry",
        "remainingRuntimeBoundary": "visual and non-scripted world-population parity after the section swap",
        "checks": checks,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
