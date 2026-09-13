#!/usr/bin/env python3
"""Audit retail PostAttackStuff sequencing against the Lua reconstruction."""
from __future__ import annotations

import csv
import json
from pathlib import Path


def audit(workspace: Path) -> dict:
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_post_attack_stuff_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    with (workspace / "ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv").open(encoding="utf-8-sig", newline="") as handle:
        catalog = {row["offset"].lower(): row for row in csv.DictReader(handle, delimiter="\t")}
    forge_bindings = (Path(r"D:\Code\ForgeFSE-retail-shadow") / "FableScriptExtender/GameInterface.cpp").read_text(encoding="utf-8-sig")
    errors: list[str] = []
    expected_strings = ["M_PostAttackStart", "M_PostAttackStart", "V_OakVale",
                        "OBJECT_TEDDY_BEAR_UNGIVEABLE", "MK_OVI_DADTRIGGER", "HERO",
                        "CS_OAKVALEINTRO_HESDEADJIM", "V_OakVale", "Q__OakValeIntro_PostAttack"]
    if retail.get("rangeSize") != 1095 or retail.get("instructionCount") != 324:
        errors.append("retail PostAttackStuff boundary differs")
    if retail.get("strings") != expected_strings:
        errors.append("retail PostAttackStuff resource order differs")
    if (retail.get("musicSets"), retail.get("dadTriggerDistance"), retail.get("heroScriptPriority"),
            retail.get("logbookStoryEntry")) != ([45, 57], 5.0, 4, 20):
        errors.append("retail PostAttackStuff constants differ")
    if retail.get("dadFoundStore") != {"site":"0x00DBEDA9", "offset":"0x51", "width":1, "value":1}:
        errors.append("retail DadFound store differs")
    direct = [(entry["site"], entry["target"]) for entry in retail.get("keyDirectCalls", [])]
    if direct.count(("0x00DBED51", "0x00CBE2FF")) != 1 or direct.count(("0x00DBED8D", "0x00CBE2FF")) != 1:
        errors.append("retail dad-distance polling helpers differ")
    if ("0x00DBECC1", "0x00CBE87F") not in direct or ("0x00DBEE7A", "0x00CBFB7D") not in direct:
        errors.append("retail logbook or cutscene helper differs")
    interface_slots = {entry["slot"] for entry in retail.get("indirectCalls", [])} - {"0x4", "0x12C"}
    catalog_slots = interface_slots - {"0x5EC"}
    for slot in catalog_slots:
        row = catalog.get(slot.lower())
        if row is None or row.get("executable", "").lower() != "true":
            errors.append(f"PostAttackStuff interface slot {slot} is not executable")
    if "PauseAllNonScriptedEntities_API = (tPauseAllNonScriptedEntities)pVTable[379]; // 0x5EC" not in forge_bindings:
        errors.append("Forge does not bind retail slot 0x5EC as PauseAllNonScriptedEntities")
    body = lua[lua.index("function PostAttackStuff("):lua.index("local function complete_quest")]
    fragments = [
        "wait_for_post_attack_section()", "CacheMusicSet(MUSIC_SET_POST_ATTACK_ENTRY)",
        "EntityTeleportToThing", "SetVillageLimbo(village, true)", "DisplayMoneyBag(false)",
        "TakeObjectFromHero", "AddLogbookStoryEntry", "NewScriptFrame()",
        "CameraResetToViewBehindHero(0.0)", "CameraDefault()", "CacheMusicSet(MUSIC_SET_POST_ATTACK_FADE_IN)",
        "FadeScreenIn()", "hero_within(Quest, trigger, DAD_TRIGGER_DISTANCE)", "F.set(Quest, F.DadFound, true)",
        "play_dead_father_scene", "SetVillageLimbo(village, false)", "SetTimeAsStopped(false)",
        "DeactivateQuest(NOVI.SECTION_POST_ATTACK", "ResetToDefaultTheme(0.0)", "StopOverrideMusic(false)",
    ]
    positions = [body.find(fragment) for fragment in fragments]
    if any(position < 0 for position in positions) or positions != sorted(positions):
        errors.append("Lua PostAttackStuff operations are missing or out of retail order")
    return {"schema":"new-oakvale-post-attack-stuff-audit/0.1", "retailBytes":retail.get("rangeSize"),
            "interfaceSlots":len(interface_slots), "luaOperations":len(fragments), "errors":errors, "ok":not errors}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
