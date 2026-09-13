#!/usr/bin/env python3
"""Classify archived New Oakvale AI runtime evidence without overgeneralizing path variance."""
from __future__ import annotations

import json
import re
from pathlib import Path


def audit(workspace: Path) -> dict:
    evidence_dir = workspace / "refs/script_recovery/new_oakvale_intro/runtime_evidence"
    observation = json.loads((evidence_dir / "ai-action-observations-20260910.json").read_text(encoding="utf-8-sig"))
    log = (evidence_dir / observation["provenByLog"]["file"]).read_text(encoding="utf-8-sig")
    waits = [int(value) for value in re.findall(r"Wait loop finished after (\d+) frames", log)]
    markers = observation["provenByLog"]["markers"]
    observed = observation["observations"]
    checks = {
        "allDeclaredLogMarkersPresent": all(marker in log for marker in markers),
        "blockingSpeechActuallyWaited": bool(waits) and all(value > 0 for value in waits),
        "successfulAffairEndpointObserved": (
            observed["wifeReachedHusbandInAtLeastOneRun"]
            and "TEXT_QST_048_AFFAIRMAN_GOOD_LAD" in log),
        "intermittentWifeShortfallPreserved": (
            observed["wifeSometimesStoppedBeforeHusband"]
            and observed["wifeIssueClassification"] == "intermittent path-completion variance"),
        "bullyShortRunPreserved": observed["bullyRunEndedShortInObservedRun"],
        "scopeIsNewOakvaleSpecific": observation["scope"] == "New Oakvale ForgeFSE override only",
    }
    return {
        "schema": "new-oakvale-ai-runtime-evidence-audit/0.1",
        "ok": all(checks.values()),
        "blockingSpeechCompletions": len(waits),
        "blockingSpeechFrameRange": [min(waits), max(waits)] if waits else [],
        "classification": observed["wifeIssueClassification"],
        "checks": checks,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
