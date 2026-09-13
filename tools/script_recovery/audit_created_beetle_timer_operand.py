#!/usr/bin/env python3
"""Prove both Created Beetle GetTimer calls consume its registered timer ID."""
from __future__ import annotations

import json
from pathlib import Path


def audit(disassembly: Path, lua_script: Path, entity_manifest: Path) -> dict:
    asm = disassembly.read_text(encoding="utf-8-sig").lower()
    lua = lua_script.read_text(encoding="utf-8-sig")
    manifest = json.loads(entity_manifest.read_text(encoding="utf-8-sig"))
    op = next(item for item in manifest["operations"] if item.get("seq") == 3)

    ordered_tokens = (
        "00db8116: ff905c010000",
        "00db8124: 8bf8",
        "00db8126: 6a05",
        "00db8128: 57",
        "00db8129: ff9264010000",
        "00db8137: 57",
        "00db8138: ff9068010000",
        "00db8159: 57",
        "00db815a: 753d",
        "00db815e: ff9068010000",
    )
    positions = tuple(asm.find(token) for token in ordered_tokens)
    checks = {
        "retailOperandFlowIsPresentAndOrdered": (
            all(position >= 0 for position in positions)
            and positions == tuple(sorted(positions))
        ),
        "luaUsesSingleTimerForSetGetAndCleanup": (
            "local timer = quest:RegisterTimer()" in lua
            and "quest:SetTimer(timer, LIFETIME_SECONDS)" in lua
            and "while quest:GetTimer(timer) ~= 0 do" in lua
            and lua.count("quest:DeregisterTimer(timer)") == 2
        ),
        "manifestUsesDirectDisassemblyEvidence": (
            op.get("evidence") == "native-disassembly"
            and "retained in EDI" in op.get("note", "")
            and "implies" not in op.get("note", "").lower()
        ),
    }
    return {"schema": "created-beetle-timer-operand-audit/0.1", "ok": all(checks.values()), "checks": checks}
