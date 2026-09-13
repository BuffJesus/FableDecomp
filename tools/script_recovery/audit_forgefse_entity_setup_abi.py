#!/usr/bin/env python3
"""Audit multi-boolean entity setup calls used by New Oakvale Init routines."""
from __future__ import annotations

import json
from pathlib import Path


def audit(forge_header: Path, forge_source: Path, victim_lua: Path, victim_manifest: Path) -> dict:
    header = forge_header.read_text(encoding="utf-8-sig")
    source = forge_source.read_text(encoding="utf-8-sig")
    lua = victim_lua.read_text(encoding="utf-8-sig")
    manifest = json.loads(victim_manifest.read_text(encoding="utf-8-sig"))
    checks = {
        "killableNativeTypeHasTwoFlags": "bool bKillable, bool bUnknown" in header,
        "killableWrapperForwardsSecondFlag": (
            "pThing, isKillable, forceDead.value_or(false)" in source),
        "informationNativeTypeHasThreeFlags": (
            "bool wabder, bool wave, bool continue_ai" in header),
        "informationWrapperForwardsAllFlags": (
            "pThing, wander.value_or(false), wave.value_or(true), continueAI.value_or(false)" in source),
        "victimPassesTwoFalseKillableFlags": (
            "quest:EntitySetAsKillable(me, false, false)" in lua),
        "victimPassesThreeFalseInformationFlags": (
            "quest:SetThingHasInformation(me, false, false, false)" in lua),
        "manifestMatchesCurrentLua": all(
            any(op.get("seq") == seq and op.get("lua") == call for op in manifest["operations"])
            for seq, call in ((3, "quest:EntitySetAsKillable(me, false, false)"),
                              (5, "quest:SetThingHasInformation(me, false, false, false)"))),
    }
    return {"schema": "forgefse-entity-setup-abi-audit/0.1",
            "ok": all(checks.values()), "checks": checks}
