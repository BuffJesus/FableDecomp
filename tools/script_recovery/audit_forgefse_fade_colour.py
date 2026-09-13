#!/usr/bin/env python3
"""Audit the implicit FadeScreenOut colour used by New Oakvale."""
from __future__ import annotations

import json
from pathlib import Path


def audit(forge_source: Path, barrel_lua: Path, barrel_manifest: Path,
          theresa_manifest: Path) -> dict:
    forge = forge_source.read_text(encoding="utf-8-sig")
    lua = barrel_lua.read_text(encoding="utf-8-sig")
    barrel = json.loads(barrel_manifest.read_text(encoding="utf-8-sig"))
    theresa = json.loads(theresa_manifest.read_text(encoding="utf-8-sig"))
    checks = {
        "forgeConstructsOpaqueBlack": "CRGBColour black = { 0, 0, 0, 255 };" in forge,
        "forgePassesColourAsThirdOperand": (
            "FadeScreenOut_API(m_pGameInterface, fadeTime, holdTime, black);" in forge),
        "barrelRetailColourIsOpaqueBlack": any(
            op.get("seq") == 63 and "0xff000000" in op.get("native", "")
            for op in barrel["operations"]),
        "theresaRetailColourIsOpaqueBlack": any(
            op.get("seq") == 50 and "{0,0,0,255}" in op.get("native", "")
            for op in theresa["operations"]),
        "barrelSourceDocumentsExactImplicitOperand": "retail-exact opaque black" in lua,
    }
    return {"schema": "forgefse-fade-colour-audit/0.1", "ok": all(checks.values()), "checks": checks}
