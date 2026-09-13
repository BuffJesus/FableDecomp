#!/usr/bin/env python3
"""Audit Barrel Thug chat speech operands against retail disassembly and Forge."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def audit(disassembly: Path, lua_file: Path, forge_root: Path) -> dict:
    asm = disassembly.read_text(encoding="utf-8-sig").lower()
    lua = lua_file.read_text(encoding="utf-8-sig")
    folder = forge_root / "FableScriptExtender"
    header = (folder / "EntityScriptingAPI.h").read_text(encoding="utf-8-sig")
    manager = (folder / "LuaManager.cpp").read_text(encoding="utf-8-sig")

    # x86 __thiscall pushes the seven explicit Speak arguments right-to-left.
    well_done = (
        "00db7271: 6a00" in asm and "00db7273: 6a01" in asm and
        "00db7275: 6a00" in asm and "00db7277: 6a02" in asm and
        "00db7289:" in asm and "?speak@" in asm)
    tempt = (
        "00db732d: 6a00" in asm and "00db732f: 6a01" in asm and
        "00db7331: 6a00" in asm and "00db7333: 6a02" in asm and
        "00db7345:" in asm and "?speak@" in asm)
    checks = {
        "retailWellDonePushesMethod2FalseTrueFalse": well_done,
        "retailTemptPushesMethod2FalseTrueFalse": tempt,
        "luaUsesMethod2": "local SCRMSG_METHOD       = 2" in lua,
        "luaPreservesTrailingBools": (
            "me:Speak(quest:GetHero(), key, method or SPEECH_METHOD, false, true, false)" in lua),
        "forgeEnumNamesMethod2RandomNoRepeat": "GROUP_SELECT_RANDOM_NO_REPEAT = 2" in header,
        "forgeFullOverloadForwardsAllOperands": (
            "return entityAPI->Speak_Blocking(pMe, target, key, method, listen, sound2D, overFade);" in manager),
    }
    return {
        "schema": "new-oakvale-barrel-thug-speech-operands-audit/0.1",
        "ok": all(checks.values()),
        "checks": checks,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--disassembly", type=Path, required=True)
    parser.add_argument("--lua", type=Path, required=True)
    parser.add_argument("--forge-root", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.disassembly, args.lua, args.forge_root)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
