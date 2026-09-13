#!/usr/bin/env python3
"""Keep Barrel Man's numeric operands tied to their named retail enums."""
from __future__ import annotations

import json
from pathlib import Path


def audit(forge_header: Path, lua_script: Path, entity_manifest: Path) -> dict:
    header = forge_header.read_text(encoding="utf-8-sig")
    lua = lua_script.read_text(encoding="utf-8-sig")
    manifest = json.loads(entity_manifest.read_text(encoding="utf-8-sig"))
    constants = {item["name"]: item for item in manifest["constants"]}
    checks = {
        "forgeNamesRunAsOne": "ENTITY_MOVE_RUN = 0x1" in header,
        "forgeNamesHealLifeAsFourteen": "HERO_ABILITY_HEAL_LIFE_SPELL = 0xE" in header,
        "manifestRecordsNamedEnums": (
            constants["MOVE_TYPE"].get("value") == 1
            and constants["MOVE_TYPE"].get("enumName") == "ENTITY_MOVE_RUN"
            and constants["IGNORED_HIT_ABILITY"].get("value") == 14
            and constants["IGNORED_HIT_ABILITY"].get("enumName") == "HERO_ABILITY_HEAL_LIFE_SPELL"
        ),
        "luaDocumentsNamedEnums": (
            "local MOVE_TYPE                = 1        -- ENTITY_MOVE_RUN" in lua
            and "local IGNORED_HIT_ABILITY      = 14       -- HERO_ABILITY_HEAL_LIFE_SPELL" in lua
        ),
        "unknownEnumLanguageRemoved": all(
            "name unknown" not in str(item).lower()
            for item in (constants["MOVE_TYPE"], constants["IGNORED_HIT_ABILITY"])
        ),
    }
    return {"schema": "barrel-man-enum-constants-audit/0.1", "ok": all(checks.values()), "checks": checks}
