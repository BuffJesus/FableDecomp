#!/usr/bin/env python3
"""Summarize a live ForgeFSE New Oakvale override run from its append-only log."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


ENTITY_NAMES = (
    "NOVI_LiveFather", "NOVI_Theresa", "NOVI_Guard", "NOVI_Villager",
    "NOVI_Bully", "NOVI_Victim", "NOVI_TeddyGirl", "NOVI_AffairMan",
    "NOVI_AffairWoman", "NOVI_AffairWife", "NOVI_BookTrader",
    "NOVI_BarrelMan", "NOVI_BarrelThug", "NOVI_Barrel",
    "NOVI_CreatedBeetle", "OVI_DeadFather",
)


def analyze(text: str) -> dict[str, object]:
    quest_marker = "LuaQuestHost for 'NewOakValeIntro/NewOakValeIntro' created"
    quest_text = text[text.find(quest_marker):] if quest_marker in text else text
    errors = [line.strip() for line in quest_text.splitlines()
              if re.search(r"(?:!!! LUA RUNTIME ERROR|C\+\+ EXCEPTION)", line)]
    bound = [name for name in ENTITY_NAMES
             if f"Binding entity '{name}'" in text and "Binding successful." in text]
    loaded = [name for name in ENTITY_NAMES
              if f"/Entities/{name}.lua" in text or f"\\Entities\\{name}.lua" in text]
    threads = [name for name in
               ("StartBarrelTimer", "WatchBarrels", "WatchForGotGold", "ManageQuestCoreMarkers")
               if f"Thread '{name}' registered" in text]
    milestones = {
        "hookInstalled": "Retail allocator override hook installed" in text,
        "overrideArmed": "Armed identity-preserving retail override for 'Q_NewOakValeIntro'" in text,
        "allocatorReplaced": "Replacing retail allocator for 'Q_NewOakValeIntro'" in text,
        "questHostCreated": "LuaQuestHost for 'NewOakValeIntro/NewOakValeIntro' created" in text,
        "questInitEntered": "Quest 'NewOakValeIntro/NewOakValeIntro': C++ Init() phase" in text,
        "questMainEntered": "Quest 'NewOakValeIntro/NewOakValeIntro': C++ Main() phase" in text,
        "entityBindingsFinalized": "[NewOakValeIntro/NewOakValeIntro]     Finalization complete." in text,
    }
    if errors:
        phase = "runtime-error"
    elif len(loaded) == len(ENTITY_NAMES):
        phase = "all-entity-hosts-loaded"
    elif loaded:
        phase = "entity-host-loading"
    elif milestones["questMainEntered"]:
        phase = "quest-main"
    elif milestones["allocatorReplaced"]:
        phase = "allocator-replaced"
    elif milestones["hookInstalled"]:
        phase = "hook-installed"
    else:
        phase = "not-observed"
    return {
        "schema": "new-oakvale-forgefse-runtime/0.1",
        "phase": phase,
        "milestones": milestones,
        "entityBindingsDeclared": len(bound),
        "entityScriptsLoaded": loaded,
        "threadsRegistered": threads,
        "errors": errors,
        "playthroughComplete": False,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--log", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    result = analyze(args.log.read_text(encoding="utf-8", errors="replace"))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result, indent=2))
    return 0 if result["milestones"]["allocatorReplaced"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
