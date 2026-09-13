#!/usr/bin/env python3
"""Prove every Guard lecture approach check uses retail DAT_013ac840 (3.0f)."""
from __future__ import annotations

import json
from pathlib import Path


def audit(disassembly: Path, lua_script: Path, entity_manifest: Path) -> dict:
    asm = disassembly.read_text(encoding="utf-8-sig").lower()
    lua = lua_script.read_text(encoding="utf-8-sig")
    manifest = json.loads(entity_manifest.read_text(encoding="utf-8-sig"))
    exact_sequences = (
        ("00dac8a1: 8b1540c83a01", "00dac8a9: 52", "00dac8b4: e8461af1ff"),
        ("00dac987: 8b1540c83a01", "00dac98f: 52", "00dac99a: e86019f1ff"),
        ("00dac9bd: a140c83a01", "00dac9c4: 50", "00dac9cf: e82b19f1ff"),
    )
    sequence_checks = []
    for load, push, call in exact_sequences:
        positions = tuple(asm.find(token) for token in (load, push, call))
        sequence_checks.append(all(pos >= 0 for pos in positions) and positions == tuple(sorted(positions)))
    checks = {
        "allThreeChecksLoadPushAndCallInOrder": all(sequence_checks),
        "retailConstantRecordedAsThree": any(
            c.get("name") == "LECTURE_DISTANCE" and c.get("value") == 3.0
            for c in manifest["constants"]),
        "luaUsesOneExactConstantForEntryAndLoop": (
            "local LECTURE_DISTANCE = 3.0" in lua
            and lua.count("NOVI.hero_within(quest, me, LECTURE_DISTANCE)") == 2),
        "manifestRemovedAssumptionLanguage": all(
            "assum" not in op.get("note", "").lower()
            for op in manifest["operations"] if op.get("seq") in (15, 22)),
    }
    return {"schema": "guard-lecture-distance-audit/0.1", "ok": all(checks.values()), "checks": checks}
