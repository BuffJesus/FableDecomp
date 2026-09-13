#!/usr/bin/env python3
"""Validate native phase records against reconstructed Lua control-flow anchors."""
from __future__ import annotations

import json
import re
from pathlib import Path

try:
    from tools.script_recovery.validate_reconstructed_package import strip_lua_comments
except ModuleNotFoundError:
    from validate_reconstructed_package import strip_lua_comments


def audit(entities_dir: Path, lua_root: Path) -> dict:
    errors: list[str] = []
    phase_count = file_count = anchor_count = enum_phase_count = 0
    for inventory in sorted(entities_dir.glob("*.json")):
        data = json.loads(inventory.read_text(encoding="utf-8-sig"))
        phases = data.get("phases", [])
        if not phases:
            continue
        file_count += 1
        lua_path = (lua_root / "NewOakValeIntro.lua" if inventory.stem == "Q_NewOakValeIntro"
                    else lua_root / "Entities" / f"{inventory.stem}.lua")
        source = strip_lua_comments(lua_path.read_text(encoding="utf-8-sig"))
        declarations = set(re.findall(
            r"^(?:local\s+)?function\s+([A-Za-z_]\w*)", source, re.MULTILINE))
        ids: set[str] = set()
        enum_values = {}
        for field in data.get("localFields", []):
            for item in field.get("enumValues", []):
                enum_values[(str(item.get("luaName")), int(item["value"]))] = field.get("name")
        for index, phase in enumerate(phases):
            phase_count += 1
            label = f"{inventory.name} phases[{index}]"
            if set(phase) != {"id", "entry", "exit", "luaFunction"}:
                errors.append(f"{label}: expected exactly id/entry/exit/luaFunction")
                continue
            for key in ("id", "entry", "exit", "luaFunction"):
                if not isinstance(phase[key], str) or not phase[key].strip():
                    errors.append(f"{label}: {key} must be non-empty text")
            normalized_id = phase["id"].casefold()
            if normalized_id in ids:
                errors.append(f"{label}: duplicate phase id {phase['id']}")
            ids.add(normalized_id)
            anchors = {
                token for token in re.findall(r"\b[A-Za-z_]\w*\b", phase["luaFunction"])
                if token in declarations
            }
            if not anchors:
                errors.append(f"{label}: no concrete Lua function anchor")
            else:
                anchor_count += len(anchors)
            enum_match = re.fullmatch(r"([A-Z][A-Z0-9_]*)\((-?\d+)\)", phase["id"])
            if enum_match:
                enum_phase_count += 1
                enum_key = (enum_match.group(1), int(enum_match.group(2)))
                if enum_key not in enum_values:
                    errors.append(f"{label}: enum phase {enum_key} absent from localFields")
                if not re.search(
                        rf"\b{re.escape(enum_key[0])}\s*=\s*{enum_key[1]}\b", source):
                    errors.append(f"{label}: enum phase {enum_key} absent from Lua PHASE table")
    return {
        "schema": "new-oakvale-phase-audit/0.1",
        "filesWithPhases": file_count,
        "phases": phase_count,
        "luaFunctionAnchors": anchor_count,
        "enumPhases": enum_phase_count,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(
        root / "refs/script_recovery/new_oakvale_intro/entities",
        root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro"), indent=2))
