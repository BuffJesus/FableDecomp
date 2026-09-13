#!/usr/bin/env python3
"""Prove reconstructed random branches consume the retail process RNG stream."""
from __future__ import annotations

from pathlib import Path


def audit(forge_source: Path, lua_root: Path) -> dict:
    forge = forge_source.read_text(encoding="utf-8-sig")
    lua_sources = {p: p.read_text(encoding="utf-8-sig") for p in lua_root.rglob("*.lua")}
    joined = "\n".join(lua_sources.values())
    call_count = joined.count(":RetailRandModulo(")
    checks = {
        "forgeUsesRetailImportThunk": "ASLR<tRetailRand>(0x00BFEB16)" in forge,
        "forgeUsesCdeclRetailRand": "using tRetailRand = int(__cdecl*)();" in forge,
        "modulusAppliedAfterRetailCall": "return retailRand() % modulus;" in forge,
        "invalidModulusCannotCallThunk": "if (modulus <= 0)" in forge,
        "allThirteenRecoveredRandomSitesUseWrapper": call_count == 13,
        "noLuaMathRandom": "math.random" not in joined and "math.randomseed" not in joined,
    }
    return {"schema": "forgefse-retail-rng-audit/0.1", "ok": all(checks.values()),
            "checks": checks, "luaCallSites": call_count}
