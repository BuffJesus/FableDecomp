#!/usr/bin/env python3
"""Audit native RemoveThing boolean effects and Forge operand forwarding."""
from __future__ import annotations

import json
from pathlib import Path


def audit(native_decompile: Path, forge_header: Path, forge_source: Path, entity_manifest: Path) -> dict:
    native = native_decompile.read_text(encoding="utf-8-sig")
    header = forge_header.read_text(encoding="utf-8-sig")
    source = forge_source.read_text(encoding="utf-8-sig")
    manifest = json.loads(entity_manifest.read_text(encoding="utf-8-sig"))
    checks = {
        "nativeSignatureHasTwoBools": "(CGameScriptInterface *this,CScriptThing *param_1,bool param_2,bool param_3)" in native,
        "secondBoolSetsThingFlag02": "if (param_3)" in native and "this_00[0x93] = (CThing)((byte)this_00[0x93] | 2);" in native,
        "firstBoolFeedsKill": "CThing::Kill(this_00,param_2);" in native,
        "forgeNativeTypePreservesOrder": "const CScriptThing* pThing, bool b1, bool b2" in header,
        "forgeLuaWrapperNamesImmediateFirst": "sol::optional<bool> immediate, sol::optional<bool> removeFromWorld" in source,
        "forgeForwardsOperandsUnswapped": "pThing, immediate.value_or(false), removeFromWorld.value_or(true)" in source,
        "createdBeetlePassesBothTrue": any(
            op.get("seq") == 4 and op.get("lua") == "quest:RemoveThing(me, true, true)"
            for op in manifest["operations"]
        ),
        "manifestNoLongerTreatsKnownEffectsAsUncertain": not manifest.get("uncertainties"),
    }
    return {"schema": "forgefse-remove-thing-abi-audit/0.1", "ok": all(checks.values()), "checks": checks}
