#!/usr/bin/env python3
"""Ensure every executable New Oakvale symbolic resource is backed by an inventory."""
from __future__ import annotations

import json
import re
from pathlib import Path

try:
    from tools.script_recovery.validate_reconstructed_package import strip_lua_comments
except ModuleNotFoundError:
    from validate_reconstructed_package import strip_lua_comments

RESOURCE_PREFIXES = (
    "TEXT_", "OBJECT_", "CREATURE_", "Q_", "QS_", "CS_", "M_",
    "REGION_", "THEME_", "MUSIC_", "NOVI_", "OVI_",
)
INVENTORY_FIELDS = (
    "textKeys", "objects", "creatures", "markersAndScriptNames", "animations",
    "cutscenes", "themes",
)
STRING_RE = re.compile(r'["\']([^"\']+)["\']')


def nested_strings(value):
    if isinstance(value, str):
        yield value
    elif isinstance(value, list):
        for item in value:
            yield from nested_strings(item)
    elif isinstance(value, dict):
        for item in value.values():
            yield from nested_strings(item)


def is_resource(value: str) -> bool:
    return value.startswith(RESOURCE_PREFIXES)


def audit(entities_dir: Path, package_dir: Path) -> dict:
    declared: set[str] = set()
    for path in sorted(entities_dir.glob("*.json")):
        data = json.loads(path.read_text(encoding="utf-8-sig"))
        declared.add(str(data.get("entity", "")))
        declared.add(str(data.get("retail", {}).get("registeredName", "")))
        for field in INVENTORY_FIELDS:
            for value in nested_strings(data.get(field, [])):
                if is_resource(value):
                    # Older inventories append human evidence in parentheses to a literal.
                    # Treat the prefix as the identifier without confusing it with dynamic Lua text.
                    declared.add(value.split(" (", 1)[0])
    declared.discard("")

    executable: set[str] = set()
    diagnostics: set[str] = set()
    for path in sorted(package_dir.rglob("*.lua")):
        source = strip_lua_comments(path.read_text(encoding="utf-8-sig"))
        for value in STRING_RE.findall(source):
            if value.startswith(("NOVI_PROBE ", "NOVI_AUTHORITY ")):
                diagnostics.add(value)
            elif is_resource(value):
                executable.add(value)

    exact = {value for value in executable if value in declared}
    dynamic_prefixes = {
        value for value in executable - exact
        if len(value) >= 16 and any(candidate.startswith(value) for candidate in declared)
    }
    missing = sorted(executable - exact - dynamic_prefixes)
    return {
        "schema": "new-oakvale-symbolic-resource-audit/0.1",
        "declaredResources": len(declared),
        "executableResources": len(executable),
        "exactResources": len(exact),
        "dynamicPrefixes": sorted(dynamic_prefixes),
        "diagnosticStringsExcluded": len(diagnostics),
        "missingResources": missing,
        "ok": not missing,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(
        root / "refs/script_recovery/new_oakvale_intro/entities",
        root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro"), indent=2))
