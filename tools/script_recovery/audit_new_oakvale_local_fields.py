#!/usr/bin/env python3
"""Check native entity-local field names and defaults against reconstructed Lua."""
from __future__ import annotations

import json
import re
from pathlib import Path

try:
    from tools.script_recovery.validate_reconstructed_package import strip_lua_comments
except ModuleNotFoundError:
    from validate_reconstructed_package import strip_lua_comments


def literal(value: str):
    value = value.strip().rstrip(",")
    if value == "true":
        return True
    if value == "false":
        return False
    if (value.startswith('"') and value.endswith('"')) or (value.startswith("'") and value.endswith("'")):
        return value[1:-1]
    try:
        return int(value, 0)
    except ValueError:
        try:
            return float(value)
        except ValueError:
            return None


def audit(entities_dir: Path, lua_entities_dir: Path) -> dict:
    errors: list[str] = []
    fields = 0
    constant_defaults = 0
    for inventory in sorted(entities_dir.glob("*.json")):
        data = json.loads(inventory.read_text(encoding="utf-8-sig"))
        rows = data.get("localFields", [])
        if not rows:
            continue
        source = strip_lua_comments(
            (lua_entities_dir / f"{inventory.stem}.lua").read_text(encoding="utf-8-sig")
        )
        constants = {}
        for match in re.finditer(r"^local\s+([A-Z][A-Z0-9_]*)\s*=\s*([^\n]+)", source, re.MULTILINE):
            parsed = literal(match.group(2))
            if parsed is not None:
                constants[match.group(1)] = parsed
        for table_match in re.finditer(
                r"^local\s+([A-Z][A-Z0-9_]*)\s*=\s*\{[ \t]*$(.*?)^\}", source,
                re.MULTILINE | re.DOTALL):
            table_name, body = table_match.groups()
            for member in re.finditer(r"([A-Z][A-Z0-9_]*)\s*=\s*([^,\n]+)", body):
                parsed = literal(member.group(2))
                if parsed is not None:
                    constants[f"{table_name}.{member.group(1)}"] = parsed
        for row in rows:
            fields += 1
            name = str(row["name"]).split(" ", 1)[0]
            if not re.search(rf"\b{re.escape(name)}\b", source):
                errors.append(f"{inventory.name}: local field {name} absent from Lua")
                continue
            assigned = []
            pattern = re.compile(rf"(?:\blocal\s+)?\b{re.escape(name)}\s*=(?!=)\s*([^\n]+)")
            for match in pattern.finditer(source):
                expression = match.group(1).strip()
                parsed = literal(expression)
                if parsed is None and re.fullmatch(r"[A-Z][A-Z0-9_]*(?:\.[A-Z][A-Z0-9_]*)?", expression):
                    parsed = constants.get(expression)
                    if parsed is not None:
                        constant_defaults += 1
                if parsed is not None:
                    assigned.append(parsed)
            if row.get("initValue") not in assigned:
                errors.append(
                    f"{inventory.name}: {name} init {row.get('initValue')!r} not assigned in Lua"
                )
    return {
        "schema": "new-oakvale-local-field-audit/0.1",
        "localFields": fields,
        "constantResolvedDefaults": constant_defaults,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(
        root / "refs/script_recovery/new_oakvale_intro/entities",
        root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities"), indent=2))
