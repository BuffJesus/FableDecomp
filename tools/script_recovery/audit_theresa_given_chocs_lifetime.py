#!/usr/bin/env python3
"""Prove NOVI_Theresa's given_chocs flag is one Main-stack local, not persisted state."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def audit(disassembly: Path, lua_script: Path, entity_manifest: Path) -> dict:
    asm = disassembly.read_text(encoding="utf-8-sig").lower()
    lua = lua_script.read_text(encoding="utf-8-sig")
    manifest = json.loads(entity_manifest.read_text(encoding="utf-8-sig"))

    writes = ("00db9e62: c644241b01", "00dba2f6: c644241b01", "00dba7e5: c644241b01")
    reads = ("00dba3db: 8a442417", "00dba93b: 8a442417", "00dbadf2: 8a442417")
    checks = {
        "mainOwnsFixedStackFrame": all(x in asm for x in (
            "00db97a0: 81ecf0010000", "00dbb2fe: 81c4f0010000", "00dbb304: c3")),
        "localInitializedOnceAfterTwoPushes": "00db982b: c644241f00" in asm,
        "threeTrueWritesTargetSameBaseSlot": all(x in asm for x in writes),
        "threeStableReadsTargetBaseSlot": all(x in asm for x in reads),
        "pushAdjustedWritesResolveToEsp17": all(x in asm for x in (
            "00db9e5e: 8d4508", "00db9e61: 50", "00dba2f2: 8d4508",
            "00dba2f5: 50", "00dba7e1: 8d4508", "00dba7e4: 50")),
        "flagAccessesAreEspRelative": all("[esp +" in asm[asm.find(x):asm.find(x) + 80]
            for x in writes + reads),
        "luaUsesRunLocalNotPersistentField": "local given_chocs = false" in lua
            and lua.count("given_chocs = true") == 2
            and "F.get(quest, F.GivenTheresaChocs)" not in lua,
        "persistentQuestFlagKeptSeparate": "F.set(quest, F.GivenTheresaChocs, true)" in lua,
        "manifestNoLongerLabelsProvedFactUncertain": not manifest.get("uncertainties"),
    }
    return {"schema": "theresa-given-chocs-lifetime-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--disassembly", type=Path, required=True)
    parser.add_argument("--lua-script", type=Path, required=True)
    parser.add_argument("--entity-manifest", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.disassembly, args.lua_script, args.entity_manifest)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
