#!/usr/bin/env python3
"""Prove AddBadDeed callers inherit its exact counter and crime-kind writes."""
from __future__ import annotations

import json
from pathlib import Path


def audit(disassembly: Path, entity_dir: Path) -> dict:
    asm = disassembly.read_text(encoding="utf-8-sig").lower()
    manifests = {
        path.stem: json.loads(path.read_text(encoding="utf-8-sig"))
        for path in entity_dir.glob("*.json")
    }
    quest = manifests["Q_NewOakValeIntro"]
    helper = next(op for op in quest["operations"] if op.get("function") == "AddBadDeed")
    inferred_fields = [
        f"{name}:{field}"
        for name, manifest in manifests.items()
        for field in manifest.get("parentFieldsWritten", [])
        if "AddBadDeed" in field and "inference" in field.lower()
    ]
    checks = {
        "manifestHelperRecordsBothWrites": (
            "BadDeedsPerformed++" in helper.get("native", "")
            and "WhichBadDeedsPerformed[kind]=1" in helper.get("native", "")
            and helper.get("evidence") == "native caller + helper disassembly"
        ),
        "retailIncrementsBadDeedCounter": "00daea77: ff4658" in asm,
        "retailLoadsKindArgumentIntoEdi": "00daea98: 8b7c240c" in asm,
        "retailReadsIndexedCrimeFlag": "00daeb67: 8a8437fc000000" in asm,
        "retailWritesIndexedCrimeFlagOnBothPaths": (
            asm.count("c68437fc00000001") == 2
            and "00daeb4b: c68437fc00000001" in asm
            and "00daebe2: c68437fc00000001" in asm
        ),
        "liveFatherRecordsViolenceIndexExactly": (
            "WhichBadDeedsPerformed[2] (via AddBadDeed)"
            in manifests["NOVI_LiveFather"]["parentFieldsWritten"]
        ),
        "theresaRecordsViolenceIndexExactly": (
            "WhichBadDeedsPerformed[2] (via AddBadDeed)"
            in manifests["NOVI_Theresa"]["parentFieldsWritten"]
        ),
        "noCallerRetainsInferenceLabel": not inferred_fields,
    }
    return {"schema": "add-bad-deed-field-propagation-audit/0.1", "ok": all(checks.values()), "checks": checks}
