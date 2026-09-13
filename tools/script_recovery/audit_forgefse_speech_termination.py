#!/usr/bin/env python3
"""Audit propagation of retail-style speech termination from Forge to Lua."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def audit(forge_root: Path) -> dict:
    folder = forge_root / "FableScriptExtender"
    header = (folder / "LuaEntityAPI.h").read_text(encoding="utf-8-sig")
    source = (folder / "LuaEntityAPI.cpp").read_text(encoding="utf-8-sig")
    manager = (folder / "LuaManager.cpp").read_text(encoding="utf-8-sig")
    registry = (folder / "ManifestFunctionRegistry.generated.h").read_text(encoding="utf-8-sig")
    aggregate = (forge_root / "ALL-INTERFACE-FUNCTIONS-FOR-FSE.h").read_text(encoding="utf-8-sig")
    start = source.find("bool LuaEntityAPI::Speak_Blocking(")
    end = source.find("void LuaEntityAPI::Converse_NonBlocking", start)
    body = source[start:end] if start >= 0 and end >= 0 else ""
    checks = {
        "publicResultIsBool": "bool Speak_Blocking(" in header and start >= 0,
        "luaOverloadsReturnResult": manager.count("return entityAPI->Speak_Blocking(") >= 3,
        "waitUsesEntityThreadTermination": "if (IsThreadTerminating())" in body,
        "terminationReturnsFalse": "Thread terminating during speech wait" in body and "return false;" in body,
        "completedSpeechReturnsTrue": "return true;" in body,
        "aggregateDeclarationIsBool": "bool Speak_Blocking(" in aggregate,
        "generatedManifestResultIsBool": '"Speak_Blocking", "Entity", "bool"' in registry,
    }
    return {"schema": "forgefse-speech-termination-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--forge-root", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.forge_root)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
