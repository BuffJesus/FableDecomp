#!/usr/bin/env python3
"""Audit Dead Father's seven PlayLoopingAnimation flags through Forge."""
from __future__ import annotations

import json
from pathlib import Path


def audit(lua_script: Path, entity_manifest: Path, lua_manager: Path,
          entity_header: Path, entity_source: Path) -> dict:
    lua = lua_script.read_text(encoding="utf-8-sig")
    manifest = json.loads(entity_manifest.read_text(encoding="utf-8-sig"))
    manager = lua_manager.read_text(encoding="utf-8-sig")
    header = entity_header.read_text(encoding="utf-8-sig")
    source = entity_source.read_text(encoding="utf-8-sig")
    exact_call = "me:PlayLoopingAnimation(ANIM_DEAD_DAD, LOOP_FOREVER, false, true, false, true, true, false, false)"
    checks = {
        "manifestRecoversSevenRetailFlags": any(
            op.get("seq") == 7 and "false, true, false, true, true, false, false" in op["native"]
            for op in manifest["operations"]),
        "luaPassesEveryRecoveredFlag": exact_call in lua,
        "registrationAcceptsSevenOptionalFlags": (
            'cscriptThing_type["PlayLoopingAnimation"]' in manager
            and all(f"sol::optional<bool> b{i}" in manager for i in range(1, 8))),
        "nativeVtableTypeHasSevenFlags": (
            "int loopCount, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7" in header),
        "wrapperDefaultsAndForwardsInOrder": all(fragment in source for fragment in (
            "b1.value_or(false), b2.value_or(true), b3.value_or(false)",
            "b4.value_or(false), b5.value_or(true), b6.value_or(false)",
            "b7.value_or(false)")),
    }
    return {"schema": "dead-father-looping-animation-abi-audit/0.1",
            "ok": all(checks.values()), "checks": checks}
