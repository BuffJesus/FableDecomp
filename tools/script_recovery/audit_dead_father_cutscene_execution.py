#!/usr/bin/env python3
"""Lock dead-father cutscene execution/restoration while reserving visual framing for QA."""
from __future__ import annotations

import json
import sys
from pathlib import Path

try:
    from tools.script_recovery.audit_new_oakvale_post_attack_stuff import audit as audit_post_attack
except ModuleNotFoundError:
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
    from script_recovery.audit_new_oakvale_post_attack_stuff import audit as audit_post_attack


def _ordered(text: str, fragments: tuple[str, ...]) -> bool:
    cursor = 0
    for fragment in fragments:
        cursor = text.find(fragment, cursor)
        if cursor < 0:
            return False
        cursor += len(fragment)
    return True


def audit(workspace: Path, forge_root: Path = Path(r"D:\Code\ForgeFSE-retail-shadow")) -> dict:
    static = audit_post_attack(workspace)
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8-sig")
    runtime = (workspace / "refs/script_recovery/new_oakvale_intro/runtime_evidence/single-authority-childhood-complete-20260910.log").read_text(encoding="utf-8-sig")
    checks = {
        "postAttackStaticBoundary": static["ok"],
        "luaCameraAndPauseBracketIsOrdered": _ordered(lua, (
            'local CUTSCENE_DEAD_FATHER  = "CS_OAKVALEINTRO_HESDEADJIM"',
            "Quest:PauseAllNonScriptedEntities(true)",
            "Quest:FixMovieSequenceCamera(true)",
            "Quest:RunCutsceneWithSetup(CUTSCENE_DEAD_FATHER",
            "Quest:FixMovieSequenceCamera(false)",
            "Quest:PauseAllNonScriptedEntities(false)")),
        "runtimeEnteredAndExitedDeadFatherMacro": _ordered(runtime, (
            "--- Running Cutscene with Custom Setup: CS_OAKVALEINTRO_HESDEADJIM ---",
            ">>>>> ENTERING BLOCKING RunCutsceneMacro_Func for 'CS_OAKVALEINTRO_HESDEADJIM' <<<<<",
            "[CutsceneState] before 'CS_OAKVALEINTRO_HESDEADJIM'",
            "[CutsceneSkipDiag] begin macro=CS_OAKVALEINTRO_HESDEADJIM",
            "[CutsceneSkipDiag] SetCutsceneMode(inCutscene=1, second=1) macro=CS_OAKVALEINTRO_HESDEADJIM",
            "[CutsceneSkipDiag] end macro=CS_OAKVALEINTRO_HESDEADJIM",
            "[CutsceneState] after 'CS_OAKVALEINTRO_HESDEADJIM'")),
        "runtimeRestoredCutsceneState": (
            "[CutsceneState] after 'CS_OAKVALEINTRO_HESDEADJIM' gui=" in runtime
            and "actionMode=0 guiActionMode=0 inCutscene=0 skippable=1" in runtime),
    }
    return {"schema": "dead-father-cutscene-execution-audit/0.1", "ok": all(checks.values()),
            "remaining": "visual camera composition parity", "checks": checks}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
