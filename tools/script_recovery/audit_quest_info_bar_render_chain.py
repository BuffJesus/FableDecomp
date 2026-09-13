#!/usr/bin/env python3
"""Audit how retail stores quest-bar colours separately from sprite resources."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path, forge_root: Path = Path(r"D:\Code\ForgeFSE-retail-shadow")) -> dict:
    support = workspace / "ghidra_out/script_recovery/native_support"
    paths = {address: support / f"{address}.c" for address in
             ("0x0064F7FC", "0x00644DF8", "0x00656B1B", "0x006463FF")}
    errors: list[str] = []
    bodies: dict[str, str] = {}
    for address, path in paths.items():
        if not path.exists():
            errors.append(f"missing {path.name}")
            continue
        text = path.read_text(encoding="utf-8", errors="replace")
        if f"function entry {address}" not in text or "DecompFuncsToDirectory.java" not in text:
            errors.append(f"invalid provenance for {path.name}")
        bodies[address] = text
    add = bodies.get("0x0064F7FC", "")
    change = bodies.get("0x00644DF8", "")
    ctor = bodies.get("0x00656B1B", "")
    if not all(fragment in add for fragment in (
            "GetScriptSpriteBySymbol(this,param_5)", "GetScriptSpriteBySymbol(this,param_6)",
            "::operator_new(0x58)", "(this_00,param_1,param_2,param_3,param_4,lVar1,lVar2,param_7)")):
        errors.append("retail AddBar does not preserve separate colour and sprite operands")
    if not all(fragment in ctor for fragment in (
            "*(float *)(this + 0x1c) = param_1", "*(float *)(this + 0x24) = param_2",
            "this[0x28] = CVar3", "this[0x2c] = CVar3",
            "*(long *)(this + 0x30) = param_5", "*(long *)(this + 0x34) = param_6")):
        errors.append("retail bar constructor field split differs")
    if not all(fragment in change for fragment in (
            "piVar2[10] = *(int *)param_2", "piVar2[0xb] = *(int *)param_3")):
        errors.append("retail ChangeBarColour no longer writes the two colour fields")
    forge = (forge_root / "FableScriptExtender/LuaQuestState.cpp").read_text(encoding="utf-8-sig")
    if not all(fragment in forge for fragment in (
            "return { b, g, r, a };", "CRGBColour c1 = ParseColorTable(color1);",
            "CRGBColour c2 = ParseColorTable(color2);",
            "AddQuestInfoBar_API(m_pGameInterface, current, max, &c1, &c2, fsTexture, fsText, scale)",
            "ChangeQuestInfoBarColour_API(m_pGameInterface, id, &c1, &c2)")):
        errors.append("Forge quest-bar colour bridge differs")
    return {"schema": "quest-info-bar-render-chain-audit/0.1", "ok": not errors,
            "nativeFunctions": len(bodies), "errors": errors,
            "finding": "bar colours are stored at +0x28/+0x2C; texture/text sprite ids are independent at +0x30/+0x34"}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
