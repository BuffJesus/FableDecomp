#!/usr/bin/env python3
"""Ensure the zero-blocker New Oakvale package contains no unresolved gap scaffold."""
from __future__ import annotations

import json
from pathlib import Path


def audit(package_root: Path, api_requirements: Path, coverage: Path) -> dict:
    lua_sources = {
        path.relative_to(package_root).as_posix(): path.read_text(encoding="utf-8-sig")
        for path in package_root.rglob("*.lua")
    }
    api = json.loads(api_requirements.read_text(encoding="utf-8-sig"))
    cov = json.loads(coverage.read_text(encoding="utf-8-sig"))
    joined = "\n".join(lua_sources.values())

    requirements = api.get("requirements", api if isinstance(api, list) else [])
    lua_surface = api.get("luaCallSurface", [])
    blocking = [row for row in requirements if row.get("blocking") is True]
    gap_statuses = [
        row for row in requirements
        if row.get("fallbackStatus") not in ("direct", "host-managed")
    ]
    required_scoped_calls = {
        (scope, row.get("operation"))
        for row in requirements if row.get("fallbackStatus") == "direct"
        for scope in (row.get("bindingScope") or "").split("|")
    }
    observed_scoped_calls = {
        (row.get("scope"), row.get("operation")) for row in lua_surface
    }
    summary = cov.get("summary", cov.get("coverage", cov))
    not_applicable = {
        (row.get("script"), row.get("nativeFunction")): row.get("status")
        for row in cov.get("rows", []) if str(row.get("status", "")).startswith("not-applicable")
    }
    expected_not_applicable = {
        ("NOVI_Barrel", "Init"): "not-applicable (retail body empty / data only)",
        ("NOVI_CreatedBeetle", "Init"): "not-applicable (retail body empty / data only)",
        ("NOVI_Villager", "QuestInit(vectors)"):
            "not-applicable (covered by Q_NewOakValeIntro.Init)",
        ("OVI_DeadFather", "OnPredicateFail"):
            "not-applicable (retail body empty / data only)",
        ("Q_NewOakValeIntro", "RegisterMain"): "not-applicable (host lifecycle)",
        ("Q_NewOakValeIntro", "destructor"): "not-applicable (host lifecycle)",
    }
    checks = {
        "allLuaFilesScanned": len(lua_sources) == 23,
        "noUnsupportedCallPlaceholder": "NOVI.unsupported" not in joined,
        "noUnknownValuePlaceholder": (
            "UNKNOWN_DISTANCE" not in joined
            and "UNKNOWN_FLOAT" not in joined
            and "unsupported_value" not in joined
        ),
        "apiInventoryHasNoBlockingRequirement": len(blocking) == 0,
        "apiInventoryHasNoGapClassification": len(gap_statuses) == 0,
        "evidenceNotesAreSeparatedFromSemanticDifferences": (
            any(row.get("evidenceNotes") for row in requirements)
            and all(
                not row.get("semanticDifferences")
                for row in requirements if row.get("fallbackStatus") == "direct"
            )
        ),
        "apiInventoryContainsNoNonbindingSentinel": not any(
            row.get("operation", "").lower() in {"n/a", "n/a (data)", "none", "unknown", "missing"}
            for row in requirements
        ),
        "apiInventoryScopesAreComplete": all(
            row.get("bindingScope") in {"Quest", "Entity", "Quest|Entity"}
            if row.get("fallbackStatus") == "direct" else row.get("operation") == "host-managed"
            for row in requirements
        ),
        "dualRegisteredPushabilityIsPreserved": any(
            row.get("operation") == "SetIsPushableByHero"
            and row.get("bindingScope") == "Quest|Entity"
            for row in requirements
        ),
        "inventoryUsesActualSpeakBinding": (
            any(row.get("operation") == "Speak" for row in requirements)
            and not any(row.get("operation") == "SpeakAndWait" for row in requirements)
        ),
        "executableLuaSurfaceIsRecordedAndBound": (
            bool(lua_surface) and all(row.get("bindingExists") for row in lua_surface)
        ),
        "everyDirectRequirementScopeIsObservedInExecutableLua": (
            required_scoped_calls <= observed_scoped_calls
        ),
        "operationMappingFlagsAreScopeExact": all(
            bool(row.get("mappedByOperationInventory"))
            == ((row.get("scope"), row.get("operation")) in required_scoped_calls)
            for row in lua_surface
        ),
        "helperOnlyCallsRemainVisible": all(
            any(row.get("scopedName") == scoped and not row.get("mappedByOperationInventory")
                for row in lua_surface)
            for scoped in ("Quest.GetHero", "Entity.ReleaseControl", "Quest.GetMasterGameState")
        ),
        "coverageHasNoUncertaintyOrBlocker": (
            summary.get("uncertain", 0) == 0
            and summary.get("apiBlocked", 0) == 0
            and summary.get("notImplemented", 0) == 0
        ),
        "coverageAssignsEveryInventoriedOperation": (
            summary.get("operations") == 747
            and sum(row.get("operations", 0) for row in cov.get("rows", [])) == 747
        ),
        "notApplicableFunctionsAreExplicitAndExact": not_applicable == expected_not_applicable,
    }
    return {"schema": "new-oakvale-no-gap-scaffolding-audit/0.1", "ok": all(checks.values()), "checks": checks}
