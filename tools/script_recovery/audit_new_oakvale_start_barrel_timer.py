#!/usr/bin/env python3
"""Audit the retail StartBarrelTimer body against Lua and Forge ABI bindings."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path, forge_root: Path = Path(r"D:\Code\ForgeFSE-retail-shadow")) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_start_barrel_timer_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    folder = forge_root / "FableScriptExtender"
    header = (folder / "GameInterface.h").read_text(encoding="utf-8-sig")
    bindings = (folder / "GameInterface.cpp").read_text(encoding="utf-8-sig")
    errors: list[str] = []
    if (retail.get("rangeSize"), retail.get("instructionCount"), retail.get("terminationCalls")) != (562, 163, 4):
        errors.append("retail StartBarrelTimer boundary or termination topology differs")
    expected_args = {"current":45.0, "maximum":0.0, "colour1":"0xFF00FF00", "colour2":"0xFF00FF00",
                     "texture":"HUD_CLOCK_ICON", "text":"", "scale":1.0}
    if retail.get("barArguments") != expected_args:
        errors.append("retail AddQuestInfoBar operands differ")
    if (retail.get("guardDistance"), retail.get("insideColour"), retail.get("outsideColour"),
            retail.get("unchangedMaximum"), retail.get("unchangedScale"), retail.get("removeOnNormalExitOnly")) != (
            2.0, "0xFF00FF00", "0xFFFF0000", -1.0, -1.0, True):
        errors.append("retail timer-loop constants differ")
    typedefs = [
        "typedef int(__thiscall* tAddQuestInfoBar)(CGameScriptInterfaceBase* This, float current, float max, const CRGBColour* pColour1, const CRGBColour* pColour2, const CCharString* pTexture, const CCharString* pText, float scale);",
        "typedef void(__thiscall* tUpdateQuestInfoBar)(CGameScriptInterfaceBase* This, int id, float current, float max, float scale);",
        "typedef void(__thiscall* tChangeQuestInfoBarColour)(CGameScriptInterfaceBase* This, int id, const CRGBColour* pColour1, const CRGBColour* pColour2);",
        "typedef void(__thiscall* tRemoveQuestInfoElement)(CGameScriptInterfaceBase* This, int id);",
    ]
    slot_bindings = [
        "AddQuestInfoBar_API = (tAddQuestInfoBar)pVTable[324]; // 0x510",
        "UpdateQuestInfoBar_API = (tUpdateQuestInfoBar)pVTable[332]; // 0x530",
        "ChangeQuestInfoBarColour_API = (tChangeQuestInfoBarColour)pVTable[333]; // 0x534",
        "RemoveQuestInfoElement_API = (tRemoveQuestInfoElement)pVTable[338]; // 0x548",
    ]
    if not all(line in header for line in typedefs): errors.append("Forge HUD-bar typedefs differ from retail")
    if not all(line in bindings for line in slot_bindings): errors.append("Forge HUD-bar slots differ from retail")
    body = lua[lua.index("function StartBarrelTimer("):lua.index("function WatchBarrels(")]
    fragments = ["GetTimer(watch_timer) > 0", "AddQuestInfoBar(BARREL_WATCH_SECONDS, 0",
                 "F.set(quest, F.GUIBarrelCounter, bar)", "MARKER_WAREHOUSE_GUARD",
                 "while not F.get(quest, F.BarrelManSpokenToHeroOnReturn)", "NewScriptFrame()",
                 "hero_within(quest, guard_point, WAREHOUSE_GUARD_POINT_DISTANCE)",
                 "ChangeQuestInfoBarColour(bar, colour, colour)", "GetTimer(watch_timer)",
                 "UpdateQuestInfoBar(bar, timer_value, INFO_BAR_UNCHANGED, INFO_BAR_UNCHANGED)",
                 "RemoveQuestInfoElement(bar)"]
    positions = []
    cursor = 0
    for fragment in fragments:
        position = body.find(fragment, cursor)
        positions.append(position)
        if position >= 0: cursor = position + len(fragment)
    if any(position < 0 for position in positions):
        errors.append("Lua StartBarrelTimer operations are missing or out of retail order")
    return {"schema":"new-oakvale-start-barrel-timer-audit/0.1", "retailBytes":retail.get("rangeSize"),
            "luaOperations":len(fragments), "errors":errors, "ok":not errors}


if __name__ == "__main__": print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
