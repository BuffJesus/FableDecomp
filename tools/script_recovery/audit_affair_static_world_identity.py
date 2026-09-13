#!/usr/bin/env python3
"""Lock static TNG identity evidence that narrows the Affair Wife route anomaly."""
from __future__ import annotations

import json
from pathlib import Path


def audit(inventory: Path, wife_manifest: Path) -> dict:
    text = inventory.read_text(encoding="utf-8-sig")
    manifest = json.loads(wife_manifest.read_text(encoding="utf-8-sig"))
    uncertainty = " ".join(manifest.get("uncertainties", []))
    runtime_observation = " ".join(manifest.get("runtimeObservations", []))
    checks = {
        "allThreeScriptNamesAreUniquePerTree": all(
            f"{name} count=1" in text
            for name in ("NOVI_AffairMan", "NOVI_AffairWoman", "NOVI_AffairWife")
        ),
        "installedWadPayloadChainRecorded": (
            "WAD entry 405" in text
            and "size 576997" in text
            and "A444FCC91B17EA72CBAAC5012439420F540D099C68529FB5E717B4D02C3B48EA" in text
            and "byte-identical" in text
        ),
        "husbandIdentityIsRecorded": (
            "UID 18446741874686306552" in text
            and "CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED" in text
        ),
        "authoredPositionsAndSectorOriginRecorded": (
            "(24.422607, 89.966431, 16.433842)" in text
            and "(94.591553, 117.561218, 9.000546)" in text
            and "(3456, 736, 0)" in text
            and "Man-Wife ~= 75.765 m" in text
        ),
        "manifestHasNoStaticUncertainty": not uncertainty,
        "manifestExcludesAuthoredDuplicateButRetainsRuntimeObservation": (
            "exactly one authored NOVI_AffairMan" in runtime_observation
            and "duplicate authored script identity is excluded" in runtime_observation
            and "rendered-model-vs-CScriptThing position divergence" in runtime_observation
        ),
    }
    return {"schema": "affair-static-world-identity-audit/0.1", "ok": all(checks.values()), "checks": checks}
