#!/usr/bin/env python3
"""Audit the Heal Life message convenience binding used by New Oakvale hit filtering."""
from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


def audit(forge_root: Path) -> dict:
    header = (forge_root / "FableScriptExtender" / "EntityScriptingAPI.h").read_text(encoding="utf-8-sig")
    source = (forge_root / "FableScriptExtender" / "LuaEntityAPI.cpp").read_text(encoding="utf-8-sig")
    start = source.find("bool LuaEntityAPI::MsgIsHitByHealLifeFromHero(")
    end = source.find("bool LuaEntityAPI::", start + 1)
    body = source[start:end] if start >= 0 and end >= 0 else ""
    checks = {
        "healLifeEnumIsRetail0xE": bool(re.search(r"HERO_ABILITY_HEAL_LIFE_SPELL\s*=\s*0xE", header)),
        "convenienceUsesNamedEnum": "HERO_ABILITY_HEAL_LIFE_SPELL" in body,
        "staleDecimal30Absent": not bool(re.search(r"static_cast<EHeroAbility>\s*\(\s*30\s*\)", body)),
    }
    return {"schema": "forgefse-hero-ability-abi-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--forge-root", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.forge_root)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
